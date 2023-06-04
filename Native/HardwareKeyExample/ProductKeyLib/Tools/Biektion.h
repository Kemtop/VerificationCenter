#pragma once
class Biektion
{
public:
	Biektion();
	~Biektion();

	// Сравниваю два массива размерностью len. Если хотя бы один бит отличается в массивах-возвращаю false.
	bool CompireArray(bool a[], bool b[], int len);

	//void Biektiv0(bool *Bits, bool block1[]);
	// Выполняет перестановку значения функции Y, обратную биекцию ряда контрольных бит и побитное сравнение.
	bool ChekBiektiv0(bool  Y[],bool *ControlBits);

	// Выполняет перестановку значения функции Y, обратную биекцию ряда контрольных бит и побитное сравнение.
	bool ChekBiektiv1(bool  Y[], bool *ControlBits);

	// Выполняет перестановку значения функции Y, обратную биекцию ряда контрольных бит и побитное сравнение.
	bool ChekBiektiv2(bool  Y[], bool *ControlBits);

	// Выполняет перестановку значения функции Y, обратную биекцию ряда контрольных бит и побитное сравнение.
	bool ChekBiektiv3(bool  Y[], bool *ControlBits);

	// Выполняет перестановку значения функции Y, обратную биекцию ряда контрольных бит и побитное сравнение.
	bool ChekBiektiv4(bool  Y[], bool *ControlBits);

	// Выполняет перестановку значения функции Y, обратную биекцию ряда контрольных бит и побитное сравнение.
	bool ChekBiektiv5(bool  Y[], bool *ControlBits);

	// Выполняет перестановку значения функции Y, обратную биекцию ряда контрольных бит и побитное сравнение.
	bool ChekBiektiv6(bool  Y[], bool *ControlBits);

	// Выполняет перестановку значения функции Y, обратную биекцию ряда контрольных бит и побитное сравнение.
	bool ChekBiektiv7(bool  Y[], bool *ControlBits);

	// Выполняет перестановку значения функции Y, обратную биекцию ряда контрольных бит и побитное сравнение.
	bool ChekBiektiv8(bool  Y[], bool *ControlBits);

};

