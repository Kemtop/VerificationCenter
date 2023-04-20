#pragma once
#include "ASN1\DerCoder\DerCoder.h"
#include "ASN1\DerCoder\DerSequence.h"
#include "ASN1\DerCoder\DerSequenceObject.h"
#include <vector>

class DerSet :public DerSequenceObject
{
public:
	//std::vector <unsigned char> binaryvector; //Cодержит двоичное представление данного объекта
	//std::vector <DerCoder> Sequence; //Последовательность объектов типа Der кодировки
	
	DerSet();
	~DerSet();

	//Помещает объект в контейнер Sequence
	//void Add(DerCoder Obj);
	//Помещает в последовательность объект типа  DerSequence
//	void AddSequencecBlock(DerSequence Obj);

      //Создает бинарное представление объекта
	void CreateBinary();
	

};

