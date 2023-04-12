#pragma once
#include "hash3411\hash3411.h"
#include "Gost3412\Utility.h"
#include "Gost3412\Sib3412.h"
#include <QDomDocument>
#include "PkContainer.h"
#include "rsa.h" //CryptLib
#include "osrng.h"
#include "Point.h"
#include "CreateSign.h"
#include <QFileInfo>
#include <QObject>
#include <QByteArray>
#include <Windows.h>
#include <QMetaProperty>
#include <QTextCodec>
#include <QVariant>
#include "GranitDbI\GranitDbI.h"
#include "TBSCertificate.h"
#include "X509Reader\X509Reader.h"
#include "SingleSign\ScKeeper.h"

//������ ���������� �����-��������� ���������� [7-���� ��������� ����������][��������� 47����][��� �������� 256���][����� ���������� ����� 4�����]
#define Key_title_len 90


using std::string;
using namespace CryptoPP;

//����� ��� �������������� � ��������� �������
class KeyService
{
public:
	KeyService();
	~KeyService();

	/*
	��������� ��������� ����, � ������ ������ ���������� 0
	�������� �������������. ����������� ������������ ��������� � ����������� ����� ���������� �����
	��������� ������ ���������� ����� container_length
	*/
	bool CheckSK(QString PathToSK, uint32_t &container_length);

	//��������� ��������� ����� �� �����. �������������� ���, �������� � ������ ����� KeyContainer. 
	bool LoadKey(QString PathToSecretKey);
	//��������� ������
	bool KeyService::CheckPassword(QString psw);
	//���������� �������� ����
	Point GetPk();
	//���������� �������� ����
	mpz_class GetPrvEcKey();

	string GetCertAlgorithmIdentifier();//���������� �������� ������� ����������� ������ ������.
	string GetPublicKeyAlgoritm();//���������� �������� ��������� �����
	string GetEcParam();//���������� OID ������������� ������, �� ��������� ������� ������ ����.

	//���������� ��������� ������. ���� �� �������� ����, ���������� false.
	bool getEcParam(string &Value);

	string getLastError(); //���������� ��������� �� ������.
	bool getKeyContainerStatus(); //���������� ������ �������� ���������� �����.

	PkContainer getContainer() const; //���������� ������ �����.

	 //��������� ���������� ����� �������� �����.
	bool  CheckKeyDateValidity();

	//��������� ������������ ��������� ����� � ����������� ������������ � �� ������� ��� ������� �����.
	bool CheckKeyCert(GranitDbI &Dbi);

	//�������� ���������� � ��������� ���������� �����.
	bool GetSkInfo(string &Familia, string &Imia, string &Otchestvo, string &DateBegin, string &DateEnd, string &DaysLeft);

	
	void enableCachedSk(); //������� ����� ����������� ���������� �����. ������ ����� ���������� ��� �������� ������.
	bool isCacheSk(); //��� �� ����������� ���� �������?

	//���������� ������ ����������.
	void getSignerIndex(string &Value); 
	
	//���������� �������� ���� �������.
	mpz_class getSignPk();

	//���������� ������ ��������� ����� �������.
	void getSignOpenKey(string &Qx, string &Qy);

	//������� ��������� ���� �������, ���� �� ��� ��������.
	void Clear();

	//���������� ���������������� ����������.
	TBSCertificate getUserCert();
	void setRoseMode(); //�������� ���������������� ������ �����.	

	//���������� �������� ���� ����� ������������.
	string getOpenDHkey();

private:
	//������� ����� ������ ������ �����.
	bool enableRose;
	
	
	string lastError; //������.
	bool KeyContainerIsLoad; //���� ���� ��� ��������� ����� ��� �������� �� �����
	bool cachedSk; //��������� ����������� ���������� �����.
	//���������� ����������������� �����������.
	TBSCertificate  tbsUserCertificate; 

	SkKeeper Skd; //������ ��������� ����, ���� ��� ���������.

	typedef int (*AreApisANSI)(char *Linear, int *Len, char *Value); //��� ������ ��� �������� ���������� �����.
	
   //�������� �������� ����� ������������ ����������. � ������ ������ HasErr=1;Err-�������� ��������� �� ������.
	ByteBlock  KeyService::GetPackKv(string & Err, bool & HasErr);

	//�������������� ��������� ���������� ����� � �������� ��� ���������, ��� ������ =1
	/*
	� ������� ������:
	=2; //�� ������� ��������� ���������� libCn3q.dll.
	=3; //�� ������� ��������� �������.
	= 4; //�������� ������� � dll
	=5 //�� ������ ��������� �����
	*/
	int UnpackSKContainer(QByteArray &C, QByteArray &Result);
	
	PkContainer KeyContainer; //��������� ���������� �����
	//������������� ���������
	bool DeserializeKeyContainer(QByteArray *Maket, QObject *object);
	//�������������� �������� ���� ����� �����-�������
	bool DecryptDH_SK(QString Password,RSA::PublicKey &publicKey, RSA::PrivateKey &secret_key);
	//�������� ��������� ����������� ������ � ����
	bool HashedPassword(QString &Passwd, QString &salt_str, uint8_t *hash);

	//�������������� ��������� ���� CryptedKey ��������� key
	bool KeyService::DecryptSK(uint8_t *key, QString &CryptedKey, QString &iv, string &p_key);
	//��������� ���� ������ ����� �����-�������. ���� ���� �� ��������� ���������� 0
    bool KeyService::GetDhPair(QString &open_key, string &private_key, RSA::PublicKey &publicKey, RSA::PrivateKey &secret_key);
	/*
	�������������� �������� ���� �������.
	��������� ������������ ��������� � ��������� �����. � ������ �� ������������ ���������� 0;
	Point &Q-�������� ���� ;mpz_class &d -�������� ����
	*/
	bool DecryptEc_SK(QString Password, Point &Q, mpz_class &d);
	/*
	��������� ������������ ��������� � ��������� �����
	ec_name-��� ������
	*/
	bool CheckEcPair(Point &Q, mpz_class &d, string ec_oid);
	//�������� ���� ��� ������������� ������������
	Point pubECkey;
	//�������� ���� ��� ������������� ������������
	mpz_class prvEcKey;

	//���������� �������� ���� ����������� � ���� � ����������.
	bool CompareKeys(TBSCertificate  &tbsUser);

	//��������� ���� �������� �����������. � ������ ������� ������ ���������. ���������� false. ����� =true.
	bool CheckValidityCert(TBSCertificate cert);
};

