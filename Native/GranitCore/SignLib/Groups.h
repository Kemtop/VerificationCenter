#pragma once
#include <string>
#include "mpirxx.h"


using std::string;
//Содержит данные эллиптической кривой
class _declspec(dllexport) Groups
{

private:
	//Параметры эллиптической кривой
	static const string Curves[5][9];	
	static const int curv_count; //Количество кривых в массиве
	int cv_index; //Индекс кривой в массиве Curves

public:
	Groups();
	//Создает кривую на основании ее имени
	Groups(string CurveName); //Принимает имя кривой
	//Создает кривую на основании  ее OID
	Groups(int x,string CurveOID);
	~Groups();
	//На основании OID задает кривую. В случае нахождения кривой возвращает true, иначе false.
	bool SetParam(string CurveOID);
	//Возвращает параметры заданной кривой
	string get_a();
	string get_b();
	string get_p();
	string get_Gx();
	string get_Gy();
	string get_q();
};

