#include "ScKeeper.h"



SkKeeper::SkKeeper()
{
	isKeep = false; //�� ������ ����.
}


SkKeeper::~SkKeeper()
{
}

void SkKeeper::SetSk(mpz_class d)
{
	user_d=d;
	isKeep = true;
}

mpz_class SkKeeper::GetSk()
{
	return user_d;
}

bool SkKeeper::hasKey()
{
	return isKeep;
}

void SkKeeper::Clear()
{
	//������� ��������� ���������.
	std::chrono::system_clock::time_point time_point;
	time_point = std::chrono::system_clock::now();  //������� ����� 

	std::chrono::system_clock::time_point now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(time_point);
	std::chrono::system_clock::time_point::duration value = now_ms.time_since_epoch();
	long duration = value.count();

	//������� ���� �������� 512����.
	for (int i = 0;i < 512;i++)
	{
		user_d >>= 1;
	}

	user_d = user_d&duration;
	user_d = (duration / 7)+5; //��� ���������� ���������

	isKeep = false;
}
