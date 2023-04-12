#include "stdafx.h"
#include "Groups.h"

//���������� ������ � �������
 const int  Groups::curv_count=5;
 //������ � ����������� ������
 const string  Groups::Curves[5][9]=
 {
	 {
       "secp384r1", 	 
	   "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffeffffffff0000000000000000ffffffff", //p
	   "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffeffffffff0000000000000000fffffffc", //a
	   "b3312fa7e23ee7e4988e056be3f82d19181d9c6efe8141120314088f5013875ac656398d8a2ed19d2a85c8edd3ec2aef",//b
	   "aa87ca22be8b05378eb1c71ef320ad746e1d3b628ba79b9859f741e082542a385502f25dbf55296c3a545e3872760ab7",// gx
	   "3617de4a96262c6f5d9e98bf9292dc29f8f41dbd289a147ce9da3113b5f0b8c00a60b1ce1d7e819d7a431d7c90ea0e5f", //gy
       "ffffffffffffffffffffffffffffffffffffffffffffffffc7634d81f4372ddf581a0db248b0a77aecec196accc52973",//n-������� ����� g. � ������������� q
       "1", //h ��������
	   "" //OID
     },
	 {   //��������� ������ �� ����34.11-2012 �.1 ������ 1
		 "test_gost3411_256",
		 "8000000000000000000000000000000000000000000000000000000000000431",//p ������ ������������� ������
		 "7",//a  ������������ ������������� ������
		 "5fbff498aa938ce739b8e022fbafef40563f6e6a3472fc2a514c0ce9dae23b7e",//b
		 "2",// gx ���������� ����� �
		 "8e2a8a0e65147d4bd6316030e16d19c85c97f0a9ca267122b96abbcea7e8fc8",// gy
		 "8000000000000000000000000000000150fe8a1892976154c59cfc193accf5b3", //n-������� ����� G. � ������������� ����������-q 256���
		 "1",//����� �������� ��� ��� �� ������������
		 "" //OID
      },
	  {   //��������� ������ �� ����34.11-2012 �.2 ������ 2
		  "test_gost3411_512",
		  //p ������ ������������� ������ p=511���
		  "4531acd1fe0023c7550d267b6b2fee80922b14b2ffb90f04d4eb7c09b5d2d15df1d852741af4704a0458047e80e4546d35b8336fac224dd81664bbf528be6373",
		  "7",//a  ������������ ������������� ������
		  //b -509 ���
		  "1cff0806a31116da29d8cfa54e57eb748bc5f377e49400fdd788b649eca1ac4361834013b2ad7322480a89ca58e0cf74bc9e540c2add6897fad0a3084f302adc",
		  // gx ���������� ����� � 510 ���
		  "24d19cc64572ee30f396bf6ebbfd7a6c5213b3b3d7057cc825f91093a68cd762fd60611262cd838dc6b60aa7eee804e28bc849977fac33b4b530f1b120248a9a",
		  // gy 510 ���
		  "2bb312a43bd2ce6e0d020613c857acddcfbf061e91e5f2c3f32447c259f39b2c83ab156d77f1496bf7eb3351e1ee4e43dc1a18b91b24640b6dbb92cb1add371e",
		   //n-������� ����� G. � ������������� ����������-q 511 ���
		  "4531acd1fe0023c7550d267b6b2fee80922b14b2ffb90f04d4eb7c09b5d2d15da82f2d7ecb1dbac719905c5eecc423f1d86e25edbe23c595d644aaf187e6e6df", 
		  "1",//����� �������� ��� ��� �� ������������
		  "" //OID
	  }
	 ,
	 {   //��� ������
		 "gost-3410-12-512-paramSetA",
		 //p ������ ������������� ������ p=512 ���
		 "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffdc7",
		 //a  ������������ ������������� ������ 512 ���
		 "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffdc4",
		 //b -512 ���
		 "e8c2505dedfc86ddc1bd0b2b6667f1da34b82574761cb0e879bd081cfd0b6265ee3cb090f30d27614cb4574010da90dd862ef9d4ebee4761503190785a71c760",
		 // gx ���������� ����� � 
		 "3",
		 // gy 512 ���
		 "7503cfe87a836ae3a61b8816e25450e6ce5e1c93acf1abc1778064fdcbefa921df1626be4fd036e93d75e6a50e3a41e98028fe5fc235f5b889a589cb5215f2a4",
		 //n-������� ����� G. � ������������� ����������-q 512 ���
		 "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff27e69532f48d89116ff22b8d4e0560609b4b38abfad2b85dcacdb1411f10b275",
		 "1",//����� �������� ��� ��� �� ������������
		 "1.2.643.7.1.2.1.2.1" //OID szOID_tc26_gost_3410_12_512_paramSetA
     },
	 {
		 "gost-3410-12-512-paramSetB",
		 //p ������ ������������� ������ p=512 ���
		 "008000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000006F",
		 //a  ������������ ������������� ������ 512 ���
		 "008000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000006C",
		 //b -512 ���
		 "687D1B459DC841457E3E06CF6F5E2517B97C7D614AF138BCBF85DC806C4B289F3E965D2DB1416D217F8B276FAD1AB69C50F78BEE1FA3106EFB8CCBC7C5140116",
		 // gx ���������� ����� � 
		 "2",
		 // gy 512 ���
		 "1A8F7EDA389B094C2C071E3647A8940F3C123B697578C213BE6DD9E6C8EC7335DCB228FD1EDF4A39152CBCAAF8C0398828041055F94CEEEC7E21340780FE41BD",
		 //n-������� ����� G. � ������������� ����������-q 512 ���
		 "00800000000000000000000000000000000000000000000000000000000000000149A1EC142565A545ACFDB77BD9D40CFA8B996712101BEA0EC6346C54374F25BD",
		 "1",
		 "1.2.643.7.1.2.1.2.2" //szOID_tc26_gost_3410_12_512_paramSetB
     }
 };




 Groups::Groups()
 {
 }

 Groups::Groups(string CurveName)
{
	//����� ������ � ��������� ������
	bool found = 0; //����� ��� ������ �� �����
	for (int i = 0;i <curv_count;i++)
	{
		if (Curves[i][0] == CurveName)
		{
			found = 1;
			cv_index = i;
			break;
		}
	}
	//�� ���� ����� ������ �� ���������� �����
	if (found == 0)
		throw("Groups::Groups()-> Can't find curve for this name");

}

//������� ������ �� ���������  �� OID
Groups::Groups(int x,string CurveOID)
{
	//����� ������ �� �� OID
	bool found = 0; //����� ��� ������ �� �����
	for (int i = 0;i <curv_count;i++)
	{
		if (Curves[i][8] == CurveOID)
		{
			found = 1;
			cv_index = i;
			break;
		}
	}
	//�� ���� ����� ������ �� ���������� �����
	if (found == 0)
		throw("Groups::Groups()-> Can't find curve for this name");
}


Groups::~Groups()
{
}

//�� ��������� OID ������ ������. � ������ ���������� ������ ���������� true, ����� false.
bool Groups::SetParam(string CurveOID)
{
	//����� ������ �� �� OID
	bool found = false; //����� ��� ������ �� �����
	for (int i = 0;i <curv_count;i++)
	{
		if (Curves[i][8] == CurveOID)
		{
			found = true;
			cv_index = i;
			break;
		}
	}

	return found;
}

string Groups::get_a()
{
	return Curves[cv_index][2];
}

string Groups::get_b()
{
	return Curves[cv_index][3];
}

string Groups::get_p()
{
	return Curves[cv_index][1];
}

string Groups::get_Gx()
{
	return Curves[cv_index][4];
}

string Groups::get_Gy()
{
	return Curves[cv_index][5];
}

string Groups::get_q()
{
	return Curves[cv_index][6];
}


/*
void Groups::operator=(Groups &g)
{
	a = g.get_a();
	b = g.get_b();
	p = g.get_p();
}
*/
/*Point Groups::generate_public_key(mpz_class d)
{
	//return Point();
}
*/