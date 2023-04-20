#pragma once
#include "mpirxx.h"
#include "Point.h"
#include "PointMath.h"
#include "Signature.h"
#include "Groups.h"
#include "hash3411.h"
#include <iostream>

//����� �������� ��������
class VerifySign
{
private:
	//����34.11-2012 �.1.2 ������� ������������ �������� ������� (�������� I)
	//���� �������� �������
	static const string gost_Q1x;
	static const string gost_Q1y;
	static const string gost_e1;
    //�������� �������� �.1.2 ������� ������������ �������� ������� (�������� I)
	static const string gost_r1;
	static const string gost_s1;
	//�.2.2 ������� ������������ �������� ������� (�������� I)
	//�.2.1.7 ���� �������� �������
	static const string gost_Q2x;
	static const string gost_Q2y;
	static const string gost_e2;
	static const string gost_r2;
	static const string gost_s2;

	//��������� �������, ��������� ������� �������� e � ������ ������ ���������� 1
	bool VerifySign1(mpz_class &e, Point &P, Point &Q, Signature &Sign);

	string LastR; //��������� ����������� �������� ������� R �������
	string LastS;//��������� ����������� �������� ������� S �������
	string  LastDigest; //��������� �������� ���������.
public:
	VerifySign();
	~VerifySign();
	
	//������������ ���� ���������� ��������� VerifySign1
	string Test();
	//��������� ������� Sign,��������� M, ������ � ������ Mlength,��������� ����.���� Q  � ������ ������ ���������� 1
	bool Verify(uint8_t *M, unsigned long long &Mlength, Point &P, Point &Q, Signature &Sign);
	string GetLastR();//���������� ��������� ����������� �������� ������� R �������
	string GetLastS();//���������� ��������� ����������� �������� ������� S �������
	string GetLastDigest();//���������� ��������� ����������� �������� ��������� ���������.
};

