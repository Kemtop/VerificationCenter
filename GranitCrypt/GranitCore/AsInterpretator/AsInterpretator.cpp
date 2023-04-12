#include "AsInterpretator.h"



AsInterpretator::AsInterpretator()
{
}


AsInterpretator::~AsInterpretator()
{
}

void AsInterpretator::Dh_Hash(QByteArray & Block, string & K_hash)
{
	Block.append(114); //Тип блока

	//Преобразовываю запись в виде 16ричной строки в бинарный массив
	QByteArray HexArray = QByteArray::fromHex(K_hash.c_str());

	//Добавляет длину блока данных
	write_len(Block, HexArray.size());

	//Копирую данные
	for (int i = 0;i <HexArray.size();i++)
	{
		Block.append(HexArray[i]);
	}
}


//Переводит шифрованный сеансовый ключ
void AsInterpretator::CryptedBlockKey(QByteArray &Block, string &K)
{
	Block.append(115); //Тип блока

					   //Преобразовываю запись в виде 16ричной строки в бинарный массив

	//Добавляет длину блока данных
	write_len(Block,K.length());

	//Копирую данные
	for (int i = 0;i <K.length();i++)
	{
		Block.append(K[i]);
	}

}

void AsInterpretator::Sign_r(QByteArray & Block, string & r)
{
	Block.append(116); //Тип блока

	//Преобразовываю запись в виде 16ричной строки в бинарный массив
	QByteArray HexArray = QByteArray::fromHex(r.c_str());

	//Добавляет длину блока данных
	write_len(Block, HexArray.size());

	//Копирую данные
	for (int i = 0;i <HexArray.size();i++)
	{
		Block.append(HexArray[i]);
	}
}

void AsInterpretator::Sign_s(QByteArray & Block, string & s)
{
	Block.append(117); //Тип блока

	 //Преобразовываю запись в виде 16ричной строки в бинарный массив
	QByteArray HexArray = QByteArray::fromHex(s.c_str());

	//Добавляет длину блока данных
	write_len(Block, HexArray.size());

	//Копирую данные
	for (int i = 0;i <HexArray.size();i++)
	{
		Block.append(HexArray[i]);
	}
}

void AsInterpretator::SetSignedIndex(QByteArray &Block, string & index)
{
	Block.append(118); //Тип блока

	//Преобразовываю запись в виде 16ричной строки в бинарный массив
	QByteArray HexArray = QByteArray::fromHex(index.c_str());

	//Добавляет длину блока данных
	write_len(Block, HexArray.size());

	//Копирую данные
	for (int i = 0;i <HexArray.size();i++)
	{
		Block.append(HexArray[i]);
	}
}

//Записывает в Block длину len
void AsInterpretator::write_len(QByteArray &Block, int len)
{
	int t1;
	uint8_t b;
	for (uint8_t i = 0;i < 4;i++)
	{
		t1 = len & 0xff;
		b = static_cast<uint8_t>(t1);
		Block.append(static_cast<char>(b));
		len >>= 8;
	}

}