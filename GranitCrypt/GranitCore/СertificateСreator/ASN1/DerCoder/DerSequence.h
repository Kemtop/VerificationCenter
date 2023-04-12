#pragma once
#include "ASN1\DerCoder\DerCoder.h"
#include "ASN1\DerCoder\DerContextSpecific.h"
#include "ASN1\DerCoder\DerSet.h"
#include "ASN1\DerCoder\DerSequenceObject.h"
#include <vector>
class DerSequence:public virtual DerSequenceObject
{
public:
	//std::vector <unsigned char> binaryvector; //Cодержит двоичное представление данного объекта
	//Последовательность объектов типа Der кодировки
//	std::vector <DerCoder> Sequence;
	//Помещает в последовательность объект типа ContextSpecific
	//void AddContextSpecificBlock(DerContextSpecific Obj);
	//Помещает в binaryvector идентификатор последовательности, длину блока данных, и содержимое контейнера Sequence
//	void SetСontainerInfo();
	//Помещает объект в контейнер Sequence
	//void Add(DerCoder Obj);
	//Помещает в последовательность объект типа  DerSequence
	//void AddSequencecBlock(DerSequence Obj);
	//Помещает в последовательность объект типа  DerSet
	//void AddSetBlock(DerSet Obj);
	//Создает бинарное представление объекта
	void CreateBinary();
	
	//Пишет вектор в файл
	//void WriteVectorToFile(std::string filename);

	DerSequence();
	~DerSequence();
};

