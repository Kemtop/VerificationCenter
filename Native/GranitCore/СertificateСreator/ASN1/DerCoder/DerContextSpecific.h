#pragma once
#include "ASN1\DerCoder\DerCoder.h"
#include "ASN1\DerCoder\DerSequenceObject.h"
#include <vector>
class DerContextSpecific:public DerSequenceObject
{
public:

	//std::vector <unsigned char> binaryvector; //C������� �������� ������������� ������� �������
	//std::vector <DerCoder> �ontainer;
	//�������� ������ � ��������� �ontainer
	//void Add(DerCoder Obj);


	DerContextSpecific();
	~DerContextSpecific();

	//� ������ �ontainer ��������� ����� �������, � ����� ������ ������������ � ���
	//void Set�ontainerInfo(unsigned  char Tag_number, bool  P_C_bit);

	//������� �������� ������������� �������
	void CreateBinary(unsigned char tag, bool IsConstructed);
};

