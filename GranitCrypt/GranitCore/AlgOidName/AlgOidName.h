#pragma once
#include <string>


using std::string;
//����� ��������� ������ �� ��������������� ����������
class AlgOidName
{
public:
	AlgOidName();
	~AlgOidName();
	//���������� ���������� � ������ ��������������. ���� �� ������ ����������-"����������"
	static string GetInfo(string val);

private:
	//��������� ������������� ������
	static const string Oid[58][3];
	//���������� ����� � �������
	static const int  line_count;
};

