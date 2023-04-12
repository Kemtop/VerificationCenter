#pragma once
#include <vector>
#include <iostream>
#include <fstream>

//Virtual class
//Базовый класс DER кодировки  ASN.1 типов

class Asn1Object
{
public:
	Asn1Object();
	~Asn1Object();
	unsigned int ObjLength; //Длина 
	unsigned char Tag; //Характеризует тип объекта

	unsigned char BlockType = 0;//Значение битов 5-1 идентификатор типа для данного блока;
	unsigned char ClassBlock = 0; // кодируют так называемый "класс" текущего блока;
	bool IsConstructedBlock = false;// должен быть установлен в 0 если текущий блок содержит

	//Начальный адрес блока данных в файле(адрес начинается с нуля)
	unsigned int ParentAdress;

     std::vector<unsigned char> ByteArray; //Контейнер для хранения данных
	 unsigned int Array_Pos=0;//Указатель текущей позиции контейнера

	 virtual void Print() { ; }
	 //Пишет байт в ByteArray
	 void WriteByte(unsigned char byte);
	 //Копирует данные из главного буфера
	 void CopyFromMainBuff(unsigned int start_pos, unsigned int end_pos, std::vector<unsigned char> *DerBuffer);

	 //Читает  байт в текущей позиции контейнера, смещает указатель позиции
     unsigned char Asn1Object::ReadByte();

	

};

