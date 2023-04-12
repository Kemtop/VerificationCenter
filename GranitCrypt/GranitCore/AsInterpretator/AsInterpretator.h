#pragma once
#include <qbytearray>
#include <string>

using std::string;
/*
 Класс реализующий набор инструментов для отправки ассиметричных параметров
 Формирует бинарные блоки которые помещаются в файл после шифрованного сообщения.
 Реализует методы преобразования данных в бинарный формат
*/
//Класс реализующий набор инструментов для отправки ассиметричных параметров 

//Удалать после выхода Release.
class AsInterpretator
{
public:
	AsInterpretator();
	~AsInterpretator();
	//[114]
	void Dh_Hash(QByteArray &Block,string &K_hash); //Переводит хэшированный открытый ключ получателя 
	//[115] 
	//Переводит шифрованный сеансовый ключ
	void CryptedBlockKey(QByteArray &Block, string &K);
	//[116]
	void Sign_r(QByteArray &Block, string &r);
	//[117]
	void Sign_s(QByteArray &Block, string &s);
	//[118] индекс ключа проверки подписи в базе
	void SetSignedIndex(QByteArray &Block, string &index);
	//Записывает в Block длину len
	void write_len(QByteArray &Block,int len );


};

/*
 Формат
 [Тип блока 1 байт] [Длина блока 4 байт] [Данные]

*/