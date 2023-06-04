#pragma once
#include <QByteArray>
#include <QDate>

class SecureFunc
{
private:
	/// <summary>
	/// Массив для работы метода SF0;
	/// </summary>
	quint64 ArSF0[100];


	/// <summary>
	/// Вращения 32 битного кольца. Сдвиг с обратной связью-"Вращение кольца".
	/// </summary>
	void TwistTheRing(int rotate, bool *R);

	/// <summary>
	/// Подсчитывает количество единиц в массиве длиной len
	/// </summary>
	/// <param name="Val"></param>
	/// <returns></returns>
	int CountUnits(bool Val[], int Len);

	/// <summary>
	/// Заполняет массив  ArSF0 данными некоторой функции.  
	/// </summary>
	void FillArSF0();

	/// <summary>
	/// Конвертирует битовый массив в UInt64.
	/// </summary>
	/// <param name="BaseNum"></param>
	/// <param name="result"></param>
	void ConvertBaseNumUint(bool *BaseNum, quint64 &result);

	/// <summary>
	/// Конвертирует битовый массив в Int64.
	/// </summary>
	/// <param name="BaseNum"></param>
	/// <param name="result"></param>
	void ConvertBaseNumInt(bool *BaseNum, qint64 &result);

	/// <summary>
	/// Преобразовывает 64битное без знаковое число в байт массив. 
	/// </summary>
	/// <param name="Arr"></param>
	/// <param name="Result"></param>
	void ConvertU64ToBool(quint64 d, bool *Result);

	/// <summary>
	/// Преобразовывает 64битное без знаковое число в байт 32 битный байт массив. 
	/// </summary>
	/// <param name="Arr"></param>
	/// <param name="Result"></param>
	void ConvertI64ToBool(qint64 d, bool *Result);

public:
	SecureFunc();
	~SecureFunc();

	// Функция для усложнения контрольной суммы.
	void SfCRC(QByteArray &Xi);

	/// <summary>
	/// Функция для получения первого блока секретных битов.
	/// </summary>
	/// <param name="BaseNum"></param>
	/// <param name="Dst"></param>
	 void SF0(bool *BaseNum, bool *Dst);

	 /// <summary>
	 /// Функция для получения второго блока секретных битов.
	 /// </summary>
	 /// <param name="BaseNum"></param>
	 /// <param name="Dst"></param>
	 void SF1(bool *BaseNum, bool *Dst);


	 /// <summary>
	 /// Функция для получения третьего блока секретных битов.
	 /// </summary>
	 /// <param name="BaseNum"></param>
	 /// <param name="Dst"></param>
	  void SF2(bool *BaseNum, bool *Dst);

	  /// <summary>
	  /// Функция для получения четвертого блока секретных битов.
	  /// </summary>
	  /// <param name="BaseNum"></param>
	  /// <param name="Dst"></param>
	  void SF3(bool *BaseNum, bool *Dst);


      void SF4(bool *BaseNum, bool *Dst);
	  void SF5(bool *BaseNum, bool *Dst);
	  void SF6(bool *BaseNum, bool *Dst);
	  void SF7(bool *BaseNum, bool *Dst);
	  void SF8(bool *BaseNum, bool *Dst);

};

