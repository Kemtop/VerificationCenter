#include "stdafx.h"
#include "CreateSign.h"
//����34.11-2012 �.1 ������ 1 �.1.1.6 ���� ������� -��������� ������������� ��������� �����
const string CreateSign::gost_d1 = "7a929ade789bb9be10ed359dd39a72c11b60961f49397eee1d19ce9891ec3b28";
//const string CreateSign::gost_d1 = "55441196065363246126355624130324183196576709222340016572108097750006097525544";

//����34.11-2012 �.1.1.7 ���� �������� �������
const string CreateSign::gost_Q1x = "7f2b49e270db6d90d8595bec458b50c58585ba1d4e9b788f6689dbd8e56fd80b";
const string CreateSign::gost_Q1y = "26f1b489d6701dd185c8413a977b3cbbaf64d1c593d26627dffb101a87ff77da";

//����34.11-2012 �.1.2 ������� ������������ �������� ������� (�������� I)
const string CreateSign::gost_e1 = "2dfbc1b372d89a1188c09c52e0eec61fce52032ab1022e8e67ece6672b043ee5";
//����34.11-2012 �.1.2 �������� 18
const string CreateSign::gost_k1 = "77105c9b20bcd3122823c8cf6fcc7b956de33814e95b7fe64fed924594dceab3";
//�������� �������� �.1.2 ������� ������������ �������� ������� (�������� I)
const string CreateSign::gost_r1="41aa28d2f1ab148280cd9ed56feda41974053554a42767b83ad043fd39dc0493";
const string CreateSign::gost_s1="1456c64ba4642a1653c235a98a60249bcd6d3f746b631df928014f6c5bf9c40"; 
//�.2.1.6 ���� ������� 508 ���
const string CreateSign::gost_d2 = "ba6048aadae241ba40936d47756d7c93091a0e8514669700ee7508e508b102072e8123b2200a0563322dad2827e2714a2636b7bfd18aadfc62967821fa18dd4";
//�.2.1.7 ���� �������� �������
//x-509 ��� 
const string CreateSign::gost_Q2x="115dc5bc96760c7b48598d8ab9e740d4c4a85a65be33c1815b5c320c854621dd5a515856d13314af69bc5b924c8b4ddff75c45415c1d9dd9dd33612cd530efe1";
//y-510���
const string CreateSign::gost_Q2y="37c7c90cd40b0f5621dc3ac1b751cfa0e2634fa0503b3d52639f5d7fb72afd61ea199441d943ffe7f0c70a2759a3cdb84c114e1f9339fdf27f35eca93677beec";
//�.2.2 ������� ������������ �������� ������� (�������� I)
//e=510 ���
const string CreateSign::gost_e2="3754f3cfacc9e0615c4f4a7c4d8dab531b09b6f9c170c533a71d147035b0c5917184ee536593f4414339976c647c5d5a407adedb1d560c4fc6777d2972075b8c";
//k=506 ���
const string CreateSign::gost_k2="359e7f4b1410feacc570456c6801496946312120b39d019d455986e364f365886748ed7a44b3e794434006011842286212273a6d14cf70ea3af71bb1ae679f1"; 
//r-510���
const string CreateSign::gost_r2="2f86fa60a081091a23dd795e1e3c689ee512a3c82ee0dcc2643c78eea8fcacd35492558486b20f1c9ec197c90699850260c93bcbcd9c5c3317e19344e173ae36";
//s-509���
const string CreateSign::gost_s2="1081b394696ffe8e6585e7a9362d26b6325f56778aadbc081c0bfbe933d52ff5823ce288e8c4f362526080df7f70ce406a6eeb1f56919cb92a9853bde73e5b4a"; 

CreateSign::CreateSign()
{
}


CreateSign::~CreateSign()
{
}

Point CreateSign::generate_public_key(mpz_class & d, Point & P)
{
	Point Q;
	Q = P; //������� ��������� ������
	PointMath M;
	Q = M.multiply(d, P);

	return Q;
}


mpz_class CreateSign::GetRandom(mpz_class q)
{
	//��������� ����������  ��� � ������� ������
	unsigned int sz = mpz_sizeinbase(q.get_mpz_t(), 2);
	int size_in_byte = sz / 8; //���������� ����
	
	uint8_t *b_array=new uint8_t[size_in_byte];

	int cnt = 0; 
	int y;
	uint8_t b=0;
	mpz_class r;

	while (true)
	{
		cnt = 0;
		//���� �� ��� ��� ���� �� ������ ������ ���������� ����
		while (cnt<size_in_byte)
		{
			y = QRandomGenerator::system()->generate();
			y = abs(y);
			//����������� ����� �� ����� -�� ��� ������� 4�����
			for (uint8_t n = 0;n<4;n++)
			{
				b = y & 0xff;
				b_array[cnt] = b;
				y >>= 8;
				cnt++;
			}

		}
		
		mpz_class rnd;
		//������������ ������ � �����
		mpz_import(rnd.get_mpz_t(), size_in_byte, 1, sizeof(b_array[0]), 0, 0, b_array);

	        //���������� ����� ������ ������� ������
		    if ( (rnd < q)&&(rnd>0) )
			{			
				r = rnd;
				break;
			}		

	}
		
	delete []b_array;
	b_array = NULL;
				
	return  r;
}

Signature CreateSign::Sign(mpz_class & d, uint8_t * M, unsigned long long & Mlength, Point & P)
{	

	if (d == 0) throw("private key for sign can't be 0");


	//����� ���������� ��� ������� ������������� ������
	uint32_t q_size = mpz_sizeinbase(P.q.get_mpz_t(), 2);

	uint8_t hash[64]; //�������� ��� ������� ���������

	//-----------------------���������� ��� ������� ���������
	Hash3411 H;
	unsigned long long mess_len; //����� ��������� � �����
	unsigned long long b = 8;
	mess_len = Mlength*b;
	int hash_len = 0;


	if (q_size < 500)
	{
		H.hash_256(M, mess_len, hash);
		hash_len = 32;
	}
	else
	{
		H.hash_512(M, mess_len, hash);
		hash_len = 64;
	}
	
	//������� �������� ��������� ���������.
	for (int a = 0;a < hash_len;a++) lastDigest.push_back(hash[a]);
		

	//-----------------------------------------------------
	mpz_class alpha;
	//������������ ������ � �����
	mpz_import(alpha.get_mpz_t(), hash_len, 1, sizeof(hash[0]), 0, 0, hash);



	mpz_class q = P.q; //������� ������������� ������

	mpz_class e;
	e = alpha%q;

	if (e == 0) e = 1;

	Point C;

	mpz_class r(0);
	mpz_class s(0);
	mpz_class k;


	PointMath Math;
	do
	{
		k = GetRandom(q); //��� 3 ��������� ���������������� ����� k; 0<k<q
		C = Math.multiply(k, P); //��� 4. �������� ����� ������������� ������ C=k*G;

		r = C.x % q;//���������� r = x mod n, ��� x � x-���������� ����� C.
		s = ((r * d) + (k * e)) % q;
	} while ((r == 0) || (s == 0));


	//�������� �������� �� ���������� ������� 0<r<q , 0<s<q,
	if (!((0 < r) && (r < q)))
		throw("CreateSign::Sign bad vector 'r'");

	if (!((0 < s) && (s < q)))
		throw("CreateSign::Sign bad vector 's'");
	
	Signature Sign;
	Sign.set_r(r);
	Sign.set_s(s);
	
	
	//��� ������� 256 ���  512
	if (hash_len == 32) Sign.set_type(0);
	else Sign.set_type(1);

	return Sign;
}

Signature CreateSign::Sign1(mpz_class &d, mpz_class &e, mpz_class &k, Point &P)
{
	//-----------------------------------------------------
	mpz_class q = P.q; //������� ������������� ������
		
	if (e == 0) e = 1;
	
	Point C;

	mpz_class r(0);
	mpz_class s(0);
	
	PointMath Math;
	do
	{
		C = Math.multiply(k, P); //��� 4. �������� ����� ������������� ������ C=k*G;
								
		r = C.x % q;//���������� r = x mod n, ��� x � x-���������� ����� C.
		s = ((r * d) + (k * e)) % q;
	} while ((r == 0) || (s == 0));


	//�������� �������� �� ���������� ������� 0<r<q , 0<s<q,
	if ( !( (0 < r) && (r < q) )  )
		throw("CreateSign::Sign bad vector 'r'");

	if ( !( (0 < s)&& (s < q) ) )
		throw("CreateSign::Sign bad vector 's'");


	Signature Sign;
	Sign.set_r(r);
	Sign.set_s(s);
	
	return Sign;
}

string CreateSign::Test()
{
	mpz_class e;
	mpz_class k;
	mpz_class d;
	mpz_class r;
	mpz_class s;
	/*
	����� �������� e,k,d �� ��������� ������� 
	  �.1.2 ������� ������������ �������� �������(�������� I)
	*/
	
	e.set_str(gost_e1, 16);
	//std::cout << e.get_str(10) << std::endl;
	k.set_str(gost_k1, 16);
	//std::cout << k.get_str(10) << std::endl;
	d.set_str(gost_d1, 16);

	//��������� ������ ��������������� ������������� ������
	Groups EC("test_gost3411_256"); 
	Point P(EC, 1); //����� ������������� ������  �.1 ������ 1

	Signature sign; 
	sign = Sign1(d,e,k,P);

	//---------��������� ����������
	//��������� �������� �� ���� 
	r.set_str(gost_r1, 16);
	s.set_str(gost_s1, 16);

	if (sign.get_r() != r)
		return "Error test sign, fail vector r in test1";

	if (sign.get_s() != s)
		return "Error test sign, fail vector s in test1";

	//------------------�.2 ������ 2 �������� 23
	//�.2.2 ������� ������������ �������� ������� (�������� I)
	e.set_str(gost_e2, 16);
	//std::cout << e.get_str(10) << std::endl;
	k.set_str(gost_k2, 16);
	//std::cout << k.get_str(10) << std::endl;
	d.set_str(gost_d2, 16);

	//��������� ������ ��������������� ������������� ������
	Groups EC2("test_gost3411_512");
	Point P2(EC2, 1); //����� ������������� ������  �.1 ������ 1

	sign = Sign1(d, e, k, P2);

	//---------��������� ����������
	//��������� �������� �� ���� 
	r.set_str(gost_r2, 16);
	s.set_str(gost_s2, 16);

	if (sign.get_r() != r)
		return "Error test sign, fail vector r in test2";

	if (sign.get_s() != s)
		return "Error test sign, fail vector s in test2";


	return "";
}

string CreateSign::TestPk()
{
	//��������� ������ ��������������� ������������� ������
	Groups EC1("test_gost3411_256");
	Point P1(EC1, 1); //����� ������������� ������  �.1 ������ 1
	mpz_class d;//�������� ����
	mpz_class Qx; //���������� ��������� �����
	mpz_class Qy;

	d.set_str(gost_d1,16);
	Point Q1; 
	//��������� �������� ����
	Q1=generate_public_key(d,P1);

	//����� ���������� ��������� � ���� �.1 ������ 1
	Qx.set_str(gost_Q1x,16);
	Qy.set_str(gost_Q1y,16);

	//���������
	if (Qx != Q1.x)
		return "Bad TestPk, error on x1";

	if (Qy != Q1.y)
		return "Bad TestPk, error on y1";

	//----����2 �.2 ������ 2
	Groups EC2("test_gost3411_512");
	Point P2(EC2, 1); //����� ������������� ������  �.1 ������ 1
	d.set_str(gost_d2, 16);
	//��������� �������� ����
	Q1 = generate_public_key(d, P2);

	//����� ���������� ��������� � ����  �.2 ������ 2
	Qx.set_str(gost_Q2x, 16);
	Qy.set_str(gost_Q2y, 16);

	//���������
	if (Qx != Q1.x)
		return "Bad TestPk, error on x2";

	if (Qy != Q1.y)
		return "Bad TestPk, error on y2";


	return "";
}

vector <unsigned char> CreateSign::getLastDigest()
{
	return lastDigest;
}


Point CreateSign::generate_public_key_512(uint8_t * raw_key, mpz_class & d, string curveOid)
{

	Groups EC2;  //������ ������.
	bool res=EC2.SetParam(curveOid); //����� ��������� �� ��������� OID
	if(res==false) throw("Error:EC curve bad OID.");
		
	//������ ����� � ���������� � ������ ������������ ����� P
	Point P(EC2, 1); //����� ������������� ������ 

	mpz_class raw_d(0); //�������� ������ �����
	mpz_import(raw_d.get_mpz_t(), 64, 1,1, 0, 0, raw_key);

	raw_d = abs(raw_d); //������ �����

	//���� �� ����� ���� =0 ��� ������������� ������
	if (raw_d <= 0) throw("Error w588:zero raw array");
	
	//���� �� ��� ��� ���� ����� ������ ������� ������ q
	uint8_t cnt = 0; //���������� �������� �����
	while (raw_d >= P.q)
	{
		if (cnt > 50) throw("Error w589: to many iteration ");
		raw_d = raw_d >> 1; 
		cnt++;
	}

	//���� �� ����� ���� =0 ��� ������������� ������
	if (raw_d <= 0) throw("Error w590:zero digit");

	//��������� �������� ����
	Point Q1;
	Q1 = generate_public_key(raw_d, P);
	d = raw_d;

	return Q1;
}