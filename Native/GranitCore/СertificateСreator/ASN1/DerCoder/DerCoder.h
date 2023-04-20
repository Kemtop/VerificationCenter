#pragma once
#include <vector>
#include "mpirxx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <QDateTime>
#include "ASN1\DerCoder\OidTokenizer.h"

class DerCoder
{
public:
	std::vector <unsigned char> binaryvector;
	//��������� ���� � binaryvector
	void WriteByte(unsigned char byte);
	
	//���������� ����� ����� ������
	void DerCoder::WriteLength(unsigned int length);

	//����� ������ � ����
	void WriteVectorToFile(std::string filename);

	//������������ BigInte�eg � DER ���������
	void IntegerToDer(mpz_class x);
	//������������ ���� � DER ���������
	void toUTCTime(QDateTime dt);
	//����������� ������ � OBJECT_IDENTIFIER
	void to_OBJECT_IDENTIFIER(std::string Identifier);
	//��������������� ����� ��� to_OBJECT_IDENTIFIER
	//����� SID �� ��������� �����
	void WriteSID(std::vector <unsigned char> *tmp_buf,unsigned long  dig);
	//������������ ������ � DerIA5String
	void toIA5String(std::string str);

	//������������ ������ � PrintableString
	void toPrintableString(std::string str);

	//������������ ������ � PrintableString
	void toUTF8String(std::string str);

	//������������ ������ � ������ Bit_String. ��������� ��������� �������� OCTET STRING
	void to_Bit_String_Encapsulates(unsigned char *byte_array,unsigned long a_len);

	//������������ ������ � ������ Octet_String � ������� ���������� OCTET_STRING
	/* ������
	    OCTET STRING, encapsulates 
		 {   OCTET_STRING    d5 f2 1e 37 51 da 3d bc 3e 5d 65 33 6e 42 7e 90 
		   72 54 4f 40 
		 }
	
	*/
	void  to_Octet_String_Encapsulates_OS(unsigned char *byte_array, unsigned long a_len);


	//������������ ������ � ������ Octet_String � ������� ���������� Bit_String
	/* ������
	     OCTET STRING, encapsulates
		     {       BIT_STRING    01 c6   }
	 
	*/
	void  to_Octet_String_Encapsulates_BS(unsigned char *byte_array, unsigned long a_len);
	//������������ ��� bool
	void to_Bool(bool flag);
	//OCTET_STRING ���������� ������������������
	void OCTET_STRING_encapsulates_SEQUENCE(std::vector <unsigned char> *binaryvector);

	//������������ ��� ������ ��������� ����� � ������������
	//�������� ���������� ����� ������������� ������ x,y  � OctetString .OctetString  ���������� � BitString
	void   BIT_STRING_encapsulates_EC_public_key(mpz_class Qx, mpz_class Qy);

	//����������� ������ �  Octet_String
	void  to_Octet_String(std::vector <unsigned char> *byte_array);

	//����������� ������ �  Octet_String
	void  to_Octet_String(unsigned char *byte_array, unsigned int len);

	//����������� ������ �  Bit_String
	void  to_Bit_String(unsigned char *byte_array,unsigned int len);
	//������ �������� NULL �������� �������
	void setNull();

	//���������� ����� ������������� ������  � OctetString
	void PointToOctetString(mpz_class Qx,mpz_class Qy);

	DerCoder();
	~DerCoder();
};

