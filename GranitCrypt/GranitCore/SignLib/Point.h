#pragma once
#include "mpirxx.h"
#include "Groups.h"
//�������� ����� ������� ��� ������ � ������ ������������� ������
class _declspec(dllexport) Point
{
private:
	string CurvOid;//������������� ������������� ������
public:
	Point();
	//bool setXY=1 ������������� ���������� ����� P
 	Point(Groups &EC,bool setXY);
	~Point();

	mpz_class x; //����������  x �����
	mpz_class y; //���������� y �����
	mpz_class a;
	mpz_class b;
	mpz_class p;//������ ������������� ������
	mpz_class q;//������� �����. � ���������� ���������� ������������ ��� n
	//�������� ���������� ����� � ��������� ������
	void operator = (Point &P);
	//���������� ��������� �������� ���������
	string get_hex_x();
	string get_hex_y();
	
	void setHexX(string sx);
	void setHexY(string sy);

	//������������� ������������� ������������� ������, � ������� ��������� �����.
	void setCurvOid(string Oid);
	//���������� ������������� ������������� ������, � ������� ��������� �����.
	string getCurvOid();
	
};

