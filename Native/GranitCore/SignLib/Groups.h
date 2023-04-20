#pragma once
#include <string>
#include "mpirxx.h"


using std::string;
//�������� ������ ������������� ������
class _declspec(dllexport) Groups
{

private:
	//��������� ������������� ������
	static const string Curves[5][9];	
	static const int curv_count; //���������� ������ � �������
	int cv_index; //������ ������ � ������� Curves

public:
	Groups();
	//������� ������ �� ��������� �� �����
	Groups(string CurveName); //��������� ��� ������
	//������� ������ �� ���������  �� OID
	Groups(int x,string CurveOID);
	~Groups();
	//�� ��������� OID ������ ������. � ������ ���������� ������ ���������� true, ����� false.
	bool SetParam(string CurveOID);
	//���������� ��������� �������� ������
	string get_a();
	string get_b();
	string get_p();
	string get_Gx();
	string get_Gy();
	string get_q();
};

