#include "stdafx.h"
#include "DerSet.h"


DerSet::DerSet()
{
}


DerSet::~DerSet()
{
}

void DerSet::CreateBinary()
{
	Set�ontainerInfo(0x00,0x11,true);
}
/*
//�������� ������ � ��������� Set
void DerSet::Add(DerCoder Obj)
{
	Sequence.push_back(Obj);
}
*/
/*
//�������� � ������������������ ������ ����  DerSequence
void DerSet::AddSequencecBlock(DerSequence Obj)
{
	DerCoder D;

	for (unsigned long i = 0;i < (unsigned long)Obj.binaryvector.size();i++)
	{
		D.binaryvector.push_back(Obj.binaryvector.at(i));
	}

	Sequence.push_back(D);
}
*/