#pragma once

#include "mpirxx.h"
#include <string>
#include <vector>
#include <QByteArray>
#include "windows.h"

using std::string;
using std::vector;

//Цифровая подпись
class Signature
{
private:
	//Вектора подписи
	mpz_class r;
	mpz_class s;
	int sign_type; //Тип подписи 0-256 1-512

	string lastError;
public:
	Signature();
	~Signature();
	
	void set_r(mpz_class _r);
	void set_s(mpz_class _s);
	mpz_class get_r();
	mpz_class get_s();
	//Устанавливает тип подписи
	void set_type(int t);
	int get_type();
	//Возвращает вектор r в 16 ричной записи
	string get_hex_r();
	//Возвращает вектор s в 16 ричной записи
	string get_hex_s();

	//Парсит бинарные данные.
	bool parceBin(vector<unsigned char> *data );

	string getLastError();

};

