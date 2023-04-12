#include "DerEncoding.h"



DerEncoding::DerEncoding()
{
}


DerEncoding::~DerEncoding()
{
}

//Читает файл в массив
void DerEncoding::ReadFileToByteArray(const char *filepath)
{
	std::ifstream ifs;
	//добавить проверки
	//Читать файл в двоичном режиме,установить указатель позиции в конец файла
	ifs.open(filepath, std::ifstream::binary | std::ifstream::ate);

	//Возвращает текущую позицию в файле,т.е. его длину
	std::ifstream::pos_type pos = ifs.tellg();

	unsigned int file_lng = pos;
	char *byte_array = new  char[file_lng];

	ifs.seekg(0, std::ifstream::beg); //Устанавливает на нулевую позицию
	ifs.read(byte_array, file_lng); //Читаю

	ifs.close();

	//Копирую в буфер
	for (int i = 0;i < file_lng;i++)
	{
		ByteArray.push_back((unsigned char)byte_array[i]);
	}
	delete[]byte_array;
	//throw std::invalid_argument("DerOutputStream::ReadTagNumber=Corrupted stream - invalid high tag number found");
}

unsigned char DerEncoding::ReadTagNumber(unsigned char tag)
{
	unsigned char byte = tag;
	unsigned char  tagNo = byte & 0x1f; //Вырезаю первые 5 бит

										/*
										В случае если идентификатор типа для блока находится в диапазоне значений 0-30 идентификационный блок
										состоит только из одного октета. Если же идентификатор типа для блока имеет значение 31 и выше, то в
										битах 5-1 выставляются все 1, а в последующих октетах кодируется нужный номер.
										*/

	if (tagNo == 0x1f)
	{
		tagNo = 0;

		unsigned char b = ReadByte(); //Читаю байт в текущей позиции

									  // X.690-0207 8.1.2.4.2
									  // "c) bits 7 to 1 of the first subsequent octet shall not all be zero."
		if ((b & 0x7f) == 0) // Note: -1 will pass
		{
			throw std::invalid_argument("DerOutputStream::ReadTagNumber=Corrupted stream - invalid high tag number found");
		}

		while ((b >= 0) && ((b & 0x80) != 0))
		{
			tagNo |= (b & 0x7f);
			tagNo <<= 7;

			b = ReadByte(); //Читаю байт в текущей позиции
		}

		if (b < 0)
			throw std::invalid_argument("DerOutputStream::ReadTagNumber=EOF found inside tag value.");

		tagNo |= (b & 0x7f);
	}

	return tagNo;
}

//Определение длины блока
unsigned int DerEncoding::ReadLength(bool *IS_indefinite_length_encoding)
{
	unsigned int length = ReadByte();


	// indefinite-length encoding
	if (length == 0x80)
	{
		*IS_indefinite_length_encoding = true;
		return 0;
	}

	/*
	//Если октет длины блока >127 тогда 8 бит =1
	а биты с 7-1 кодируют сколько байт необходимо для передачи длины блока
	*/

	if (length > 127)
	{
		unsigned char size = length & 0x7f; //Сколько байт требуется для передачи длины

											// Note: The invalid long form "0xff" (see X.690 8.1.3.5c) will be caught here
		if (size > 4)
		{
			throw std::invalid_argument("DerOutputStream::ReadLength=DER length more than 4 bytes: " + std::to_string(size));
			return 0;
		}

		length = 0;
		unsigned char next;
		for (int i = 0; i < size; i++)
		{
			next = ReadByte();
			length = (length << 8) + next;
		}

	}

	return length;

}


//Печатает тип текущего блока
std::string  DerEncoding::PrintThisTagType()
{
	std::string Name;
	//класс блока UNIVERSAL;
	if (ClassBlock == 0x00)
	{
		if (IsConstructedBlock == true)
		{
			//Тип данных в блоке
			switch (BlockType)
			{
			case 0x10: Name = "SEQUENCE";  break;
			case 0x11: Name = "SET"; break;

			default: Name = "Undetermined ConstructedBlock "; break;
			}
		}
		else
		{
			switch (BlockType)
			{
			case 0x01: Name = "BOOLEAN"; break;
			case 0x02: Name = "INTEGER"; break;
			case 0x03: Name = "BIT STRING"; break;
			case 0x04: Name = "OCTETSTRING"; break;
			case 0x05: Name = "NULL"; break;
			case 0x06: Name = "OBJECT IDENTIFIER"; break;
			case 0x16: Name = "IA5 STRING"; break;
			case 0x17: Name = "UTCTime"; break;
			case 0x13: Name = "PrintableString"; break;
				
			
			default:
				Name = "Other BlockType=" + std::to_string((int)BlockType);
				break;
			}

		}

	}
	else
	{
		if(ClassBlock == 0x80) Name = "[" + std::to_string(BlockType) + "]";
		else	
		Name = "UNIVERSAL_class only available"+ std::to_string(BlockType);
	}
	/*

	switch (Tag)
	{
	case 0x30: Name = "SEQUENCE";  break;
	case 0x31: Name = "SET"; break;
	case 0x02: Name = "INTEGER"; break;
	case 0x03: Name = "BIT STRING"; break;
	case 0x05: Name = "NULL"; break;
	case 0x06: Name = "OBJECT IDENTIFIER"; break;
	case 0x17: Name = "UTCTime"; break;
	case 0x13: Name = "PrintableString"; break;

	default:
		Name = "Other tag=" + std::to_string((int)Tag);
		break;
	}
	*/
	return Name;
}


std::string DerEncoding::GetString()
{

	string str = "";
		

	switch (Tag)
	{

	case 0x03: return	GetBIT_STRING();  break;
	case 0x30: return str;  break;
	case 0x31: return str; break;
	case 0x01: return GetBoolean(); break;
		//INTEGER
	case 0x02: return GetHex(); break;
		//OCTET STRING
	case 0x04: return GetHex(); break;
		//OBJECT_IDENTIFIE
	case 0x06: return GetOBJECT_IDENTIFIEStr(); break;
		//PrintableString
	case 0x13: return "'" + GetPrintableStringStr() + "'"; break;
		//IA5 STRING
	case 0x16: return  "'" + GetIA5_STRING() +"'"  ; break;
		//UTCTime
	case 0x17: return GetPrintableStringStr(); break;




	default:
		return str;
		break;
	}


}


std::string DerEncoding::GetHex()
{
	constexpr char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	unsigned int len = ObjLength;
	std::string s(ObjLength * 3, ' ');

	unsigned char byte;

	for (unsigned int i = 0; i < len; ++i)
	{
		byte = ByteArray.at(i);
		s[2 * i] = hexmap[(byte & 0xF0) >> 4];
		s[2 * i + 1] = hexmap[byte & 0x0F];
	}
	return s;
}


std::string DerEncoding::GetHex(unsigned char byte)
{
	constexpr char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	std::string s(4, ' ');

	s[0] = '0';
	s[1] = 'x';
	s[2] = hexmap[(byte & 0xF0) >> 4];
	s[3] = hexmap[byte & 0x0F];

	return s;
}


mpz_class DerEncoding::OctetStringToMpz()
{
	mpz_class U;

	unsigned int len = ObjLength;
	unsigned char *buff = new unsigned char[len];

	for (unsigned int i = 0; i < len; i++)
	{
		buff[i] = ByteArray.at(i);
	}
			
	size_t l = (size_t)len;
	//Преобразование массива в большое число
	mpz_import(U.get_mpz_t(), l, -1, sizeof(buff[0]), 0, 0, buff);

	delete [] buff;
	
	return U;
}

void DerEncoding::OctetStringToQ(mpz_class *Qx, mpz_class *Qy)
{

	unsigned int len = ObjLength/2;
	unsigned char Xbuff[32]; //= new unsigned char[len];
		unsigned char Ybuff[32];// = new unsigned char[len];

	for (unsigned int i = 0; i < len; i++)
	{
		Xbuff[i] = ByteArray.at(i);
	}

	for (unsigned int i = len; i < ObjLength; i++)
	{
		Ybuff[i] = ByteArray.at(i);
	}

	size_t l1 = (size_t)len;
	size_t l2 = (size_t)len;
	//Преобразование массива в большое число

	mpz_import(Qx->get_mpz_t(), l1, -1, sizeof(Xbuff[0]), 0, 0, Xbuff);
	mpz_import(Qy->get_mpz_t(), l2, -1, sizeof(Ybuff[0]), 0, 0, Ybuff);

	//delete[] Xbuff;
	//delete[] Ybuff;
		
}


//Преобразовывает INTEGER в  строку
std::string DerEncoding::GetINTEGERStr()
{
	string g;
	return g;
}

//Преобразовывает OBJECT IDENTIFIE в  строку
std::string DerEncoding::GetOBJECT_IDENTIFIEStr()
{
	unsigned long long value = 0;
	bool			first = true; //Первый SID 
	string Oid;

	// -9 223 372 036 854 775 808 до 9 223 372 036 854 775 807

	for (int i = 0; i != (int)ByteArray.size(); i++)
	{
		unsigned char b = ByteArray.at(i);

		//private const long LONG_LIMIT = (long.MaxValue >> 7) - 0x7f;

		long LONG_LIMIT = 0xffffffff;

		if (value <= LONG_LIMIT)
		{
			value += (b & 0x7f);

			if ((b & 0x80) == 0)             // end of number reached
			{
				if (first)
				{
					if (value < 40)
					{
						Oid.append("0");
					}
					else if (value < 80)
					{
						Oid.append("1");
						value -= 40;
					}
					else
					{
						Oid.append("2");
						value -= 80;
					}
					first = false;
				}

				Oid.append(".");
				Oid.append(std::to_string(value));

				value = 0;
			}
			else
			{
				value <<= 7;
			}
		}
		else
		{
			throw std::invalid_argument("Oid Big value ne realizovano.");
			/*
			if (bigValue == null)
			{
			bigValue = BigInteger.ValueOf(value);
			}
			bigValue = bigValue.Or(BigInteger.ValueOf(b & 0x7f));
			if ((b & 0x80) == 0)
			{
			if (first)
			{
			objId.Append('2');
			bigValue = bigValue.Subtract(BigInteger.ValueOf(80));
			first = false;
			}

			objId.Append('.');
			objId.Append(bigValue);
			bigValue = null;
			value = 0;
			}
			else
			{
			bigValue = bigValue.ShiftLeft(7);
			}
			*/
		}
	}

	return Oid;
}

//PrintableString
std::string DerEncoding::GetPrintableStringStr()
{

	unsigned int len = ObjLength;//(int)ByteArray.size();

	char * str = new char[len + 1];
	str[len] = '\0';

	//Копирую в масссив
	for (unsigned int i = 0; i < len; i++)
	{
		str[i] = (char)ByteArray.at(i);
	}

	std::string s(str);
	delete[] str;

	return s;
}


//BIT STRING
std::string DerEncoding::GetBIT_STRING()
{
	 char table[]= { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	 unsigned char ubyte;
	 string buffer;

	for (unsigned int i = 0; i != ObjLength; i++)
	{
		ubyte = ByteArray.at(i);
		buffer.append(1,table[(ubyte >> 4) & 0xf]);
		buffer.append(1,table[ubyte & 0xf]);
	}

	return buffer;
}

//IA5 STRING
std::string  DerEncoding::GetIA5_STRING()
{
	unsigned int len = ObjLength;//(int)ByteArray.size();


	char * str = new char[len + 1];
	str[len] = '\0';

	//Копирую в масссив
	for (unsigned int i = 0; i < len; i++)
	{
		str[i] = (char)ByteArray.at(i);
	}

	std::string s(str);
	delete[] str;
	
	return s;

}

//BOOLEAN
std::string DerEncoding::GetBoolean()
{
	unsigned char b= ByteArray.at(0);
	if (b == 0x00) return "FALSE";
	else return "TRUE";

}