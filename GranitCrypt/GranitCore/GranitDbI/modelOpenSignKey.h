#pragma once
#include <qstring.h>

//������ �������� �� �������� �����.
class modelOpenSignKey
{
public:
	modelOpenSignKey();
	~modelOpenSignKey();
	
	 std::string Index; //������.
	 QString FIO; //���
	 std::string Qx; //���������� ��������� ����� �������� �������.
	 std::string Qy;
};

