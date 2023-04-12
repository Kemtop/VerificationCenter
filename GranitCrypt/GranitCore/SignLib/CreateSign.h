#pragma once
#include "mpirxx.h"
#include "Point.h"
#include "PointMath.h"
#include "Signature.h"
#include "Groups.h"
#include "hash3411.h"
#include <QRandomGenerator>
#include <iostream>
#include <vector>

using std::vector;
//Класс для создания ЭЦП
class _declspec(dllexport) CreateSign
{
public:
	CreateSign();
	~CreateSign();

	//ГОСТ34.11-2012 А.1 Пример 1 А.1.1.6 Ключ подписи -строковое представление открытого ключа
	static const string gost_d1;
	//Ключ проверки подписи
	static const string gost_Q1x;
	static const string gost_Q1y;
	//ГОСТ34.11-2012 А.1.2 Процесс формирования цифровой подписи (алгоритм I)
	static const string gost_e1;
	static const string gost_k1; //ГОСТ34.11-2012 А.1.2 страница 18
	//Значение векторов А.1.2 Процесс формирования цифровой подписи (алгоритм I)
	static const string gost_r1;
	static const string gost_s1;
	//А.2.1.6 Ключ подписи
	static const string gost_d2;
	//А.2.1.7 Ключ проверки подписи
	static const string gost_Q2x;
	static const string gost_Q2y;
	//А.2.2 Процесс формирования цифровой подписи (алгоритм I)
	static const string gost_e2;
	static const string gost_k2;
	static const string gost_r2;
	static const string gost_s2;

	//Генерирует закрытый ключ на основании открытого
	Point generate_public_key(mpz_class &d, Point &P);
	/*
	  Генерирует закрытый ключ  на основании открытого 512 битного ключа содержащегося в массиве raw_key
	  raw_key -должен иметь длину 64байта
	  В случает если  raw_key больще порядка эллиптической кривой, делаю простое преобразование в виде сдвига в право до тех пор пока
	  не станет меньше порядка кривой q. Возвращается d содержащее "правильный ключ". Возвращается точка эллиптической кривой 
	  являющаяся открытым колючем.
	  string curveOid-OID кривой
	  В случае ошибки возникает исключение.
	*/
	Point generate_public_key_512(uint8_t *raw_key, mpz_class &d,string curveOid);


	//Генерирует случайное число k размерностью q бит, и удовлетворяющее условию 0<k<n;
	mpz_class GetRandom(mpz_class q);
	//Подписывает сообщение M, длиной Mlength-длина в байтах, используя закрытый ключ d и точку P
	Signature Sign(mpz_class &d,uint8_t *M,unsigned long long &Mlength, Point &P);
	//Формирует подпись используя внешние значения e,k. Используется для реализации гост-овских примеров.
	Signature Sign1(mpz_class &d, mpz_class &e, mpz_class &k, Point &P);
	//Выполняет тестирование алгоритма формирования подписи на основании примеров ГОСТ34.11-2012 
	string Test();
	//Выполняет тестирование алгоритма создания открытого ключа для двух примеров в ГОСТ
	string TestPk();

	//Возвращает значение последнего рассчитанного дайджеста сообщения. Значение хэш функции сообщения.  
	vector <unsigned char> getLastDigest();

private:
	 //Значение последнего рассчитанного дайджеста сообщения. Значение хэш функции сообщения.  
	vector <unsigned char> lastDigest;
};

