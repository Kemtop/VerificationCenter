#include "LibGranitK.h"

#include "SingleSign\rfcSignedData.h" //Удалить!!
#include "SingleSign\binASN1SignedData.h"

__declspec(dllexport)
int Tst()
{
	MessageBoxA(GetActiveWindow(), "Dll test", "Dll say: Hello!", MB_ICONERROR);
	return 123;
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
	errflag = libKs.LoadKey(PathToSecretKey); //Загружаю секретный ключ в объект libKs.
	
	if (!errflag) //Возникла ошибка.
	{
		strError = libKs.getLastError(); //Получаю сообщение об ошибке.
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

	bool errflag= libKs.GetSkInfo(_Familia,_Imia,_Otchestvo,_DateBegin,_DateEnd,_DaysLeft);
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
	libKs.enableCachedSk();  

	QString Password_ = QString::fromLocal8Bit(pswd.c_str());

	bool errflag = libKs.CheckPassword(Password_);    
	if (!errflag)
	{
		string Err = "";
		Err = libKs.getLastError(); //Получаю сообщение об ошибке.
		strcpy(Error, Err.c_str());
		Password_ = "ноябрь"; //Затираю, что б в памяти не висел.
		return 0; //Возникла ошибка.
	}		

	Password_ = "Кевв"; //Затираю, что в памяти не висел.
	return 1;
}


//Проверяет валидность сертификата для данного ключа.
__declspec(dllexport)
int CheckCertForKey(char *Error)
{	

	//Проверяет соответствие закрытого ключа и сертификата находящегося в БД системы для данного ключа.
	//Сроки действия, а также цепочки сертификатов.
	bool errflag = libKs.CheckKeyCert(dBi);
			
	if (!errflag)
	{
		string Err = "";
		Err = libKs.getLastError();
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

	Sg.setKeyService(libKs); //Передаю объект для работы с ключами.
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

	Sg.setKeyService(libKs); //Передаю объект для работы с ключами.
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