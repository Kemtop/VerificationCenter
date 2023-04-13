#include "LibGranitK.h"

#include "SingleSign\rfcSignedData.h" //�������!!
#include "SingleSign\binASN1SignedData.h"

__declspec(dllexport)
int ShowTestWindow(char * message)
{
	MessageBoxA(GetActiveWindow(), "Dll test", message, MB_ICONERROR);
	return 123;
}

/*
 �������������� ���� ��� ���������� � ���������� ������.
*/
__declspec(dllexport)
int SetSingleModeChipper()
{
  keyService.setRoseMode(); //���������������� ������ �������������.	
  return 1;
}

//��������� ������ � ���������� ����� ��� ������ ���������� ������.
__declspec(dllexport)
int CheckPasswordForSecretKey(char *password, char *error)
{	
	string pswd(password);

	QString Password_ = QString::fromLocal8Bit(pswd.c_str());
	
	bool errflag = keyService.CheckPassword(Password_);

	if (!errflag)
	{
		string Err = "";
		Err = keyService.getLastError(); //������� ��������� �� ������.
		strcpy(error, Err.c_str());
		Password_ = "*****"; //�������, ��� � � ������ �� �����.
		return 0; //�������� ������.
	}

	Password_ = "*****"; //�������, ��� � � ������ �� �����.
	return 1;
}

//��� �� �������� ��������� ���������� �����?
__declspec(dllexport)
int GetKeyContainerStatus()
{
	return keyService.getKeyContainerStatus();
}

//������� �����.
__declspec(dllexport)
int CryptFiles(char *filesPath,char *Password)
{
	QTextCodec *StrCodec; //��� �������������� ���������
	StrCodec = QTextCodec::codecForName("Windows-1251"); //��������� ���������
	QString filesPath_ = StrCodec->toUnicode(string(filesPath).c_str()); //�������������� ������ � QString.

	ShowTestWindow(filesPath);

	//QStringList &paths_to_file, QString Password
	string Dh_OpenKey = keyService.getOpenDHkey();//������� �������� ���� ����� �����-�������.
	/*
	CipherWorkerData cgDate; //������ ��� ��������� ����������. 
	cgDate.setRoseMode(true); //�������� �������������� ����� ����������.
	cgDate.setProcessFiles(filesPath_);
	cgDate.setPassword(Password);
	cgDate.setRAsimOpenKey(Dh_OpenKey);
	cgDate.setKeyContainer(Ks.getContainer());
	cgDate.setSignerIndex("003");//�� ������������.
	cgDate.setSignatureDH("abc");
	cgDate.setProcessMode(1);//����� ������-���������� ������.

	int f_count = paths_to_file.count(); //���������� ������
	allCurProcessFiles = f_count;

	initchW(); //������������� ������� ����������.
	chW->setData(cgDate);
	chW->start();//�������� �����.
	typeProcessWorking = 1;*/

	return 0;
}

/*
�������� ��������� ���� �� ���������� ���� � ������ libKs.
*/
__declspec(dllexport)
int LoadSecretKey(char *LastError, char * PathToKey)
{	
	string Path = string(PathToKey); //���� � ����� ���������� �����.
	string strError;
	bool errflag;
		
	QTextCodec *StrCodec; //��� �������������� ���������
	StrCodec = QTextCodec::codecForName("Windows-1251"); //��������� ���������
	QString PathToSecretKey = StrCodec->toUnicode(Path.c_str()); //�������������� ������ � QString.
	errflag = keyService.LoadKey(PathToSecretKey); //�������� ��������� ���� � ������ libKs.
	
	if (!errflag) //�������� ������.
	{
		strError = keyService.getLastError(); //������� ��������� �� ������.
		strcpy(LastError, strError.c_str()); //������� ���������.
		return 0;
	}

	return 1;
}

//�������� ���������� � ��������� ���������� �����.
__declspec(dllexport)
int SkInfo(char *Familia, char *Imia, char *Otchestvo, char *DateBegin, char *DateEnd,char *DaysLeft)
{
	//�������� ���������� � ��������� ���������� �����.
	string _Familia, _Imia, _Otchestvo, _DateBegin, _DateEnd, _DaysLeft;

	bool errflag= keyService.GetSkInfo(_Familia,_Imia,_Otchestvo,_DateBegin,_DateEnd,_DaysLeft);
	if (!errflag) return 0; //�������� ������.

	strcpy(Familia,_Familia.c_str()); //������� ��������.
	strcpy(Imia, _Imia.c_str()); //������� ��������.
	strcpy(Otchestvo, _Otchestvo.c_str()); //������� ��������.
	strcpy(DateBegin, _DateBegin.c_str()); //������� ��������.
	strcpy(DateEnd, _DateEnd.c_str()); //������� ��������.
	strcpy(DaysLeft, _DaysLeft.c_str()); //������� ��������.

	return 1;
}

//��������� ������������ ������ � ���������� ����� ������������ =0 ������
__declspec(dllexport)
int ChkPassword(char *Password, char *Error)
{
	string pswd(Password);

	//��������� ������������ ������ � ���������� ����� ������������ =0 ������
	
	//������� ����� ���������� ��������� ���������� ���������� ����� � ������� libKs �� ����� �������� ������. 
	keyService.enableCachedSk();

	QString Password_ = QString::fromLocal8Bit(pswd.c_str());

	bool errflag = keyService.CheckPassword(Password_);
	if (!errflag)
	{
		string Err = "";
		Err = keyService.getLastError(); //������� ��������� �� ������.
		strcpy(Error, Err.c_str());
		Password_ = "*****"; //�������, ��� � � ������ �� �����.
		return 0; //�������� ������.
	}		

	Password_ = "*****"; //�������, ��� � ������ �� �����.
	return 1;
}


//��������� ���������� ����������� ��� ������� �����.
__declspec(dllexport)
int CheckCertForKey(char *Error)
{	

	//��������� ������������ ��������� ����� � ����������� ������������ � �� ������� ��� ������� �����.
	//����� ��������, � ����� ������� ������������.
	bool errflag = keyService.CheckKeyCert(dBi);
			
	if (!errflag)
	{
		string Err = "";
		Err = keyService.getLastError();
		strcpy(Error, Err.c_str());
		return 0; //�������� ������.
	}
		
	return 1;
}


//�������� ���� ������ ������������.
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
		return 0; //�������� ������.
	}

	return 1;
}


/*
��������� ������� ������ ������������ � ������� Message ������ Mlen.
�������� �������� �������(�������� RFC 5652) � SignStr,����� ������� � Slen.
� ������ ������ ���������� ���������. ���� ������� ������������ �������-���������� ������ ������.
������������ ����� ��������� ������ �������� 2��, ��� ������� ����� ��������� ������ �� ���������)
*/
__declspec(dllexport)
int SignData(uint8_t *Message, int Mlen,char *SignStr, uint32_t *Slen, char *Error)
{
	string _SignStr="";
	uint32_t _Slen;

	Sg.setKeyService(keyService); //������� ������ ��� ������ � �������.
	bool errFlag=Sg.GrKSignData(Message, Mlen, _SignStr, _Slen);
	
	if (!errFlag)
	{
		string Err = "";
		Err = Sg.getLastError();
		strcpy(Error, Err.c_str());
		return 0; //�������� ������.
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

	Sg.setKeyService(keyService); //������� ������ ��� ������ � �������.
	Sg.setDb(dBi);//������ ��� ������ � �� ������� "�������".
	bool errFlag=Sg.GrKCheckSign(Message, Mlen,SignValue, _FIOp);
	
	if (!errFlag)
	{
		string Err = "";
		Err = Sg.getLastError();
		strcpy(Error, Err.c_str());
		return 0; //�������� ������.
	}

	strcpy(FIOp, _FIOp.c_str());

	return 1;
}

//������ ���������������� ������.
__declspec(dllexport)
void ClearLib()
{
	Sg.ClearRAM();
	
}

//��������� ��.
__declspec(dllexport)
void closeDb()
{
	dBi.CloseDb(); //����������� ����� ����� �� ������ ����� ��������.
}

//���������� ����� ������ ����������� ��� ������� ����������.
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
	
	string Dh_OpenKey = keyService.getOpenDHkey();//������� �������� ���� ����� �����-�������.

	CipherWorkerData cgDate; //������ ��� ��������� ����������. 
	cgDate.setRoseMode(true); //�������� �������������� ����� ����������.
	cgDate.setProcessFiles(paths_to_file);
	cgDate.setPassword(password_);
	cgDate.setRAsimOpenKey(Dh_OpenKey);
	cgDate.setKeyContainer(keyService.getContainer());
	cgDate.setSignerIndex("003");//�� ������������.
	cgDate.setSignatureDH("abc");
	cgDate.setProcessMode(1);//����� ������-���������� ������.
	
	return cgDate;
}