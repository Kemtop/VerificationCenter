#pragma once
#include "ASN1\DerCoder\DerCoder.h"
#include "ASN1\DerCoder\DerContextSpecific.h"
#include "ASN1\DerCoder\DerSet.h"
#include "ASN1\DerCoder\DerSequenceObject.h"
#include <vector>
class DerSequence:public virtual DerSequenceObject
{
public:
	//std::vector <unsigned char> binaryvector; //C������� �������� ������������� ������� �������
	//������������������ �������� ���� Der ���������
//	std::vector <DerCoder> Sequence;
	//�������� � ������������������ ������ ���� ContextSpecific
	//void AddContextSpecificBlock(DerContextSpecific Obj);
	//�������� � binaryvector ������������� ������������������, ����� ����� ������, � ���������� ���������� Sequence
//	void Set�ontainerInfo();
	//�������� ������ � ��������� Sequence
	//void Add(DerCoder Obj);
	//�������� � ������������������ ������ ����  DerSequence
	//void AddSequencecBlock(DerSequence Obj);
	//�������� � ������������������ ������ ����  DerSet
	//void AddSetBlock(DerSet Obj);
	//������� �������� ������������� �������
	void CreateBinary();
	
	//����� ������ � ����
	//void WriteVectorToFile(std::string filename);

	DerSequence();
	~DerSequence();
};

