#pragma once
#include "mpirxx.h"
#include "Point.h"
#include <iostream>
//����� ������������ ��� ���������� �������������� �������� � ������������� ������
class PointMath
{
public:
	PointMath();
	~PointMath();
	
	//���������� ����� Q1 ���� � ����� ���������� ���������
	Point PointMath::add_it_self(Point Q1);
	//�������� ���� �����,���������� ���������
	Point add(Point Q1, Point Q2);
	//������� ����� P �� �����,���������� ���������
	Point multiply(mpz_class k, Point P);
	
	
};

