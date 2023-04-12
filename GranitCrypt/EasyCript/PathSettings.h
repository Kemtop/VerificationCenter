#pragma once
#include <qstring.h>
//������ ����, ����������� ��� ������ ���������.
class PathSettings
{
public:
	PathSettings();
	~PathSettings();

	QString GetPathDb(); //���� � ���� ������ �������.
	QString GetPathSK(); //���� � ���������� �����.


	void SetPathDb(QString val); //���� � ���� ������ �������.
	void SetPathSK(QString val); //���� � ���������� �����.

private:
	 QString PathDb; //���� � ���� ������ �������.
	 QString PathSK; //���� � ���������� �����.
};

