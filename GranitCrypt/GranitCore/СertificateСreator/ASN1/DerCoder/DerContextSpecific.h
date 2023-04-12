#pragma once
#include "ASN1\DerCoder\DerCoder.h"
#include "ASN1\DerCoder\DerSequenceObject.h"
#include <vector>
class DerContextSpecific:public DerSequenceObject
{
public:

	//std::vector <unsigned char> binaryvector; //Cодержит двоичное представление данного объекта
	//std::vector <DerCoder> Сontainer;
	//Помещает объект в контейнер Сontainer
	//void Add(DerCoder Obj);


	DerContextSpecific();
	~DerContextSpecific();

	//В начало Сontainer добавляет класс объекта, и длину данных содержащихся в нем
	//void SetСontainerInfo(unsigned  char Tag_number, bool  P_C_bit);

	//Создает бинарное представление объекта
	void CreateBinary(unsigned char tag, bool IsConstructed);
};

