#include "ASreader.h"
#include "ASblockData.h"

ASreader::ASreader()
{
	begin_sign_block=0;
}


ASreader::~ASreader()
{
}

//Читает блок содержащий вектора подписи;
int ASreader::ReadSignData(QByteArray SignBlock)
{
	uint8_t title[5]; //Заголовок 5 байт [тип][длина]
	int block_len = 0; //Длина блока данных.

	int f_len = SignBlock.length();//Длина блока
	int cur_pos = 0;//Текущая позиция.

	while (cur_pos < f_len)
	{
		QByteArray tl = SignBlock.mid(cur_pos,5); //Копирую 5 байт.
		std::memcpy(title,tl.constData(),5); //Копирую блок в title.
		cur_pos += 5; //Сдвигаем позицию указателя.

		if ( !((title[0]==116)||(title[0] == 117) ) ) return 1; //Блок не содержит данные о подписи.

		ASblockData B;
		B.type = title[0]; //Тип
		
		DecodeLen(title, block_len); //Получаю длину блока данных
		B.len = block_len; //Длина блока данных
		B.data = SignBlock.mid(cur_pos, block_len); //Чтение блока данных.       
		cur_pos += block_len;
		//Не получилось считать блок данных требуемого размера
		if (block_len != B.data.size()) return 2;

		Blocks.append(B);
	}

	return 0;
}										   
bool ASreader::Read(QFile & fileIn, qint64 f_len)
{
	//Определение длины шифрованнного блока
	/*
	[7 байт что то вроде кода версии][Заголовок 47][ХЭШ ГОСТ 34.11 сообщения идущего за ним 64байт][Размер блока шифрованных данных 8байт]
	7+47+64=118

	*/

	//Устанавливаю текущую позицию(считываемый файл) на начало определения размера блока данных
	if (!fileIn.seek(118)) return 0;

	//Считывание размера блока данных
	uint8_t arr_len[8];
	char F_block[32];

	if (fileIn.read(F_block, 8) != 8)	return 0;
	for (uint8_t i = 0;i < 8;i++)  arr_len[i] = static_cast<uint8_t>(F_block[i]);

	//Преобразовываю массив в длину
	quint64 data_len = 0;
	quint64 tmp = 0;

	for (uint8_t i = 0;i < 8;i++)
	{
		tmp = 0;
		tmp = tmp | arr_len[i];
		tmp = tmp << i * 8;
		data_len = data_len | tmp;
	}


	//Вычисляю позицию в которой заканчиваются шифрованные данные
	data_len = data_len + 118 + 8; //Позиция конца блока данных

	 //Устанавливаю текущую позицию(считываемый файл) на начало блока данных
	if (!fileIn.seek(data_len)) return 0;
	//Цикл по всем блокам пока не достигну конца файла
	quint64 pos = data_len;

	uint8_t title[5]; //Заголовок 5 байт [тип][длина]
	int block_len=0;

	quint64 begin_pos = 0; //Позиция начала блока
	while (fileIn.pos() < f_len)
	{
		begin_pos = fileIn.pos(); //Считываю позицию начала блока данных, нужно для получения начала блока подписи
		fileIn.read(reinterpret_cast<char*>(title), 5);
		ASblockData B;
		B.type =title[0]; //Тип
		
		//Если встречаем начальный блок подписи
		if (title[0] == 116) begin_sign_block = begin_pos;

		DecodeLen(title, block_len); //Получаю длину блока данных
		B.len = block_len; //Длина блока данных
		B.data=fileIn.read(block_len);
		//Не получилось считать блок данных требуемого размера
		if (block_len != B.data.size()) return 0;

		Blocks.append(B);
		
	}

	return 1;
}

void ASreader::DecodeLen(uint8_t *t, int & len)
{
	//Преобразовываю массив в длину
	len = 0;
	int tmp = 0;

	for (uint8_t i = 1;i < 5;i++)
	{
		tmp = 0;
		tmp =static_cast<int>(t[i]);
		tmp = tmp << (i-1)* 8;
	    len = len | tmp;
	}
}


bool ASreader::GetSignVector(QByteArray & r, QByteArray & s)
{		
	//Цикл по всем блокам 
	bool found = 0;
	//Поиск r
	for (int i = 0;i < Blocks.count();i++)
	{
		//Если найден блок содержащий вектор r подписи
		if (Blocks[i].type == 116)
		{
			r = Blocks[i].data;
			
			found = 1;
			break;
		}
	}
	//Блок не найден
	if (found == 0) return 0;
	
	//Поиск s
	found = 0; //сброс флага
	for (int i = 0;i < Blocks.count();i++)
	{
		//Если найден блок содержащий вектор r подписи
		if (Blocks[i].type == 117)
		{
			s = Blocks[i].data;
			found = 1;
			break;
		}
	}
	//Блок не найден
	if (found == 0) return 0;

	return true;
}

bool ASreader::GetSignKeyIndex(string & k_index)
{
	
	//Цикл по всем блокам 
	bool found = 0;
	for (int i = 0;i <Blocks.count();i++)
	{
		//Если найден блок содержащий шифрованный сеансовый ключ
		if (Blocks[i].type == 118)
		{
			//Преобразовываю в строку
			//string t1(Blocks[i].data.data(), Blocks[i].data.size());
			k_index = Blocks[i].data.toHex().toStdString();
//			k_index = t1;
			found = 1;
			break;
		}
	}
	//Блок не найден
	if (found == 0) return false;

	return true;
}
