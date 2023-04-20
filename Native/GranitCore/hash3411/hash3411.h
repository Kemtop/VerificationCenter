//---------------------------------------------------------------------------

#ifndef hash3411H
#define hash3411H
//---------------------------------------------------------------------------
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <string>

using namespace std;


#define TEST_VECTORS 2
#define MAX_MESSAGE_LENGTH 72 // in bytes

//Класс содержащий методы для генерации ХЭШ функции согласно ГОСТ 34.11-2012
class _declspec(dllexport) Hash3411
{

  private:	// User declarations
	// Matrix A for MixColumns (L) function
	static const unsigned long long A[64];
	// Substitution for SubBytes function
	static const unsigned char Sbox[256];
	// Substitution for Transposition (P) function
	static const unsigned char Tau[64];
	// Constant values for KeySchedule function
    static unsigned char C[12][64];

    void AddModulo512(const unsigned char *a,const unsigned char *b,unsigned char *c);
    void AddXor512(const void *a,const void *b,void *c);
    void S(unsigned char *state);
    void L(unsigned char *state);
    void P(unsigned char *state);
    void KeySchedule(unsigned char *K,int i);
    void E(unsigned char *K,const unsigned char *m, unsigned char *state);
    void g_N(const unsigned char *N,unsigned char *h,const unsigned char *m);
    void hash_X(unsigned char *IV,const unsigned char *message,unsigned long long length,unsigned char *out);


	//Константы для тестирования из ГОСТ
	static const unsigned char Message[TEST_VECTORS][MAX_MESSAGE_LENGTH];
	static const unsigned char Hash_512[TEST_VECTORS][64];
	static const unsigned char Hash_256[TEST_VECTORS][32];
	static const int MessageLength[TEST_VECTORS];


  public:		// User declarations
    void hash_512(const unsigned char *message,unsigned long long length,unsigned char *out);
    void hash_256(const unsigned char *message,unsigned long long length,unsigned char *out);

   //Выполняет тестирование на основе примеров из ГОСТ если возникает ошибка -возвращает сообщение
    string self_testing();

};

#endif
