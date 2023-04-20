#include "stdafx.h"
#include "Point.h"


Point::Point()
{
	x = 0;
	y = 0;
}


Point::Point(Groups &EC,bool setXY)
{
	//Cчитываю нужные параметры кривой
	string Ca, Cb, Cp,Cq;
	Ca = EC.get_a();
	Cb = EC.get_b();
	Cp = EC.get_p();
	Cq = EC.get_q();

	//Преобразовываю строки в 16ричном формате в большие числа
	mpz_class a1(Ca,16);
	mpz_class b1(Cb, 16);
	mpz_class p1(Cp, 16);
	mpz_class q1(Cq, 16);

	a = a1;
	b = b1;
	p = p1;
	q = q1;
	//Задавать координаты точке
	if (setXY)
	{
		string Cx, Cy;
		Cx=EC.get_Gx();
		Cy=EC.get_Gy();
		mpz_class x1(Cx,16);
		mpz_class y1(Cy,16);
		x = x1;
		y = y1;
	}
	else
	{
		x = 0;
		y = 0;
	}

}


Point::~Point()
{
}



void Point::operator=(Point & P)
{
	x = P.x;
    y = P.y;
	p = P.p;
	a = P.a;
	b = P.b;
	q = P.q;
	CurvOid = P.CurvOid;
}

string Point::get_hex_x()
{
	return x.get_str(16);
}

string Point::get_hex_y()
{
	return y.get_str(16);;
}

void Point::setHexX(string sx)
{
	mpz_class x_(sx,16);
	x = x_;
}

void Point::setHexY(string sy)
{
	mpz_class y_(sy, 16);
	y = y_;
}

void Point::setCurvOid(string Oid)
{
	CurvOid = Oid;
}

string Point::getCurvOid()
{
	string v = CurvOid;
	return v;
}



