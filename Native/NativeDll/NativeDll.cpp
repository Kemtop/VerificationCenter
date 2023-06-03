#include "NativeDll.h"

#include "SingleSign\rfcSignedData.h" //Удалить!!
#include "SingleSign\binASN1SignedData.h"


//Конвертируем char->QString.
QString toQString(char *val)
{
	string pswd(val);
	return QString::fromLocal8Bit(pswd.c_str());
}

__declspec(dllexport)
int ShowTestWindow(char * message)
{
	MessageBoxA(GetActiveWindow(), "Dll test", message, MB_ICONERROR);
	return 123;
}

/*
 Инициализирует ключ для применения в шифровании данных.
*/
__declspec(dllexport)
int SetSingleModeChipper()
{
  keyService.setRoseMode(); //Модифицированная версия криптосистемы.	
  return 1;
}

//Проверяет пароль к секретному ключу для режима шифрование данных.
__declspec(dllexport)
int CheckPasswordForSecretKey(char *password, char *error)
{	
	QString Password_ = toQString(password);
	
	bool errflag = keyService.CheckPassword(Password_);

	if (!errflag)
	{
		string Err = "";
		Err = keyService.getLastError(); //Получаю сообщение об ошибке.
		strcpy(error, Err.c_str());
		Password_ = "*****"; //Затираю, что б в памяти не висел.
		return 0; //Возникла ошибка.
	}

	Password_ = "*****"; //Затираю, что б в памяти не висел.
	return 1;
}

//Был ли загружен контейнер секретного ключа?
__declspec(dllexport)
int GetKeyContainerStatus()
{
	return keyService.getKeyContainerStatus();
}

/*
Загружаю секретный ключ по указанному пути в объект libKs.
*/
__declspec(dllexport)
int LoadSecretKey(char *LastError, char * PathToKey)
{	
	string Path = string(PathToKey); //Путь к файлу секретного ключа.
	string strError;
	bool errflag;
		
	QTextCodec *StrCodec; //Для преобразования кодировки
	StrCodec = QTextCodec::codecForName("Windows-1251"); //Установка кодировки
	QString PathToSecretKey = StrCodec->toUnicode(Path.c_str()); //Преобразовываю строку в QString.
	errflag = keyService.LoadKey(PathToSecretKey); //Загружаю секретный ключ в объект libKs.
	
	if (!errflag) //Возникла ошибка.
	{
		strError = keyService.getLastError(); //Получаю сообщение об ошибке.
		strcpy(LastError, strError.c_str()); //Передаю сообщение.
		return 0;
	}

	return 1;
}

//Получает информацию о владельце секретного ключа.
__declspec(dllexport)
int SkInfo(char *Familia, char *Imia, char *Otchestvo, char *DateBegin, char *DateEnd,char *DaysLeft)
{
	//Получает информацию о владельце секретного ключа.
	string _Familia, _Imia, _Otchestvo, _DateBegin, _DateEnd, _DaysLeft;

	bool errflag= keyService.GetSkInfo(_Familia,_Imia,_Otchestvo,_DateBegin,_DateEnd,_DaysLeft);
	if (!errflag) return 0; //Возникла ошибка.

	strcpy(Familia,_Familia.c_str()); //Передаю значение.
	strcpy(Imia, _Imia.c_str()); //Передаю значение.
	strcpy(Otchestvo, _Otchestvo.c_str()); //Передаю значение.
	strcpy(DateBegin, _DateBegin.c_str()); //Передаю значение.
	strcpy(DateEnd, _DateEnd.c_str()); //Передаю значение.
	strcpy(DaysLeft, _DaysLeft.c_str()); //Передаю значение.

	return 1;
}

//Проверяет правильность пароля к секретному ключу пользователя =0 ошибка
__declspec(dllexport)
int ChkPassword(char *Password, char *Error)
{
	string pswd(Password);

	//Проверяет правильность пароля к секретному ключу пользователя =0 ошибка
	
	//Включаю режим сохранения некоторых параметров секретного ключа в объекте libKs во время проверки пароля. 
	keyService.enableCachedSk();

	QString Password_ = QString::fromLocal8Bit(pswd.c_str());

	bool errflag = keyService.CheckPassword(Password_);
	if (!errflag)
	{
		string Err = "";
		Err = keyService.getLastError(); //Получаю сообщение об ошибке.
		strcpy(Error, Err.c_str());
		Password_ = "*****"; //Затираю, что б в памяти не висел.
		return 0; //Возникла ошибка.
	}		

	Password_ = "*****"; //Затираю, что в памяти не висел.
	return 1;
}


//Проверяет валидность сертификата для данного ключа.
__declspec(dllexport)
int CheckCertForKey(char *Error)
{	

	//Проверяет соответствие закрытого ключа и сертификата находящегося в БД системы для данного ключа.
	//Сроки действия, а также цепочки сертификатов.
	bool errflag = keyService.CheckKeyCert(dBi);
			
	if (!errflag)
	{
		string Err = "";
		Err = keyService.getLastError();
		strcpy(Error, Err.c_str());
		return 0; //Возникла ошибка.
	}
		
	return 1;
}


//Открытие базы данных сертификатов.
__declspec(dllexport)
int OpenCertDb(char * PathPlugins, char * PathDb, char * Error)
{
	string pth(PathDb); //
	string _PathPlugins(PathPlugins);

	string Err = "";
	
	bool errflag = dBi.OpenDb(_PathPlugins,pth,Err);
	if (!errflag)
	{
		strcpy(Error, Err.c_str());
		return 0; //Возникла ошибка.
	}

	return 1;
}


/*
Выполняет подпись данных содержащихся в массиве Message длиной Mlen.
Помещает цифровую подпись(согласно RFC 5652) в SignStr,длину подпись в Slen.
В случае ошибки возвращает сообщение. Если подпись сформирована успешно-возвращает пустую строку.
Теоретически можно подписать массив размером 2Гб, при желании можно увеличить размер до пентобайт)
*/
__declspec(dllexport)
int SignData(uint8_t *Message, int Mlen,char *SignStr, uint32_t *Slen, char *Error)
{
	string _SignStr="";
	uint32_t _Slen;

	Sg.setKeyService(keyService); //Передаю объект для работы с ключами.
	bool errFlag=Sg.GrKSignData(Message, Mlen, _SignStr, _Slen);
	
	if (!errFlag)
	{
		string Err = "";
		Err = Sg.getLastError();
		strcpy(Error, Err.c_str());
		return 0; //Возникла ошибка.
	}
	
	*Slen = _Slen;
	strcpy(SignStr, _SignStr.c_str());
	
	return 1;
}


__declspec(dllexport)
int LibCheckSign(uint8_t *Message, int Mlen,char *SignStr, char *FIOp, char *Error)
{
	
	string SignValue(SignStr);
	string _FIOp(FIOp);

	Sg.setKeyService(keyService); //Передаю объект для работы с ключами.
	Sg.setDb(dBi);//Объект для работы с БД системы "ГранитК".
	bool errFlag=Sg.GrKCheckSign(Message, Mlen,SignValue, _FIOp);
	
	if (!errFlag)
	{
		string Err = "";
		Err = Sg.getLastError();
		strcpy(Error, Err.c_str());
		return 0; //Возникла ошибка.
	}

	strcpy(FIOp, _FIOp.c_str());

	return 1;
}

//Чистит конфиденциальные данные.
__declspec(dllexport)
void ClearLib()
{
	Sg.ClearRAM();
	
}

//Закрывает БД.
__declspec(dllexport)
void closeDb()
{
	dBi.CloseDb(); //Обязательно нужен иначе БД нельзя будет обновить.
}

//Загружает ассиметричный ключ шифрования.
__declspec(dllexport)
int LoadAsymmetricKey(char *password, char *error)
{
	string Dh_OpenKey = keyService.getOpenDHkey();//Получаю открытый ключ схемы Диффи-Хэлмана.

	//Кэширую ключ асимметричного шифрования.
	//Преобразовываю строку в 16 ричном виде в бинарный формат
	QByteArray HexArray = QByteArray::fromHex(Dh_OpenKey.c_str());
	//Преобразовываю ключ	
	ArraySource as_pk(reinterpret_cast<uint8_t*>(HexArray.data()), HexArray.size(), true);//pumpAll
	asymmetricKeyR.Load(as_pk); //Загружаю ключ.

	return 1;
}

//Инициализирует механизм шифрования.
__declspec(dllexport)
int InitCipher()
{
	cipherWorker = new CipherWorker();
	cipherWorker->asymmetricKeyR = asymmetricKeyR;
	return 1;
}
//Удаляет объект механизма шифрования.
int ClearCipher()
{
	delete cipherWorker;
	cipherWorker = NULL;
	return 1;
}

//Загружает ключ подписи с использованием эллиптической кривой
__declspec(dllexport)
int LoadSignedSecretKey(char *password, char *error)
{
	//Получаю закрытый ключ подписи
	Point Q1;		

	//user_d-закрытый ключ подписи
	bool result = cipherWorker->DecryptEc_SK(Q1, user_d, toQString(password), keyService.getContainer());
	
	const string errorTest = "Ошибка K5 : Не удалось считать ключ.";
	
	if(result)
	{
		//Для наглядности передаю ключ подписи. Что делает код более уязвимым для создания дампа процесса.Переделать если вы так об этом беспокоитесь.
		strcpy(error, string(user_d.get_str(16)).c_str());
		return 1;
	}
	else
	{
		strcpy(error, errorTest.c_str());
		return 0;
	}
}

//Шифрует файл.
__declspec(dllexport)
int CryptLocalFile(char *src_path, char *dst_dir, char *error)
{
	keyService.setRoseMode(); //Модифицированная версия криптосистемы.
	uint8_t session_key[32]; //Сеансовый ключ для блочного шифра.
	//Обнуление.
	for (int i = 0; i < 32; i++) session_key[i] = 0; //Муссор приведи в порядок!

	uint8_t Reg[32]; //Регистр сдвига для гаммирования.
	Cipher3412 Cipher; //Объект содержащий базовые методы блочного шифрования ГОСТ 3412.
	Gost3413 BlockGost; //Алгоритм гаммирования.
	
	BlockGost.generateIV(session_key);//Формирую случайное число размером 32байта, которое является сеансовым ключом.
	Cipher.deploymentEncryptionRoundKeys(session_key);//Развертывание секретного ключа.

	//Шифрование секретного ключа
	string crypted_session_key; //Шифрованный сеансовый ключ.
	//Шифрую сессионный ключ, асимметричным алгоритмом.
	cipherWorker->CryptedSessionKey(session_key, crypted_session_key);

	BlockGost.generateIV(Reg);//Формирую случайный начальный вектор(32байта) и помещаю его в регистр
	//iv должен быть разный для всех файлов! Не менять код!
	 //Шифрую файл
	bool result = cipherWorker->CryptFile(toQString(src_path), toQString(dst_dir), Cipher, Reg, crypted_session_key, user_d, keyService.getContainer());

	if (!result)
	{
		strcpy(error, cipherWorker->getLastError().c_str());
		return 0;
	}
	
	return 1;
}

//Генерирует 32 байтное число(int256) использую псевдослучайный генератор чисел библиотеки.
__declspec(dllexport)
int GenerateRand256(uint8_t *array, uint8_t *getZero)
{
	for (int i = 0; i < 32; i++) array[i] = 0;
	if (*getZero == 1) return 1;
	Gost3413 BlockGost; //Алгоритм гаммирования.
	BlockGost.generateIV(array);//Формирую случайное число размером 32байта, которое является сеансовым ключом.
	return 1;
}

//Из контейнера возвращает открытый ключ Диффи-Хеллмана.
__declspec(dllexport)
string GetDhOpenKey()
{
  return keyService.getOpenDHkey();
}

//Возвращает контейнер ключа.
__declspec(dllexport)
PkContainer GetKeyContainer()
{
  return keyService.getContainer();
}

//Проверяет секретный ключ пользователя.
__declspec(dllexport)
int CheckUserKey(char *pathToKey, char *error)
{
	uint32_t l;
	string path(pathToKey);
	QString path_ = QString::fromLocal8Bit(path.c_str());
	
	bool errflag = keyService.CheckSK(path_, l);

	if (!errflag)
	{
		string Err = keyService.getLastError(); //Получаю сообщение об ошибке.
		strcpy(error, Err.c_str());
		return 0; //Возникла ошибка.
	}
	
	return 1;
}

//Дешифрует файл.
__declspec(dllexport)
int DecryptLocalFile(char *src_path, char *dst_dir, char *keyPassword, char *error)
{
	//Расшифровую файл
	keyService.setRoseMode(); //Модифицированная версия криптосистемы.
	cipherWorker->IsDisabledEventProcess = true; //Отключаем генерацию событий Qt.
	cipherWorker->setRoseMode(true);
	bool result = cipherWorker->DecryptingFile(toQString(src_path), toQString(dst_dir), "", toQString(keyPassword), keyService.getContainer());
	
	if (!result)
	{
		strcpy(error, cipherWorker->getLastError().c_str());
		return 0;
	}

	return 1;
}

//Имена функций в dll не должны совпадать с именами в lib!