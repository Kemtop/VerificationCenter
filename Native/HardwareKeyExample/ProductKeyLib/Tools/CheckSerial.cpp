#include "CheckSerial.h"



CheckSerial::CheckSerial()
{
}


CheckSerial::~CheckSerial()
{
}

int CheckSerial::CheckSelfTest(QString ProductKey)
{

	//Проверка контрольной суммы серийного номера.
	if (!CheckKeySum(ProductKey))
		return 1;

	//Непосредственная проверка самого серийного номера
	if (!SerialValidatorSeltTest(ProductKey))
		return 2;

	//Получаю дату окончания действия ключа продукта.  
	QDate endDatePk = GetDateEnd(ProductKey);
	QString str = endDatePk.toString("dd.MM.yyyy"); //"Срок окончания ключа: " 

	QDate curDate = QDateTime::currentDateTime().date();

	if (endDatePk >= curDate)  return 3;

	return 0;
}

int CheckSerial::CheckProductKey(QString ProductKey, std::string &DateEnd)
{
	//Проверка контрольной суммы серийного номера.
	if (!CheckKeySum(ProductKey))

		//Непосредственная проверка самого серийного номера
		if (!SerialValidator(ProductKey))
			return 0;

	//Получаю дату окончания действия ключа продукта.  
	QDate endDatePk = GetDateEnd(ProductKey);
	QDate curDate = QDateTime::currentDateTime().date();

	//Если текущая дата больше даты окончания действия ключа.
	if (endDatePk <= curDate)  return 0;

	QString str = endDatePk.toString("dd.MM.yyyy"); //"Срок окончания ключа: " 
	DateEnd = str.toStdString();

	return 1;
}

QString CheckSerial::GetSnBlock(QString ProductKey, QString & ControlSum)
{
	QStringList list = ProductKey.split('-');
	if (list.size() < 5)	return "";

	//Cам ключ продукта.
	QString pk = "a51" + list.at(0) + "k" + list.at(1) + list.at(2) + list.at(3) + "p";

	ControlSum = list.at(4); //Контрольная сумма.

	return pk;
}

bool CheckSerial::ConvertBaseNum(QBitArray BaseNum, bool * Arr)
{

	//Обязательно! очищаю массив размерности 32 бита.
	for (int i = 0;i < 32;i++) Arr[i] = 0;

	int Cnt = 0;
	//Копирую 
	for (int i = 0; i < 26; i++)
	{
		Arr[i] = BaseNum[i];
		if (Arr[i] == true) Cnt++;
	}

	//Если базовое число содержит более 26 единиц.
	if (Cnt > 26) return false;

	return true;

}

bool CheckSerial::ConvertControlBits(QBitArray BaseNum, bool * Arr)
{
	//Копирую 
	for (int i = 0; i < BaseNum.size(); i++)
	{
		Arr[i] = BaseNum[i];
	}

	return false;
}

bool CheckSerial::CheckKeySum(QString ProductKey)
{
	//Получаю строку для вычисления контрольной суммы и саму контрольную сумму.
	QString SnCrc = "";
	QString str = GetSnBlock(ProductKey, SnCrc);

	if (str.isEmpty()) return false; //Ошибка.

	QByteArray  baCrcPk = str.toUtf8(); //Преобразовываем строковое представление в байт массив.
	SecureFunc Sf;
	Sf.SfCRC(baCrcPk); //Модификация массива согласно алгоритму.

	QByteArray baHash1 = QCryptographicHash::hash(baCrcPk, QCryptographicHash::Sha1);

	QString CrcStr_ = baHash1.toHex();
	CrcStr_ = CrcStr_.toUpper();
	QString CrcStr = CrcStr_.right(5);

	//Сравниваю контрольную сумму.
	if (SnCrc != CrcStr) return false; //Не верная контрольная сумма.

	return true;
}

bool CheckSerial::SerialValidatorSeltTest(QString ProductKey)
{
	QString BaseNum = ""; //36 ричное представление базового числа.
	QString ControlBits = "";  //36 ричное представление контрольных битов.

		   //Выполняю парсинг серийного номера. Разделяю на базовое число и контрольные биты.
	if (!GetSnData(ProductKey, BaseNum, ControlBits))
		return false;

	//Конвертирование 36 ричного базового числа в байт массив.
	Scale36 Ser;
	Ser.FromString(BaseNum);
	QBitArray baBaseNum = Ser.ToBitArray();//Базовый номер в бинарном представлении.

	Scale36 Ser1;
	Ser1.FromString(ControlBits);
	QBitArray baControlBits = Ser1.ToBitArray(); //Контрольные биты в бинарном представлении.

	if (baControlBits.size() > 80) return false;

	bool biteArr[80]; //Преобразовываю в простой массив.
	ConvertControlBits(baControlBits, biteArr);


	bool aBaseNum[32]; //Преобразовываю BitArray  в массив.
	if (!ConvertBaseNum(baBaseNum, aBaseNum)) return false;


	bool Res[9];

	//На основании базового числа,вычисляю значения функций, и сравниваю результаты.
	for (int i = 0;i < 9;i++)
	{
		Res[i] = ChkBits(aBaseNum, biteArr, i);
	}


	//Проверка правильности вычисления блоков.
	for (int i = 0;i < 9;i++)
	{
		if (!Res[i]) return false;
	}


	//Проверяем работу единичных методов.
	Res[0] = ChkBit0(aBaseNum, biteArr);
	Res[1] = ChkBit1(aBaseNum, biteArr);
	Res[2] = ChkBit2(aBaseNum, biteArr);
	Res[3] = ChkBit3(aBaseNum, biteArr);
	Res[4] = ChkBit4(aBaseNum, biteArr);
	Res[5] = ChkBit5(aBaseNum, biteArr);
	Res[6] = ChkBit6(aBaseNum, biteArr);
	Res[7] = ChkBit7(aBaseNum, biteArr);
	Res[8] = ChkBit8(aBaseNum, biteArr);


	//Проверка правильности вычисления блоков.
	for (int i = 0;i < 9;i++)
	{
		if (!Res[i]) return false;
	}


	return true;
}

bool CheckSerial::SerialValidator(QString ProductKey)
{
	QString BaseNum = ""; //36 ричное представление базового числа.
	QString ControlBits = "";  //36 ричное представление контрольных битов.

							   //Выполняю парсинг серийного номера. Разделяю на базовое число и контрольные биты.
	if (!GetSnData(ProductKey, BaseNum, ControlBits))
		return false;

	//Конвертирование 36 ричного базового числа в байт массив.
	Scale36 Ser;
	Ser.FromString(BaseNum);
	QBitArray baBaseNum = Ser.ToBitArray();//Базовый номер в бинарном представлении.

	Scale36 Ser1;
	Ser1.FromString(ControlBits);
	QBitArray baControlBits = Ser1.ToBitArray(); //Контрольные биты в бинарном представлении.

	if (baControlBits.size() > 80) return false;

	bool biteArr[80]; //Преобразовываю в простой массив.
	ConvertControlBits(baControlBits, biteArr);


	bool aBaseNum[32]; //Преобразовываю BitArray  в массив.
	if (!ConvertBaseNum(baBaseNum, aBaseNum)) return false;


	//Проверка правильности вычисления блоков.

	int day = QDateTime::currentDateTime().date().day();
	int modula = day % 9;

	//В зависимости от дня мы проверяем нужный нам блок, тем самым усложняя задачу взломщику с дизасемблером.
	switch (modula)
	{
	case 0:
		return ChkBit0(aBaseNum, biteArr);

	case 1:
		return ChkBit1(aBaseNum, biteArr);

	case 2:
		return ChkBit2(aBaseNum, biteArr);

	case 3:
		return ChkBit3(aBaseNum, biteArr);

	case 4:
		return ChkBit4(aBaseNum, biteArr);

	case 5:
		return ChkBit5(aBaseNum, biteArr);

	case 6:
		return  ChkBit6(aBaseNum, biteArr);

	case 7:
		return  ChkBit7(aBaseNum, biteArr);

	case 8:
		return  ChkBit8(aBaseNum, biteArr);

	case 9:
		return  ChkBit8(aBaseNum, biteArr);

	default:
		return false;
	}


	return true;

}

bool CheckSerial::GetSnData(QString ProductKey, QString & BaseNum, QString & ControlBits)
{
	QStringList list = ProductKey.split('-');

	if (list.size() < 5) //Менее 5 блоков.
	{
		return false;
	}

	BaseNum = list.at(0);
	ControlBits = list.at(1) + list.at(2) + list.at(3);    //Передаю контрольный биты.

	return true;
}

bool CheckSerial::ChkBits(bool *X, bool *ControlBits, int n)
{
	SecureFunc Sf;
	Biektion Be; //Биекции.

	bool Y[32]; //Результат выполнения функции.

	bool res = false; //Результат проверки.

	switch (n)
	{
	case 0:
		Sf.SF0(X, Y); //На основании базового числа получаю первую функцию.
		res = Be.ChekBiektiv0(Y, ControlBits); //Делает обратные перестановки и сравнивает ряды.
		break;

	case 1:
		Sf.SF1(X, Y); //На основании базового числа получаю первую функцию.
		res = Be.ChekBiektiv1(Y, ControlBits); //Делает обратные перестановки и сравнивает ряды.
		break;

	case 2:
		Sf.SF2(X, Y); //На основании базового числа получаю первую функцию.
		res = Be.ChekBiektiv2(Y, ControlBits); //Делает обратные перестановки и сравнивает ряды.
		break;

	case 3:
		Sf.SF3(X, Y); //На основании базового числа получаю первую функцию.
		res = Be.ChekBiektiv3(Y, ControlBits); //Делает обратные перестановки и сравнивает ряды.
		break;

	case 4:
		Sf.SF4(X, Y); //На основании базового числа получаю первую функцию.
		res = Be.ChekBiektiv4(Y, ControlBits); //Делает обратные перестановки и сравнивает ряды.
		break;

	case 5:
		Sf.SF5(X, Y); //На основании базового числа получаю первую функцию.
		res = Be.ChekBiektiv5(Y, ControlBits); //Делает обратные перестановки и сравнивает ряды.
		break;

	case 6:
		Sf.SF6(X, Y); //На основании базового числа получаю первую функцию.
		res = Be.ChekBiektiv6(Y, ControlBits); //Делает обратные перестановки и сравнивает ряды.
		break;

	case 7:
		Sf.SF7(X, Y); //На основании базового числа получаю первую функцию.
		res = Be.ChekBiektiv7(Y, ControlBits); //Делает обратные перестановки и сравнивает ряды.
		break;

	case 8:
		Sf.SF8(X, Y); //На основании базового числа получаю первую функцию.
		res = Be.ChekBiektiv8(Y, ControlBits); //Делает обратные перестановки и сравнивает ряды.
		break;
	}


	return res;
}

bool CheckSerial::ChkBit0(bool *X, bool *ControlBits)
{
	SecureFunc Sf;
	Biektion Be; //Биекции.

	bool Y[32]; //Результат выполнения функции.

	bool res = false; //Результат проверки.


	Sf.SF0(X, Y); //На основании базового числа получаю первую функцию.
	res = Be.ChekBiektiv0(Y, ControlBits); //Делает обратные перестановки и сравнивает ряды.


	return res;
}

bool CheckSerial::ChkBit1(bool *X, bool *ControlBits)
{
	SecureFunc Sf;
	Biektion Be; //Биекции.

	bool Y[32]; //Результат выполнения функции.

	bool res = false; //Результат проверки.

	Sf.SF1(X, Y); //На основании базового числа получаю первую функцию.
	res = Be.ChekBiektiv1(Y, ControlBits); //Делает обратные перестановки и 


	return res;
}

bool CheckSerial::ChkBit2(bool *X, bool *ControlBits)
{
	SecureFunc Sf;
	Biektion Be; //Биекции.

	bool Y[32]; //Результат выполнения функции.

	bool res = false; //Результат проверки.

	Sf.SF2(X, Y); //На основании базового числа получаю первую функцию.
	res = Be.ChekBiektiv2(Y, ControlBits); //Делает обратные перестановки и сравнивает ряды.


	return res;
}

bool CheckSerial::ChkBit3(bool *X, bool *ControlBits)
{
	SecureFunc Sf;
	Biektion Be; //Биекции.

	bool Y[32]; //Результат выполнения функции.

	bool res = false; //Результат проверки.

	Sf.SF3(X, Y); //На основании базового числа получаю первую функцию.
	res = Be.ChekBiektiv3(Y, ControlBits); //Делает обратные перестановки и сравнивает ряды.


	return res;
}

bool CheckSerial::ChkBit4(bool *X, bool *ControlBits)
{
	SecureFunc Sf;
	Biektion Be; //Биекции.

	bool Y[32]; //Результат выполнения функции.

	bool res = false; //Результат проверки.

	Sf.SF4(X, Y); //На основании базового числа получаю первую функцию.
	res = Be.ChekBiektiv4(Y, ControlBits); //Делает обратные перестановки и сравнивает ряды.


	return res;
}

bool CheckSerial::ChkBit5(bool *X, bool *ControlBits)
{
	SecureFunc Sf;
	Biektion Be; //Биекции.

	bool Y[32]; //Результат выполнения функции.

	bool res = false; //Результат проверки.

	Sf.SF5(X, Y); //На основании базового числа получаю первую функцию.
	res = Be.ChekBiektiv5(Y, ControlBits); //Делает обратные перестановки и сравнивает ряды.


	return res;
}

bool CheckSerial::ChkBit6(bool *X, bool *ControlBits)
{
	SecureFunc Sf;
	Biektion Be; //Биекции.

	bool Y[32]; //Результат выполнения функции.

	bool res = false; //Результат проверки.

	Sf.SF6(X, Y); //На основании базового числа получаю первую функцию.
	res = Be.ChekBiektiv6(Y, ControlBits); //Делает обратные перестановки и сравнивает ряды.


	return res;
}

bool CheckSerial::ChkBit7(bool *X, bool *ControlBits)
{
	SecureFunc Sf;
	Biektion Be; //Биекции.

	bool Y[32]; //Результат выполнения функции.

	bool res = false; //Результат проверки.

	Sf.SF7(X, Y); //На основании базового числа получаю первую функцию.
	res = Be.ChekBiektiv7(Y, ControlBits); //Делает обратные перестановки и сравнивает ряды.


	return res;
}

bool CheckSerial::ChkBit8(bool *X, bool *ControlBits)
{
	SecureFunc Sf;
	Biektion Be; //Биекции.

	bool Y[32]; //Результат выполнения функции.

	bool res = false; //Результат проверки.

	Sf.SF8(X, Y); //На основании базового числа получаю первую функцию.
	res = Be.ChekBiektiv8(Y, ControlBits); //Делает обратные перестановки и сравнивает ряды.


	return res;
}

QDate CheckSerial::GetDateEnd(QString ProductKey)
{
	QString BitBlock = GetBits(ProductKey);
	Scale36 Sbits;
	Sbits.FromString(BitBlock);
	QBitArray bits = Sbits.ToBitArray();
	QBitArray dt = QBitArray(14);
	dt[0] = bits[27];
	dt[1] = bits[30];
	dt[2] = bits[36];
	dt[3] = bits[47];
	dt[4] = bits[61];
	dt[5] = bits[45];
	dt[6] = bits[9];
	dt[7] = bits[20];
	dt[8] = bits[43];
	dt[9] = bits[59];
	dt[10] = bits[21];
	dt[11] = bits[35];
	dt[12] = bits[63];
	dt[13] = bits[69];
	int countDay = 0;

	for (int i = 0; i < 14; ++i)
	{
		if (dt[i]) countDay += pow(2, i);
	}

	QDate date = QDate(2019, 1, 1);
	date = date.addDays(countDay);
	return date;
}

QString CheckSerial::GetBits(QString ProductKey)
{
	QStringList list = ProductKey.split('-');
	if (list.size() < 5)	return "";

	QString BitBlock = list.at(1) + list.at(2) + list.at(3);

	return BitBlock;
}
