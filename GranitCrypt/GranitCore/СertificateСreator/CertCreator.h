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
Удалить!
//Параметры точки на эллиптической кривой
struct CurveParam {
mpz_class p; //Модуль эллиптической кривой
mpz_class a;//Коэффициенты эллиптической кривой
mpz_class b;
mpz_class Xp; //Координаты точки
mpz_class Yp;
mpz_class q;//Порядок циклической подгруппы группы точек эллиптической кривой
};
*/

//Класс для создания сертификатов
class _declspec(dllexport) CertCreator
{

private:
	//Возвращает блок вида
	/* SET
	{
	SEQUENCE{
	OID
	Utf8 String
	}
	}
	*/
	DerSet GetIncapsulatedSetUtf8(string OID, string Value);
	//Возвращает блок вида
	/* SET
	{
	   SEQUENCE{
	             OID
	              Printable String
	           }
	}*/
	DerSet GetIncapsulatedSetPntStr(string OID, string Value);
	
	/*
	Возвращает блок вида
	SET
	{
	   SEQUENCE{
	   OID
	   IA5 String
	   }
	}*/
	DerSet GetIncapsulatedSetIA5(string OID, string Value);

	/*
	Возвращает блок вида
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
	publicKeyParamSet – идентификатор параметров открытого ключа ;
	digestParamSet – идентификатор алгоритма и параметров ;
	encryptionParamSet – идентификатор алгоритма и параметров
	*/
	DerSequence GetPublicKeyAlgorithm(string publicKeyParamSet, string digestParamSet, string encryptionParamSet);
	//Создает и возвращает расширение сертификата
	DerContextSpecific GetExtensions(TBSCertificate Tbs);

    //Закрытый ключ, для подписи сертификата
	mpz_class private_key;
	//Открытый ключ проверки подписи.
	Point PubKey;

	//Подписывает сертификат
	/*
	Согласно ГОСТ 34.10 подпись представлене двумя векторами r и s
	Пример подписи
	5e 5b 9b 80 5b 01 14 7a 84 92 c4 a1 62 64 3a c6
	15 dc 77 7b 91 74 10 8f 3d c2 76 a4 1f 98 7a f3

	e9 32 3a 5e 88 dd 87 fb 7c 72 43 83 bf fe 7c ec
	d4 b9 ff a2 ac 33 be ef 73 a5 a1 f7 43 40 4f 6b
	где вектор r
	0xE9323A5E88DD87FB7C724383BFFE7CECD4B9FFA2AC33BEEF73A5A1F743404F6B
	вектор s
	0x5E5B9B805B01147A8492C4A162643AC615DC777B9174108F3DC276A41F987AF3
	Сначала идет вектор s потом вектор r
	Возращает BitString
	*/
	DerCoder  SignRootCertificate(DerSequence tbs, Point pub_key, mpz_class prv_key); //Подписывает корневой сертификат
	 //Подписывает  сертификат
	DerCoder  SignCertificate(DerSequence tbs, Point pub_key, mpz_class prv_key);
	/*
	   Проверяет соответствие ключевых пар-закрытого ключа d,открытого Q.
	   В случае ошибки возвращает 1(не совпали x) или 2(не совпали y).Верно=0
	*/
	int CheckKeyPair(Point Q, mpz_class d, Point &P);

	//Конвертирует подпись в вектор. 
	vector<uint8_t> Sign512ToVector(Signature Sign);	

	mpz_class CRandom(mpz_class q, int KeySize);

	//Обращает порядок байт в vector. Заполняет данными src
	void CertCreator::ReverseByteOrder(vector<unsigned char>& src, vector<unsigned char>& dst);
	//Вычисляет значение ХЭШ функции ГОСТ 34.11-2012 длиной 256 для байт находящихся в векторе
	void GetHash256(vector<unsigned char>& x, unsigned char *hash); //Не используется!!!!
	//Вычисляет значение ХЭШ функции ГОСТ 34.11-2012 длиной 512 для байт находящихся в векторе
	void GetHash512(vector<unsigned char>& x, unsigned char *hash);

	//Значение последнего рассчитанного дайджеста сообщения. Значение хэш функции сообщения.  
	vector <unsigned char> lastDigest;

public:

	CertCreator();
	~CertCreator();
	//Создает сертификат CA
	void CreateCA(TBSCertificate Tbs, string save_path);

	void CreateCertificate(TBSCertificate Tbs, string save_path);
	void CreateRootCertificate(TBSCertificate Tbs, QByteArray &OutBa);
	void CreateCACertificate(TBSCertificate Tbs, QByteArray &OutBa);
	//Сохраняет сертификат пользователя по пути, а так же в массив.
	void CreateUserCertificate(TBSCertificate Tbs,QByteArray &WrBa);

	
	//Устанавливает закрытый ключ-необходимо для подписи сертификатов
	void SetPrivateKey(mpz_class u_private_key);	

	//Устанавливает ключ проверки подписи,для не самопосных сертификатов. Планируется авто проверка сертификата.
	void SetPubKey(Point Q);

	//Возвращает значение последнего рассчитанного дайджеста сообщения. Значение хэш функции сообщения.  
	vector <unsigned char> getLastDigest();
};

