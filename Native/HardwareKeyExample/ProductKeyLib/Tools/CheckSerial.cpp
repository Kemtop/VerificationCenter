#include "CheckSerial.h"

CheckSerial::CheckSerial()
{
}

//Проверка ключа продукта.	В случае проблем возвращает 0, если все ок возвращает 1.
//Так же возвращает дату окончания действия ключа, если ключ валидный.
int CheckSerial::CheckProductKey(QString productKey, std::string &dateEnd)
{
	//Проверка контрольной суммы серийного номера.
	if (!CheckKeySum(productKey)) return ERROR_BAD_CRC;

	//Непосредственная проверка самого серийного номера
	if (!SerialValidator(productKey)) return ERROR_BAD_SERIAL;

	//Получаю дату окончания действия ключа продукта.  
	QDate endDatePk = GetDateEnd(productKey);
	QDate curDate = QDateTime::currentDateTime().date();

	//Если текущая дата больше даты окончания действия ключа.
	if (endDatePk <= curDate) return ERROR_KEY_DATE_END;

	QString str = endDatePk.toString("dd.MM.yyyy"); //"Срок окончания ключа: " 
	dateEnd = str.toStdString();

	return NO_SERIAL_ERROR;
}

//Получаю данные для вычисления контрольной суммы ключа продукта, а так же саму контрольную сумму.
//В случае проблем возвращает false.
QString CheckSerial::GetSnBlock(QString productKey, QString & controlSum)
{
	QStringList list = productKey.split('-');
	if (list.size() < 5) return "";

	//Cам ключ продукта. 
	QString pk = MASK_STR_0 + list.at(0) + MASK_STR_1 + list.at(1) + list.at(2) + list.at(3) + MASK_STR_2;

	controlSum = list.at(4); //Контрольная сумма.
	controlSum = controlSum.replace(0, ' ').replace(" ", ""); //Контрольная сумма.
	
	return pk;
}

//Конвертирует базовый номер из BitArray в массив bool, считает количество единиц и проверяет.
bool CheckSerial::ConvertBaseNum(QBitArray baseNum, bool * arr)
{
	//Обязательно! очищаю массив размерности 32 бита.
	for (int i = 0;i < 32;i++) arr[i] = 0;

	int Cnt = 0;
	//Копирую 
	for (int i = 0; i < 26; i++)
	{
		arr[i] = baseNum[i];
		if (arr[i] == true) Cnt++;
	}

	//Если базовое число содержит более 26 единиц.
	if (Cnt > 26) return false;

	return true;
}

//Конвертирует контрольные биты из BitArray в массив bool.
bool CheckSerial::ConvertControlBits(QBitArray baseNum, bool * arr)
{
	//Копирую 
	for (int i = 0; i < baseNum.size(); i++)
	{
		arr[i] = baseNum[i];
	}

	return false;
}

//Проверяет контрольную сумму ключа продукта.
bool CheckSerial::CheckKeySum(QString productKey)
{
	//Получаю строку для вычисления контрольной суммы и саму контрольную сумму.
	QString SnCrc = "";
	QString str = GetSnBlock(productKey, SnCrc);

	if (str.isEmpty()) return false; //Ошибка.

	QByteArray  baCrcPk = str.toUtf8(); //Преобразовываем строковое представление в байт массив.
	SecureFunc Sf;
	Sf.SfCRC(baCrcPk); //Модификация массива согласно алгоритму.

	QByteArray baHash1 = QCryptographicHash::hash(baCrcPk, QCryptographicHash::Sha1);

	QString CrcStr = baHash1.toHex().toUpper().right(5);

	//Сравниваю контрольную сумму.
	if (SnCrc.toUpper() != CrcStr) return false; //Неверная контрольная сумма.

	return true;
}

bool CheckSerial::SerialValidatorSeltTest(QString productKey)
{
	QString BaseNum = ""; //36 ричное представление базового числа.
	QString ControlBits = "";  //36 ричное представление контрольных битов.

		   //Выполняю парсинг серийного номера. Разделяю на базовое число и контрольные биты.
	if (!GetSnData(productKey, BaseNum, ControlBits))
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
	   
	return true;
}

bool CheckSerial::SerialValidator(QString productKey)
{
	QString BaseNum = ""; //36 ричное представление базового числа.
	QString ControlBits = "";  //36 ричное представление контрольных битов.

							   //Выполняю парсинг серийного номера. Разделяю на базовое число и контрольные биты.
	if (!GetSnData(productKey, BaseNum, ControlBits))
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
	for (int i = 0; i < 9; i++)
	{
		Res[i] = ChkBits(aBaseNum, biteArr, i);
	}
	
	//Проверка правильности вычисления блоков.
	for (int i = 0; i < 9; i++)
	{
		if (!Res[i]) return false;
	}
	
	return true;
}

//Выполняет парсинг ключа продукта.
bool CheckSerial::GetSnData(QString productKey, QString & baseNum, QString & controlBits)
{
	QStringList list = productKey.split('-');

	if (list.size() < 5) //Менее 5 блоков.
	{
		return false;
	}

	baseNum = list.at(0);
	controlBits = list.at(1) + list.at(2) + list.at(3);    //Передаю контрольный биты.

	return true;
}

//Проверяет правильность n блока контрольных бит.
bool CheckSerial::ChkBits(bool *x, bool *controlBits, int n)
{
	SecureFunc Sf;
	Biektion Be; //Биекции.

	bool y[32]; //Результат выполнения функции.

	bool res = false; //Результат проверки.

	switch (n)
	{
	case 0:
		Sf.SF0(x, y); //На основании базового числа получаю первую функцию.
		res = Be.ChekBiektiv0(y, controlBits); //Делает обратные перестановки и сравнивает ряды.
		break;

	case 1:
		Sf.SF1(x, y); //На основании базового числа получаю первую функцию.
		res = Be.ChekBiektiv1(y, controlBits); //Делает обратные перестановки и сравнивает ряды.
		break;

	case 2:
		Sf.SF2(x, y); //На основании базового числа получаю первую функцию.
		res = Be.ChekBiektiv2(y, controlBits); //Делает обратные перестановки и сравнивает ряды.
		break;

	case 3:
		Sf.SF3(x, y); //На основании базового числа получаю первую функцию.
		res = Be.ChekBiektiv3(y, controlBits); //Делает обратные перестановки и сравнивает ряды.
		break;

	case 4:
		Sf.SF4(x, y); //На основании базового числа получаю первую функцию.
		res = Be.ChekBiektiv4(y, controlBits); //Делает обратные перестановки и сравнивает ряды.
		break;

	case 5:
		Sf.SF5(x, y); //На основании базового числа получаю первую функцию.
		res = Be.ChekBiektiv5(y, controlBits); //Делает обратные перестановки и сравнивает ряды.
		break;

	case 6:
		Sf.SF6(x, y); //На основании базового числа получаю первую функцию.
		res = Be.ChekBiektiv6(y, controlBits); //Делает обратные перестановки и сравнивает ряды.
		break;

	case 7:
		Sf.SF7(x, y); //На основании базового числа получаю первую функцию.
		res = Be.ChekBiektiv7(y, controlBits); //Делает обратные перестановки и сравнивает ряды.
		break;

	case 8:
		Sf.SF8(x, y); //На основании базового числа получаю первую функцию.
		res = Be.ChekBiektiv8(y, controlBits); //Делает обратные перестановки и сравнивает ряды.
		break;
	}
	
	return res;
}

// Возвращает дату окончания действия серийного номера.
QDate CheckSerial::GetDateEnd(QString productKey)
{
	QString BitBlock = GetBits(productKey);
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

//Возвращает блок хранящий контрольные биты.
QString CheckSerial::GetBits(QString productKey)
{
	QStringList list = productKey.split('-');
	if (list.size() < 5) return "";

	QString BitBlock = list.at(1) + list.at(2) + list.at(3);

	return BitBlock;
}
