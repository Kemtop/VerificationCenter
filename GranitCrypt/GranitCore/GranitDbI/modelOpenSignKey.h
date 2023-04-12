#pragma once
#include <qstring.h>

//Модель сведений об открытом ключе.
class modelOpenSignKey
{
public:
	modelOpenSignKey();
	~modelOpenSignKey();
	
	 std::string Index; //Индекс.
	 QString FIO; //ФИО
	 std::string Qx; //Координаты открытого ключа проверки подписи.
	 std::string Qy;
};

