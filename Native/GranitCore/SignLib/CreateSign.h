#pragma once
#include "mpirxx.h"
#include "Point.h"
#include "PointMath.h"
#include "Signature.h"
#include "Groups.h"
#include "hash3411.h"
#include <QRandomGenerator>
#include <iostream>
#include <vector>

using std::vector;
//����� ��� �������� ���
class _declspec(dllexport) CreateSign
{
public:
	CreateSign();
	~CreateSign();

	//����34.11-2012 �.1 ������ 1 �.1.1.6 ���� ������� -��������� ������������� ��������� �����
	static const string gost_d1;
	//���� �������� �������
	static const string gost_Q1x;
	static const string gost_Q1y;
	//����34.11-2012 �.1.2 ������� ������������ �������� ������� (�������� I)
	static const string gost_e1;
	static const string gost_k1; //����34.11-2012 �.1.2 �������� 18
	//�������� �������� �.1.2 ������� ������������ �������� ������� (�������� I)
	static const string gost_r1;
	static const string gost_s1;
	//�.2.1.6 ���� �������
	static const string gost_d2;
	//�.2.1.7 ���� �������� �������
	static const string gost_Q2x;
	static const string gost_Q2y;
	//�.2.2 ������� ������������ �������� ������� (�������� I)
	static const string gost_e2;
	static const string gost_k2;
	static const string gost_r2;
	static const string gost_s2;

	//���������� �������� ���� �� ��������� ���������
	Point generate_public_key(mpz_class &d, Point &P);
	/*
	  ���������� �������� ����  �� ��������� ��������� 512 ������� ����� ������������� � ������� raw_key
	  raw_key -������ ����� ����� 64�����
	  � ������� ����  raw_key ������ ������� ������������� ������, ����� ������� �������������� � ���� ������ � ����� �� ��� ��� ����
	  �� ������ ������ ������� ������ q. ������������ d ���������� "���������� ����". ������������ ����� ������������� ������ 
	  ���������� �������� �������.
	  string curveOid-OID ������
	  � ������ ������ ��������� ����������.
	*/
	Point generate_public_key_512(uint8_t *raw_key, mpz_class &d,string curveOid);


	//���������� ��������� ����� k ������������ q ���, � ��������������� ������� 0<k<n;
	mpz_class GetRandom(mpz_class q);
	//����������� ��������� M, ������ Mlength-����� � ������, ��������� �������� ���� d � ����� P
	Signature Sign(mpz_class &d,uint8_t *M,unsigned long long &Mlength, Point &P);
	//��������� ������� ��������� ������� �������� e,k. ������������ ��� ���������� ����-������ ��������.
	Signature Sign1(mpz_class &d, mpz_class &e, mpz_class &k, Point &P);
	//��������� ������������ ��������� ������������ ������� �� ��������� �������� ����34.11-2012 
	string Test();
	//��������� ������������ ��������� �������� ��������� ����� ��� ���� �������� � ����
	string TestPk();

	//���������� �������� ���������� ������������� ��������� ���������. �������� ��� ������� ���������.  
	vector <unsigned char> getLastDigest();

private:
	 //�������� ���������� ������������� ��������� ���������. �������� ��� ������� ���������.  
	vector <unsigned char> lastDigest;
};

