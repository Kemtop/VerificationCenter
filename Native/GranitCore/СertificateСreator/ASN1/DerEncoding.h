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
#include <QTextStream> //Удалить!

using namespace std;

//Кодировка Der
class DerEncoding:public virtual Asn1Object 
{
public:
	DerEncoding();
	~DerEncoding();


	//Считывает из бинарного массива Tag

	//Получает идентификатор типа для данного блока
	 unsigned char ReadTagNumber(unsigned char tag);

	unsigned int ReadLength(bool *IS_indefinite_length_encoding);

	//Читает файл в массив
	void ReadFileToByteArray(const char *filepath);
	
	//Возвращает название  тип текущего блока
	std::string  PrintThisTagType();

	std::string GetHex();
	std::string GetHex(unsigned char byte);


	//Преобразовыыввает объект в строку, кроме SEQUENCE  SET
	std::string GetString();

	//Преобразовывает INTEGER в  строку
	std::string GetINTEGERStr();

	//Преобразовывает OBJECT IDENTIFIE в  строку
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
	//конвертирует OctetString в большое число
	mpz_class OctetStringToMpz();

	void OctetStringToQ(mpz_class *Qx, mpz_class *Qy);
};

