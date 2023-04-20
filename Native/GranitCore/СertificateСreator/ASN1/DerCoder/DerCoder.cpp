#include "stdafx.h"
#include "DerCoder.h"



DerCoder::DerCoder()
{
}


DerCoder::~DerCoder()
{
}

//��������� ���� � binaryvector
void DerCoder::WriteByte(unsigned char byte)
{
	binaryvector.push_back(byte);
}


//���������� ����� ����� ������
void DerCoder::WriteLength(unsigned int length)
{
	if (length > 127)
	{
		unsigned int size = 1;//������� ���� ��������� ��� �������� �����
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


//������������ BigInte�eg � DER ���������
void DerCoder::IntegerToDer(mpz_class x)
{
	int a_len = (mpz_sizeinbase(x.get_mpz_t(), 2) + 7) / 8; //����� ������

	//char *pbuff= (char*)malloc(sizeof(char) * a_len);
	unsigned char *buff = new unsigned char[a_len];

	size_t *count = (size_t*)malloc(sizeof(size_t));

	mpz_export((void *)buff, count, 1, sizeof(unsigned char), 1, 0, x.get_mpz_t());


	//������ �����
	WriteByte(0x02); //��� ������-Integer
	WriteLength(a_len); //�����
	//������ ������

	for (int i = 0;i < a_len;i++)
	{
		WriteByte(buff[i]);
	}

	delete[] buff;
}


void DerCoder::WriteVectorToFile(std::string filename)
{
	//�������� ������ � ����
	std::ofstream file;

	file.open(filename, std::ios::out | std::ios::binary);
	file.write((const char*)&binaryvector[0], binaryvector.size());
	file.close();

}


//������������ ���� � DER ���������
void DerCoder::toUTCTime(QDateTime dt)
{
	QString str = dt.toString("yyMMddhhmmss") + "Z";
	QByteArray b_array;
	b_array = str.toLatin1(); //������ � ������

	std::string  c_str(b_array.data());

	//������ ������ 
	WriteByte(0x17); //��� ������-UtcTime = 0x17

	int str_len = c_str.length();

	WriteLength((unsigned int)str_len); //�����
	//������ ������
	for (int i = 0;i < str_len;i++)
	{
		WriteByte((unsigned char)c_str[i]);
	}

}


//����������� ������ � OBJECT_IDENTIFIER
void DerCoder::to_OBJECT_IDENTIFIER(std::string Identifier)
{
	OidTokenizer OID = OidTokenizer(Identifier);
	//������� ������ SID
	std::string str_SID = OID.NextToken();
	unsigned long first = std::stoul(str_SID);
	first = first * 40;

	str_SID = OID.NextToken();//������� ����������� SID � ���� ������


	//������ ��� �������� ������
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
		
	while (OID.HasMoreTokens()==true) //���� � OID ���� SID�
	{
		str_SID = OID.NextToken();//������� ����������� SID � ���� ������
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

	//������ ������
	WriteByte(0x06);
	
	unsigned long b_len = (unsigned long)tmp_buf.size();

	WriteLength(b_len);
    
	for (unsigned long i = 0;i < (unsigned long)tmp_buf.size();i++)
	{
		binaryvector.push_back(tmp_buf.at(i));
	}

}


//��������������� ����� ��� to_OBJECT_IDENTIFIER
//����� SID �� ��������� �����
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
		

	while (dig >= 0x80) //���� � ����� 8� ���=1
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


//������������ ������ � DerIA5String
void DerCoder::toIA5String(std::string str)
{
	//������ ������ 
	WriteByte(0x16); //��� ������

	int str_len = str.length();

	WriteLength((unsigned int)str_len); //�����
										//������ ������
	for (int i = 0;i < str_len;i++)
	{
		WriteByte((unsigned char)str[i]);
	}
}

//������������ ������ � PrintableString
void DerCoder::toPrintableString(std::string str)
{
	//������ ������ 
	WriteByte(0x13); //��� ������

	int str_len = str.length();

	WriteLength((unsigned int)str_len); //�����
										//������ ������
	for (int i = 0;i < str_len;i++)
	{
		WriteByte((unsigned char)str[i]);
	}
}

void DerCoder::toUTF8String(std::string str)
{
	//������ ������ 
	WriteByte(0x0c); //��� ������

	int str_len = str.length();

	WriteLength((unsigned int)str_len); //�����
										//������ ������
	for (int i = 0;i < str_len;i++)
	{
		WriteByte((unsigned char)str[i]);
	}
}


//������������ ������ � ������ Bit_String. ��������� ��������� �������� OCTET STRING
void DerCoder::to_Bit_String_Encapsulates(unsigned char *byte_array, unsigned long a_len)
{
	WriteByte(0x03); //��� ������_Bit_String

	/*
	 � ����� ������������ 3 ������. ������-00 ���� ��� �������������� ����� Bit_String
	 ����� ����� ���������� ��� OCTET STRING 0x04, ����� OCTET STRING.
	*/
	a_len = a_len + 3; 
	WriteLength((unsigned int)a_len);
	WriteByte(0x00);
	//���� OCTET STRING
	WriteByte(0x04);//��� ������ OCTET STRING
	a_len = a_len - 3;
	WriteLength((unsigned int)a_len);

	for (unsigned long i = 0;i < a_len;i++)
	{
		WriteByte(byte_array[i]);
	}

}

//������������ ������ � ������ Octet_String � ������� ���������� OCTET_STRING
void  DerCoder::to_Octet_String_Encapsulates_OS(unsigned char *byte_array, unsigned long a_len)
{
	WriteByte(0x04); //��� ������ OCTET_STRING
	a_len = a_len + 2; //� ����� ������ ���� ��� 0x04 � ����� ���������� ����� ������
	WriteLength((unsigned int)a_len);
	WriteByte(0x04); //��� ������ OCTET_STRING
	a_len = a_len - 2;
	WriteLength((unsigned int)a_len);
	for (unsigned long i = 0;i < a_len;i++)
	{
		WriteByte(byte_array[i]);
	}
}

//������������ ������ � ������ Octet_String � ������� ���������� Bit_String
void  DerCoder::to_Octet_String_Encapsulates_BS(unsigned char *byte_array, unsigned long a_len)
{
	WriteByte(0x04); //��� ������ OCTET_STRING
	a_len = a_len + 2; //� ����� ������ ���� ��� 0x03 � ����� ���������� ����� ������
	WriteLength((unsigned int)a_len);
	WriteByte(0x03); //��� ������ OCTET_STRING
	a_len = a_len - 2;
	WriteLength((unsigned int)a_len);
	for (unsigned long i = 0;i < a_len;i++)
	{
		WriteByte(byte_array[i]);
	}

}


//������������ ��� bool
void DerCoder::to_Bool(bool flag)
{
	WriteByte(0x01); //��� ������ BOOLEAN
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

//OCTET_STRING ���������� ������������������
void DerCoder::OCTET_STRING_encapsulates_SEQUENCE(std::vector <unsigned char> *binaryvector)
{
	WriteByte(0x04); //��� ������ OCTET_STRING
	unsigned long len =(unsigned long) binaryvector->size();
	WriteLength(len);
	for (unsigned long i = 0;i < len;i++)
	{
		WriteByte(binaryvector->at(i));
	}

}

//������������ ��� ������ ��������� ����� � ������������
//�������� ���������� ����� ������������� ������ x,y  � OctetString .OctetString  ���������� � BitString
void DerCoder::BIT_STRING_encapsulates_EC_public_key(mpz_class Qx, mpz_class Qy)
{

	int x_len = (mpz_sizeinbase(Qx.get_mpz_t(), 2) + 7) / 8; //����� ������
	int y_len = (mpz_sizeinbase(Qy.get_mpz_t(), 2) + 7) / 8; //����� ������

	unsigned char *Xbuff = new unsigned char[x_len];
	unsigned char *Ybuff = new unsigned char[y_len];

	size_t *count = (size_t*)malloc(sizeof(size_t));

	//-1.. 1 ������� ��� � �����
	mpz_export((void *)Xbuff, count, -1, sizeof(unsigned char), 1, 0, Qx.get_mpz_t());
	mpz_export((void *)Ybuff, count, -1, sizeof(unsigned char), 1, 0, Qy.get_mpz_t());

	int full_len = x_len + y_len; //����� ����� ������� ���� ���������

	/*
	    C��������� ��������������� ��� �����������.
		� tk26iok ��������� ��� ������� ����������� � ������� �������� ���� ���������� ���:
		03 43 04 40. ��� 43 ����� �����.������-������ � ����� ������������ ������ ���??
		� ������ ������� 
		03 84 04 80-����� �������� �� 2 �����.
		  ��� ������� ������������ ���������� ������������ ��
		  � ��� ������ ���������� ���
		  "03 43 00 04 40"-�� ������ 
		  "03 43 00 04 40"-������ ���
		  "03 43 00 04 40"-���������������-"�������� ���������� "�������� �������������� �����" "
		  �.� ����� ����������� ��������������� ����� 00 � ���������� �����
	*/

	WriteByte(0x03); //��� ������_Bit_String
	//����� Bit_String
	unsigned int bs_len = (unsigned int)full_len + 4;//3
	WriteLength(bs_len);
	WriteByte(0x00); //������ ��� �������� _Bit_String   
		
	
	//���� OCTET STRING
	WriteByte(0x04); //��� ������_OCTET_String
	WriteLength((unsigned int)full_len);


	//�������� ���� � ����������� ���� ����� �� ����� ������� ����� ������
	
	//������ x ����������
	for (int i = 0;i < x_len;i++)
	{
		
		WriteByte(Xbuff[i]);
	}


	//������ y ����������
	for (int i = 0;i < y_len;i++)
	{
		WriteByte(Ybuff[i]);
	}
	
	/*
	for (int i = x_len-1;i>=0 ;i--)
	{

		WriteByte(Xbuff[i]);
	}
	//������ y ����������
	for (int i = y_len-1;i >=0;i--)
	{
		WriteByte(Ybuff[i]);
	}
	*/


	delete[] Xbuff;
	delete[] Ybuff;

}

//���������� ����� ������������� ������  � OctetString
void DerCoder::PointToOctetString(mpz_class Qx, mpz_class Qy)
{

	int x_len = (mpz_sizeinbase(Qx.get_mpz_t(), 2) + 7) / 8; //����� ������
	int y_len = (mpz_sizeinbase(Qy.get_mpz_t(), 2) + 7) / 8; //����� ������

	unsigned char *Xbuff = new unsigned char[x_len];
	unsigned char *Ybuff = new unsigned char[y_len];

	size_t *count = (size_t*)malloc(sizeof(size_t));


	mpz_export((void *)Xbuff, count, -1, sizeof(unsigned char), 1, 0, Qx.get_mpz_t());
	mpz_export((void *)Ybuff, count, -1, sizeof(unsigned char), 1, 0, Qy.get_mpz_t());

	int full_len = x_len + y_len; //����� ����� ������� ���� ���������

	//���� OCTET STRING
	WriteByte(0x04); //��� ������_OCTET_String
	WriteLength((unsigned int)full_len);


	//������ x ����������
	for (int i = 0;i < x_len;i++)
	{

		WriteByte(Xbuff[i]);
	}

	//������ y ����������
	for (int i = 0;i < y_len;i++)
	{
		WriteByte(Ybuff[i]);
	}

	delete[] Xbuff;
	delete[] Ybuff;


}




//����������� ������ ����� a_len � Octet_String
void  DerCoder::to_Octet_String(std::vector <unsigned char> *byte_array)
{
	WriteByte(0x04); //��� ������_OCTET_String
	unsigned int a_len = (unsigned int)byte_array->size();

	WriteLength(a_len);

	for (unsigned long i = 0;i < a_len;i++)
	{
		binaryvector.push_back(byte_array->at(i));
	}
}

//����������� ������ �  Bit_String
void   DerCoder::to_Bit_String(unsigned char *byte_array, unsigned int len)
{
	WriteByte(0x03); //��� ������_OCTET_String
	WriteLength(len+1);
	WriteByte(0x00); //���������� �� ������������ ����

	for (unsigned int i = 0;i < len;i++)
	{
		WriteByte(byte_array[i]);	
	}

}

//����������� ������ �  Octet_String
void  DerCoder::to_Octet_String(unsigned char *byte_array, unsigned int len)
{
	WriteByte(0x04); //��� ������_OCTET_String
	
	WriteLength(len);

	for (unsigned int i = 0;i < len;i++)
	{
		WriteByte(byte_array[i]);
	}
}

//������ �������� NULL �������� �������
void DerCoder::setNull()
{
	WriteByte(0x05); //��� ������ NULL
	WriteLength(0);

}
