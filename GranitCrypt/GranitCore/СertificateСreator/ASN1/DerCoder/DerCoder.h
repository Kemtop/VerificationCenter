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
	//Вставляет байт в binaryvector
	void WriteByte(unsigned char byte);
	
	//Записывает длину блока данных
	void DerCoder::WriteLength(unsigned int length);

	//Пишет вектор в файл
	void WriteVectorToFile(std::string filename);

	//Конвертирует BigInteпeg в DER кодировку
	void IntegerToDer(mpz_class x);
	//Конвертирует дату в DER кодировку
	void toUTCTime(QDateTime dt);
	//Конвертирую строку в OBJECT_IDENTIFIER
	void to_OBJECT_IDENTIFIER(std::string Identifier);
	//Вспомогательный метод для to_OBJECT_IDENTIFIER
	//Пишет SID во временный буфер
	void WriteSID(std::vector <unsigned char> *tmp_buf,unsigned long  dig);
	//Конвертирует строку в DerIA5String
	void toIA5String(std::string str);

	//Конвертирует строку в PrintableString
	void toPrintableString(std::string str);

	//Конвертирует строку в PrintableString
	void toUTF8String(std::string str);

	//Конвертирует массив в строку Bit_String. Формирует вложенное значение OCTET STRING
	void to_Bit_String_Encapsulates(unsigned char *byte_array,unsigned long a_len);

	//Конвертирует массив в строку Octet_String в которую помещается OCTET_STRING
	/* Пример
	    OCTET STRING, encapsulates 
		 {   OCTET_STRING    d5 f2 1e 37 51 da 3d bc 3e 5d 65 33 6e 42 7e 90 
		   72 54 4f 40 
		 }
	
	*/
	void  to_Octet_String_Encapsulates_OS(unsigned char *byte_array, unsigned long a_len);


	//Конвертирует массив в строку Octet_String в которую помещается Bit_String
	/* Пример
	     OCTET STRING, encapsulates
		     {       BIT_STRING    01 c6   }
	 
	*/
	void  to_Octet_String_Encapsulates_BS(unsigned char *byte_array, unsigned long a_len);
	//Конвертирует тип bool
	void to_Bool(bool flag);
	//OCTET_STRING содержащая последовательность
	void OCTET_STRING_encapsulates_SEQUENCE(std::vector <unsigned char> *binaryvector);

	//Используется для записи открытого ключа в сертификатах
	//Помещает координаты точки эллиптической кривой x,y  в OctetString .OctetString  помещается в BitString
	void   BIT_STRING_encapsulates_EC_public_key(mpz_class Qx, mpz_class Qy);

	//Ковертирует массив в  Octet_String
	void  to_Octet_String(std::vector <unsigned char> *byte_array);

	//Ковертирует массив в  Octet_String
	void  to_Octet_String(unsigned char *byte_array, unsigned int len);

	//Ковертирует массив в  Bit_String
	void  to_Bit_String(unsigned char *byte_array,unsigned int len);
	//Задает значение NULL текущему объекту
	void setNull();

	//Координаты точки эллиптической кривой  в OctetString
	void PointToOctetString(mpz_class Qx,mpz_class Qy);

	DerCoder();
	~DerCoder();
};

