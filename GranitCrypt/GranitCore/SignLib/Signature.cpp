#include "stdafx.h"
#include "Signature.h"



Signature::Signature()
{
}


Signature::~Signature()
{
}

void Signature::set_r(mpz_class _r)
{
	r = _r;
}

void Signature::set_s(mpz_class _s)
{
	s = _s;
}

mpz_class Signature::get_r()
{
	return r;
}

mpz_class Signature::get_s()
{
	return s;
}

void Signature::set_type(int t)
{
	sign_type = t;
}

int Signature::get_type()
{
	return sign_type;
}

string Signature::get_hex_r()
{
	return r.get_str(16);
}

string Signature::get_hex_s()
{
	return s.get_str(16);
}

bool Signature::parceBin(vector<unsigned char>* data)
{
	if (data->size() != 128)
	{
		lastError = "Ошибка ЭЦП 01: Размер ЭЦП не равен 128. Сейчас " + std::to_string(data->size());
		return false;
	}
	
	 QByteArray   tba(reinterpret_cast<const char*>(data->data()), data->size());


	 QByteArray s, r; //Вектора подписи полученные из блока данных подписи.

	 r = tba.mid(0,64);
	 s= tba.mid(64, 64);
	 

	//Преобразование векторов QByteArray в mpz_class
	 mpz_class r_;
	 mpz_class s_;

	 size_t r_len = static_cast<size_t>(r.size());
	 size_t s_len = static_cast<size_t>(s.size());

	 mpz_import(r_.get_mpz_t(), r_len, 1, 1, 0, 0, reinterpret_cast<uint8_t *>(r.data()));
	 mpz_import(s_.get_mpz_t(), s_len, 1, 1, 0, 0, reinterpret_cast<uint8_t *>(s.data()));
	 
	 set_r(r_);
	 set_s(s_);
	 set_type(1); //512бит


	return true;
}

string Signature::getLastError()
{
	return lastError;
}
