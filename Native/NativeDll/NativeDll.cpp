#include "NativeDll.h"

#include "SingleSign\rfcSignedData.h" //�������!!
#include "SingleSign\binASN1SignedData.h"


//������������ char->QString.
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
	QString Password_ = toQString(password);
	
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

//��������� ������������� ���� ����������.
__declspec(dllexport)
int LoadAsymmetricKey(char *password, char *error)
{
	string Dh_OpenKey = keyService.getOpenDHkey();//������� �������� ���� ����� �����-�������.

	//������� ���� �������������� ����������.
	//�������������� ������ � 16 ������ ���� � �������� ������
	QByteArray HexArray = QByteArray::fromHex(Dh_OpenKey.c_str());
	//�������������� ����	
	ArraySource as_pk(reinterpret_cast<uint8_t*>(HexArray.data()), HexArray.size(), true);//pumpAll
	asymmetricKeyR.Load(as_pk); //�������� ����.

	return 1;
}

//�������������� �������� ����������.
__declspec(dllexport)
int InitCipher()
{
	cipherWorker = new CipherWorker();
	cipherWorker->asymmetricKeyR = asymmetricKeyR;
	return 1;
}
//������� ������ ��������� ����������.
int ClearCipher()
{
	delete cipherWorker;
	cipherWorker = NULL;
	return 1;
}

//��������� ���� ������� � �������������� ������������� ������
__declspec(dllexport)
int LoadSignedSecretKey(char *password, char *error)
{
	//������� �������� ���� �������
	Point Q1;		

	//user_d-�������� ���� �������
	bool result = cipherWorker->DecryptEc_SK(Q1, user_d, toQString(password), keyService.getContainer());
	
	const string errorTest = "������ K5 : �� ������� ������� ����.";
	
	if(result)
	{
		//��� ����������� ������� ���� �������. ��� ������ ��� ����� �������� ��� �������� ����� ��������.���������� ���� �� ��� �� ���� ������������.
		strcpy(error, string(user_d.get_str(16)).c_str());
		return 1;
	}
	else
	{
		strcpy(error, errorTest.c_str());
		return 0;
	}
}

//������� ����.
__declspec(dllexport)
int CryptLocalFile(char *src_path, char *dst_dir, char *error)
{
	keyService.setRoseMode(); //���������������� ������ �������������.
	uint8_t session_key[32]; //��������� ���� ��� �������� �����.
	//���������.
	for (int i = 0; i < 32; i++) session_key[i] = 0; //������ ������� � �������!

	uint8_t Reg[32]; //������� ������ ��� ������������.
	Cipher3412 Cipher; //������ ���������� ������� ������ �������� ���������� ���� 3412.
	Gost3413 BlockGost; //�������� ������������.
	
	BlockGost.generateIV(session_key);//�������� ��������� ����� �������� 32�����, ������� �������� ��������� ������.
	Cipher.deploymentEncryptionRoundKeys(session_key);//������������� ���������� �����.

	//���������� ���������� �����
	string crypted_session_key; //����������� ��������� ����.
	//������ ���������� ����, ������������� ����������.
	cipherWorker->CryptedSessionKey(session_key, crypted_session_key);

	BlockGost.generateIV(Reg);//�������� ��������� ��������� ������(32�����) � ������� ��� � �������
	//iv ������ ���� ������ ��� ���� ������! �� ������ ���!
	 //������ ����
	bool result = cipherWorker->CryptFile(toQString(src_path), toQString(dst_dir), Cipher, Reg, crypted_session_key, user_d, keyService.getContainer());

	if (!result)
	{
		strcpy(error, cipherWorker->getLastError().c_str());
		return 0;
	}
	
	return 1;
}

//���������� 32 ������� �����(int256) ��������� ��������������� ��������� ����� ����������.
__declspec(dllexport)
int GenerateRand256(uint8_t *array, uint8_t *getZero)
{
	for (int i = 0; i < 32; i++) array[i] = 0;
	if (*getZero == 1) return 1;
	Gost3413 BlockGost; //�������� ������������.
	BlockGost.generateIV(array);//�������� ��������� ����� �������� 32�����, ������� �������� ��������� ������.
	return 1;
}

//�� ���������� ���������� �������� ���� �����-��������.
__declspec(dllexport)
string GetDhOpenKey()
{
  return keyService.getOpenDHkey();
}

//���������� ��������� �����.
__declspec(dllexport)
PkContainer GetKeyContainer()
{
  return keyService.getContainer();
}

//��������� ��������� ���� ������������.
__declspec(dllexport)
int CheckUserKey(char *pathToKey, char *error)
{
	uint32_t l;
	string path(pathToKey);
	QString path_ = QString::fromLocal8Bit(path.c_str());
	
	bool errflag = keyService.CheckSK(path_, l);

	if (!errflag)
	{
		string Err = keyService.getLastError(); //������� ��������� �� ������.
		strcpy(error, Err.c_str());
		return 0; //�������� ������.
	}
	
	return 1;
}

//��������� ����.
__declspec(dllexport)
int DecryptLocalFile(char *src_path, char *dst_dir, char *keyPassword, char *error)
{
	//����������� ����
	keyService.setRoseMode(); //���������������� ������ �������������.
	cipherWorker->IsDisabledEventProcess = true; //��������� ��������� ������� Qt.
	cipherWorker->setRoseMode(true);
	bool result = cipherWorker->DecryptingFile(toQString(src_path), toQString(dst_dir), "", toQString(keyPassword), keyService.getContainer());
	
	if (!result)
	{
		strcpy(error, cipherWorker->getLastError().c_str());
		return 0;
	}

	return 1;
}

//����� ������� � dll �� ������ ��������� � ������� � lib!