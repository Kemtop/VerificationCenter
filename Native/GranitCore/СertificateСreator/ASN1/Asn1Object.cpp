#include "Asn1Object.h"



Asn1Object::Asn1Object()
{
	Array_Pos = 0;
}


Asn1Object::~Asn1Object()
{
}

//����� ���� � ByteArray
void Asn1Object::WriteByte(unsigned char byte)
{
	ByteArray.push_back(byte);
}

void Asn1Object::CopyFromMainBuff(unsigned int start_pos, unsigned int end_pos, std::vector<unsigned char> *DerBuffer)
{
	unsigned char byte;
	while (start_pos < end_pos)
	{	
		//����� ���� �� �������� �������
		byte=DerBuffer->at(start_pos);
		ByteArray.push_back(byte);
		
		start_pos++;
	}
}


//����� ���� � ������� �������
unsigned char Asn1Object::ReadByte()
{
	unsigned char byte = ByteArray.at(Array_Pos);
	Array_Pos++;//������� �������
	return byte;
}

