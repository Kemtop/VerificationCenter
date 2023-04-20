#pragma once

#include "mpirxx.h"
#include <string>
#include <vector>
#include <QByteArray>
#include "windows.h"

using std::string;
using std::vector;

//�������� �������
class Signature
{
private:
	//������� �������
	mpz_class r;
	mpz_class s;
	int sign_type; //��� ������� 0-256 1-512

	string lastError;
public:
	Signature();
	~Signature();
	
	void set_r(mpz_class _r);
	void set_s(mpz_class _s);
	mpz_class get_r();
	mpz_class get_s();
	//������������� ��� �������
	void set_type(int t);
	int get_type();
	//���������� ������ r � 16 ������ ������
	string get_hex_r();
	//���������� ������ s � 16 ������ ������
	string get_hex_s();

	//������ �������� ������.
	bool parceBin(vector<unsigned char> *data );

	string getLastError();

};

