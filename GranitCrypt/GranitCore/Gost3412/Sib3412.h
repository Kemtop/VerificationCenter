#pragma once
#include <vector>

#include <iostream>
#include <map>
using std::map;
#include <stdexcept>

#include <vector>
using std::vector;

#include <iostream>
#include <cstring>
using std::cerr;
using std::endl;

#include "Gost3412\Utility.h"

//typedef unsigned char BYTE;
typedef unsigned short WORD;

#define BLOCK_LENGTH 16
using std::vector;


class  Sib3412
{

private:
	//keys — итерационные ключи, которые вычисляются один раз, во время инициализации объекта по заданному ключу.
	vector <ByteBlock> keys;
	//static
	bool is_init;
	
	//Константы перестановки
	static const unsigned char nonlinear_transform_perm[];
	//Константы перестановки
	map <unsigned char, unsigned char> direct_permutation, inverse_permutation;
	//использующиеся для развёртывания ключа.
	vector<ByteBlock> iteration_constants;

	static const vector<WORD> linear_transform_coeff;
	static const WORD linear_transform_modulus;


	// Инициализация  перестановок-контейнеров direct_permutation, inverse_permutation;
	void init_perms();
	void init_consts();
	void init_vectors();

	void nonlinear_transform_direct128(unsigned char * target);
	void nonlinear_transform_inverse128(unsigned char * target);
	WORD multiply(WORD lhs, WORD rhs);
	void xor128(unsigned char * dst, const unsigned char * lhs, const unsigned char * rhs);
	unsigned char linear_transform_core128(const unsigned char * target);
	void linear_transform_direct128(unsigned char * target);
	void linear_transform_inverse128(unsigned char * target);
	void iteration_linear_transform_direct128(unsigned char * target);
	void iteration_linear_transform_inverse128(unsigned char * target);
	void encrypt128(unsigned char * target);
	void decrypt128(unsigned char * target);
	
	void keys_transform128(unsigned char * k1, unsigned char * k2, int iconst);
	void key_derivation128(unsigned char * k1, unsigned char * k2, unsigned char * k3, unsigned char * k4, int ipair);

public:
	/*
	флаг, который указывает, создавались ли когда либо объекты типа Kuznyechik.
	Данный флаг необходим в связи с тем, что на момент запуска программы многие коэффициенты
	и параметры алгоритма отсутствуют. При первой инициализации они высчитываются и хранятся в памяти до завершения программы.
	*/
	static const int block_lenght{ BLOCK_LENGTH };

	Sib3412(const ByteBlock & key);
	Sib3412(const Sib3412 & rhs);
	~Sib3412();
	void encrypt(const ByteBlock & src, ByteBlock & dst);
	void decrypt(const ByteBlock & src, ByteBlock & dst);
	void ECB_Encrypt(const ByteBlock & src, ByteBlock & dst);
	//Шифрование в режиме CFB  режим гаммирования с обратной связью 
	void CFB_Encrypt(const ByteBlock & src, ByteBlock & dst, const ByteBlock & init_vec);
	//Расшифровывание в режиме CFB  режим гаммирования с обратной связью
	void CFB_Decrypt(const ByteBlock & src, ByteBlock & dst, const ByteBlock & init_vec);

};

