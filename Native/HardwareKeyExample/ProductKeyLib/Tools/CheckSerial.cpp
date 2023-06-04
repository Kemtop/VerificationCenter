#include "CheckSerial.h"



CheckSerial::CheckSerial()
{
}


CheckSerial::~CheckSerial()
{
}

int CheckSerial::CheckSelfTest(QString ProductKey)
{

	//�������� ����������� ����� ��������� ������.
	if (!CheckKeySum(ProductKey))
		return 1;

	//���������������� �������� ������ ��������� ������
	if (!SerialValidatorSeltTest(ProductKey))
		return 2;

	//������� ���� ��������� �������� ����� ��������.  
	QDate endDatePk = GetDateEnd(ProductKey);
	QString str = endDatePk.toString("dd.MM.yyyy"); //"���� ��������� �����: " 

	QDate curDate = QDateTime::currentDateTime().date();

	if (endDatePk >= curDate)  return 3;

	return 0;
}

int CheckSerial::CheckProductKey(QString ProductKey, std::string &DateEnd)
{
	//�������� ����������� ����� ��������� ������.
	if (!CheckKeySum(ProductKey))

		//���������������� �������� ������ ��������� ������
		if (!SerialValidator(ProductKey))
			return 0;

	//������� ���� ��������� �������� ����� ��������.  
	QDate endDatePk = GetDateEnd(ProductKey);
	QDate curDate = QDateTime::currentDateTime().date();

	//���� ������� ���� ������ ���� ��������� �������� �����.
	if (endDatePk <= curDate)  return 0;

	QString str = endDatePk.toString("dd.MM.yyyy"); //"���� ��������� �����: " 
	DateEnd = str.toStdString();

	return 1;
}

QString CheckSerial::GetSnBlock(QString ProductKey, QString & ControlSum)
{
	QStringList list = ProductKey.split('-');
	if (list.size() < 5)	return "";

	//C�� ���� ��������.
	QString pk = "a51" + list.at(0) + "k" + list.at(1) + list.at(2) + list.at(3) + "p";

	ControlSum = list.at(4); //����������� �����.

	return pk;
}

bool CheckSerial::ConvertBaseNum(QBitArray BaseNum, bool * Arr)
{

	//�����������! ������ ������ ����������� 32 ����.
	for (int i = 0;i < 32;i++) Arr[i] = 0;

	int Cnt = 0;
	//������� 
	for (int i = 0; i < 26; i++)
	{
		Arr[i] = BaseNum[i];
		if (Arr[i] == true) Cnt++;
	}

	//���� ������� ����� �������� ����� 26 ������.
	if (Cnt > 26) return false;

	return true;

}

bool CheckSerial::ConvertControlBits(QBitArray BaseNum, bool * Arr)
{
	//������� 
	for (int i = 0; i < BaseNum.size(); i++)
	{
		Arr[i] = BaseNum[i];
	}

	return false;
}

bool CheckSerial::CheckKeySum(QString ProductKey)
{
	//������� ������ ��� ���������� ����������� ����� � ���� ����������� �����.
	QString SnCrc = "";
	QString str = GetSnBlock(ProductKey, SnCrc);

	if (str.isEmpty()) return false; //������.

	QByteArray  baCrcPk = str.toUtf8(); //��������������� ��������� ������������� � ���� ������.
	SecureFunc Sf;
	Sf.SfCRC(baCrcPk); //����������� ������� �������� ���������.

	QByteArray baHash1 = QCryptographicHash::hash(baCrcPk, QCryptographicHash::Sha1);

	QString CrcStr_ = baHash1.toHex();
	CrcStr_ = CrcStr_.toUpper();
	QString CrcStr = CrcStr_.right(5);

	//��������� ����������� �����.
	if (SnCrc != CrcStr) return false; //�� ������ ����������� �����.

	return true;
}

bool CheckSerial::SerialValidatorSeltTest(QString ProductKey)
{
	QString BaseNum = ""; //36 ������ ������������� �������� �����.
	QString ControlBits = "";  //36 ������ ������������� ����������� �����.

		   //�������� ������� ��������� ������. �������� �� ������� ����� � ����������� ����.
	if (!GetSnData(ProductKey, BaseNum, ControlBits))
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


	//��������� ������ ��������� �������.
	Res[0] = ChkBit0(aBaseNum, biteArr);
	Res[1] = ChkBit1(aBaseNum, biteArr);
	Res[2] = ChkBit2(aBaseNum, biteArr);
	Res[3] = ChkBit3(aBaseNum, biteArr);
	Res[4] = ChkBit4(aBaseNum, biteArr);
	Res[5] = ChkBit5(aBaseNum, biteArr);
	Res[6] = ChkBit6(aBaseNum, biteArr);
	Res[7] = ChkBit7(aBaseNum, biteArr);
	Res[8] = ChkBit8(aBaseNum, biteArr);


	//�������� ������������ ���������� ������.
	for (int i = 0;i < 9;i++)
	{
		if (!Res[i]) return false;
	}


	return true;
}

bool CheckSerial::SerialValidator(QString ProductKey)
{
	QString BaseNum = ""; //36 ������ ������������� �������� �����.
	QString ControlBits = "";  //36 ������ ������������� ����������� �����.

							   //�������� ������� ��������� ������. �������� �� ������� ����� � ����������� ����.
	if (!GetSnData(ProductKey, BaseNum, ControlBits))
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


	//�������� ������������ ���������� ������.

	int day = QDateTime::currentDateTime().date().day();
	int modula = day % 9;

	//� ����������� �� ��� �� ��������� ������ ��� ����, ��� ����� �������� ������ ��������� � �������������.
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

	if (list.size() < 5) //����� 5 ������.
	{
		return false;
	}

	BaseNum = list.at(0);
	ControlBits = list.at(1) + list.at(2) + list.at(3);    //������� ����������� ����.

	return true;
}

bool CheckSerial::ChkBits(bool *X, bool *ControlBits, int n)
{
	SecureFunc Sf;
	Biektion Be; //�������.

	bool Y[32]; //��������� ���������� �������.

	bool res = false; //��������� ��������.

	switch (n)
	{
	case 0:
		Sf.SF0(X, Y); //�� ��������� �������� ����� ������� ������ �������.
		res = Be.ChekBiektiv0(Y, ControlBits); //������ �������� ������������ � ���������� ����.
		break;

	case 1:
		Sf.SF1(X, Y); //�� ��������� �������� ����� ������� ������ �������.
		res = Be.ChekBiektiv1(Y, ControlBits); //������ �������� ������������ � ���������� ����.
		break;

	case 2:
		Sf.SF2(X, Y); //�� ��������� �������� ����� ������� ������ �������.
		res = Be.ChekBiektiv2(Y, ControlBits); //������ �������� ������������ � ���������� ����.
		break;

	case 3:
		Sf.SF3(X, Y); //�� ��������� �������� ����� ������� ������ �������.
		res = Be.ChekBiektiv3(Y, ControlBits); //������ �������� ������������ � ���������� ����.
		break;

	case 4:
		Sf.SF4(X, Y); //�� ��������� �������� ����� ������� ������ �������.
		res = Be.ChekBiektiv4(Y, ControlBits); //������ �������� ������������ � ���������� ����.
		break;

	case 5:
		Sf.SF5(X, Y); //�� ��������� �������� ����� ������� ������ �������.
		res = Be.ChekBiektiv5(Y, ControlBits); //������ �������� ������������ � ���������� ����.
		break;

	case 6:
		Sf.SF6(X, Y); //�� ��������� �������� ����� ������� ������ �������.
		res = Be.ChekBiektiv6(Y, ControlBits); //������ �������� ������������ � ���������� ����.
		break;

	case 7:
		Sf.SF7(X, Y); //�� ��������� �������� ����� ������� ������ �������.
		res = Be.ChekBiektiv7(Y, ControlBits); //������ �������� ������������ � ���������� ����.
		break;

	case 8:
		Sf.SF8(X, Y); //�� ��������� �������� ����� ������� ������ �������.
		res = Be.ChekBiektiv8(Y, ControlBits); //������ �������� ������������ � ���������� ����.
		break;
	}


	return res;
}

bool CheckSerial::ChkBit0(bool *X, bool *ControlBits)
{
	SecureFunc Sf;
	Biektion Be; //�������.

	bool Y[32]; //��������� ���������� �������.

	bool res = false; //��������� ��������.


	Sf.SF0(X, Y); //�� ��������� �������� ����� ������� ������ �������.
	res = Be.ChekBiektiv0(Y, ControlBits); //������ �������� ������������ � ���������� ����.


	return res;
}

bool CheckSerial::ChkBit1(bool *X, bool *ControlBits)
{
	SecureFunc Sf;
	Biektion Be; //�������.

	bool Y[32]; //��������� ���������� �������.

	bool res = false; //��������� ��������.

	Sf.SF1(X, Y); //�� ��������� �������� ����� ������� ������ �������.
	res = Be.ChekBiektiv1(Y, ControlBits); //������ �������� ������������ � 


	return res;
}

bool CheckSerial::ChkBit2(bool *X, bool *ControlBits)
{
	SecureFunc Sf;
	Biektion Be; //�������.

	bool Y[32]; //��������� ���������� �������.

	bool res = false; //��������� ��������.

	Sf.SF2(X, Y); //�� ��������� �������� ����� ������� ������ �������.
	res = Be.ChekBiektiv2(Y, ControlBits); //������ �������� ������������ � ���������� ����.


	return res;
}

bool CheckSerial::ChkBit3(bool *X, bool *ControlBits)
{
	SecureFunc Sf;
	Biektion Be; //�������.

	bool Y[32]; //��������� ���������� �������.

	bool res = false; //��������� ��������.

	Sf.SF3(X, Y); //�� ��������� �������� ����� ������� ������ �������.
	res = Be.ChekBiektiv3(Y, ControlBits); //������ �������� ������������ � ���������� ����.


	return res;
}

bool CheckSerial::ChkBit4(bool *X, bool *ControlBits)
{
	SecureFunc Sf;
	Biektion Be; //�������.

	bool Y[32]; //��������� ���������� �������.

	bool res = false; //��������� ��������.

	Sf.SF4(X, Y); //�� ��������� �������� ����� ������� ������ �������.
	res = Be.ChekBiektiv4(Y, ControlBits); //������ �������� ������������ � ���������� ����.


	return res;
}

bool CheckSerial::ChkBit5(bool *X, bool *ControlBits)
{
	SecureFunc Sf;
	Biektion Be; //�������.

	bool Y[32]; //��������� ���������� �������.

	bool res = false; //��������� ��������.

	Sf.SF5(X, Y); //�� ��������� �������� ����� ������� ������ �������.
	res = Be.ChekBiektiv5(Y, ControlBits); //������ �������� ������������ � ���������� ����.


	return res;
}

bool CheckSerial::ChkBit6(bool *X, bool *ControlBits)
{
	SecureFunc Sf;
	Biektion Be; //�������.

	bool Y[32]; //��������� ���������� �������.

	bool res = false; //��������� ��������.

	Sf.SF6(X, Y); //�� ��������� �������� ����� ������� ������ �������.
	res = Be.ChekBiektiv6(Y, ControlBits); //������ �������� ������������ � ���������� ����.


	return res;
}

bool CheckSerial::ChkBit7(bool *X, bool *ControlBits)
{
	SecureFunc Sf;
	Biektion Be; //�������.

	bool Y[32]; //��������� ���������� �������.

	bool res = false; //��������� ��������.

	Sf.SF7(X, Y); //�� ��������� �������� ����� ������� ������ �������.
	res = Be.ChekBiektiv7(Y, ControlBits); //������ �������� ������������ � ���������� ����.


	return res;
}

bool CheckSerial::ChkBit8(bool *X, bool *ControlBits)
{
	SecureFunc Sf;
	Biektion Be; //�������.

	bool Y[32]; //��������� ���������� �������.

	bool res = false; //��������� ��������.

	Sf.SF8(X, Y); //�� ��������� �������� ����� ������� ������ �������.
	res = Be.ChekBiektiv8(Y, ControlBits); //������ �������� ������������ � ���������� ����.


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
