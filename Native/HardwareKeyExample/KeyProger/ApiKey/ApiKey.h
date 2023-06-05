#pragma once

#include <QFileInfo>
#include <streambuf>
#include <QCoreApplication>
#include "CheckSerial.h"
#include "UsbKey.h"

//��������� ��� �������������� � USB ������.
class  ApiKey
{
public:
	ApiKey();
		
	void InitUsb(); //�������������� usb.	
	bool DeviceIsConnected(); //���������� �� ����������.

	std::string Serial(QString keyName); //���������� ���� ��������.
	std::string HWSerial(); //���������� �������� ����� ����������.
	QDate LastDate();
	void SetLastDate(QDate days); //���� � ���������� ���� ��������� ����.

	
	QString LastError; //��������� ��������� �� ������.

	//��������� ������������ ��������� ������.
	bool CheckValidSerial(QString serial, QString defaultSerial);

	//��������� �������� ����� ��������.
	bool SaveSerial(QString serial, QString defaultSerial);

	//��������� ������ n ����� rsa.
	bool SaveRsaModule(QString keyFileName, QString vectorPath, QString cryptArrayKeyName);

private:
	UsbKey usb;
};
