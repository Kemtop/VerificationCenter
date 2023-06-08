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

//Интерфейс для взаимодействия с USB ключем.
class  ApiKey
{
public:
	ApiKey();
		
	void InitUsb(); //Инициализирует usb.	
	bool DeviceIsConnected(); //Подключено ли устройство.

	std::string ProductSerial(QString keyName); //Возвращает ключ продукта.
	std::string HWSerial(); //Возвращает серийный номер устройства.
	QDate LastDate();
	void SetLastDate(QDate days); //Пишу в аппаратный блок последнюю дату.
		
	QString LastError; //Последнее сообщение об ошибке.

	//Проверяет корректность серийного номера.
	bool CheckValidSerial(QString serial, QString defaultSerial);

	//Сохраняет серийный номер продукта.
	bool SaveSerial(QString serial, QString defaultSerial);

	//Сохраняет модуль n ключа rsa.
	bool SaveRsaModule(QString keyFileName, QString vectorPath, QString cryptArrayKeyName);

	//Считывает и проверяет ключ продукта, если все в порядке возвращается 1, иначе 0 и сообщение об ошибке.
	//Когда остается менее 10 дней до окончания срока действия -возвращает errMessage.
	int CheсkProduckKey(std::string &errMessage, uint8_t *phyKey, int len);

	void SendPublicKey(QString rsaKeyFileName);

private:
	UsbKey usb;

	/*
   Получает 32  числа ряда функции a(n) = 7^n + 8^n + 9^n.
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

   используется как ключ шифрования контейнера rsa.
*/
	void GetCipherKey(QByteArray &ba);

	//«Шифрует» (просто xor) массив in, на ключе key.
	QByteArray Encrypt(QByteArray & in, QByteArray & key);

	void Decrypt(uint8_t *in, int o_size, QByteArray key);

	//Шифрует ключ RSA.
	bool EncodeRsaKey(QString rsaKeyFileName, QString protectProjectName, QString arrayName);

	//Получает ключ продукта из аппаратного ключа, использует ключ шифрования rsa.
	std::string GetProductKey(uint8_t *phyKey, int len);

	std::string GetSerial(QString fileName); //Возвращает шифрованный ключ продукта.
};