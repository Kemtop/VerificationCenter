#include "CheckSerial.h"

CheckSerial::CheckSerial()
{
}

//�������� ����� ��������.	� ������ ������� ���������� 0, ���� ��� �� ���������� 1.
//��� �� ���������� ���� ��������� �������� �����, ���� ���� ��������.
int CheckSerial::CheckProductKey(QString productKey, std::string &dateEnd)
{
	//�������� ����������� ����� ��������� ������.
	if (!CheckKeySum(productKey)) return ERROR_BAD_CRC;

	//���������������� �������� ������ ��������� ������
	if (!SerialValidator(productKey)) return ERROR_BAD_SERIAL;

	//������� ���� ��������� �������� ����� ��������.  
	QDate endDatePk = GetDateEnd(productKey);
	QDate curDate = QDateTime::currentDateTime().date();

	//���� ������� ���� ������ ���� ��������� �������� �����.
	if (endDatePk <= curDate) return ERROR_KEY_DATE_END;

	QString str = endDatePk.toString("dd.MM.yyyy"); //"���� ��������� �����: " 
	dateEnd = str.toStdString();

	return NO_SERIAL_ERROR;
}

//������� ������ ��� ���������� ����������� ����� ����� ��������, � ��� �� ���� ����������� �����.
//� ������ ������� ���������� false.
QString CheckSerial::GetSnBlock(QString productKey, QString & controlSum)
{
	QStringList list = productKey.split('-');
	if (list.size() < 5) return "";

	//C�� ���� ��������. 
	QString pk = MASK_STR_0 + list.at(0) + MASK_STR_1 + list.at(1) + list.at(2) + list.at(3) + MASK_STR_2;

	controlSum = list.at(4); //����������� �����.
	controlSum = controlSum.replace(0, ' ').replace(" ", ""); //����������� �����.
	
	return pk;
}

//������������ ������� ����� �� BitArray � ������ bool, ������� ���������� ������ � ���������.
bool CheckSerial::ConvertBaseNum(QBitArray baseNum, bool * arr)
{
	//�����������! ������ ������ ����������� 32 ����.
	for (int i = 0;i < 32;i++) arr[i] = 0;

	int Cnt = 0;
	//������� 
	for (int i = 0; i < 26; i++)
	{
		arr[i] = baseNum[i];
		if (arr[i] == true) Cnt++;
	}

	//���� ������� ����� �������� ����� 26 ������.
	if (Cnt > 26) return false;

	return true;
}

//������������ ����������� ���� �� BitArray � ������ bool.
bool CheckSerial::ConvertControlBits(QBitArray baseNum, bool * arr)
{
	//������� 
	for (int i = 0; i < baseNum.size(); i++)
	{
		arr[i] = baseNum[i];
	}

	return false;
}

//��������� ����������� ����� ����� ��������.
bool CheckSerial::CheckKeySum(QString productKey)
{
	//������� ������ ��� ���������� ����������� ����� � ���� ����������� �����.
	QString SnCrc = "";
	QString str = GetSnBlock(productKey, SnCrc);

	if (str.isEmpty()) return false; //������.

	QByteArray  baCrcPk = str.toUtf8(); //��������������� ��������� ������������� � ���� ������.
	SecureFunc Sf;
	Sf.SfCRC(baCrcPk); //����������� ������� �������� ���������.

	QByteArray baHash1 = QCryptographicHash::hash(baCrcPk, QCryptographicHash::Sha1);

	QString CrcStr = baHash1.toHex().toUpper().right(5);

	//��������� ����������� �����.
	if (SnCrc.toUpper() != CrcStr) return false; //�������� ����������� �����.

	return true;
}

bool CheckSerial::SerialValidatorSeltTest(QString productKey)
{
	QString BaseNum = ""; //36 ������ ������������� �������� �����.
	QString ControlBits = "";  //36 ������ ������������� ����������� �����.

		   //�������� ������� ��������� ������. �������� �� ������� ����� � ����������� ����.
	if (!GetSnData(productKey, BaseNum, ControlBits))
		return false;

	//��������������� 36 ������� �������� ����� � ���� ������.
	Scale36 Ser;
	Ser.FromString(BaseNum);
	QBitArray baBaseNum = Ser.ToBitArray();//������� ����� � �������� �������������.

	Scale36 Ser1;
	Ser1.FromString(ControlBits);
	QBitArray baControlBits = Ser1.ToBitArray(); //����������� ���� � �������� �������������.

	if (baControlBits.size() > 80) return false;

	bool biteArr[80]; //�������������� � ������� ������.
	ConvertControlBits(baControlBits, biteArr);


	bool aBaseNum[32]; //�������������� BitArray  � ������.
	if (!ConvertBaseNum(baBaseNum, aBaseNum)) return false;
	
	bool Res[9];

	//�� ��������� �������� �����,�������� �������� �������, � ��������� ����������.
	for (int i = 0;i < 9;i++)
	{
		Res[i] = ChkBits(aBaseNum, biteArr, i);
	}
	
	//�������� ������������ ���������� ������.
	for (int i = 0;i < 9;i++)
	{
		if (!Res[i]) return false;
	}
	   
	return true;
}

bool CheckSerial::SerialValidator(QString productKey)
{
	QString BaseNum = ""; //36 ������ ������������� �������� �����.
	QString ControlBits = "";  //36 ������ ������������� ����������� �����.

							   //�������� ������� ��������� ������. �������� �� ������� ����� � ����������� ����.
	if (!GetSnData(productKey, BaseNum, ControlBits))
		return false;

	//��������������� 36 ������� �������� ����� � ���� ������.
	Scale36 Ser;
	Ser.FromString(BaseNum);
	QBitArray baBaseNum = Ser.ToBitArray();//������� ����� � �������� �������������.

	Scale36 Ser1;
	Ser1.FromString(ControlBits);
	QBitArray baControlBits = Ser1.ToBitArray(); //����������� ���� � �������� �������������.

	if (baControlBits.size() > 80) return false;

	bool biteArr[80]; //�������������� � ������� ������.
	ConvertControlBits(baControlBits, biteArr);


	bool aBaseNum[32]; //�������������� BitArray  � ������.
	if (!ConvertBaseNum(baBaseNum, aBaseNum)) return false;

	bool Res[9];

	//�� ��������� �������� �����,�������� �������� �������, � ��������� ����������.
	for (int i = 0; i < 9; i++)
	{
		Res[i] = ChkBits(aBaseNum, biteArr, i);
	}
	
	//�������� ������������ ���������� ������.
	for (int i = 0; i < 9; i++)
	{
		if (!Res[i]) return false;
	}
	
	return true;
}

//��������� ������� ����� ��������.
bool CheckSerial::GetSnData(QString productKey, QString & baseNum, QString & controlBits)
{
	QStringList list = productKey.split('-');

	if (list.size() < 5) //����� 5 ������.
	{
		return false;
	}

	baseNum = list.at(0);
	controlBits = list.at(1) + list.at(2) + list.at(3);    //������� ����������� ����.

	return true;
}

//��������� ������������ n ����� ����������� ���.
bool CheckSerial::ChkBits(bool *x, bool *controlBits, int n)
{
	SecureFunc Sf;
	Biektion Be; //�������.

	bool y[32]; //��������� ���������� �������.

	bool res = false; //��������� ��������.

	switch (n)
	{
	case 0:
		Sf.SF0(x, y); //�� ��������� �������� ����� ������� ������ �������.
		res = Be.ChekBiektiv0(y, controlBits); //������ �������� ������������ � ���������� ����.
		break;

	case 1:
		Sf.SF1(x, y); //�� ��������� �������� ����� ������� ������ �������.
		res = Be.ChekBiektiv1(y, controlBits); //������ �������� ������������ � ���������� ����.
		break;

	case 2:
		Sf.SF2(x, y); //�� ��������� �������� ����� ������� ������ �������.
		res = Be.ChekBiektiv2(y, controlBits); //������ �������� ������������ � ���������� ����.
		break;

	case 3:
		Sf.SF3(x, y); //�� ��������� �������� ����� ������� ������ �������.
		res = Be.ChekBiektiv3(y, controlBits); //������ �������� ������������ � ���������� ����.
		break;

	case 4:
		Sf.SF4(x, y); //�� ��������� �������� ����� ������� ������ �������.
		res = Be.ChekBiektiv4(y, controlBits); //������ �������� ������������ � ���������� ����.
		break;

	case 5:
		Sf.SF5(x, y); //�� ��������� �������� ����� ������� ������ �������.
		res = Be.ChekBiektiv5(y, controlBits); //������ �������� ������������ � ���������� ����.
		break;

	case 6:
		Sf.SF6(x, y); //�� ��������� �������� ����� ������� ������ �������.
		res = Be.ChekBiektiv6(y, controlBits); //������ �������� ������������ � ���������� ����.
		break;

	case 7:
		Sf.SF7(x, y); //�� ��������� �������� ����� ������� ������ �������.
		res = Be.ChekBiektiv7(y, controlBits); //������ �������� ������������ � ���������� ����.
		break;

	case 8:
		Sf.SF8(x, y); //�� ��������� �������� ����� ������� ������ �������.
		res = Be.ChekBiektiv8(y, controlBits); //������ �������� ������������ � ���������� ����.
		break;
	}
	
	return res;
}

// ���������� ���� ��������� �������� ��������� ������.
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

//���������� ���� �������� ����������� ����.
QString CheckSerial::GetBits(QString productKey)
{
	QStringList list = productKey.split('-');
	if (list.size() < 5) return "";

	QString BitBlock = list.at(1) + list.at(2) + list.at(3);

	return BitBlock;
}
