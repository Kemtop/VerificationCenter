#include "stdafx.h"
#include "DerCoder.h"



DerCoder::DerCoder()
{
}


DerCoder::~DerCoder()
{
}

//Вставляет байт в binaryvector
void DerCoder::WriteByte(unsigned char byte)
{
	binaryvector.push_back(byte);
}


//Записывает длину блока данных
void DerCoder::WriteLength(unsigned int length)
{
	if (length > 127)
	{
		unsigned int size = 1;//сколько байт требуется для передачи длины
		unsigned int val = length;

		while ((val >>= 8) != 0)
		{
			size++;
		}

		WriteByte((unsigned char)(size | 0x80));

		for (int i = (size - 1) * 8; i >= 0; i -= 8)
		{
			WriteByte((unsigned char)(length >> i));
		}
	}
	else
	{
		WriteByte((unsigned char)length);
	}
}


//Конвертирует BigInteпeg в DER кодировку
void DerCoder::IntegerToDer(mpz_class x)
{
	int a_len = (mpz_sizeinbase(x.get_mpz_t(), 2) + 7) / 8; //Длина буфера

	//char *pbuff= (char*)malloc(sizeof(char) * a_len);
	unsigned char *buff = new unsigned char[a_len];

	size_t *count = (size_t*)malloc(sizeof(size_t));

	mpz_export((void *)buff, count, 1, sizeof(unsigned char), 1, 0, x.get_mpz_t());


	//Запись числа
	WriteByte(0x02); //Тип данных-Integer
	WriteLength(a_len); //Длина
	//Запись данных

	for (int i = 0;i < a_len;i++)
	{
		WriteByte(buff[i]);
	}

	delete[] buff;
}


void DerCoder::WriteVectorToFile(std::string filename)
{
	//Сохраняю вектор в файл
	std::ofstream file;

	file.open(filename, std::ios::out | std::ios::binary);
	file.write((const char*)&binaryvector[0], binaryvector.size());
	file.close();

}


//Конвертирует дату в DER кодировку
void DerCoder::toUTCTime(QDateTime dt)
{
	QString str = dt.toString("yyMMddhhmmss") + "Z";
	QByteArray b_array;
	b_array = str.toLatin1(); //Строку в массив

	std::string  c_str(b_array.data());

	//Запись данных 
	WriteByte(0x17); //Тип данных-UtcTime = 0x17

	int str_len = c_str.length();

	WriteLength((unsigned int)str_len); //Длина
	//Запись данных
	for (int i = 0;i < str_len;i++)
	{
		WriteByte((unsigned char)c_str[i]);
	}

}


//Конвертирую строку в OBJECT_IDENTIFIER
void DerCoder::to_OBJECT_IDENTIFIER(std::string Identifier)
{
	OidTokenizer OID = OidTokenizer(Identifier);
	//Получаю первый SID
	std::string str_SID = OID.NextToken();
	unsigned long first = std::stoul(str_SID);
	first = first * 40;

	str_SID = OID.NextToken();//Получаю последующий SID в виде строки


	//Вектор для хранения данных
	std::vector <unsigned char> tmp_buf;

	if (str_SID.length() <= 18)
	{
		first = first + std::stoul(str_SID);
		WriteSID(&tmp_buf,first);
	}
	else
	{
		throw std::overflow_error("SID ti big");
	}
		
	while (OID.HasMoreTokens()==true) //Пока в OID есть SIDы
	{
		str_SID = OID.NextToken();//Получаю последующий SID в виде строки
		first= std::stoul(str_SID);

		if (str_SID.length() <= 18)
		{
			WriteSID(&tmp_buf,first);
		}
		else
		{
			throw std::overflow_error("SID ti big");
		}
	}

	//Запись данных
	WriteByte(0x06);
	
	unsigned long b_len = (unsigned long)tmp_buf.size();

	WriteLength(b_len);
    
	for (unsigned long i = 0;i < (unsigned long)tmp_buf.size();i++)
	{
		binaryvector.push_back(tmp_buf.at(i));
	}

}


//Вспомогательный метод для to_OBJECT_IDENTIFIER
//Пишет SID во временный буфер
void  DerCoder::WriteSID(std::vector <unsigned char> *tmp_buf,unsigned long	dig)
{
	unsigned char result[9];
	unsigned char pos =0;
	while (pos < 9)
	{
		result[pos] = 0;
		pos++;
	}

	pos = 8;
	result[pos] = (unsigned char)(dig & 0x7f);
		

	while (dig >= 0x80) //Пока у числа 8й бит=1
	{
		pos--;
		dig >>= 7;
		result[pos] = (unsigned char)((dig & 0x7f) | 0x80);
	}

	while (pos<9)
	{
		tmp_buf->push_back(result[pos]);
		pos++;
	}
	
}


//Конвертирует строку в DerIA5String
void DerCoder::toIA5String(std::string str)
{
	//Запись данных 
	WriteByte(0x16); //Тип данных

	int str_len = str.length();

	WriteLength((unsigned int)str_len); //Длина
										//Запись данных
	for (int i = 0;i < str_len;i++)
	{
		WriteByte((unsigned char)str[i]);
	}
}

//Конвертирует строку в PrintableString
void DerCoder::toPrintableString(std::string str)
{
	//Запись данных 
	WriteByte(0x13); //Тип данных

	int str_len = str.length();

	WriteLength((unsigned int)str_len); //Длина
										//Запись данных
	for (int i = 0;i < str_len;i++)
	{
		WriteByte((unsigned char)str[i]);
	}
}

void DerCoder::toUTF8String(std::string str)
{
	//Запись данных 
	WriteByte(0x0c); //Тип данных

	int str_len = str.length();

	WriteLength((unsigned int)str_len); //Длина
										//Запись данных
	for (int i = 0;i < str_len;i++)
	{
		WriteByte((unsigned char)str[i]);
	}
}


//Конвертирует массив в строку Bit_String. Формирует вложенное значение OCTET STRING
void DerCoder::to_Bit_String_Encapsulates(unsigned char *byte_array, unsigned long a_len)
{
	WriteByte(0x03); //Тип данных_Bit_String

	/*
	 К длине прибавляется 3 октета. Первый-00 ноль как информационный октет Bit_String
	 далее будет следовкать тэг OCTET STRING 0x04, длина OCTET STRING.
	*/
	a_len = a_len + 3; 
	WriteLength((unsigned int)a_len);
	WriteByte(0x00);
	//Пишу OCTET STRING
	WriteByte(0x04);//Тип данных OCTET STRING
	a_len = a_len - 3;
	WriteLength((unsigned int)a_len);

	for (unsigned long i = 0;i < a_len;i++)
	{
		WriteByte(byte_array[i]);
	}

}

//Конвертирует массив в строку Octet_String в которую помещается OCTET_STRING
void  DerCoder::to_Octet_String_Encapsulates_OS(unsigned char *byte_array, unsigned long a_len)
{
	WriteByte(0x04); //Тип данных OCTET_STRING
	a_len = a_len + 2; //В блоке данных идет тип 0x04 и длина вложенного блока данных
	WriteLength((unsigned int)a_len);
	WriteByte(0x04); //Тип данных OCTET_STRING
	a_len = a_len - 2;
	WriteLength((unsigned int)a_len);
	for (unsigned long i = 0;i < a_len;i++)
	{
		WriteByte(byte_array[i]);
	}
}

//Конвертирует массив в строку Octet_String в которую помещается Bit_String
void  DerCoder::to_Octet_String_Encapsulates_BS(unsigned char *byte_array, unsigned long a_len)
{
	WriteByte(0x04); //Тип данных OCTET_STRING
	a_len = a_len + 2; //В блоке данных идет тип 0x03 и длина вложенного блока данных
	WriteLength((unsigned int)a_len);
	WriteByte(0x03); //Тип данных OCTET_STRING
	a_len = a_len - 2;
	WriteLength((unsigned int)a_len);
	for (unsigned long i = 0;i < a_len;i++)
	{
		WriteByte(byte_array[i]);
	}

}


//Конвертирует тип bool
void DerCoder::to_Bool(bool flag)
{
	WriteByte(0x01); //Тип данных BOOLEAN
	WriteLength(0x01);

	if (flag == true)
	{
		WriteByte(0xFF);
	}
	else
	{
		WriteByte(0x00);
	}

}

//OCTET_STRING содержащая последовательность
void DerCoder::OCTET_STRING_encapsulates_SEQUENCE(std::vector <unsigned char> *binaryvector)
{
	WriteByte(0x04); //Тип данных OCTET_STRING
	unsigned long len =(unsigned long) binaryvector->size();
	WriteLength(len);
	for (unsigned long i = 0;i < len;i++)
	{
		WriteByte(binaryvector->at(i));
	}

}

//Используется для записи открытого ключа в сертификатах
//Помещает координаты точки эллиптической кривой x,y  в OctetString .OctetString  помещается в BitString
void DerCoder::BIT_STRING_encapsulates_EC_public_key(mpz_class Qx, mpz_class Qy)
{

	int x_len = (mpz_sizeinbase(Qx.get_mpz_t(), 2) + 7) / 8; //Длина буфера
	int y_len = (mpz_sizeinbase(Qy.get_mpz_t(), 2) + 7) / 8; //Длина буфера

	unsigned char *Xbuff = new unsigned char[x_len];
	unsigned char *Ybuff = new unsigned char[y_len];

	size_t *count = (size_t*)malloc(sizeof(size_t));

	//-1.. 1 старший бит в перед
	mpz_export((void *)Xbuff, count, -1, sizeof(unsigned char), 1, 0, Qx.get_mpz_t());
	mpz_export((void *)Ybuff, count, -1, sizeof(unsigned char), 1, 0, Qy.get_mpz_t());

	int full_len = x_len + y_len; //Общая длина вектора двух координат

	/*
	    Cуществует неоднозначность при кодировании.
		В tk26iok приведены два примера сертификата у которых открытый ключ кодируется как:
		03 43 04 40. Где 43 длина блока.Вопрос-почему в длине присутствует лишний бит??
		В другом примере 
		03 84 04 80-длина завышена на 2 байта.
		  При анализе существующих российских сертификатов УЦ
		  в них строка кодируется как
		  "03 43 00 04 40"-УЦ Орбита 
		  "03 43 00 04 40"-Крипто Про
		  "03 43 00 04 40"-Газинформсервис-"Корневой сертификат "Головной удостоверяющий центр" "
		  Т.е видно присутствие дополнительного байта 00 и правильной длины
	*/

	WriteByte(0x03); //Тип данных_Bit_String
	//Длина Bit_String
	unsigned int bs_len = (unsigned int)full_len + 4;//3
	WriteLength(bs_len);
	WriteByte(0x00); //Пустой бит значения _Bit_String   
		
	
	//Пишу OCTET STRING
	WriteByte(0x04); //Тип данных_OCTET_String
	WriteLength((unsigned int)full_len);


	//Открытый ключ в сертификате идет задом на перед старшим битом вперед
	
	//Запись x координаты
	for (int i = 0;i < x_len;i++)
	{
		
		WriteByte(Xbuff[i]);
	}


	//Запись y координаты
	for (int i = 0;i < y_len;i++)
	{
		WriteByte(Ybuff[i]);
	}
	
	/*
	for (int i = x_len-1;i>=0 ;i--)
	{

		WriteByte(Xbuff[i]);
	}
	//Запись y координаты
	for (int i = y_len-1;i >=0;i--)
	{
		WriteByte(Ybuff[i]);
	}
	*/


	delete[] Xbuff;
	delete[] Ybuff;

}

//Координаты точки эллиптической кривой  в OctetString
void DerCoder::PointToOctetString(mpz_class Qx, mpz_class Qy)
{

	int x_len = (mpz_sizeinbase(Qx.get_mpz_t(), 2) + 7) / 8; //Длина буфера
	int y_len = (mpz_sizeinbase(Qy.get_mpz_t(), 2) + 7) / 8; //Длина буфера

	unsigned char *Xbuff = new unsigned char[x_len];
	unsigned char *Ybuff = new unsigned char[y_len];

	size_t *count = (size_t*)malloc(sizeof(size_t));


	mpz_export((void *)Xbuff, count, -1, sizeof(unsigned char), 1, 0, Qx.get_mpz_t());
	mpz_export((void *)Ybuff, count, -1, sizeof(unsigned char), 1, 0, Qy.get_mpz_t());

	int full_len = x_len + y_len; //Общая длина вектора двух координат

	//Пишу OCTET STRING
	WriteByte(0x04); //Тип данных_OCTET_String
	WriteLength((unsigned int)full_len);


	//Запись x координаты
	for (int i = 0;i < x_len;i++)
	{

		WriteByte(Xbuff[i]);
	}

	//Запись y координаты
	for (int i = 0;i < y_len;i++)
	{
		WriteByte(Ybuff[i]);
	}

	delete[] Xbuff;
	delete[] Ybuff;


}




//Ковертирует массив длины a_len в Octet_String
void  DerCoder::to_Octet_String(std::vector <unsigned char> *byte_array)
{
	WriteByte(0x04); //Тип данных_OCTET_String
	unsigned int a_len = (unsigned int)byte_array->size();

	WriteLength(a_len);

	for (unsigned long i = 0;i < a_len;i++)
	{
		binaryvector.push_back(byte_array->at(i));
	}
}

//Ковертирует массив в  Bit_String
void   DerCoder::to_Bit_String(unsigned char *byte_array, unsigned int len)
{
	WriteByte(0x03); //Тип данных_OCTET_String
	WriteLength(len+1);
	WriteByte(0x00); //Количество не используемых байт

	for (unsigned int i = 0;i < len;i++)
	{
		WriteByte(byte_array[i]);	
	}

}

//Ковертирует массив в  Octet_String
void  DerCoder::to_Octet_String(unsigned char *byte_array, unsigned int len)
{
	WriteByte(0x04); //Тип данных_OCTET_String
	
	WriteLength(len);

	for (unsigned int i = 0;i < len;i++)
	{
		WriteByte(byte_array[i]);
	}
}

//Задает значение NULL текущему объекту
void DerCoder::setNull()
{
	WriteByte(0x05); //Тип данных NULL
	WriteLength(0);

}
