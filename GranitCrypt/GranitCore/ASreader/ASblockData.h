#pragma once
#include <qbytearray.h>

//������ ����� ������, � ������� ���������� ��������� ��� �������������� ����������.
class ASblockData
{
public:
	ASblockData();
	~ASblockData();

	uint8_t type; //������ ����-��� �����
	int len; //����� �����
	QByteArray data;

};

