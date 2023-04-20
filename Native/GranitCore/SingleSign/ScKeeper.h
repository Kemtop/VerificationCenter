#pragma once
#include "mpirxx.h"
#include <chrono>
#include <windows.h>

//Хранит секретный ключ пользователя.
class SkKeeper
{
	mpz_class  user_d;
	bool isKeep;//Хранит ли ключ?
public:
	SkKeeper();
	~SkKeeper();
	//Установить секретный ключ.
	void SetSk(mpz_class d);
	//Получить секретный ключ.
	mpz_class GetSk();

	bool hasKey(); //Есть ли ключ в объекте?
	//Чистит память.
	void Clear();

};

