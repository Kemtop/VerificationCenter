#include "stdafx.h"
#include "DerSequence.h"


DerSequence::DerSequence()
{
}


DerSequence::~DerSequence()
{
}

/*
//�������� � ������������������ ������ ���� ContextSpecific
void DerSequence::AddContextSpecificBlock(DerContextSpecific Obj)
{
	DerCoder D;

	for (unsigned long i = 0;i < (unsigned long)Obj.binaryvector.size();i++)
	{
		D.binaryvector.push_back(Obj.binaryvector.at(i));
	}

	Sequence.push_back(D);
}


//�������� � binaryvector ������������� ������������������, ����� ����� ������, � ���������� ���������� Sequence
void DerSequence::Set�ontainerInfo()
{
	//��������� ���������� ���� ������������ � ����������
	unsigned long data_len = 0;

	for (unsigned long i = 0;i < (unsigned long)Sequence.size();i++)
	{
		data_len = data_len + (unsigned long)Sequence.at(i).binaryvector.size();
	}

	unsigned  char Identifier_octet = 0;//��� ����� Universal(00) Context - specific class tag 10 ��� 8 ��� 7
	 // Constructed
	Identifier_octet = Identifier_octet | 0x20; //������������ 6 ��� � 1
	Identifier_octet = Identifier_octet | 0x10; //��� -������������������
	

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
	for (unsigned long i = 0;i < (unsigned long)Sequence.size();i++)
	{
		for (unsigned long j = 0;j < (unsigned long)Sequence.at(i).binaryvector.size();j++)
		{
			Identifier_octet = Sequence.at(i).binaryvector.at(j);
			binaryvector.push_back(Identifier_octet);
		}
	}


}
*/

//������� �������� ������������� �������
void DerSequence::CreateBinary()
{
	Set�ontainerInfo(0,0x10,true);
}

/*
void DerSequence::WriteVectorToFile(std::string filename)
{
	//�������� ������ � ����
	std::ofstream file;

	file.open(filename, std::ios::out | std::ios::binary);
	file.write((const char*)&binaryvector[0], binaryvector.size());
	file.close();

}
*/
/*
//�������� ������ � ��������� Sequence
void DerSequence::Add(DerCoder Obj)
{
	Sequence.push_back(Obj);
}


//�������� � ������������������ ������ ����  DerSequence
void DerSequence::AddSequencecBlock(DerSequence Obj)
{
	DerCoder D;

	for (unsigned long i = 0;i < (unsigned long)Obj.binaryvector.size();i++)
	{
		D.binaryvector.push_back(Obj.binaryvector.at(i));
	}

	Sequence.push_back(D);
	
}
/*
//�������� � ������������������ ������ ����  DerSet
void DerSequence::AddSetBlock(DerSet Obj)
{
	DerCoder D;

	for (unsigned long i = 0;i < (unsigned long)Obj.binaryvector.size();i++)
	{
		D.binaryvector.push_back(Obj.binaryvector.at(i));
	}

	Sequence.push_back(D);
}
*/