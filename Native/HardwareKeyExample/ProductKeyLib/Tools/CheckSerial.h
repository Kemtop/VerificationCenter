#pragma once
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include "SecureFunc.h"
#include "Biektion.h"
#include "scale36.h"

//Класс проверки серийного номера.
class CheckSerial
{
public:
	CheckSerial();
	~CheckSerial();

	//Проверка ключа продукта. Метод использовать только для отладки!
	int CheckSelfTest(QString ProductKey);

	//Проверка ключа продукта.	В случае проблем возвращает 0, если все ок возвращает 1.
	//Так же возвращает дату окончания действия ключа, если ключ валидный.
	int CheckProductKey(QString ProductKey,std::string &DateEnd);


private:

	/// Получаю данные для вычисления контрольной суммы ключа продукта, а так же саму контрольную сумму.
	//В случае проблем возвращает пустую строку.
	QString GetSnBlock(QString ProductKey,QString &ControlSum);
	
	/// <summary>
	/// Конвертирует базовый номер из BitArray в массив bool, считает количество единиц и проверяет.
	/// </summary>
	/// <param name="BaseNum"></param>
	/// <param name="Arr"></param>
	bool ConvertBaseNum(QBitArray BaseNum,bool *Arr);

	// Конвертирует контрольные биты из BitArray в массив bool.
	bool ConvertControlBits(QBitArray BaseNum, bool * Arr);

	/// Проверяет контрольную сумму ключа продукта.
	bool CheckKeySum(QString ProductKey);

	// Проверяет серийный номер. Используется только для само тестирования!
	bool SerialValidatorSeltTest(QString ProductKey);

	// Проверяет серийный номер. Используется только для само тестирования!
	bool SerialValidator(QString ProductKey);


	// Выполняет парсинг ключа продукта.
	bool GetSnData(QString ProductKey, QString &BaseNum, QString &ControlBits);

	/// Возвращает дату окончания действия серийного номера.
	QDate GetDateEnd(QString ProductKey);

	//Возвращает блок хранящий контрольные биты.
	QString GetBits(QString ProductKey);

	// Проверяет правильность n блока контрольных бит. Используется только для само тестирования.
	//Ни в коем случае не использовать в рабочем приложении! Это серьезно снижает устойчивость к взлому.
	bool ChkBits(bool *X, bool *ControlBits, int n);

	//Выполняет проверку контрольного блока n.
	//Такая сложность нужна для запутывания дизасемблера злоумышленника.
	bool ChkBit0(bool *X, bool *ControlBits);
	bool ChkBit1(bool *X, bool *ControlBits);
	bool ChkBit2(bool *X, bool *ControlBits);
	bool ChkBit3(bool *X, bool *ControlBits);
	bool ChkBit4(bool *X, bool *ControlBits);
	bool ChkBit5(bool *X, bool *ControlBits);
	bool ChkBit6(bool *X, bool *ControlBits);
	bool ChkBit7(bool *X, bool *ControlBits);
	bool ChkBit8(bool *X, bool *ControlBits);


};

