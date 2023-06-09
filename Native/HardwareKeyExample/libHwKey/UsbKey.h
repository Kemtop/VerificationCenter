#pragma once

#include "libhwkey_global.h"
#include "stdint.h"
#include <vector>
#include "files.h"
#include <QFileInfo>
#include <QDate>
#include "libusb.h"

#define DEV_VID 0x0483  //Идентификатор USB устройства (ключа). 
#define DEV_PID 0x5711

#define DEV_INTERFACE 0x00

/*
  Структура сообщения(верхний уровень см. модель OSI).
  [Команда 1 байт][Данные n байт]

  Структура пакета(транспортный уровень)
  [Команда 1 байт][Номер пакета 1 байт.][Данные n(байт) = MaxPackSize-6 = 58;][Контрольная сумма, 4 байта CRC32]
  */

//Максимальный размер сообщения.
#define MaxMessageSize 0x800  //2048

//Максимальный размер одного пакета, 64 байта.
#define MaxPackSize 0x40

#define ENDP_IN 0x81
#define ENDP_OUT 0x01

#define TIMEOUT 200000

#define BEGIN_YEAR 2019 //Дата относительно которой считается текущая дата (ключ хранит количество дней).
#define BEGIN_MONTH 1
#define BEGIN_DAY 1

#define GET_HW_ID_COMMAND 0x3C //Идентификатор команды «Получить серийный номер устройства».
#define SAVE_SERIAL_COMMAND 0xa0 //Сохранить серийный номер продукта.
#define SAVE_DATE_COMMAND 0x15 //Сохранить дату (в днях).
#define GET_DATE_COMMAND 0x17 //Получить дату (в днях).
#define SAVE_RSA_KEY_COMMAND 0xa9 //Сохранить модуль n открытого ключа rsa (256 байт).
#define GET_CRYPT_SERIAL_COMMAND 0x07 //Получить шифрованный ключом серийный номер продукта.
#define GET_RANDOM_COMMAND 0x50 //Получить случайное число.

#define ANSWER_BAD_CRC 0xFF //Неверный CRC
#define ANSWER_UNKNOWN_COMMAND 0xF0

#define TRANSPORT_START_PACKET_TRANSMISSION 0xb0 //Начать пакетную передачу.
#define TRANSPORT_SENDING_PACKAGE 0xb1 //Отправка пакета.
#define TRANSPORT_EXECUTE_COMMAND 0xb2 //Обработать сообщение на стороне usb устройства.
#define TRANSPORT_GET_RESULT 0xb3 //Получить ответ от USB устройства. Ответ пакетный.

#define SERIAL_NUM_LEN 29 //Длина серийного номера (ключа продукта).

#define CRC_LEN 32 //Длина контрольной суммы.
#define SESSION_KEY_LEN 16 //Длина сеансового ключа.
#define CIPHER_SESSION_KEY_LEN 256 //Длина шифрованного rsa сеансового ключа.
#define CIPHER_PRODUCT_KEY_LEN 32 //Размер блока с зашифрованным ключем продукта.
#define UPDATE_DATE_PERIOD 7 //Период обновления даты ключа.

enum USBERROR
{
	NOT_CONNECTED = -1,
	NO_GET_INTERFACE = -2,
	ERROR_WRITE = -3,
	ERROR_READ = -4,
	UNKNOWN_COMMAND = -5
};

//Интерфейс для взаимодействия с USB ключем.
class  UsbKey
{
public:
	UsbKey();

	//Последнее сообщение об ошибке.
	QString LastError;

	//Проверяет подключено ли устройство.
	bool IsConnected();
	//Загружает модуль n rsa ключа.
	void SendRSAKey(uint8_t key[CIPHER_SESSION_KEY_LEN]);

	std::string GetHWSerial();
	void SetSerial(uint8_t serial[SERIAL_NUM_LEN]); //Сохраняет серийный номер продукта.
	void SetLastDate(uint16_t days);
	void SetLastDate(QDate lastDate); 	//Пишу в аппаратный блок последнюю дату.
		
	uint16_t GetLastDate();
	QDate GetLastHwDate(); //Получаю последнюю дату из аппаратного ключа.

	std::vector<uint16_t> GetRandData(); //Генерация случайного числа.
	std::vector<uint8_t> GetCryptProductSerial(); //Возвращает шифрованный ключ продукта.

	void InitUsb();

private:	
	/*
	   Отправка пакета USB ключу, и получение ответа.
	   command - команда транспортного уровня, 1 байт
	   inBuffer - данные для отправки
	   inLen - длина отправляемых данных
	   outBuffer - данные получаемые от устройства
	   outLen - ожидаемый размер ответа

	   Команды транспортного уровня:
	   0xb0 - начало пакетной передачи.
	   0xb1 - отправка пакета
	   0xb2 - обработать сообщение на стороне usb устройства. Ответ от устройства содержит размер данных, которые будут получены
	   после обработки сообщения. Ответ два байта.
	   0xb3 - получить ответ от USB устройства. Ответ пакетный.
	*/
	int Command(uint8_t command, std::vector<uint8_t> inBuffer, int inLen, std::vector<uint8_t>& outBuffer, int outLen);
	
	/*
	  Метод который отправляет сообщение USB ключу и считывает ответ ключа. 
	  command - команда;
	  inBuffer - данные для передачи;
	  inLen - длина данных которые хотим передать USB ключу;
	  outBuffer - выходной буфер, содержит ответ USB ключа.
	*/
	int Packcom(uint8_t command, std::vector<uint8_t> inBuffer, int inLen, std::vector<uint8_t>& outBuffer);

	//Возвращает пользовательские данные. Удаляет в начале 1 байт – команду.
	std::vector<uint8_t> GetDataFromPacket(std::vector<uint8_t>& outBuffer);
};
