#pragma once

#include "libhwkey_global.h"
#include "stdint.h"
#include <vector>
#include "rsa.h"
#include "files.h"
#include "osrng.h"
#include <QFileInfo>
#include <streambuf>




#define DEV_VID 0x0483
#define DEV_PID 0x5711

#define DEV_INTERFACE 0x00


//using namespace CryptoPP;

/*
  Структура сообщения(верхний уровень см. модель OSI).
  [Команда 1 байт][Данные n байт]

  Структура пакета(транспортный уровень)
  [Команда 1 байт][Номер пакета 1 байт.][Данные n(байт)=MaxPackSize-6=58;][Контрольная сумма,4 байта CRC32]
  */


//Максимальный размер сообщения.
#define MaxMessageSize 0x800  //2048

//Максимальный размер одного пакета, 64 байта.
#define MaxPackSize 0x40

#define ENDP_IN 0x81
#define ENDP_OUT 0x01

#define TIMEOUT 200000

enum USBERROR
{
	NOT_CONNECTED = -1,
	NO_GET_INTERFACE = -2,
	ERROR_WRITE = -3,
	ERROR_READ = -4,
	UNKNOWN_COMMAND = -5
};

//
//Интерфейс для взаимодействия с USB ключем.
//LIBHWKEY_EXPORT
class  UsbKey
{
public:
	UsbKey();
	//Проверяет подключено ли устройство.
	bool isConnected();

	void sendRSAKey(uint8_t key[256]);
	std::string getSerial();




	std::string getHWSerial();
	void setSerial(uint8_t serial[29]);
	void setLastDate(uint16_t days);
	//Пишу в аппаратный блок последнюю дату.
	void setLastDate(QDate lastDate);

	
	uint16_t getLastDate();
	QDate getLastHwDate(); //Получаю последнюю дату из аппаратного ключа.

	std::vector<uint16_t> getRandData();

	void SendPublicKey();
	void InitUsb();

	//Транслирует ключ шифрования канального уровня.
	int translateChannelKey();

	//Считывает и проверяет ключ продукта, если все в порядке возвращается 1, иначе 0 и сообщение об ошибке.
	//Когда остается менее 10 дней до окончания срока действия-возвращает errMessage.
	int cheсkProduckKey(std::string &errMessage, uint8_t *phyKey, int len);


private:
	//Получает ключ продукта из аппаратного ключа, использует ключ шифрования канального уровня.
	std::string getProductKey(uint8_t *phyKey, int len);

	/*
	   Отправка пакета USB ключу, и получение ответа.
	   command-команда транспортного уровня, 1 байт
	   inBuffer-данные для отправки
	   inLen-длина отправляемых данных
	   outBuffer-данные получаемые от устройства
	   outLen-ожидаемый размер ответа

	   Команды транспортного уровня:
	   0xb0-начало пакетной передачи.
	   0xb1-отправка пакета
	   0xb2-обработать сообщение на стороне usb устройства. Ответ от устройства содержит размер данных, которые будут получены
	   после обработки сообщения. Ответ два байта.
	   0xb3-получить ответ от USB устройства. Ответ пакетный.
	*/
	int command(uint8_t command, std::vector<uint8_t> inBuffer, int inLen, std::vector<uint8_t>& outBuffer, int outLen);
	/*
	  Метод который отправляет сообщение USB ключу и считывает ответ ключа. 
	  command -команда;
	  inBuffer-данные для передачи;
	  inLen-длина данных которые хотим передать USB ключу;
	  outBuffer-выходной буфер, содержит ответ USB ключа.
	*/
	int packcom(uint8_t command, std::vector<uint8_t> inBuffer, int inLen, std::vector<uint8_t>& outBuffer);

	/*
	   Получает 32  числа ряда.	a(n) = 7^n + 8^n + 9^n.
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

	*/
	void getTr1(QByteArray &ba);

	//Маскирую массив In, используя значения mask.
	QByteArray vanishAlg(QByteArray &In, QByteArray &mask);

	void unVanish(uint8_t *in, int o_size);
};
