#pragma once
#include "mpirxx.h"
#include <chrono>
#include <windows.h>

//������ ��������� ���� ������������.
class SkKeeper
{
	mpz_class  user_d;
	bool isKeep;//������ �� ����?
public:
	SkKeeper();
	~SkKeeper();
	//���������� ��������� ����.
	void SetSk(mpz_class d);
	//�������� ��������� ����.
	mpz_class GetSk();

	bool hasKey(); //���� �� ���� � �������?
	//������ ������.
	void Clear();

};

