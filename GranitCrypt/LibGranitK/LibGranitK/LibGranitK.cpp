#include "LibGranitK.h"

#include "SingleSign\rfcSignedData.h" //�������!!
#include "SingleSign\binASN1SignedData.h"

__declspec(dllexport)
int Tst()
{
	MessageBoxA(GetActiveWindow(), "Dll test", "Dll say: Hello!", MB_ICONERROR);
	return 123;
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
	errflag = libKs.LoadKey(PathToSecretKey); //�������� ��������� ���� � ������ libKs.
	
	if (!errflag) //�������� ������.
	{
		strError = libKs.getLastError(); //������� ��������� �� ������.
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

	bool errflag= libKs.GetSkInfo(_Familia,_Imia,_Otchestvo,_DateBegin,_DateEnd,_DaysLeft);
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
	libKs.enableCachedSk();  

	QString Password_ = QString::fromLocal8Bit(pswd.c_str());

	bool errflag = libKs.CheckPassword(Password_);    
	if (!errflag)
	{
		string Err = "";
		Err = libKs.getLastError(); //������� ��������� �� ������.
		strcpy(Error, Err.c_str());
		Password_ = "������"; //�������, ��� � � ������ �� �����.
		return 0; //�������� ������.
	}		

	Password_ = "����"; //�������, ��� � ������ �� �����.
	return 1;
}


//��������� ���������� ����������� ��� ������� �����.
__declspec(dllexport)
int CheckCertForKey(char *Error)
{	

	//��������� ������������ ��������� ����� � ����������� ������������ � �� ������� ��� ������� �����.
	//����� ��������, � ����� ������� ������������.
	bool errflag = libKs.CheckKeyCert(dBi);
			
	if (!errflag)
	{
		string Err = "";
		Err = libKs.getLastError();
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

	Sg.setKeyService(libKs); //������� ������ ��� ������ � �������.
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

	Sg.setKeyService(libKs); //������� ������ ��� ������ � �������.
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