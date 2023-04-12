#pragma once
#include <qfile>
#include <string>
#include "ASblockData.h"


using std::string;
//Читает блоки данных представленных в формате [тип блока 1 байт][длина блока данных 4 байта][данные]
class ASreader
{

private:
	//Из 4-х байт собирает длину len
	void DecodeLen(uint8_t *t, int &len);
public:
	ASreader();
	~ASreader();
	QList<ASblockData> Blocks;//Считанные блоки данных
	//Читает блок содержащий вектора подписи;
	int ReadSignData(QByteArray SignBlock);
	//Читает блоки из файла, в случае ошибки возвращает 0
	bool  Read(QFile &fileIn,qint64 f_len);
	//Позиция в файле начала блока подписи
	qint64 begin_sign_block;
	//Возвращает вектора подписи r,s. В случае ошибки(например отсутствует блок) возвращает 0
	bool GetSignVector(QByteArray &r, QByteArray &s);
	//Возвращает индекс открытого ключа подписи в БД
	bool GetSignKeyIndex(string &k_index);
};

