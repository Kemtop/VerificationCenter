#pragma once
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include "SecureFunc.h"
#include "Biektion.h"
#include "scale36.h"

#define MASK_STR_0 "a51" //Маскирующие строки.
#define MASK_STR_1 "k"
#define MASK_STR_2 "p" 

#define NO_SERIAL_ERROR 0
#define ERROR_BAD_CRC 1
#define ERROR_BAD_SERIAL 2
#define ERROR_KEY_DATE_END 3 //Истек срок действия серийного номера (ключа продукта).

//Класс проверки серийного номера.
class CheckSerial
{
public:
	CheckSerial();

	//Проверка ключа продукта.	В случае проблем возвращает 0, если все ок возвращает 1.
	//Так же возвращает дату окончания действия ключа, если ключ валидный.
	int CheckProductKey(QString productKey, std::string &dateEnd);
	
private:

	//Получаю данные для вычисления контрольной суммы ключа продукта, а так же саму контрольную сумму.
	//В случае проблем возвращает пустую строку.
	QString GetSnBlock(QString productKey,QString &controlSum);

	//Конвертирует базовый номер из BitArray в массив bool, считает количество единиц и проверяет.
	bool ConvertBaseNum(QBitArray baseNum,bool *arr);

	//Конвертирует контрольные биты из BitArray в массив bool.
	bool ConvertControlBits(QBitArray baseNum, bool * arr);

	//Проверяет контрольную сумму ключа продукта.
	bool CheckKeySum(QString productKey);

	//Проверяет серийный номер. Используется только для само тестирования.
	bool SerialValidatorSeltTest(QString productKey);

	//Проверяет серийный номер.
	bool SerialValidator(QString productKey);
	
	//Выполняет парсинг ключа продукта.
	bool GetSnData(QString productKey, QString &baseNum, QString &controlBits);

	//Возвращает дату окончания действия серийного номера.
	QDate GetDateEnd(QString productKey);

	//Возвращает блок хранящий контрольные биты.
	QString GetBits(QString productKey);

	//Проверяет правильность n блока контрольных бит.
	bool ChkBits(bool *x, bool *controlBits, int n);
};

