#pragma once
#include "mpirxx.h"
#include "Point.h"
#include <iostream>
//Класс предназначен для выполнения арифметических действий с эллиптической кривой
class PointMath
{
public:
	PointMath();
	~PointMath();
	
	//Складывает точку Q1 саму с собой возвращает результат
	Point PointMath::add_it_self(Point Q1);
	//Сложение двух точек,возвращает результат
	Point add(Point Q1, Point Q2);
	//Умножаю точку P на число,возвращает результат
	Point multiply(mpz_class k, Point P);
	
	
};

