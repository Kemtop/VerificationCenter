#pragma once
#include "filters.h"
#include "files.h" 
#include "integer.h" 
#include "osrng.h"  
#include "asn.h"  
#include "oids.h" 
#include "queue.h" 

using CryptoPP::OID;
using CryptoPP::word32;
using CryptoPP::byte;
using CryptoPP::SecByteBlock;
using std::vector;


using std::string;
class PubKeyData
{
public:
	PubKeyData();
	~PubKeyData();

	string signatureAlgorithm; //Алгоритм цифровой подписи сертификата
	/*
	1.2.643.7.1.1.1.2
	Алгоритм ГОСТ Р 34.10-2012 для ключей длины 512 бит, используемый при экспорте/импорте ключей
	Групповые идентификаторы криптографических параметров алгоритмов
	*/
	string AlgoritmOID; //Идентификатор алгоритма для которого использовать открытый ключ
	//Идентификатор эллиптической кривой
	string EсOID;
	//Идентификатор функции хэширования
	string  HOid;
	//Открытый ключ
	vector<byte> PubKey;

	void SetAlgoritmOID(OID sigAlgOID); //Устанавливает идентификатор алгоритма
	//Устанавливает идентификатор кривой
	void SetEсOID(OID EcOID);
	//Устанавливает идентификатор хэш алгоритма
	void SetHashOID(OID HashOID);
	//Копирую закрытый ключ
	void SetPubKey(SecByteBlock pk);
		 
};

