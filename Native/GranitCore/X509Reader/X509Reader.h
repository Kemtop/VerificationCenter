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

	//Читаю байт массив и преобразовываю его в TBSCertificate.
	void ReadBa(QByteArray ba, TBSCertificate &dataTbs);

	//Проверяет подпись корневого сертификата по пути на диске path.
	string CheckRootSign(QString path, TBSCertificate &dataTbs,bool &SignStatus);
	//Проверяет подпись корневого сертификата в байт массиве. 
	string CheckRootSign(QByteArray bf, TBSCertificate &dataTbs, bool &SignStatus);
	//Проверяет подпись  сертификата Удостоверяющего Центра в байт массиве. 
	string CheckCASign(QByteArray baCA, TBSCertificate &tbsCA, TBSCertificate &TbsRoot, bool &SignStatus);

	/*
	  Проверяет подпись сертификата корневого сертификата.
	  В корневом сертификате ключ проверки подписи находится в самом сертификате.
	*/
	string VerifiRootSign(TBSCertificate &tbs, bool &SignStatus);
	/*
	Проверяет подпись не корневого сертификата.
	Где:
	  tbs-данные проверяемого сертификата;
	  PubKeyTbs-данные  сертификата содержащего ключ проверки подписи(открытый ключ);
	  SignStatus-результат проверки.true-правильная подпись.
	*/
	string VerifiCertSign(TBSCertificate &tbs, TBSCertificate &PubKeyTbs, bool &SignStatus);

	//Читает данные подписи.
	bool readSignData(rfcSignedData &sD,QByteArray in);

	string getLastError();

private:
	string lastError; //Последняя возникшая ошибка.

	//Извлекает из массива содержимое сертификат в base64,если он в нем кодирован.
	QByteArray TrFromBase64(QByteArray cer);
	//Пишет массив в файл по указанному пути. Используется для отладки
	void WriteToFile(QByteArray arr,QString path);
	/*
	  Получает информацию для проверки подписи сертификата.
       tbs-последовательность tbsCertificate;
       signatureAlgorithm-OID алгоритма подписи сертификата;
       signatureValue-цифровая подпись сертификата.
	   Заполняет вектора в tbs:
	   vector<unsigned char> tbsBin;//Бинарное представление блока подлежащего подписи.
	   vector<unsigned char> signValBin; //Бинарное представление подписи сертификата.
	   Устанавливает алгоритм подписи сертификата.
	*/
	void GetBinSignInfo(QByteArray baCert, TBSCertificate &tbs);

	//Извлекает основную информацию из TBS сертификата.
	void GetTbsInfo(QByteArray baCert,TBSCertificate &Tbs);
	//Извлекает закрытый ключ длиной 512бит из SecByteBlock содержащий BitString в vector.
	vector<uint8_t> GetPubKey512(SecByteBlock pk);



	/*
	  Получает информацию для проверки подписи сертификата.
	  QByteArray ba_cert-бинарные данные сертификата
	  tbs-последовательность tbsCertificate;
	  signatureAlgorithm-OID алгоритма подписи сертификата;
	  signatureValue-цифровая подпись сертификата.

	*/
	void GetInfoForVerifi(QByteArray ba_cert, vector<unsigned char> &tbs, string &signatureAlgorithm, vector<unsigned char> &signatureValue);

	void GetPublicKeyFromCert(CryptoPP::BufferedTransformation & certin, PubKeyData &pd, TBSCertificate &Tbs);
	//Конвертирует ByteQueue в Vector
	void ByteQueueToVector(ByteQueue &Bq,vector<unsigned char> &v);
	//Конвертирует OID в String 
	string OIDtoString(OID xOID);
	//Конвертирует CryptoPP::Integer -большое число, в mpz_class большое число 
	mpz_class IntToMpz(Integer d);
	//Конвертирует CryptoPP::Integer -большое число, в Integer
	int CpIntToInt(Integer d);
	//Считывает данные издателя
	void GetIssuerData(BERSequenceDecoder &issuer, TBSCertificate &Tbs);
	void GetValidityData(BERSequenceDecoder &validity, TBSCertificate &Tbs); //Считываю период действия сертификатам
	void GetSubjectData(BERSequenceDecoder &issuer, TBSCertificate &Tbs);//Считывает данные владельца сертификата
	
																		 //Обращает порядок байт в vector. Заполняет данными src
	void ReverseByteOrder(vector<unsigned char> &src, vector<unsigned char> &dst);

	/*
	  Проверяет подпись сертификата.
	    pubkey-данные открытого ключа сертификата;
		signatureAlgorithm-OID алгоритма подписи сертификата;
		signatureValue-цифровая подпись сертификата
		УДАЛИТЬ!!!!!!!
	*/
	bool VerifiSign(PubKeyData pubkey,string signatureAlgorithm, vector<unsigned char> signatureValue,
		vector<unsigned char> &tbs, TBSCertificate &retdataTbs);

	/*
	Проверяет подпись сертификата.
	pubkey-данные открытого ключа сертификата;
	signatureAlgorithm-OID алгоритма подписи сертификата;
	signatureValue-цифровая подпись сертификата
	*/
	string VerifiSign(PubKeyData pubkey, vector<unsigned char> signatureValue,
		vector<unsigned char> &tbs, TBSCertificate &retdataTbs,bool &SignStatus);
	//На основании OID определяет длину ключа, а также проверяет допустимость используемых алгоритмов
	string GetKeyLen(PubKeyData pubkey,int &KeyLen);
	//На основании OID определяет длину ключа, а также проверяет допустимость используемых алгоритмов
	string GetKeyLen(TBSCertificate &tbs, int &KeyLen);



	//На основании OID определяет длину дайджеста, а также проверяет допустимость используемых алгоритмов
	string GetHasLen(PubKeyData pubkey, int &HashLen);
	//На основании OID определяет длину дайджеста, а также проверяет допустимость используемых алгоритмов
	string GetHasLen(TBSCertificate &tbs, int &HashLen);


	//Возвращает подпись сертификата с длиной 512байт.
	Signature Get512CerSign(vector<unsigned char> &signatureValue);

	//Возвращает открытый ключ сертификата с длиной 512байт.
	Point Get512PubKey(PubKeyData pubkey, Groups EC);
	


};

