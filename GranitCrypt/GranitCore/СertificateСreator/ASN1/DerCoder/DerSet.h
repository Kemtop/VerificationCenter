#pragma once
#include "ASN1\DerCoder\DerCoder.h"
#include "ASN1\DerCoder\DerSequence.h"
#include "ASN1\DerCoder\DerSequenceObject.h"
#include <vector>

class DerSet :public DerSequenceObject
{
public:
	//std::vector <unsigned char> binaryvector; //C������� �������� ������������� ������� �������
	//std::vector <DerCoder> Sequence; //������������������ �������� ���� Der ���������
	
	DerSet();
	~DerSet();

	//�������� ������ � ��������� Sequence
	//void Add(DerCoder Obj);
	//�������� � ������������������ ������ ����  DerSequence
//	void AddSequencecBlock(DerSequence Obj);

      //������� �������� ������������� �������
	void CreateBinary();
	

};

