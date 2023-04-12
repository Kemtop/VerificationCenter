#pragma once
#include <qfile>
#include <string>
#include "ASblockData.h"


using std::string;
//������ ����� ������ �������������� � ������� [��� ����� 1 ����][����� ����� ������ 4 �����][������]
class ASreader
{

private:
	//�� 4-� ���� �������� ����� len
	void DecodeLen(uint8_t *t, int &len);
public:
	ASreader();
	~ASreader();
	QList<ASblockData> Blocks;//��������� ����� ������
	//������ ���� ���������� ������� �������;
	int ReadSignData(QByteArray SignBlock);
	//������ ����� �� �����, � ������ ������ ���������� 0
	bool  Read(QFile &fileIn,qint64 f_len);
	//������� � ����� ������ ����� �������
	qint64 begin_sign_block;
	//���������� ������� ������� r,s. � ������ ������(�������� ����������� ����) ���������� 0
	bool GetSignVector(QByteArray &r, QByteArray &s);
	//���������� ������ ��������� ����� ������� � ��
	bool GetSignKeyIndex(string &k_index);
};

