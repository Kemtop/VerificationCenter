#pragma once
#include "mpirxx.h"
#include "Point.h"
#include "PointMath.h"
#include "Signature.h"
#include "Groups.h"
#include "hash3411.h"
#include <iostream>

//Класс проверки подписей
class VerifySign
{
private:
	//ГОСТ34.11-2012 А.1.2 Процесс формирования цифровой подписи (алгоритм I)
	//Ключ проверки подписи
	static const string gost_Q1x;
	static const string gost_Q1y;
	static const string gost_e1;
    //Значение векторов А.1.2 Процесс формирования цифровой подписи (алгоритм I)
	static const string gost_r1;
	static const string gost_s1;
	//А.2.2 Процесс формирования цифровой подписи (алгоритм I)
	//А.2.1.7 Ключ проверки подписи
	static const string gost_Q2x;
	static const string gost_Q2y;
	static const string gost_e2;
	static const string gost_r2;
	static const string gost_s2;

	//Проверяет подпись, используя внешний параметр e В случае успеха возвращает 1
	bool VerifySign1(mpz_class &e, Point &P, Point &Q, Signature &Sign);

	string LastR; //Последнее вычисленное значение вектора R подписи
	string LastS;//Последнее вычисленное значение вектора S подписи
	string  LastDigest; //Последний дайджест сообщения.
public:
	VerifySign();
	~VerifySign();
	
	//Тестирование ГОСТ параметров используя VerifySign1
	string Test();
	//Проверяет подпись Sign,сообщения M, длиной в байтах Mlength,используя откр.ключ Q  в случае успеха возвращает 1
	bool Verify(uint8_t *M, unsigned long long &Mlength, Point &P, Point &Q, Signature &Sign);
	string GetLastR();//Возвращает последнее вычисленное значение вектора R подписи
	string GetLastS();//Возвращает последнее вычисленное значение вектора S подписи
	string GetLastDigest();//Возвращает последнее вычисленное значение дайджеста сообщения.
};

