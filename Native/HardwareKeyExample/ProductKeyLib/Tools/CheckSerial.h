#pragma once
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include "SecureFunc.h"
#include "Biektion.h"
#include "scale36.h"

//����� �������� ��������� ������.
class CheckSerial
{
public:
	CheckSerial();
	~CheckSerial();

	//�������� ����� ��������. ����� ������������ ������ ��� �������!
	int CheckSelfTest(QString ProductKey);

	//�������� ����� ��������.	� ������ ������� ���������� 0, ���� ��� �� ���������� 1.
	//��� �� ���������� ���� ��������� �������� �����, ���� ���� ��������.
	int CheckProductKey(QString ProductKey,std::string &DateEnd);


private:

	/// ������� ������ ��� ���������� ����������� ����� ����� ��������, � ��� �� ���� ����������� �����.
	//� ������ ������� ���������� ������ ������.
	QString GetSnBlock(QString ProductKey,QString &ControlSum);
	
	/// <summary>
	/// ������������ ������� ����� �� BitArray � ������ bool, ������� ���������� ������ � ���������.
	/// </summary>
	/// <param name="BaseNum"></param>
	/// <param name="Arr"></param>
	bool ConvertBaseNum(QBitArray BaseNum,bool *Arr);

	// ������������ ����������� ���� �� BitArray � ������ bool.
	bool ConvertControlBits(QBitArray BaseNum, bool * Arr);

	/// ��������� ����������� ����� ����� ��������.
	bool CheckKeySum(QString ProductKey);

	// ��������� �������� �����. ������������ ������ ��� ���� ������������!
	bool SerialValidatorSeltTest(QString ProductKey);

	// ��������� �������� �����. ������������ ������ ��� ���� ������������!
	bool SerialValidator(QString ProductKey);


	// ��������� ������� ����� ��������.
	bool GetSnData(QString ProductKey, QString &BaseNum, QString &ControlBits);

	/// ���������� ���� ��������� �������� ��������� ������.
	QDate GetDateEnd(QString ProductKey);

	//���������� ���� �������� ����������� ����.
	QString GetBits(QString ProductKey);

	// ��������� ������������ n ����� ����������� ���. ������������ ������ ��� ���� ������������.
	//�� � ���� ������ �� ������������ � ������� ����������! ��� �������� ������� ������������ � ������.
	bool ChkBits(bool *X, bool *ControlBits, int n);

	//��������� �������� ������������ ����� n.
	//����� ��������� ����� ��� ����������� ������������ ��������������.
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

