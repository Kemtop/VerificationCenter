#include "LibGranitK.h"

#include "SingleSign\rfcSignedData.h" //Удалить!!
#include "SingleSign\binASN1SignedData.h"

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
	string pswd(password);

	QString Password_ = QString::fromLocal8Bit(pswd.c_str());
	
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

//Шифрует файлы.
__declspec(dllexport)
int CryptFiles(char *filesPath,char *Password)
{
	QTextCodec *StrCodec; //Для преобразования кодировки
	StrCodec = QTextCodec::codecForName("Windows-1251"); //Установка кодировки
	QString filesPath_ = StrCodec->toUnicode(string(filesPath).c_str()); //Преобразовываю строку в QString.

	ShowTestWindow(filesPath);

	//QStringList &paths_to_file, QString Password
	string Dh_OpenKey = keyService.getOpenDHkey();//Получаю открытый ключ схемы Диффи-Хэлмана.
	/*
	CipherWorkerData cgDate; //Данные для механизма шифрования. 
	cgDate.setRoseMode(true); //Включает альтернативный режим шифрования.
	cgDate.setProcessFiles(filesPath_);
	cgDate.setPassword(Password);
	cgDate.setRAsimOpenKey(Dh_OpenKey);
	cgDate.setKeyContainer(Ks.getContainer());
	cgDate.setSignerIndex("003");//Не используется.
	cgDate.setSignatureDH("abc");
	cgDate.setProcessMode(1);//Режим работы-шифрование файлов.

	int f_count = paths_to_file.count(); //Количество файлов
	allCurProcessFiles = f_count;

	initchW(); //Инициализация объекта шифрования.
	chW->setData(cgDate);
	chW->start();//Запускаю поток.
	typeProcessWorking = 1;*/

	return 0;
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

//Возвращает набор данных необходимых для объекта шифрования.
__declspec(dllexport)
CipherWorkerData GetCipherWorkerData(string *pathsToFiles, int filesCount, char *password)
{
	string pswd(password);
	QString password_ = QString::fromLocal8Bit(pswd.c_str());

	QStringList paths_to_file;

	for (int i = 0; i < filesCount; i++)
	{
		paths_to_file.append(pathsToFiles[i].c_str());
	}
	
	string Dh_OpenKey = keyService.getOpenDHkey();//Получаю открытый ключ схемы Диффи-Хэлмана.

	CipherWorkerData cgDate; //Данные для механизма шифрования. 
	cgDate.setRoseMode(true); //Включает альтернативный режим шифрования.
	cgDate.setProcessFiles(paths_to_file);
	cgDate.setPassword(password_);
	cgDate.setRAsimOpenKey(Dh_OpenKey);
	cgDate.setKeyContainer(keyService.getContainer());
	cgDate.setSignerIndex("003");//Не используется.
	cgDate.setSignatureDH("abc");
	cgDate.setProcessMode(1);//Режим работы-шифрование файлов.
	
	return cgDate;
}