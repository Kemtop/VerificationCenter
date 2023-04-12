#include "stdafx.h"
#include "DerContextSpecific.h"


DerContextSpecific::DerContextSpecific()
{
}


DerContextSpecific::~DerContextSpecific()
{
}


//Создает бинарное представление объекта
void DerContextSpecific::CreateBinary(unsigned char tag,bool IsConstructed)
{
	SetСontainerInfo(0x80, tag,IsConstructed);
}

/*
//Помещает объект в контейнер Сontainer
void DerContextSpecific::Add(DerCoder Obj)
{
	Сontainer.push_back(Obj);
}


//В начало Сontainer добавляет класс объекта, и длину данных содержащихся в нем
void DerContextSpecific::SetСontainerInfo(unsigned  char Tag_number, bool  P_C_bit)
{
	//Определяю количество байт содержащихся в контейнере
	unsigned long data_len = 0;

	for (unsigned long i = 0;i < (unsigned long)Сontainer.size();i++)
	{
	   data_len= data_len+ (unsigned long)Сontainer.at(i).binaryvector.size();
	}

	unsigned  char Identifier_octet=0x80;//Context - specific class tag 10 бит 8 бит 7
	// Constructed
	if (P_C_bit == true) Identifier_octet = Identifier_octet | 0x20; //Устанавливаю 6 бит в 1
	
	//Оставляю только биты с 5 по 1
	Tag_number = Tag_number & 0x1f;

	Identifier_octet = Identifier_octet | Tag_number;

	//Добавляю Identifier_octet
	binaryvector.push_back(Identifier_octet);
	
	//Записываю длину
	if (data_len > 127)
	{
		unsigned long size = 1;//сколько байт требуется для передачи длины
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

	//Записываю блок данных
	for (unsigned long i = 0;i < (unsigned long)Сontainer.size();i++)
	{
		for (unsigned long j = 0;j < (unsigned long)Сontainer.at(i).binaryvector.size();j++)
		{
			Identifier_octet = Сontainer.at(i).binaryvector.at(j);
			binaryvector.push_back(Identifier_octet);
		}
	}


}


void DerContextSpecific::WriteVectorToFile(std::string filename)
{
	//Сохраняю вектор в файл
	std::ofstream file;

	file.open(filename, std::ios::out | std::ios::binary);
	file.write((const char*)&binaryvector[0], binaryvector.size());
	file.close();

}
*/