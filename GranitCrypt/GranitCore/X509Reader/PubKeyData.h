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

	string signatureAlgorithm; //�������� �������� ������� �����������
	/*
	1.2.643.7.1.1.1.2
	�������� ���� � 34.10-2012 ��� ������ ����� 512 ���, ������������ ��� ��������/������� ������
	��������� �������������� ����������������� ���������� ����������
	*/
	string AlgoritmOID; //������������� ��������� ��� �������� ������������ �������� ����
	//������������� ������������� ������
	string E�OID;
	//������������� ������� �����������
	string  HOid;
	//�������� ����
	vector<byte> PubKey;

	void SetAlgoritmOID(OID sigAlgOID); //������������� ������������� ���������
	//������������� ������������� ������
	void SetE�OID(OID EcOID);
	//������������� ������������� ��� ���������
	void SetHashOID(OID HashOID);
	//������� �������� ����
	void SetPubKey(SecByteBlock pk);
		 
};

