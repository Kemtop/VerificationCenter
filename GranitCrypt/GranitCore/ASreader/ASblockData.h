#pragma once
#include <qbytearray.h>

//Хранит блоки данных, в которых передается иформация для ассиметричного шифрования.
class ASblockData
{
public:
	ASblockData();
	~ASblockData();

	uint8_t type; //Первый байт-тип блока
	int len; //Длина блока
	QByteArray data;

};

