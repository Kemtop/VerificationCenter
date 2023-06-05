#include "UsbKey.h"

#include "AES\inc\aes.h"
#include "AES\inc\aes_blocks_dec.h"
#include "libusb.h"
#include <CheckSerial.h>
#include <sstream>

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

	//Отправляю команду транспортного уровня USB ключу-начать пакетную передачу.
	//USB ключ готовит буфер размером MessLen для приема сообщения. Ответа не ожидаем.
	this->Command(0xb0, inbuff, 2, outbuff, 0);
	
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
		this->Command(0xb1, inbuff, maxpdsize + 1, outbuff, 0);
	}

	inbuff.clear(); 
	outbuff.clear();
	/* 
	   Посылаю команду транспортного уровня
	    0xb2-обработать сообщение на стороне usb устройства. Ответ от устройства содержит размер данных, которые будут получены
	   после обработки сообщения.
	*/
	this->Command(0xb2, inbuff, 0, outbuff, 2);

	int answSize = (outbuff.at(0) << 8) | outbuff.at(1);
	packcount = (answSize == 0) ? 0 : answSize / maxpdsize + 1;
	for (int i = 0; i < packcount; ++i)
	{
		inbuff.clear(); outbuff.clear();
		int pdsize = ((i + 1) == packcount) ? answSize - i * maxpdsize : maxpdsize;
		inbuff.push_back((uint8_t)i);
		this->Command(0xb3, inbuff, 1, outbuff, maxpdsize + 1);
		for (int j = 0; j < pdsize; j++) outBuffer.push_back(outbuff.at(j + 1));
	}
	return 0;
}

void UsbKey::GetCipherKey(QByteArray & ba)
{
	//a(n) = 7 ^ n + 8 ^ n + 9 ^ n
	quint64 add1=0;
	quint64 add2=0;
	quint64 add3=0;
	quint64 a = 0;

	quint64 data[16];
	int pos = 0;
	for (int i = 2;i < 18;i++)
	{
		 add1 = 7;
		 add2 = 8;
		 add3 = 9;

		 //Возводим в степень.
		 for (int b = 0;b < i;b++)
		 {
			 add1 = add1*7;
			 add2 = add2 * 8;
			 add3 = add3 * 9;
		 }

		 a = add1 + add2 + add3;
		 data[pos] = a;
		 pos++;
	}
	/*
		0		1584   //06 30
		4		13058  //33 02
		5		108624  //01 A8 50
		6		911234  //0D E7 82
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

	  //Всегда есть два байта. Берем их.
	uint8_t t = 0;
	for (int i = 0;i < 16;i++)
	{
		a = data[i] & 0xFF;
		t = static_cast<uint8_t>(a);
		ba.append(t);

		a = data[i] & 0xFF00;
		a = a >> 8;
		t = static_cast<uint8_t>(a);
		ba.append(t);
	}
}

//«Шифрует» (просто xor) массив in, на ключе key.
QByteArray UsbKey::Encrypt(QByteArray & in, QByteArray & key)
{
	QByteArray out;
	int o_size = in.size();
	out.resize(o_size);

	uint8_t x1 = 0;
	uint8_t x2 = 0;
	uint8_t y = 0;
	int m_pos = 0;
	
	for (int i = 0;i < o_size;i++)
	{
		x1=static_cast<uint8_t>(in[i]);
		x2 = static_cast<uint8_t>(key[m_pos]);
		y = x1 ^ x2;

		out[i] = static_cast<char>(y);

		if (m_pos < 31) m_pos++;
		else m_pos = 0;
	}
	
	return out;
}

void UsbKey::SendRSAKey(uint8_t key[256])
{
	std::vector<uint8_t> in, out;
	for (int i = 0; i < 256; ++i) in.push_back(key[i]);
	Packcom(SAVE_RSA_KEY_COMMAND, in, 256, out);
}

std::string UsbKey::GetSerial(QString fileName)
{
	std::vector<uint8_t> key;
	std::vector<uint8_t> cryptVector;
	std::string Serial;
	
	uint8_t CryptSerial[MaxPackSize];

	std::vector<uint8_t> in, out;
	Packcom(GET_CRYPT_SERIAL_COMMAND, in, 0, out);

	uint8_t cryptkey[256];
	for (int i = 0; i < 256; ++i) cryptkey[i] = out.at(i + 1);
	std::stringstream ss;
	std::string strn;

	CryptoPP::Integer c, r;

	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::RSA::PrivateKey privateKey;
	CryptoPP::FileSource inkey(fileName.toStdString().c_str(), true); //Добавить проверку исключений!
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

std::string UsbKey::GetProductKey(uint8_t *phyKey, int len)
{
	std::vector<uint8_t> key;
	std::vector<uint8_t> cryptVector;
	std::string Serial;
	//
	uint8_t CryptSerial[MaxPackSize];

	std::vector<uint8_t> in, out;
	Packcom(GET_CRYPT_SERIAL_COMMAND, in, 0, out);

	uint8_t cryptkey[256];
	for (int i = 0; i < 256; ++i) cryptkey[i] = out.at(i + 1);
	std::stringstream ss;
	std::string strn;


	CryptoPP::Integer c, r;
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::RSA::PrivateKey privateKey;

	QByteArray keyCipher;
	GetCipherKey(keyCipher);
	Decrypt(phyKey, len, keyCipher); //Восстанавливаю.
	CryptoPP::ArraySource as2(phyKey,len, true); //pumpAll

	privateKey.Load(as2);
		
	CryptoPP::RSA::PublicKey publicKey(privateKey);
	c = CryptoPP::Integer((const byte *)cryptkey, sizeof(cryptkey));
	r = privateKey.CalculateInverse(rng, c);

	for (int i = 0; i < 16; ++i)
	{
		key.push_back(r.GetByte(15 - i));
	}

	for (int i = 0; i < 32; ++i) cryptVector.push_back(out.at(i + 257));

	for (size_t i = 0; i < cryptVector.size(); ++i) CryptSerial[i] = cryptVector.at(i);
	aes128_block_dec(CryptSerial, key.data(), (uint8_t)cryptVector.size());
	for (size_t i = 0; i < cryptVector.size(); ++i) Serial += CryptSerial[i];

	return Serial;
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
void UsbKey::SetSerial(uint8_t serial[29])
{
	std::vector<uint8_t> in, out;
	in.assign(serial, serial + 29);
	Packcom(SAVE_SERIAL_COMMAND, in, 29, out);
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
	for (int i = 0; i < 28; i++) data.push_back(out.at(2 * i + 1) << 8 | out.at(2 * i + 2));
	return data;
}

void UsbKey::SendPublicKey(QString rsaKeyFileName)
{
	CryptoPP::RSA::PrivateKey privateKey;

	CryptoPP::FileSource in(rsaKeyFileName.toStdString().c_str(), true);
	privateKey.Load(in);

	CryptoPP::Integer n = privateKey.GetModulus();
	uint8_t key[256];
	for (int i = 0; i < 256; ++i)
	{
		key[i] = n.GetByte(255 - i);
	}
	SendRSAKey(key);
}

void UsbKey::InitUsb()
{
   libusb_init(NULL);   // инициализация
   libusb_set_debug(NULL, 0);  // уровень вывода отладочных сообщений
}

//Шифрует ключ RSA.
bool UsbKey::EncodeRsaKey(QString rsaKeyFileName, QString protectProjectName, QString arrayName)
{
	QFileInfo file_info(rsaKeyFileName);
	qint64 f_size = file_info.size();
	
	//Размер файла не удалось получить или размер файла меньше заголовка
	if (f_size == 0)
	{
		LastError = "Размер файла не удалось получить или размер файла меньше заголовка";
		return false;
	}

	QFile file(rsaKeyFileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		LastError = "Не удается открыть файл ключа.";
		return false;
	}

	QByteArray aR = file.readAll();

	if (f_size != aR.size())
	{
		LastError = "Не удается считать файл ключа.";
		return false;
	}
	
	QByteArray cipherKey; //Получаю ряд.
	GetCipherKey(cipherKey);
	QByteArray out = Encrypt(aR, cipherKey);

	QByteArray hex = out.toHex();
	//Проверить длину массива должна быть кратна 2!.
	QString a(hex);
	
	QString str = "#define " + arrayName + " {";
	QString tmp1;
	QString tmp2;

	bool noFirst = false;

	int pos = 0;
	for (;pos < hex.size();)
	{
		if (noFirst) str = str + ",";
		tmp1 = hex[pos];
		pos++;
		tmp2 = hex[pos];
		pos++;
		str =str+ "0x" + tmp1 + tmp2;

		noFirst = true;
	}

	str = str + "}";

	QString outDir = SOLUTION_DIR + protectProjectName;

    QFile file2(outDir);
	if (!file2.open(QIODevice::WriteOnly))
	{
		LastError = "Не удается сохранить массив.";
		return false;
	}

	file2.write(str.toUtf8());
	file2.close();
		
	return false;
}

int UsbKey::CheсkProduckKey(std::string & errMessage, uint8_t * phyKey, int len)
{
	std::string productKey;
	errMessage = "";

	try
	{
		//Получает ключ продукта из аппаратного ключа, использует ключ шифрования канального уровня.
		 productKey = GetProductKey(phyKey, len);
	}
	catch (...)
	{
		errMessage = "Unknown error:: Error code::703.";
		return 0;
	}
	
	CheckSerial chkS; //Объект для проверки ключа продукта.
	std::string dtEnd = "";
	QString prk = QString::fromStdString(productKey);

	int  res = 15; //Любое число не 0 или 1. В случае исключения должно быть значение.

	//Проверяю правильность ключа продукта и получаю дату окончания действия ключа продукта.
	try
	{
	  res = chkS.CheckProductKey(prk, dtEnd); 
	}
	catch (...)
	{
		errMessage = "Core parameter failure::Code Error 667.";
		return 0;
	}
	
	//Проблемы с ключом продукта.
	if (res == 0)
	{
		errMessage = "Linear alignment error of the main unit::Code Error 13578.";
		return 0; //Проблемы с ключом продукта.
	}

	//Проверка дат.
	//Получаю дату окончания действия ключа продукта.  
	QString prkDateEnd = QString::fromStdString(dtEnd);

	QDate endDatePk = QDate::fromString(prkDateEnd, "dd.MM.yyyy"); 
	QDate curDate = QDateTime::currentDateTime().date();
   
	//Подходит срок окончания действия серийного номера. Скажем пользователю об этом.
	if (endDatePk <= curDate.addDays(10))
	{
		errMessage = "The hardware unit will stop working in less than 10d.";
	}
	
	//Проверка подделки системного времени.
	try
	{
		QDate LastDate = GetLastHwDate(); //Получаю последнюю дату записанную в  аппаратный ключ.
		//Если текущая дата меньше, последней записанной в аппаратный ключ. Явная подделка времени на ПК пользователя.
		if (curDate < LastDate)
		{
			errMessage = "Object represents a particular::Code Error 2987.";
			return 0;
		}

		//Вычисляю разность текущей даты и последней даты записанной в аппаратный ключ.
		quint64 r = 23;
		r = LastDate.daysTo(curDate);

		if (r > 7) //Обновляю последнюю дату.
		{
			SetLastDate(curDate); //Пишу дату в аппаратный блок.
		}

		//До окончания действия ключа продукта вшитого в аппаратный блок  осталось менее 7дней.
		quint64 r1 = curDate.daysTo(endDatePk); //Сколько дней осталось до окончания действия ключа.
		
		if ((r1>-1)&&(r1 < 7)) //Меньше 7ми или текущая(r1=0), но не меньше текущей.
		{
			SetLastDate(curDate); //Пишу дату в аппаратный блок.
		}
		
		if ((r1 < 0) || (r1 == 0)) //Перевели время, или текущая дата.
		{
			curDate.addDays(1);
			SetLastDate(curDate); //Пишу дату в аппаратный блок.
			//После следующего запуска программа перестанет работать.
		}
	}
	catch (...)
	{
		errMessage = "No particular linear ground thread::Code Error 709.";
		return 0;
	}

	return 1;
}

void UsbKey::Decrypt(uint8_t * in, int o_size, QByteArray key)
{
	uint8_t x1 = 0;
	uint8_t x2 = 0;
	uint8_t y = 0;
	int m_pos = 0;
	
	for (int i = 0;i < o_size;i++)
	{
		x1 = in[i];
		x2 = static_cast<uint8_t>(key[m_pos]);
		y = x1 ^ x2;

		in[i] = y;

		if (m_pos < 31) m_pos++;
		else m_pos = 0;
	}
}
