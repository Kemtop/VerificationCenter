#pragma once
#include "filters.h"
#include "files.h" 
#include "integer.h" 
#include "osrng.h"  
#include "asn.h"  
#include "oids.h" 
#include "queue.h" 
#include "PubKeyData.h"
#include "mpirxx.h"
#include "Signature.h"
#include "Groups.h"
#include "Point.h"
#include "VerifySign.h"
#include "TBSCertificate.h"
#include <QDateTime>
#include <QFile>
#include "AlgOidName.h"
#include "SingleSign\rfcSignedData.h"
#include "windows.h"
#include <QTextCodec>

//#include "CryptLib\cryptlib.h"
using CryptoPP::Exception;
using CryptoPP::StringSource;
using CryptoPP::StringSink;
using CryptoPP::FileSink;
using CryptoPP::Integer;
using CryptoPP::AutoSeededRandomPool;
namespace ASN1 = CryptoPP::ASN1;
using CryptoPP::DERSequenceEncoder;
using CryptoPP::BERSequenceDecoder;
using CryptoPP::DEREncodeTextString;
using CryptoPP::ByteQueue;
using CryptoPP::OID;
using CryptoPP::SecByteBlock;
using CryptoPP::BERGeneralDecoder;
using CryptoPP::BERDecodeUnsigned;
using CryptoPP::BERDecodeOctetString;
using CryptoPP::BERDecodeBitString;
using CryptoPP::BERSetDecoder;
using CryptoPP::word32;
using CryptoPP::INTEGER;
using CryptoPP::BufferedTransformation;
using std::vector;



class X509Reader
{
public:
	X509Reader();
	~X509Reader();
	string Read(QString path, TBSCertificate &dataTbs);

	//����� ���� ������ � �������������� ��� � TBSCertificate.
	void ReadBa(QByteArray ba, TBSCertificate &dataTbs);

	//��������� ������� ��������� ����������� �� ���� �� ����� path.
	string CheckRootSign(QString path, TBSCertificate &dataTbs,bool &SignStatus);
	//��������� ������� ��������� ����������� � ���� �������. 
	string CheckRootSign(QByteArray bf, TBSCertificate &dataTbs, bool &SignStatus);
	//��������� �������  ����������� ��������������� ������ � ���� �������. 
	string CheckCASign(QByteArray baCA, TBSCertificate &tbsCA, TBSCertificate &TbsRoot, bool &SignStatus);

	/*
	  ��������� ������� ����������� ��������� �����������.
	  � �������� ����������� ���� �������� ������� ��������� � ����� �����������.
	*/
	string VerifiRootSign(TBSCertificate &tbs, bool &SignStatus);
	/*
	��������� ������� �� ��������� �����������.
	���:
	  tbs-������ ������������ �����������;
	  PubKeyTbs-������  ����������� ����������� ���� �������� �������(�������� ����);
	  SignStatus-��������� ��������.true-���������� �������.
	*/
	string VerifiCertSign(TBSCertificate &tbs, TBSCertificate &PubKeyTbs, bool &SignStatus);

	//������ ������ �������.
	bool readSignData(rfcSignedData &sD,QByteArray in);

	string getLastError();

private:
	string lastError; //��������� ��������� ������.

	//��������� �� ������� ���������� ���������� � base64,���� �� � ��� ���������.
	QByteArray TrFromBase64(QByteArray cer);
	//����� ������ � ���� �� ���������� ����. ������������ ��� �������
	void WriteToFile(QByteArray arr,QString path);
	/*
	  �������� ���������� ��� �������� ������� �����������.
       tbs-������������������ tbsCertificate;
       signatureAlgorithm-OID ��������� ������� �����������;
       signatureValue-�������� ������� �����������.
	   ��������� ������� � tbs:
	   vector<unsigned char> tbsBin;//�������� ������������� ����� ����������� �������.
	   vector<unsigned char> signValBin; //�������� ������������� ������� �����������.
	   ������������� �������� ������� �����������.
	*/
	void GetBinSignInfo(QByteArray baCert, TBSCertificate &tbs);

	//��������� �������� ���������� �� TBS �����������.
	void GetTbsInfo(QByteArray baCert,TBSCertificate &Tbs);
	//��������� �������� ���� ������ 512��� �� SecByteBlock ���������� BitString � vector.
	vector<uint8_t> GetPubKey512(SecByteBlock pk);



	/*
	  �������� ���������� ��� �������� ������� �����������.
	  QByteArray ba_cert-�������� ������ �����������
	  tbs-������������������ tbsCertificate;
	  signatureAlgorithm-OID ��������� ������� �����������;
	  signatureValue-�������� ������� �����������.

	*/
	void GetInfoForVerifi(QByteArray ba_cert, vector<unsigned char> &tbs, string &signatureAlgorithm, vector<unsigned char> &signatureValue);

	void GetPublicKeyFromCert(CryptoPP::BufferedTransformation & certin, PubKeyData &pd, TBSCertificate &Tbs);
	//������������ ByteQueue � Vector
	void ByteQueueToVector(ByteQueue &Bq,vector<unsigned char> &v);
	//������������ OID � String 
	string OIDtoString(OID xOID);
	//������������ CryptoPP::Integer -������� �����, � mpz_class ������� ����� 
	mpz_class IntToMpz(Integer d);
	//������������ CryptoPP::Integer -������� �����, � Integer
	int CpIntToInt(Integer d);
	//��������� ������ ��������
	void GetIssuerData(BERSequenceDecoder &issuer, TBSCertificate &Tbs);
	void GetValidityData(BERSequenceDecoder &validity, TBSCertificate &Tbs); //�������� ������ �������� ������������
	void GetSubjectData(BERSequenceDecoder &issuer, TBSCertificate &Tbs);//��������� ������ ��������� �����������
	
																		 //�������� ������� ���� � vector. ��������� ������� src
	void ReverseByteOrder(vector<unsigned char> &src, vector<unsigned char> &dst);

	/*
	  ��������� ������� �����������.
	    pubkey-������ ��������� ����� �����������;
		signatureAlgorithm-OID ��������� ������� �����������;
		signatureValue-�������� ������� �����������
		�������!!!!!!!
	*/
	bool VerifiSign(PubKeyData pubkey,string signatureAlgorithm, vector<unsigned char> signatureValue,
		vector<unsigned char> &tbs, TBSCertificate &retdataTbs);

	/*
	��������� ������� �����������.
	pubkey-������ ��������� ����� �����������;
	signatureAlgorithm-OID ��������� ������� �����������;
	signatureValue-�������� ������� �����������
	*/
	string VerifiSign(PubKeyData pubkey, vector<unsigned char> signatureValue,
		vector<unsigned char> &tbs, TBSCertificate &retdataTbs,bool &SignStatus);
	//�� ��������� OID ���������� ����� �����, � ����� ��������� ������������ ������������ ����������
	string GetKeyLen(PubKeyData pubkey,int &KeyLen);
	//�� ��������� OID ���������� ����� �����, � ����� ��������� ������������ ������������ ����������
	string GetKeyLen(TBSCertificate &tbs, int &KeyLen);



	//�� ��������� OID ���������� ����� ���������, � ����� ��������� ������������ ������������ ����������
	string GetHasLen(PubKeyData pubkey, int &HashLen);
	//�� ��������� OID ���������� ����� ���������, � ����� ��������� ������������ ������������ ����������
	string GetHasLen(TBSCertificate &tbs, int &HashLen);


	//���������� ������� ����������� � ������ 512����.
	Signature Get512CerSign(vector<unsigned char> &signatureValue);

	//���������� �������� ���� ����������� � ������ 512����.
	Point Get512PubKey(PubKeyData pubkey, Groups EC);
	


};

