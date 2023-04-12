#pragma once

#include "libcn3q_global.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

#define TEST_VECTORS 2
#define MAX_MESSAGE_LENGTH 72 // in bytes

//LIBCN3Q_EXPORT
class  libCn3q
{
private:
	unsigned char doVal;
	unsigned char TpPoint;
	bool getNoStatic;

	//Константы для тестирования из ГОСТ
	static const unsigned char Message[TEST_VECTORS][MAX_MESSAGE_LENGTH];
	static const unsigned char Hash_512[TEST_VECTORS][64];
	static const unsigned char Hash_256[TEST_VECTORS][32];
	static const int MessageLength[TEST_VECTORS];

	// Matrix A for MixColumns (L) function
	static const unsigned long long A[64];
	// Substitution for SubBytes function
	static const unsigned char Sbox[256];
	// Substitution for Transposition (P) function
	static const unsigned char Tau[64];
	// Constant values for KeySchedule function
	static unsigned char C[12][64];

	void AddModulo512(const unsigned char *a, const unsigned char *b, unsigned char *c);
	void AddXor512(const void *a, const void *b, void *c);
	void S(unsigned char *state);
	void L(unsigned char *state);
	void P(unsigned char *state);
	void KeySchedule(unsigned char *K, int i);
	void E(unsigned char *K, const unsigned char *m, unsigned char *state);
	void g_N(const unsigned char *N, unsigned char *h, const unsigned char *m);
	void hash_X(unsigned char *IV, const unsigned char *message, unsigned long long length, unsigned char *out);


public:		// User declarations
	libCn3q();
	//Получает константу из массива Message, где val-позиция в массиве. Используется только для запутывания злоумышленника.
	unsigned char getNoLinear1(int a, int b);
	
	//Получает константу из массива Hash_512, где val-позиция в массиве. Используется только для запутывания злоумышленника.
	unsigned char getNoLinear2(int a, int b);

	//Получает константу из массива Hash_256, где val-позиция в массиве. Используется только для запутывания злоумышленника.
	unsigned char getNoLinear3(int a, int b);

	//Получает константу из массива MessageLength, где val-позиция в массиве. Используется только для запутывания злоумышленника.
	unsigned char rawMultiply(int val);

	//Возвращает константу TEST_VECTORS.
	unsigned char noGhxEnd();

	//Возвращает константу MAX_MESSAGE_LENGTH.
	unsigned char isGdxHandle();

	void setTpPoint(unsigned int val);

	void riseEdge();
	void litEdge();

	unsigned int IMA_MAP_ANPOINT();
	unsigned int IMA_MAP_CURV();
	unsigned int IMA_MAP_EXIT();
	unsigned int IMA_MAP_MUXFIRST();
	unsigned int IMA_MAP_Z_FLAG(); //Вернет 25.
	unsigned int IMA_MAP_X_FLAG();
	unsigned int IMA_MAP_HANDLE_POINT();
	unsigned int IMA_MAP_SLEW();
	unsigned int IMA_MAP_GEAR_B();
	unsigned int IMA_MAP_DIR_SIGMA();
	unsigned int IMA_MAP_PPS();

	unsigned int TTN_UPX_MASK();
	unsigned int TTN_UPX_SHIFT();
	unsigned int TTN_UPX_ANY();
	unsigned int TTN_UPX_A1_SHIFT();
	unsigned int TTN_UPX_ARP();
	unsigned int TTN_UPX_ATTR();
	unsigned int TTN_UPX_D_HA();
	unsigned int TTN_UPX_PROPERTY();
	unsigned int TTN_UPX_PIC();
	unsigned int TTN_UPX_FREE();
	unsigned int TTN_UPX_STR_A();
	unsigned int TTN_UPX_STR_W();
	unsigned int TTN_UPX_SPEC();
	unsigned int TTN_UPX_USAGE();
	unsigned int TTN_UPX_WARNING();
	unsigned int TTN_UPX_BODY();
	unsigned int TTN_UPX_OFSPACE();
	unsigned int TTN_UPX_EXISTING();
	unsigned int TTN_UPX_SIGNATURE();
	unsigned int TTN_UPX_IDENTIFIER();
	unsigned int TTN_UPX_KIND();
	unsigned int TTN_UPX_C();
	unsigned int TTN_UPX_Y();
	unsigned int TTN_UPX_INFO();
	unsigned int TTN_UPX_HH_STR();
	unsigned int TTN_UPX_L();

	void hash_512(const unsigned char *message, unsigned long long length, unsigned char *out);
	void hash_256(const unsigned char *message, unsigned long long length, unsigned char *out);

	//Выполняет тестирование на основе примеров из ГОСТ если возникает ошибка -возвращает сообщение
	string self_testing();

	int getArrA(unsigned int *arr);

	void TwistTheRing(int rotate, unsigned char *Ring); //Вращение байт в 32 байтном кольце.

	//Формирует последовательность треугольных чисел.
	/*
	Треугольное число — один из типов фигурных чисел, определяемый как число точек, которые могут быть расставлены 
	 в форме правильного треугольника.
    	Очевидно, с чисто арифметической точки зрения, n-е треугольное число — это сумма n первых натуральных чисел.
	*/
	int createTriangularNumbers(unsigned int *series,int n);

};

/*
11,15,3,9,25,136,65,26,111,63,17,55,253,112,7,96,31,22,59,80,217,92,73,7,85,99,9,10,1,220,201,2,136,139,3,13,65.
*/

//Значения упаковщика контейнера ключа.
__declspec(dllexport)
int AreApisANSI(char *Linear, int *Len, char *Value)
{
	libCn3q LS;
	unsigned int ar1[37];
	LS.getArrA(ar1); //Получаю некоторый набор констант.

	//Привожу к однобайтному числу.
	unsigned char series[35];
	unsigned int tmp = 0;
	unsigned char tmp1 = 0;

	unsigned char count = 0;
	for (int i = 0;i < 35;i++)
	{
		tmp = ar1[i];
		tmp = tmp & 0xff; //Обрезал лишнее.
		tmp1 = static_cast<unsigned char>(tmp);
		series[i] = tmp1;
		count++;
	}

	unsigned char out[32];
	unsigned long long length = static_cast<unsigned long long>(count);
	LS.hash_256(series,length,out); 

	int y = 1;
	y = y << 4;
	int rotate = y;
	rotate++;
	LS.TwistTheRing(rotate, out);
	rotate = rotate + 6;

	std::stringstream ss;
	ss << std::hex << rotate;
	for (int i = 0; i<32; i++)
		ss << std::hex <<static_cast<int>(out[i]);


	std::string valueStr= ss.str();
	
	string v1 = valueStr;

	strcpy(Linear, v1.c_str());

	return 16537;
}


//Значения упаковщика макета.
__declspec(dllexport)
int ArrangeIWC(char *Pointer)
{
	libCn3q LS;
	unsigned int  ser[32];
	LS.createTriangularNumbers(ser, 32);
	unsigned char  series[32];


	unsigned char count = 0;
	unsigned int tmp = 0;
	unsigned char tmp1 = 0;

	for (int i = 0;i < 32;i++)
	{
		tmp = ser[i];
		tmp = tmp & 0xff; //Обрезал лишнее.
		tmp1 = static_cast<unsigned char>(tmp);
		series[i] = tmp1;
		count++;
	}


	unsigned char out[32];
 	unsigned long long length = static_cast<unsigned long long>(count);
	LS.hash_256(series, length, out);

	std::stringstream ss;

	for (int i = 0; i<32; i++)
		ss << std::hex << static_cast<int>(out[i]);

	std::string valueStr = ss.str();
	string v2 = valueStr;


	strcpy(Pointer, v2.c_str());
	return 32768;
}

//Устанавливает некоторое значение адреса.
__declspec(dllexport)
unsigned int setTpPointA(unsigned int val)
{
	libCn3q LS;
	LS.setTpPoint(val);
	return 75;
}


__declspec(dllexport)
unsigned int environmentGradient(unsigned int val)
{
	if (val == 128)
	{
		//Запускаю тест.
		libCn3q LS;
		string ret=LS.self_testing();
		if (!ret.empty()) return 0;
	}
	else
	{
		return 0;
	}
	
	return 1;
}