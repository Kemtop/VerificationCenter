#pragma once

#include <QFileInfo>
#include <streambuf>
#include <QCoreApplication>
#include "CheckSerial.h"
#include "UsbKey.h"

//Интерфейс для взаимодействия с USB ключем.
class  ApiKey
{
public:
	ApiKey();
		
	void InitUsb(); //Инициализирует usb.	
	bool DeviceIsConnected(); //Подключено ли устройство.

	std::string Serial(QString keyName); //Возвращает ключ продукта.
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

private:
	UsbKey usb;
};
