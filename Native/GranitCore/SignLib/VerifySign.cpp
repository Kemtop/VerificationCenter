#include "stdafx.h"
#include "VerifySign.h"

//����34.11-2012 �.1.2 ������� ������������ �������� ������� (�������� I)
//����34.11-2012 �.1.1.7 ���� �������� �������
const string VerifySign::gost_Q1x = "7f2b49e270db6d90d8595bec458b50c58585ba1d4e9b788f6689dbd8e56fd80b";
const string VerifySign::gost_Q1y = "26f1b489d6701dd185c8413a977b3cbbaf64d1c593d26627dffb101a87ff77da";
const string VerifySign::gost_e1 = "2dfbc1b372d89a1188c09c52e0eec61fce52032ab1022e8e67ece6672b043ee5";
//�������� �������� �.1.2 ������� ������������ �������� ������� (�������� I)
const string VerifySign::gost_r1 = "41aa28d2f1ab148280cd9ed56feda41974053554a42767b83ad043fd39dc0493";
const string VerifySign::gost_s1 = "1456c64ba4642a1653c235a98a60249bcd6d3f746b631df928014f6c5bf9c40"; 

//�.2.1.7 ���� �������� �������
//x-509 ��� 
const string VerifySign::gost_Q2x = "115dc5bc96760c7b48598d8ab9e740d4c4a85a65be33c1815b5c320c854621dd5a515856d13314af69bc5b924c8b4ddff75c45415c1d9dd9dd33612cd530efe1";
//y-510���
const string VerifySign::gost_Q2y = "37c7c90cd40b0f5621dc3ac1b751cfa0e2634fa0503b3d52639f5d7fb72afd61ea199441d943ffe7f0c70a2759a3cdb84c114e1f9339fdf27f35eca93677beec";

//�.2.2 ������� ������������ �������� ������� (�������� I)
//e=510 ���
const string VerifySign::gost_e2 = "3754f3cfacc9e0615c4f4a7c4d8dab531b09b6f9c170c533a71d147035b0c5917184ee536593f4414339976c647c5d5a407adedb1d560c4fc6777d2972075b8c";
//r-510���
const string VerifySign::gost_r2 = "2f86fa60a081091a23dd795e1e3c689ee512a3c82ee0dcc2643c78eea8fcacd35492558486b20f1c9ec197c90699850260c93bcbcd9c5c3317e19344e173ae36";
//r-509���
const string VerifySign::gost_s2 = "1081b394696ffe8e6585e7a9362d26b6325f56778aadbc081c0bfbe933d52ff5823ce288e8c4f362526080df7f70ce406a6eeb1f56919cb92a9853bde73e5b4a"; 



VerifySign::VerifySign()
{
}


VerifySign::~VerifySign()
{
}

bool VerifySign::VerifySign1(mpz_class & e, Point & P, Point & Q, Signature &Sign)
{
	
	if (e == 0) e = 1;

	mpz_class v, z1, z2;
	mpz_class r,s,q;
	mpz_class R(0);

	//������� ������������� ������
	q = P.q;
	//������� �������� ��������
	r = Sign.get_r();
	s = Sign.get_s();

	//�������� ����������� �� �����������  0<r<q � 0<s<q
	//�� ���������� ������� 0<r<q
	if ((r < 0) || (r > q)) return false;
	//�� ���������� ������� 0<s<q
	if ((s < 0) || (s > q)) return false;
	
		
	int HasInv = mpz_invert(v.get_mpz_t(), e.get_mpz_t(), q.get_mpz_t());
	//�������� �� ������������� ��������� ����� ����������������� ���������
	if (HasInv == 0) return false;

	z1 = (s*v) % q;
	z2 = q + ((-(r * v)) % q);

	Point X1;
	Point X2;

	PointMath Math;

	X1 = Math.multiply(z1, P);
	X2 = Math.multiply(z2, Q);

	Point C;
	C = Math.add(X1, X2);
		
	R = C.x%q;

	if (R == r) return true; //������� �����
	else return false; //������� �� �����
	

	return 0;
}

string VerifySign::Test()
{
	//��������� ������ ��������������� ������������� ������
	Groups EC1("test_gost3411_256");
	Point P1(EC1, 1); //����� ������������� ������  �.1 ������ 1

	Point Q;
	mpz_class Qx, Qy,e,r,s;
	Signature Sign; //�������
	bool �orrect_sign = 0;// ���� ������������ �������

	Q = P1;
	//����� �������� ��������� ����� ������������
	Qx.set_str(gost_Q1x,16);
	Qy.set_str(gost_Q1y,16);
	Q.x = Qx;
	Q.y = Qy;

	e.set_str(gost_e1, 16);
	r.set_str(gost_r1, 16);
	s.set_str(gost_s1, 16);
	Sign.set_r(r);
	Sign.set_s(s);
	�orrect_sign = VerifySign1(e, P1, Q, Sign);
	if (!�orrect_sign)
		return "Bad verify test1 ";

	//-----------������ 2 ����-----------------------
	Groups EC2("test_gost3411_512");
	Point P2(EC2, 1); //����� ������������� ������  �.2 ������ 2

	Q = P2;
	//����� �������� ��������� ����� ������������
	Qx.set_str(gost_Q2x, 16);
	Qy.set_str(gost_Q2y, 16);
	Q.x = Qx;
	Q.y = Qy;

	e.set_str(gost_e2, 16);
	r.set_str(gost_r2, 16);
	s.set_str(gost_s2, 16);
	Sign.set_r(r);
	Sign.set_s(s);
	�orrect_sign = VerifySign1(e, P2, Q, Sign);
	if (!�orrect_sign)
		return "Bad verify test2 ";
	

	return "";
}

bool VerifySign::Verify(uint8_t * M, unsigned long long & Mlength, Point & P, Point & Q, Signature & Sign)
{
	//�������� ��� ����������� ���������------------
	Hash3411 H;

	unsigned long long mess_len; //����� ��������� � �����
	unsigned long long x = 8;
	mess_len = Mlength*x;


	unsigned char hash[64];
	int hash_len = 0;
	//� ����������� �� ���� ������� �������� ��� ������� �������
	if (Sign.get_type() == 1)
	{
		H.hash_512(M, mess_len, hash);
		hash_len = 64;
	}	 	
	else
	{
		H.hash_256(M, mess_len, hash);
		hash_len = 32;
	}
		

	mpz_class v, z1, z2;
	mpz_class r, s, q,e, alpha;
	mpz_class R(0);

	//������� ������������� ������
	q = P.q;
	//������� �������� ��������
	r = Sign.get_r();
	s = Sign.get_s();



	//�������� ����������� �� �����������  0<r<q � 0<s<q
	//�� ���������� ������� 0<r<q
	if ((r < 0) || (r > q)) return false;
	//�� ���������� ������� 0<s<q
	if ((s < 0) || (s > q)) return false;
	//����� ������ ����� ���� ������� ����� ����� 0 z1 ������ ������ ����, ��������� ���������� ��������
	if ((r == 0) || (s == 0)) return false;

	//������������  �������� ��� ������� �  �����
	mpz_import(alpha.get_mpz_t(), hash_len, 1,1, 0, 0, hash);
	//�������� �������� ��������� ��� ����������� ��� �������� �����������.
	LastDigest = alpha.get_str(16);

	e = alpha%q;
	if (e == 0) e = 1;
	
	int HasInv = mpz_invert(v.get_mpz_t(), e.get_mpz_t(), q.get_mpz_t());
	//�������� �� ������������� ��������� ����� ����������������� ���������
	if (HasInv == 0) return false;

	z1 = (s*v) % q;
	z2 = q + ((-1*(r * v)) % q);

	Point X1;
	Point X2;
	PointMath Math;
	Point C;

	//��� ��� ��� ���������� ��������� ����� �� ����, ������ ��������.
	if(z1<=0) throw("Verify z1 can't zero");
	if(z2 <= 0) throw("Verify z2 can't zero");
	

	X1 = Math.multiply(z1, P);
	X2 = Math.multiply(z2, Q);

	C = Math.add(X1, X2);
	
	R = C.x%q;
		
	string s_r = r.get_str(16);
	string s_R=R.get_str(16);

	LastR = s_R;	//�������� ������� ��� ����������� ��� �������� �����������.

	if (R == r) return true; //������� �����
	else return false; //������� �� �����
	
	return false;
}

string VerifySign::GetLastR()
{
	return LastR;
}

string VerifySign::GetLastS()
{
	return LastS;
}

string VerifySign::GetLastDigest()
{
	return LastDigest;
}
