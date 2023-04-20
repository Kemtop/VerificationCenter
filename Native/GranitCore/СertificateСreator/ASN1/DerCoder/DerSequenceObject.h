#pragma once
#include "ASN1\DerCoder\DerCoder.h"
#include <vector>
#include <QByteArray>
#include <QFile>
#include <QString>

//�������� ��� ���� ����������� SET,SEQUENCE
class DerSequenceObject
{


protected:
	//��������� ��������� � ���� ��������� �������
	void Set�ontainerInfo(unsigned  char class_tag, unsigned  char Tag_number, bool  P_C_bit);
public:
	std::vector <unsigned char> binaryvector; //C������� �������� ������������� ������� �������
	std::vector <DerCoder> Sequence; //������������������ �������� ���� Der ���������
	void Add(DerSequenceObject �ontainer);
	//�������� ������ � ��������� Sequence
	void Add(DerCoder Obj);

	DerSequenceObject();
	~DerSequenceObject();

	//����� ������ � ����
	void WriteVectorToFile(std::string filename);

	//����� ������ � ���� � ������� base64.�������� �������������� �����������.
	void WriteCertB64(std::string filename);
	
	//����� ������ � ������ � ������� base64.�������� �������������� �����������.
	void WriteCertB64(QByteArray &WArr);

	//����� ������ � ������ � �������� �������.
	void WriteData(QByteArray &WArr);

};

