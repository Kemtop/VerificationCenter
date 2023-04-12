#pragma once
#include <string>


using std::string;
//Класс получения данных об идентификаторах алгоритмов
class AlgOidName
{
public:
	AlgOidName();
	~AlgOidName();
	//Возвращает информацию о данном идентификаторе. Если не найден возвращает-"неизвестно"
	static string GetInfo(string val);

private:
	//Параметры эллиптической кривой
	static const string Oid[58][3];
	//Количество строк в массиве
	static const int  line_count;
};

