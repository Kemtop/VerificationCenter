#pragma once
#include "ASN1\DerCoder\DerCoder.h"
#include <vector>
#include <QByteArray>
#include <QFile>
#include <QString>

//Родитель для всех контейнеров SET,SEQUENCE
class DerSequenceObject
{


protected:
	//Оформляет контейнер в виде бинарного вектора
	void SetСontainerInfo(unsigned  char class_tag, unsigned  char Tag_number, bool  P_C_bit);
public:
	std::vector <unsigned char> binaryvector; //Cодержит двоичное представление данного объекта
	std::vector <DerCoder> Sequence; //Последовательность объектов типа Der кодировки
	void Add(DerSequenceObject Сontainer);
	//Помещает объект в контейнер Sequence
	void Add(DerCoder Obj);

	DerSequenceObject();
	~DerSequenceObject();

	//Пишет вектор в файл
	void WriteVectorToFile(std::string filename);

	//Пишет вектор в файл в формате base64.Добавляя идентификаторы сертификата.
	void WriteCertB64(std::string filename);
	
	//Пишет вектор в массив в формате base64.Добавляя идентификаторы сертификата.
	void WriteCertB64(QByteArray &WArr);

	//Пишет вектор в массив в бинарном формате.
	void WriteData(QByteArray &WArr);

};

