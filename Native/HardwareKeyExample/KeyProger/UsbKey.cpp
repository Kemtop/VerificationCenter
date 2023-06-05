#include "UsbKey.h"

#include "aes_blocks_dec.h"
#include "rsa.h"
#include "files.h"
#include "osrng.h"

uint32_t crc32(const void *data, size_t len, const uint32_t POLY = 0x04C11DB7)
{
	uint8_t mod = len % 4;
	const unsigned char *buffer = (const unsigned char*)data;
	uint32_t crc = -1;

	while (len--)
	{
		crc = crc ^ (*buffer++ << 24);
		for (int bit = 0; bit < 8; bit++)
		{
			if (crc & (1L << 31)) crc = (crc << 1) ^ POLY;
			else                  crc = (crc << 1);
		}
	}

	if (mod)
	for (int i = 0; i < 4 - mod; ++i)
	{
		crc = crc ^ (0x00 << 24);
		for (int bit = 0; bit < 8; bit++)
		{
			if (crc & (1L << 31)) crc = (crc << 1) ^ POLY;
			else                  crc = (crc << 1);
		}
	}

	crc = ~crc;
	//разворачиваем байты
	uint32_t result = 0;
	result |= (uint8_t)crc << 24;
	result |= (uint8_t)(crc >> 8) << 16;
	result |= (uint8_t)(crc >> 16) << 8;
	result |= (uint8_t)(crc >> 24);
	return result;
}


UsbKey::UsbKey()
{
	libusb_init(NULL);   // инициализация
	libusb_set_debug(NULL, 0);  // уровень вывода отладочных сообщений
}

bool UsbKey::isConnected()
{
	libusb_device_handle *dev = libusb_open_device_with_vid_pid(NULL, DEV_VID, DEV_PID);
	return dev != NULL;
}

int UsbKey::command(uint8_t command, std::vector<uint8_t> inBuffer, int inLen, std::vector<uint8_t>& outBuffer, int outLen)
{
	uint8_t buffer[MaxPackSize];

	libusb_device_handle *dev = libusb_open_device_with_vid_pid(NULL, DEV_VID, DEV_PID);
	if (dev == NULL) return NOT_CONNECTED;
	if (libusb_kernel_driver_active(dev, DEV_INTERFACE))
		libusb_detach_kernel_driver(dev, DEV_INTERFACE);
	else
		return NOT_CONNECTED;

	if (libusb_claim_interface(dev, DEV_INTERFACE) < 0)
	{
		libusb_attach_kernel_driver(dev, DEV_INTERFACE);
		libusb_close(dev);
		return NO_GET_INTERFACE;
	}

	buffer[0] = command;
	for (int i = 0; i < inLen; ++i)
	{
		buffer[i + 1] = inBuffer.at(i);
	}

	bool send = true;
	int nread;
	while (send)
	{
		uint32_t crc = crc32(buffer, inLen + 1);

		buffer[inLen + 1] = (uint8_t)(crc >> 24);
		buffer[inLen + 2] = (uint8_t)(crc >> 16);
		buffer[inLen + 3] = (uint8_t)(crc >> 8);
		buffer[inLen + 4] = (uint8_t)crc;

		if (libusb_bulk_transfer(dev, ENDP_OUT, buffer, MaxPackSize, &nread, TIMEOUT))
			return ERROR_WRITE;
		if (libusb_bulk_transfer(dev, ENDP_IN, buffer, MaxPackSize, &nread, TIMEOUT))
			return ERROR_READ;

		if (buffer[0] == 0xF0) return UNKNOWN_COMMAND;

		if (buffer[0] == 0xFF) continue; //если ответило, что неверный CRC - посылаем команду еще раз

		crc = crc32(buffer, outLen + 1);
		uint32_t buffcrc = buffer[outLen + 1] << 24 | buffer[outLen + 2] << 16 | buffer[outLen + 3] << 8 | buffer[outLen + 4];

		if (crc != buffcrc) continue; //если crc в ответе не корректно - посылаем команду еще раз

		send = false;
	}

	for (int i = 1; i < outLen + 1; ++i) outBuffer.push_back(buffer[i]);

	libusb_attach_kernel_driver(dev, DEV_INTERFACE);
	libusb_close(dev);

	return 0;
}

int UsbKey::packcom(uint8_t command, std::vector<uint8_t> inBuffer, int inLen, std::vector<uint8_t>& outBuffer)
{
	
	uint8_t SendBuff[MaxMessageSize]; //Буфер хранящий сообщение. [Команда 1 байт][Данные(inBuffer) n байт.]

	std::vector<uint8_t> inbuff;
	std::vector<uint8_t> outbuff;
	int MessLen = inLen + 1; //Длина отправляемого сообщения.
	int maxpdsize = (MaxPackSize - 6); //Размер данных содержащихся в одном пакете.
	int packcount = MessLen / maxpdsize + 1; //Количество необходимых пакетов для отправки сообщений.

	//Формируем сообщение.
	SendBuff[0] = command;
	for (int i = 0; i < inLen; ++i)
	{
		SendBuff[i + 1] = inBuffer.at(i);
	}

	inbuff.clear(); //Очищаем контейнер.
	inbuff.push_back((uint8_t)(MessLen >> 8));//Старшая часть слова.
	inbuff.push_back((uint8_t)MessLen);//Младшая часть слова.
	//{inbuff   [Длина сообщения-старшая часть.][Длина сообщения-младшая часть.]}

	//Отправляю команду транспортного уровня USB ключу-начать пакетную передачу.
	//USB ключ готовит буфер размером MessLen для приема сообщения. Ответа не ожидаем.
	this->command(0xb0, inbuff, 2, outbuff, 0);
	
	//int pdsize = 0; //Размер данных в текущем пакете.

	//В цикле передаем все сообщение.
	for (int i = 0; i < packcount; ++i)
	{
		inbuff.clear();
		outbuff.clear();

		int pdsize=maxpdsize; //Размер данных в текущем пакете.

		if ((i + 1) == packcount) //Если пакет последний.
		{
			pdsize = MessLen - i * maxpdsize; //Количество данных для последнего пакета(Размер сообщения- уже переданные данные).
		}
				

		inbuff.push_back((uint8_t)i); //Помещаем в буфер номер пакета.
		//Помещаем в буфер очередную порцию данных, отправляемую в текущем пакете. 
		for (int j = 0; j < pdsize; ++j) inbuff.push_back(SendBuff[i*maxpdsize + j]);
		//При необходимости дополняем нулями.
		for (int j = pdsize; j < maxpdsize; ++j) inbuff.push_back(0x00);
		//Отправляем пакет. Не ожидаем ответа от устройства.
		this->command(0xb1, inbuff, maxpdsize + 1, outbuff, 0);
	}

	inbuff.clear(); 
	outbuff.clear();
	/* 
	   Посылаю команду транспортного уровня
	    0xb2-обработать сообщение на стороне usb устройства. Ответ от устройства содержит размер данных, которые будут получены
	   после обработки сообщения.
	*/
	this->command(0xb2, inbuff, 0, outbuff, 2);

	int answSize = (outbuff.at(0) << 8) | outbuff.at(1);
	packcount = (answSize == 0) ? 0 : answSize / maxpdsize + 1;
	for (int i = 0; i < packcount; ++i)
	{
		inbuff.clear(); outbuff.clear();
		int pdsize = ((i + 1) == packcount) ? answSize - i * maxpdsize : maxpdsize;
		inbuff.push_back((uint8_t)i);
		this->command(0xb3, inbuff, 1, outbuff, maxpdsize + 1);
		for (int j = 0; j < pdsize; j++) outBuffer.push_back(outbuff.at(j + 1));
	}
	return 0;
}

void UsbKey::sendRSAKey(uint8_t key[256])
{
	std::vector<uint8_t> in, out;
	for (int i = 0; i < 256; ++i) in.push_back(key[i]);
	packcom(0xa9, in, 256, out);
}

std::string UsbKey::getSerial()
{
	std::vector<uint8_t> key;
	std::vector<uint8_t> cryptVector;
	std::string Serial;
	//
	uint8_t CryptSerial[MaxPackSize];

	std::vector<uint8_t> in, out;
	packcom(0x07, in, 0, out);

	uint8_t cryptkey[256];
	for (int i = 0; i < 256; ++i) cryptkey[i] = out.at(i + 1);
	std::stringstream ss;
	std::string strn;

	CryptoPP::Integer c, r;

	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::RSA::PrivateKey privateKey;
	CryptoPP::FileSource inkey("prkey", true);
	privateKey.Load(inkey);
	CryptoPP::RSA::PublicKey publicKey(privateKey);
	c = CryptoPP::Integer((const byte *)cryptkey, sizeof(cryptkey));
	r = privateKey.CalculateInverse(rng, c);

	for (int i = 0; i < 16; ++i)
	{
		key.push_back(r.GetByte(15 - i));
	}

	for (int i = 0; i < 32; ++i) cryptVector.push_back(out.at(i+257));

	for (size_t i = 0; i < cryptVector.size(); ++i) CryptSerial[i] = cryptVector.at(i);
	aes128_block_dec(CryptSerial, key.data(), (uint8_t)cryptVector.size());
	for (size_t i = 0; i < cryptVector.size(); ++i) Serial += CryptSerial[i];

	return Serial;
}

std::string UsbKey::getHWSerial()
{
	std::vector<uint8_t> in, out;
	std::string str;
	packcom(0x3C, in, 0, out);
	for (size_t i = 1; i < out.size(); ++i) str += out[i];
	return str;
}

void UsbKey::setSerial(uint8_t serial[29])
{
	std::vector<uint8_t> in, out;
	in.assign(serial, serial + 29);
	packcom(0xa0, in, 29, out);
}

void UsbKey::setLastDate(uint16_t days)
{
	std::vector<uint8_t> in, out;
	in.push_back((uint8_t)(days >> 8));
	in.push_back((uint8_t)days);
	packcom(0x15, in, 2, out);
}

uint16_t UsbKey::getLastDate()
{
	std::vector<uint8_t> in, out;
	if (packcom(0x17, in, 0, out)) return 0;
	return out.at(1) << 8 | out.at(2);
}

std::vector<uint16_t> UsbKey::getRandData()
{
	std::vector<uint8_t> in, out;
	std::vector<uint16_t> data;
	if (packcom(0x50, in, 0, out)) return data;
	for (int i = 0; i < 28; i++) data.push_back(out.at(2 * i + 1) << 8 | out.at(2 * i + 2));
	return data;
}

void UsbKey::SendPublicKey()
{
	CryptoPP::RSA::PrivateKey privateKey;

	CryptoPP::FileSource in("prkey", true);
	privateKey.Load(in);

	CryptoPP::Integer n = privateKey.GetModulus();
	uint8_t key[256];
	for (int i = 0; i < 256; ++i)
	{
		key[i] = n.GetByte(255 - i);
	}
	sendRSAKey(key);
}
