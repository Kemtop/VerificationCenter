#include "stdafx.h"
#include "DerSequenceObject.h"

DerSequenceObject::DerSequenceObject()
{
}


DerSequenceObject::~DerSequenceObject()
{
}

void DerSequenceObject::Add(DerSequenceObject �ontainer)
{
	DerCoder D;

	for (unsigned long i = 0;i < (unsigned long)�ontainer.binaryvector.size();i++)
	{
		D.binaryvector.push_back(�ontainer.binaryvector.at(i));
	}

	Sequence.push_back(D);

}

//�������� ������ � ��������� Sequence
void DerSequenceObject::Add(DerCoder Obj)
{
	Sequence.push_back(Obj);
}

//� ������ �ontainer ��������� ����� �������, � ����� ������ ������������ � ���
void DerSequenceObject::Set�ontainerInfo(unsigned  char class_tag, unsigned  char Tag_number, bool  P_C_bit)
{
	//��������� ���������� ���� ������������ � ����������
	unsigned long data_len = 0;

	for (unsigned long i = 0;i < (unsigned long)Sequence.size();i++)
	{
		data_len = data_len + (unsigned long)Sequence.at(i).binaryvector.size();
	}

	unsigned  char Identifier_octet = class_tag;//Context - specific class tag 10 ��� 8 ��� 7
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
	for (unsigned long i = 0;i < (unsigned long)Sequence.size();i++)
	{
		for (unsigned long j = 0;j < (unsigned long)Sequence.at(i).binaryvector.size();j++)
		{
			Identifier_octet = Sequence.at(i).binaryvector.at(j);
			binaryvector.push_back(Identifier_octet);
		}
	}


}

void DerSequenceObject::WriteVectorToFile(std::string filename)
{
	//�������� ������ � ����
	std::ofstream file;

	file.open(filename, std::ios::out | std::ios::binary);
	file.write((const char*)&binaryvector[0], binaryvector.size());
	file.close();

}

void DerSequenceObject::WriteCertB64(std::string filename)
{	
	QByteArray arr(reinterpret_cast<const char*>(binaryvector.data()), binaryvector.size());
	QByteArray B64 = arr.toBase64(); //����������� � Base64

	QByteArray head("-----BEGIN CERTIFICATE-----\r\n");
	QByteArray footer("\r\n-----END CERTIFICATE-----");

	QByteArray toFile;
	
	//�������� ���������
	for (int i = 0;i < head.size();i++)
	{
		toFile.append(head[i]);
	}

	//�������� ����������
	int cnt = 0;
	for (int i = 0;i < B64.size();i++)
	{
		toFile.append(B64[i]);
		cnt++;
		if (cnt == 64)
		{
			toFile.append("\r\n");
			cnt = 0;
		}
	}
	//�������� ���������
	for (int i = 0;i < footer.size();i++)
	{
		toFile.append(footer[i]);
	}

	QFile file(QString::fromStdString(filename));
	file.open(QIODevice::WriteOnly);
	file.write(toFile);
	file.close();
}


//����� ������ � ������ � ������� base64.�������� �������������� �����������.
void DerSequenceObject::WriteCertB64(QByteArray &WArr)
{
	QByteArray arr(reinterpret_cast<const char*>(binaryvector.data()), binaryvector.size());
	QByteArray B64 = arr.toBase64(); //����������� � Base64

	QByteArray head("-----BEGIN CERTIFICATE-----\r\n");
	QByteArray footer("\r\n-----END CERTIFICATE-----");

	QByteArray toFile;

	//�������� ���������
	for (int i = 0;i < head.size();i++)
	{
		toFile.append(head[i]);
	}

	//�������� ����������
	int cnt = 0;
	for (int i = 0;i < B64.size();i++)
	{
		toFile.append(B64[i]);
		cnt++;
		if (cnt == 64)
		{
			toFile.append("\r\n");
			cnt = 0;
		}
	}
	//�������� ���������
	for (int i = 0;i < footer.size();i++)
	{
		toFile.append(footer[i]);
	}

	WArr = toFile;
}

void DerSequenceObject::WriteData(QByteArray & WArr)
{
	QByteArray arr(reinterpret_cast<const char*>(binaryvector.data()), binaryvector.size());
	WArr = arr;
}
