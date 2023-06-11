#include "Biektion.h"

Biektion::Biektion()
{
}

Biektion::~Biektion()
{
}

bool Biektion::CompireArray(bool a[], bool b[], int len)
{
	//Сравниваю биты.
	bool Res = false;
	for (int i = 0; i < len; i++)
	{
		Res = a[i] ^ b[i]; //сложение по модулю 2 возвращает 1 если биты отличаются.
		if (Res) return false; //1-Биты не совпали, ошибка.
	}

	return true;
}

bool Biektion::ChekBiektiv0(bool Y[], bool * ControlBits)
{
	bool hashBits[7];

	hashBits[0] = Y[9];
	hashBits[1] = Y[3];
	hashBits[2] = Y[10];
	hashBits[3] = Y[6];
	hashBits[4] = Y[1];
	hashBits[5] = Y[8];
	hashBits[6] = Y[4];

	bool CBits[7];
	CBits[0] = ControlBits[76];
	CBits[1] = ControlBits[24];
	CBits[2] = ControlBits[75];
	CBits[3] = ControlBits[0];
	CBits[4] = ControlBits[28];
	CBits[5] = ControlBits[58];
	CBits[6] = ControlBits[38];


	//Сравниваю массивы.
	if (!CompireArray(hashBits, CBits, 7))
		return false;

	return true;

}

bool Biektion::ChekBiektiv1(bool Y[], bool * ControlBits)
{
	bool hashBits[7];

	hashBits[0] = Y[5];
	hashBits[1] = Y[11];
	hashBits[2] = Y[1];
	hashBits[3] = Y[6];
	hashBits[4] = Y[10];
	hashBits[5] = Y[7];
	hashBits[6] = Y[0];


	bool CBits[7];
	CBits[0] = ControlBits[44];
	CBits[1] = ControlBits[66];
	CBits[2] = ControlBits[11];
	CBits[3] = ControlBits[6];
	CBits[4] = ControlBits[33];
	CBits[5] = ControlBits[60];
	CBits[6] = ControlBits[34];

	//Сравниваю массивы.
	if (!CompireArray(hashBits, CBits, 7))
		return false;

	return true;
}

bool Biektion::ChekBiektiv2(bool Y[], bool * ControlBits)
{
	//Функция перестановки.
	bool hashBits[7];
	hashBits[0] = Y[6];
	hashBits[1] = Y[2];
	hashBits[2] = Y[8];
	hashBits[3] = Y[11];
	hashBits[4] = Y[3];
	hashBits[5] = Y[0];
	hashBits[6] = Y[1];


	bool CBits[7];
	CBits[0] = ControlBits[52];
	CBits[1] = ControlBits[10];
	CBits[2] = ControlBits[67];
	CBits[3] = ControlBits[1];
	CBits[4] = ControlBits[73];
	CBits[5] = ControlBits[29];
	CBits[6] = ControlBits[2];


	//Сравниваю массивы.
	if (!CompireArray(hashBits, CBits, 7))
		return false;

	return true;
}

bool Biektion::ChekBiektiv3(bool Y[], bool * ControlBits)
{
	bool hashBits[7];
	hashBits[0] = Y[8];
	hashBits[1] = Y[3];
	hashBits[2] = Y[4];
	hashBits[3] = Y[5];
	hashBits[4] = Y[9];
	hashBits[5] = Y[10];
	hashBits[6] = Y[11];


	bool CBits[7];
	CBits[0] = ControlBits[70];
	CBits[1] = ControlBits[54];
	CBits[2] = ControlBits[50];
	CBits[3] = ControlBits[17];
	CBits[4] = ControlBits[41];
	CBits[5] = ControlBits[62];
	CBits[6] = ControlBits[39];
	//Сравниваю массивы.
	if (!CompireArray(hashBits, CBits, 7))
		return false;

	return true;

}

bool Biektion::ChekBiektiv4(bool Y[], bool * ControlBits)
{
	bool hashBits[7];
	bool CBits[7];

	hashBits[0] = Y[7];
	hashBits[1] = Y[5];
	hashBits[2] = Y[11];
	hashBits[3] = Y[1];
	hashBits[4] = Y[8];
	hashBits[5] = Y[3];
	hashBits[6] = Y[9];

	CBits[0] = ControlBits[22];
	CBits[1] = ControlBits[4];
	CBits[2] = ControlBits[53];
	CBits[3] = ControlBits[26];
	CBits[4] = ControlBits[37];
	CBits[5] = ControlBits[31];
	CBits[6] = ControlBits[71];


	//Сравниваю массивы.
	if (!CompireArray(hashBits, CBits, 7))
		return false;

	return true;
}

bool Biektion::ChekBiektiv5(bool Y[], bool * ControlBits)
{
	bool hashBits[7];
	bool CBits[7];


	hashBits[0] = Y[1];
	hashBits[1] = Y[6];
	hashBits[2] = Y[4];
	hashBits[3] = Y[5];
	hashBits[4] = Y[0];
	hashBits[5] = Y[7];
	hashBits[6] = Y[11];

	CBits[0] = ControlBits[16];
	CBits[1] = ControlBits[40];
	CBits[2] = ControlBits[56];
	CBits[3] = ControlBits[5];
	CBits[4] = ControlBits[65];
	CBits[5] = ControlBits[19];
	CBits[6] = ControlBits[49];

	//Сравниваю массивы.
	if (!CompireArray(hashBits, CBits, 7))
		return false;

	return true;
}

bool Biektion::ChekBiektiv6(bool Y[], bool * ControlBits)
{
	bool hashBits[7];
	bool CBits[7];

	hashBits[0] = Y[10];
	hashBits[1] = Y[6];
	hashBits[2] = Y[2];
	hashBits[3] = Y[3];
	hashBits[4] = Y[0];
	hashBits[5] = Y[1];
	hashBits[6] = Y[4];

	//
	CBits[0] = ControlBits[68];
	CBits[1] = ControlBits[25];
	CBits[2] = ControlBits[57];
	CBits[3] = ControlBits[48];
	CBits[4] = ControlBits[7];
	CBits[5] = ControlBits[51];
	CBits[6] = ControlBits[42];


	//Сравниваю массивы.
	if (!CompireArray(hashBits, CBits, 7))
		return false;

	return true;
}

bool Biektion::ChekBiektiv7(bool Y[], bool * ControlBits)
{
	bool hashBits[7];
	bool CBits[7];

	hashBits[0] = Y[11];
	hashBits[1] = Y[5];
	hashBits[2] = Y[1];
	hashBits[3] = Y[7];
	hashBits[4] = Y[0];
	hashBits[5] = Y[3];
	hashBits[6] = Y[8];


	CBits[0] = ControlBits[32];
	CBits[1] = ControlBits[55];
	CBits[2] = ControlBits[13];
	CBits[3] = ControlBits[14];
	CBits[4] = ControlBits[72];
	CBits[5] = ControlBits[8];
	CBits[6] = ControlBits[74];


	//Сравниваю массивы.
	if (!CompireArray(hashBits, CBits, 7))
		return false;

	return true;
}

bool Biektion::ChekBiektiv8(bool Y[], bool * ControlBits)
{
	bool hashBits[7];
	bool CBits[7];

	hashBits[0] = Y[5];
	hashBits[1] = Y[10];
	hashBits[2] = Y[1];
	hashBits[3] = Y[0];
	hashBits[4] = Y[6];
	hashBits[5] = Y[7];
	hashBits[6] = Y[9];

	CBits[0] = ControlBits[3];
	CBits[1] = ControlBits[18];
	CBits[2] = ControlBits[46];
	CBits[3] = ControlBits[23];
	CBits[4] = ControlBits[15];
	CBits[5] = ControlBits[64];
	CBits[6] = ControlBits[12];

	//Сравниваю массивы.
	if (!CompireArray(hashBits, CBits, 7))
		return false;

	return true;
}



