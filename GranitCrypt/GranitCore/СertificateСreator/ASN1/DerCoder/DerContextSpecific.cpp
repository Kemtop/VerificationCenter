#include "stdafx.h"
#include "DerContextSpecific.h"


DerContextSpecific::DerContextSpecific()
{
}


DerContextSpecific::~DerContextSpecific()
{
}


//������� �������� ������������� �������
void DerContextSpecific::CreateBinary(unsigned char tag,bool IsConstructed)
{
	Set�ontainerInfo(0x80, tag,IsConstructed);
}

/*
//�������� ������ � ��������� �ontainer
void DerContextSpecific::Add(DerCoder Obj)
{
	�ontainer.push_back(Obj);
}


//� ������ �ontainer ��������� ����� �������, � ����� ������ ������������ � ���
void DerContextSpecific::Set�ontainerInfo(unsigned  char Tag_number, bool  P_C_bit)
{
	//��������� ���������� ���� ������������ � ����������
	unsigned long data_len = 0;

	for (unsigned long i = 0;i < (unsigned long)�ontainer.size();i++)
	{
	   data_len= data_len+ (unsigned long)�ontainer.at(i).binaryvector.size();
	}

	unsigned  char Identifier_octet=0x80;//Context - specific class tag 10 ��� 8 ��� 7
	// Constructed
	if (P_C_bit == true) Identifier_octet = Identifier_octet | 0x20; //������������ 6 ��� � 1
	
	//�������� ������ ���� � 5 �� 1
	Tag_number = Tag_number & 0x1f;

	Identifier_octet = Identifier_octet | Tag_number;

	//�������� Identifier_octet
	binaryvector.push_back(Identifier_octet);
	
	//��������� �����
	if (data_len > 127)
	{
		unsigned long size = 1;//������� ���� ��������� ��� �������� �����
		unsigned long val = data_len;

		while ((val >>= 8) != 0)
		{
			size++;
		}

		Identifier_octet = (unsigned char)(size | 0x80);
		binaryvector.push_back(Identifier_octet);
		

		for (int i = (size - 1) * 8; i >= 0; i -= 8)
		{
			Identifier_octet = (unsigned char)(data_len >> i);
			binaryvector.push_back(Identifier_octet);
		}
	}
	else
	{
		binaryvector.push_back((unsigned char)(data_len));
	}

	//��������� ���� ������
	for (unsigned long i = 0;i < (unsigned long)�ontainer.size();i++)
	{
		for (unsigned long j = 0;j < (unsigned long)�ontainer.at(i).binaryvector.size();j++)
		{
			Identifier_octet = �ontainer.at(i).binaryvector.at(j);
			binaryvector.push_back(Identifier_octet);
		}
	}


}


void DerContextSpecific::WriteVectorToFile(std::string filename)
{
	//�������� ������ � ����
	std::ofstream file;

	file.open(filename, std::ios::out | std::ios::binary);
	file.write((const char*)&binaryvector[0], binaryvector.size());
	file.close();

}
*/