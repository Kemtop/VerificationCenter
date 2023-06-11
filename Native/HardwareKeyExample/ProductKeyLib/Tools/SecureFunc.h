#pragma once
#include <QByteArray>
#include <QDate>

#define SF_LEN 100

class SecureFunc
{
private:

	// Массив для работы метода SF0;
	quint64 ArSF0[SF_LEN];
		
	//Вращения 32 битного кольца. Сдвиг с обратной связью-"Вращение кольца".	
	void TwistTheRing(int rotate, bool *r);
		
	//Подсчитывает количество единиц в массиве длиной len
	int CountUnits(bool val[], int len);
		
	//Заполняет массив  ArSF0 данными некоторой функции.  	
	void FillArSF0();
		
	//Конвертирует битовый массив в UInt64.
	void ConvertBaseNumUint(bool *baseNum, quint64 &result);
		
	//Конвертирует битовый массив в Int64.
	void ConvertBaseNumInt(bool *BaseNum, qint64 &result);
		
	//Преобразовывает 64битное без знаковое число в байт массив.
	void ConvertU64ToBool(quint64 d, bool *Result);

	//Преобразовывает 64битное без знаковое число в байт 32 битный байт массив. 
	void ConvertI64ToBool(qint64 d, bool *Result);

public:

	//Функция для усложнения контрольной суммы.
	void SfCRC(QByteArray &xi);

	//Функция для получения первого блока секретных битов.
	void SF0(bool *BaseNum, bool *Dst);

	//Функция для получения второго блока секретных битов.
	void SF1(bool *BaseNum, bool *Dst);

	//Функция для получения третьего блока секретных битов.
	void SF2(bool *BaseNum, bool *Dst);

    //Функция для получения четвертого блока секретных битов.
	void SF3(bool *BaseNum, bool *Dst);

	void SF4(bool *BaseNum, bool *Dst);
	void SF5(bool *BaseNum, bool *Dst);
	void SF6(bool *BaseNum, bool *Dst);
	void SF7(bool *BaseNum, bool *Dst);
	void SF8(bool *BaseNum, bool *Dst);
};

