#include "stdafx.h"
#include "PointMath.h"



PointMath::PointMath()
{
}


PointMath::~PointMath()
{
}


Point PointMath::add_it_self(Point Q1)
{
	mpz_class dx, dy, lambda, Inv;


	dy = (3 * Q1.x*Q1.x) + Q1.a; //3*x*x+a
	dx = 2 * Q1.y; //2*y

	if (dx < 0)	dx += Q1.p;
	if (dy < 0)	dy += Q1.p;

	//mpz_invert(mpz_t rop, const mpz_t op1, const mpz_t op2)
	//Compute the inverse of op1 modulo op2 and put the result in rop.If the inverse exists, 
	//the return value is non - zero and rop will satisfy 0 <= rop < abs(op2) (with rop = 0 possible only when abs(op2) = 1,
	//i.e., in the somewhat degenerate zero ring).If an inverse doesn’t exist the return value is zero and rop is undefined.
	//The behaviour of this function is undefined when op2 is zero.

	//If an inverse doesn’t exist
	int Inv_Flag = 0;
	//Добавить проверку отсутствия инверсии
	Inv_Flag = mpz_invert(Inv.get_mpz_t(), dx.get_mpz_t(), Q1.p.get_mpz_t());

	lambda = (dy*Inv) % Q1.p;

	Point Q2;
	Q2.x = (lambda*lambda - (2 * Q1.x)) % Q1.p;
	Q2.y = (lambda*(Q1.x - Q2.x) - Q1.y) % Q1.p;


	if (Q2.x < 0) Q2.x += Q1.p;
	if (Q2.y < 0) Q2.y += Q1.p;

	Q2.p = Q1.p;
	Q2.a = Q1.a;
	Q2.b = Q1.b;

	return Q2;
}

Point PointMath::add(Point Q1, Point Q2)
{
	mpz_class dx, dy, lambda;

	//Сделать  проверка на бесконечность: P + O = O + P = P для любой точки поля и быстрый расчет

	dx = Q2.x - Q1.x;
	dy = Q2.y - Q1.y;

	//Вычисление лямда. Будем считать что точки разные
	// lambda = (y1 - y2) / (x1 - x2)  (mod m) 

	mpz_class Inv;

	//Сделать проверку на невозможность получения числа мультипликативно обратному
	int HasInv = mpz_invert(Inv.get_mpz_t(), dx.get_mpz_t(), Q1.p.get_mpz_t());

	lambda = (dy*Inv) % Q1.p;
	if (lambda < 0)lambda += Q1.p;

	Point Q3;

	Q3.a = Q1.a;
	Q3.b = Q1.b;
	Q3.p = Q1.p;

	Q3.x = (lambda*lambda - Q1.x - Q2.x) % Q1.p;
	Q3.y = (lambda*(Q1.x - Q3.x) - Q1.y) % Q1.p;

	if (Q3.x < 0) Q3.x += Q1.p;
	if (Q3.y < 0) Q3.y += Q1.p;

	return Q3;
}


//умножение точки на число k необходимо выполнить последовательное сложение.Можно воспользоваться методом быстрого возведения в степень.
//Оптимизировать используя метод окон
Point  PointMath::multiply(mpz_class k, Point P)
{
	Point P1;
	P1 = P;

	//Не умеет умножать на 0
	if (k == 0) throw("PointMath::multiply Can't multyply P to zero");

	k = k - 1;

	while (k != 0)
	{
		if ((k % 2) != 0)
		{
			if ((P1.x == P.x) || (P1.y == P.y))
			{
				P1 = add_it_self(P1);
			}
			else
			{
				P1 = add(P, P1);
			}

			k = k - 1;
		}
		k = k / 2;
		P = add_it_self(P);
	}
	return P1;
}

