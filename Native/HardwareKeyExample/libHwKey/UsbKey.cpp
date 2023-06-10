#include "UsbKey.h"

//Расчет контрольной суммы.
uint32_t Crc32(const void *data, size_t len, const uint32_t POLY = 0x04C11DB7)
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

bool UsbKey::IsConnected()
{
	libusb_device_handle *dev = libusb_open_device_with_vid_pid(NULL, DEV_VID, DEV_PID);
	return dev != NULL;
}

int UsbKey::Command(uint8_t command, std::vector<uint8_t> inBuffer, int inLen, std::vector<uint8_t>& outBuffer, int outLen)
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
		uint32_t crc = Crc32(buffer, inLen + 1);

		buffer[inLen + 1] = (uint8_t)(crc >> 24);
		buffer[inLen + 2] = (uint8_t)(crc >> 16);
		buffer[inLen + 3] = (uint8_t)(crc >> 8);
		buffer[inLen + 4] = (uint8_t)crc;

		if (libusb_bulk_transfer(dev, ENDP_OUT, buffer, MaxPackSize, &nread, TIMEOUT))
			return ERROR_WRITE;
		if (libusb_bulk_transfer(dev, ENDP_IN, buffer, MaxPackSize, &nread, TIMEOUT))
			return ERROR_READ;

		if (buffer[0] == ANSWER_UNKNOWN_COMMAND) return UNKNOWN_COMMAND;

		if (buffer[0] == ANSWER_BAD_CRC) continue; //если ответило, что неверный CRC - посылаем команду еще раз

		crc = Crc32(buffer, outLen + 1);
		uint32_t buffcrc = buffer[outLen + 1] << 24 | buffer[outLen + 2] << 16 | buffer[outLen + 3] << 8 | buffer[outLen + 4];

		if (crc != buffcrc) continue; //если crc в ответе не корректно - посылаем команду еще раз

		send = false;
	}

	for (int i = 1; i < outLen + 1; ++i) outBuffer.push_back(buffer[i]);

	libusb_attach_kernel_driver(dev, DEV_INTERFACE);
	libusb_close(dev);

	return 0;
}

int UsbKey::Packcom(uint8_t command, std::vector<uint8_t> inBuffer, int inLen, std::vector<uint8_t>& outBuffer)
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

	//Отправляю команду транспортного уровня USB ключу - начать пакетную передачу.
	//USB ключ готовит буфер размером MessLen для приема сообщения. Ответа не ожидаем.
	this->Command(TRANSPORT_START_PACKET_TRANSMISSION, inbuff, 2, outbuff, 0);

	//В цикле передаем все сообщение.
	for (int i = 0; i < packcount; ++i)
	{
		inbuff.clear();
		outbuff.clear();

		int pdsize=maxpdsize; //Размер данных в текущем пакете.

		if ((i + 1) == packcount) //Если пакет последний.
		{
			pdsize = MessLen - i * maxpdsize; //Количество данных для последнего пакета(Размер сообщения - уже переданные данные).
		}
				

		inbuff.push_back((uint8_t)i); //Помещаем в буфер номер пакета.
		//Помещаем в буфер очередную порцию данных, отправляемую в текущем пакете. 
		for (int j = 0; j < pdsize; ++j) inbuff.push_back(SendBuff[i*maxpdsize + j]);
		//При необходимости дополняем нулями.
		for (int j = pdsize; j < maxpdsize; ++j) inbuff.push_back(0x00);
		//Отправляем пакет. Не ожидаем ответа от устройства.
		this->Command(TRANSPORT_SENDING_PACKAGE, inbuff, maxpdsize + 1, outbuff, 0);
	}

	inbuff.clear(); 
	outbuff.clear();
	/* 
	   Посылаю команду транспортного уровня
	    0xb2-обработать сообщение на стороне usb устройства. Ответ от устройства содержит размер данных, которые будут получены
	   после обработки сообщения.
	*/
	this->Command(TRANSPORT_EXECUTE_COMMAND, inbuff, 0, outbuff, 2);

	int answSize = (outbuff.at(0) << 8) | outbuff.at(1);
	packcount = (answSize == 0) ? 0 : answSize / maxpdsize + 1;
	for (int i = 0; i < packcount; ++i)
	{
		inbuff.clear(); outbuff.clear();
		int pdsize = ((i + 1) == packcount) ? answSize - i * maxpdsize : maxpdsize;
		inbuff.push_back((uint8_t)i);
		this->Command(TRANSPORT_GET_RESULT, inbuff, 1, outbuff, maxpdsize + 1);
		for (int j = 0; j < pdsize; j++) outBuffer.push_back(outbuff.at(j + 1));
	}
	return 0;
}

void UsbKey::InitUsb()
{
	libusb_init(NULL);   // инициализация
	libusb_set_debug(NULL, 0);  // уровень вывода отладочных сообщений
}

void UsbKey::SendRSAKey(uint8_t key[CIPHER_SESSION_KEY_LEN])
{
	std::vector<uint8_t> in, out;
	for (int i = 0; i < CIPHER_SESSION_KEY_LEN; ++i) in.push_back(key[i]);
	Packcom(SAVE_RSA_KEY_COMMAND, in, CIPHER_SESSION_KEY_LEN, out);
}

std::string UsbKey::GetHWSerial()
{
	std::vector<uint8_t> in, out;
	std::string str;
	Packcom(GET_HW_ID_COMMAND, in, 0, out);
	for (size_t i = 1; i < out.size(); ++i) str += out[i];
	return str;
}

//Сохраняет серийный номер продукта.
void UsbKey::SetSerial(uint8_t serial[SERIAL_NUM_LEN])
{
	std::vector<uint8_t> in, out;
	in.assign(serial, serial + SERIAL_NUM_LEN);
	Packcom(SAVE_SERIAL_COMMAND, in, SERIAL_NUM_LEN, out);
}

void UsbKey::SetLastDate(uint16_t days)
{
	std::vector<uint8_t> in, out;
	in.push_back((uint8_t)(days >> 8));
	in.push_back((uint8_t)days);
	Packcom(SAVE_DATE_COMMAND, in, 2, out);
}

void UsbKey::SetLastDate(QDate lastDate)
{
	QDate dat(BEGIN_YEAR, BEGIN_MONTH, BEGIN_DAY);
	uint16_t days = (uint16_t)dat.daysTo(lastDate);
	SetLastDate(days);
}

uint16_t UsbKey::GetLastDate()
{
	std::vector<uint8_t> in, out;
	if (Packcom(GET_DATE_COMMAND, in, 0, out)) return 0;
	return out.at(1) << 8 | out.at(2);
}

QDate UsbKey::GetLastHwDate()
{
	uint16_t days = GetLastDate();
	QDate dat(BEGIN_YEAR, BEGIN_MONTH, BEGIN_DAY);
	return dat.addDays(days);
}

std::vector<uint16_t> UsbKey::GetRandData()
{
	std::vector<uint8_t> in, out;
	std::vector<uint16_t> data;
	if (Packcom(GET_RANDOM_COMMAND, in, 0, out)) return data;
	//for (int i = 0; i < 28; i++) data.push_back(out.at(2 * i + 1) << 8 | out.at(2 * i + 2));
	//return data;
}

//Возвращает шифрованный ключ продукта.
std::vector<uint8_t> UsbKey::GetCryptProductSerial()
{
	std::vector<uint8_t> in, out;
	Packcom(GET_CRYPT_SERIAL_COMMAND, in, 0, out);
	return GetDataFromPacket(out);
}

//Задает состояние выходам на плате. Возвращает текущее состояние.
std::vector<uint8_t> UsbKey::SetOutputs(uint8_t data)
{
	std::vector<uint8_t> in, out;
	in.push_back((uint8_t) data);
	Packcom(SET_LED_VALUE, in, in.size(), out);
	return out;
}

//Возвращает состояние входов.
std::vector<uint8_t> UsbKey::InputStatus() {
	std::vector<uint8_t> in, out;
	Packcom(INPUT_PORTS_STATUS, in, 0, out);
	return out;
}

//Возвращает пользовательские данные. Удаляет в начале 1 байт – команду.
std::vector<uint8_t> UsbKey::GetDataFromPacket(std::vector<uint8_t>& buffer) {
	buffer.erase(buffer.begin());
	return buffer;
}