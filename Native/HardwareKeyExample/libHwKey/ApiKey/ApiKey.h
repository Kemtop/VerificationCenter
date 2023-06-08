#pragma once

#include "integer.h"
#include <QFileInfo>
#include <streambuf>
#include <QCoreApplication>
#include "CheckSerial.h"
#include "UsbKey.h"
#include "stdint.h"
#include <vector>
#include "rsa.h"
#include "files.h"
#include "osrng.h"
#include <QFileInfo>
#include <streambuf>

//��������� ��� �������������� � USB ������.
class  ApiKey
{
public:
	ApiKey();
		
	void InitUsb(); //�������������� usb.	
	bool DeviceIsConnected(); //���������� �� ����������.

	std::string ProductSerial(QString keyName); //���������� ���� ��������.
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

	//��������� � ��������� ���� ��������, ���� ��� � ������� ������������ 1, ����� 0 � ��������� �� ������.
	//����� �������� ����� 10 ���� �� ��������� ����� �������� -���������� errMessage.
	int Che�kProduckKey(std::string &errMessage, uint8_t *phyKey, int len);

	void SendPublicKey(QString rsaKeyFileName);

private:
	UsbKey usb;

	/*
   �������� 32  ����� ���� ������� a(n) = 7^n + 8^n + 9^n.
   0        3
   1		24
   2		194
   3		1584
   4		13058
   5		108624
   6		911234
   7		7703664
   8		65588738
   9		561991824
   10		4843001474
   11		41948320944
   12		364990300418
   13		3188510652624
   14		27953062038914
   15		245823065693424
   16		2167728096132098
   17		19161612027339024  //44 13 62 76 36 A1 10
   18		169737447404391554 //2 5B 07 4F 21 9E CC 82

   ������������ ��� ���� ���������� ���������� rsa.
*/
	void GetCipherKey(QByteArray &ba);

	//�������� (������ xor) ������ in, �� ����� key.
	QByteArray Encrypt(QByteArray & in, QByteArray & key);

	void Decrypt(uint8_t *in, int o_size, QByteArray key);

	//������� ���� RSA.
	bool EncodeRsaKey(QString rsaKeyFileName, QString protectProjectName, QString arrayName);

	//�������� ���� �������� �� ����������� �����, ���������� ���� ���������� rsa.
	std::string GetProductKey(uint8_t *phyKey, int len);

	std::string GetSerial(QString fileName); //���������� ����������� ���� ��������.
};