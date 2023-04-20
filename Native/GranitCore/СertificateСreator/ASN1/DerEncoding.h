#pragma once
#include "mpirxx.h"
#include "Asn1Object.h"
#include <vector>
#include <stdio.h>
#include <string> 
#include <iostream>
#include <fstream>
//#include "Asn1Tags.h"
//#include "DerInteger.h"
//#include "DerObjectIdentifier.h"
//#include "DerPrintableString.h"
#include "DerEncoding.h"
#include "Asn1Tags.h"
#include <QTextStream> //�������!

using namespace std;

//��������� Der
class DerEncoding:public virtual Asn1Object 
{
public:
	DerEncoding();
	~DerEncoding();


	//��������� �� ��������� ������� Tag

	//�������� ������������� ���� ��� ������� �����
	 unsigned char ReadTagNumber(unsigned char tag);

	unsigned int ReadLength(bool *IS_indefinite_length_encoding);

	//������ ���� � ������
	void ReadFileToByteArray(const char *filepath);
	
	//���������� ��������  ��� �������� �����
	std::string  PrintThisTagType();

	std::string GetHex();
	std::string GetHex(unsigned char byte);


	//����������������� ������ � ������, ����� SEQUENCE  SET
	std::string GetString();

	//��������������� INTEGER �  ������
	std::string GetINTEGERStr();

	//��������������� OBJECT IDENTIFIE �  ������
	std::string GetOBJECT_IDENTIFIEStr();
	//PrintableString
	std::string GetPrintableStringStr();
	//UtcTime
	std::string GetUTCTimeStr();
	//BIT STRING
	std::string GetBIT_STRING();
	//IA5 STRING
	std::string GetIA5_STRING();
	//BOOLEAN
	std::string GetBoolean();
	//������������ OctetString � ������� �����
	mpz_class OctetStringToMpz();

	void OctetStringToQ(mpz_class *Qx, mpz_class *Qy);
};

