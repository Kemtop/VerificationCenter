#pragma once
#include "mpirxx.h"
#include <string>
#include <QDateTime>
#include "ASN1\DerCoder\DerSequence.h"
#include "ASN1\DerCoder\DerContextSpecific.h"
#include "ASN1\DerCoder\DerSet.h"
#include "CertCreator.h"
#include "ExtensionField.h"
#include "hash3411\hash3411.h"
#include <chrono>
#include "TBSCertificate.h"
#include "CreateSign.h"
#include "VerifySign.h"

using namespace std;


/*
�������!
//��������� ����� �� ������������� ������
struct CurveParam {
mpz_class p; //������ ������������� ������
mpz_class a;//������������ ������������� ������
mpz_class b;
mpz_class Xp; //���������� �����
mpz_class Yp;
mpz_class q;//������� ����������� ��������� ������ ����� ������������� ������
};
*/

//����� ��� �������� ������������
class _declspec(dllexport) CertCreator
{

private:
	//���������� ���� ����
	/* SET
	{
	SEQUENCE{
	OID
	Utf8 String
	}
	}
	*/
	DerSet GetIncapsulatedSetUtf8(string OID, string Value);
	//���������� ���� ����
	/* SET
	{
	   SEQUENCE{
	             OID
	              Printable String
	           }
	}*/
	DerSet GetIncapsulatedSetPntStr(string OID, string Value);
	
	/*
	���������� ���� ����
	SET
	{
	   SEQUENCE{
	   OID
	   IA5 String
	   }
	}*/
	DerSet GetIncapsulatedSetIA5(string OID, string Value);

	/*
	���������� ���� ����
	SEQUENCE
	{
	OBJECT IDENTIFIER
	1.2.643.2.2.19
	SEQUENCE
	{
	OBJECT IDENTIFIER
	1.2.643.2.2.35.1
	OBJECT IDENTIFIER
	1.2.643.2.2.30.1
	}
	}
	publicKeyParamSet � ������������� ���������� ��������� ����� ;
	digestParamSet � ������������� ��������� � ���������� ;
	encryptionParamSet � ������������� ��������� � ����������
	*/
	DerSequence GetPublicKeyAlgorithm(string publicKeyParamSet, string digestParamSet, string encryptionParamSet);
	//������� � ���������� ���������� �����������
	DerContextSpecific GetExtensions(TBSCertificate Tbs);

    //�������� ����, ��� ������� �����������
	mpz_class private_key;
	//�������� ���� �������� �������.
	Point PubKey;

	//����������� ����������
	/*
	�������� ���� 34.10 ������� ������������ ����� ��������� r � s
	������ �������
	5e 5b 9b 80 5b 01 14 7a 84 92 c4 a1 62 64 3a c6
	15 dc 77 7b 91 74 10 8f 3d c2 76 a4 1f 98 7a f3

	e9 32 3a 5e 88 dd 87 fb 7c 72 43 83 bf fe 7c ec
	d4 b9 ff a2 ac 33 be ef 73 a5 a1 f7 43 40 4f 6b
	��� ������ r
	0xE9323A5E88DD87FB7C724383BFFE7CECD4B9FFA2AC33BEEF73A5A1F743404F6B
	������ s
	0x5E5B9B805B01147A8492C4A162643AC615DC777B9174108F3DC276A41F987AF3
	������� ���� ������ s ����� ������ r
	��������� BitString
	*/
	DerCoder  SignRootCertificate(DerSequence tbs, Point pub_key, mpz_class prv_key); //����������� �������� ����������
	 //�����������  ����������
	DerCoder  SignCertificate(DerSequence tbs, Point pub_key, mpz_class prv_key);
	/*
	   ��������� ������������ �������� ���-��������� ����� d,��������� Q.
	   � ������ ������ ���������� 1(�� ������� x) ��� 2(�� ������� y).�����=0
	*/
	int CheckKeyPair(Point Q, mpz_class d, Point &P);

	//������������ ������� � ������. 
	vector<uint8_t> Sign512ToVector(Signature Sign);	

	mpz_class CRandom(mpz_class q, int KeySize);

	//�������� ������� ���� � vector. ��������� ������� src
	void CertCreator::ReverseByteOrder(vector<unsigned char>& src, vector<unsigned char>& dst);
	//��������� �������� ��� ������� ���� 34.11-2012 ������ 256 ��� ���� ����������� � �������
	void GetHash256(vector<unsigned char>& x, unsigned char *hash); //�� ������������!!!!
	//��������� �������� ��� ������� ���� 34.11-2012 ������ 512 ��� ���� ����������� � �������
	void GetHash512(vector<unsigned char>& x, unsigned char *hash);

	//�������� ���������� ������������� ��������� ���������. �������� ��� ������� ���������.  
	vector <unsigned char> lastDigest;

public:

	CertCreator();
	~CertCreator();
	//������� ���������� CA
	void CreateCA(TBSCertificate Tbs, string save_path);

	void CreateCertificate(TBSCertificate Tbs, string save_path);
	void CreateRootCertificate(TBSCertificate Tbs, QByteArray &OutBa);
	void CreateCACertificate(TBSCertificate Tbs, QByteArray &OutBa);
	//��������� ���������� ������������ �� ����, � ��� �� � ������.
	void CreateUserCertificate(TBSCertificate Tbs,QByteArray &WrBa);

	
	//������������� �������� ����-���������� ��� ������� ������������
	void SetPrivateKey(mpz_class u_private_key);	

	//������������� ���� �������� �������,��� �� ���������� ������������. ����������� ���� �������� �����������.
	void SetPubKey(Point Q);

	//���������� �������� ���������� ������������� ��������� ���������. �������� ��� ������� ���������.  
	vector <unsigned char> getLastDigest();
};

