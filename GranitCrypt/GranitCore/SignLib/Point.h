#pragma once
#include "mpirxx.h"
#include "Groups.h"
//Содержит набор методов для работы с точкой эллиптической кривой
class _declspec(dllexport) Point
{
private:
	string CurvOid;//Идентификатор эллиптической кривой
public:
	Point();
	//bool setXY=1 устанавливать координаты точки P
 	Point(Groups &EC,bool setXY);
	~Point();

	mpz_class x; //Координата  x точки
	mpz_class y; //Координата y точки
	mpz_class a;
	mpz_class b;
	mpz_class p;//модуль эллиптической кривой
	mpz_class q;//порядок точки. В зарубежной литераторе обозначается как n
	//Копирует координаты точки и параметры кривой
	void operator = (Point &P);
	//Возвращает строковое значение координат
	string get_hex_x();
	string get_hex_y();
	
	void setHexX(string sx);
	void setHexY(string sy);

	//Устанавливает идентификатор эллиптической кривой, к которой относится точка.
	void setCurvOid(string Oid);
	//Возвращает идентификатор эллиптической кривой, к которой относится точка.
	string getCurvOid();
	
};

