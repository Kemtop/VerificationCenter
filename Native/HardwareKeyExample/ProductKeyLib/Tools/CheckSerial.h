#pragma once
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include "SecureFunc.h"
#include "Biektion.h"
#include "scale36.h"

#define MASK_STR_0 "a51" //����������� ������.
#define MASK_STR_1 "k"
#define MASK_STR_2 "p" 

#define NO_SERIAL_ERROR 0
#define ERROR_BAD_CRC 1
#define ERROR_BAD_SERIAL 2
#define ERROR_KEY_DATE_END 3 //����� ���� �������� ��������� ������ (����� ��������).

//����� �������� ��������� ������.
class CheckSerial
{
public:
	CheckSerial();

	//�������� ����� ��������.	� ������ ������� ���������� 0, ���� ��� �� ���������� 1.
	//��� �� ���������� ���� ��������� �������� �����, ���� ���� ��������.
	int CheckProductKey(QString productKey, std::string &dateEnd);
	
private:

	//������� ������ ��� ���������� ����������� ����� ����� ��������, � ��� �� ���� ����������� �����.
	//� ������ ������� ���������� ������ ������.
	QString GetSnBlock(QString productKey,QString &controlSum);

	//������������ ������� ����� �� BitArray � ������ bool, ������� ���������� ������ � ���������.
	bool ConvertBaseNum(QBitArray baseNum,bool *arr);

	//������������ ����������� ���� �� BitArray � ������ bool.
	bool ConvertControlBits(QBitArray baseNum, bool * arr);

	//��������� ����������� ����� ����� ��������.
	bool CheckKeySum(QString productKey);

	//��������� �������� �����. ������������ ������ ��� ���� ������������.
	bool SerialValidatorSeltTest(QString productKey);

	//��������� �������� �����.
	bool SerialValidator(QString productKey);
	
	//��������� ������� ����� ��������.
	bool GetSnData(QString productKey, QString &baseNum, QString &controlBits);

	//���������� ���� ��������� �������� ��������� ������.
	QDate GetDateEnd(QString productKey);

	//���������� ���� �������� ����������� ����.
	QString GetBits(QString productKey);

	//��������� ������������ n ����� ����������� ���.
	bool ChkBits(bool *x, bool *controlBits, int n);
};

