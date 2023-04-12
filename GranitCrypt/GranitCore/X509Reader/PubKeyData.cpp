#include "stdafx.h"
#include "PubKeyData.h"


PubKeyData::PubKeyData()
{
}


PubKeyData::~PubKeyData()
{
}

void PubKeyData::SetAlgoritmOID(OID sigAlgOID)
{
	vector<word32> I;
	I = sigAlgOID.GetValues();
	string str;

	//Конвертирую
	for (int i = 0;i < I.size();i++)
	{
		if (i != 0) str = str + ".";
		str = str + std::to_string(I[i]);
	}

	AlgoritmOID = str;
}

void PubKeyData::SetEсOID(OID EcOID)
{
	vector<word32> I;
	I = EcOID.GetValues();
	string str;

	//Конвертирую
	for (int i = 0;i < I.size();i++)
	{
		if (i != 0) str = str + ".";
		str = str + std::to_string(I[i]);
	}

	EсOID = str;
}

void PubKeyData::SetHashOID(OID HashOID)
{
	vector<word32> I;
	I = HashOID.GetValues();
	string str;

	//Конвертирую
	for (int i = 0;i < I.size();i++)
	{
		if (i != 0) str = str + ".";
		str = str + std::to_string(I[i]);
	}

	HOid =str;
}

void PubKeyData::SetPubKey(SecByteBlock pk)
{
	//В Bit String coдержится OctetString. Дабы не колдовать -извлечем значение.
	//Так как строка содержитcя больше 127 бит появляеться дополнительный байт описывающий длину

	byte os_b1 = pk[0]; //Должно быть 04
	byte os_b2 = pk[1]; //81
	byte os_b3 = pk[2]; //80
	byte os_b4 = pk[3];

	//Копирую открытый ключ
	for (int i = 3;i < pk.size();i++)
		PubKey.push_back(pk[i]);
}
