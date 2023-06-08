#include "ApiKey.h"
#include "AES\inc\aes.h"
#include "AES\inc\aes_blocks_dec.h"
#include "libusb.h"
#include <CheckSerial.h>
#include <sstream>


ApiKey::ApiKey()
{

}

//Проверяет корректность серийного номера.
bool ApiKey::CheckValidSerial(QString serial, QString defaultSerial)
{
	QString curentSerial = serial.mid(0, 5);
	if (curentSerial == defaultSerial)
	{
		LastError = "Нельзя установить серийный номер с неверным базовым числом.";
		return false;
	}
	
	QRegExp reg("[0-9,A-Z]{5}-[0-9,A-Z]{5}-[0-9,A-Z]{5}-[0-9,A-Z]{5}-[0-9,A-Z]{5}");
	int match = reg.indexIn(serial);
	if (match < 0)
	{
		LastError =  "Указанный серийный номер не соответствует формату";
		return false;
	}

	CheckSerial Chk;
	std::string strDtEnd = "";
	int result = Chk.CheckProductKey(serial, strDtEnd);
	if (result == 0)
	{
		LastError = "Ошибка", "Указан не верный ключ продукта.";
		return false;
	}
	   
	return true;
}

//Сохраняет серийный номер продукта.
bool ApiKey::SaveSerial(QString serial, QString defaultSerial) {

	if (!CheckValidSerial(serial, defaultSerial)) return false;

	usb.SetSerial((uint8_t *)(serial.toStdString().c_str()));

	return true;
}

//Сохраняет модуль n ключа rsa.
bool ApiKey::SaveRsaModule(QString keyFileName, QString vectorPath, QString cryptArrayKeyName) {

	//Проверяю наличие ключа RSA в текущей папке.
	QString path = QCoreApplication::applicationDirPath() + "/" + keyFileName;

	QFileInfo zfile(path);
	//Проверяю существует ли файл. Если существует проверяю-это файл а не директория.
	if (!(zfile.exists() && zfile.isFile()))
	{
		LastError = "Отсутствует ключ rsa.";
		return false;
	}

	if (!EncodeRsaKey(keyFileName, vectorPath, cryptArrayKeyName))
	{
		LastError = usb.LastError;
		return false;
	}

	SendPublicKey(keyFileName);

	return true;
}

//Инициализирует usb.
void ApiKey::InitUsb() {
	usb.InitUsb();
}

//Подключено ли устройство.
bool ApiKey::DeviceIsConnected() {
	return usb.IsConnected();
}

//Возвращает серийный номер устройства.
std::string ApiKey::HWSerial() {
	return usb.GetHWSerial();
}

QDate ApiKey::LastDate() {
	return usb.GetLastHwDate();
}

//Пишу в аппаратный блок последнюю дату.
void ApiKey::SetLastDate(QDate days) {
	usb.SetLastDate(days);
}

int ApiKey::CheсkProduckKey(std::string & errMessage, uint8_t * phyKey, int len)
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
		QDate LastDate = usb.GetLastHwDate(); //Получаю последнюю дату записанную в  аппаратный ключ.
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

		if ((r1 > -1) && (r1 < 7)) //Меньше 7ми или текущая(r1=0), но не меньше текущей.
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

//Шифрует ключ RSA.
bool ApiKey::EncodeRsaKey(QString rsaKeyFileName, QString protectProjectName, QString arrayName)
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
	for (; pos < hex.size();)
	{
		if (noFirst) str = str + ",";
		tmp1 = hex[pos];
		pos++;
		tmp2 = hex[pos];
		pos++;
		str = str + "0x" + tmp1 + tmp2;

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

void ApiKey::GetCipherKey(QByteArray & ba)
{
	//a(n) = 7 ^ n + 8 ^ n + 9 ^ n
	quint64 add1 = 0;
	quint64 add2 = 0;
	quint64 add3 = 0;
	quint64 a = 0;

	quint64 data[16];
	int pos = 0;
	for (int i = 2; i < 18; i++)
	{
		add1 = 7;
		add2 = 8;
		add3 = 9;

		//Возводим в степень.
		for (int b = 0; b < i; b++)
		{
			add1 = add1 * 7;
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
	for (int i = 0; i < 16; i++)
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
QByteArray ApiKey::Encrypt(QByteArray & in, QByteArray & key)
{
	QByteArray out;
	int o_size = in.size();
	out.resize(o_size);

	uint8_t x1 = 0;
	uint8_t x2 = 0;
	uint8_t y = 0;
	int m_pos = 0;

	for (int i = 0; i < o_size; i++)
	{
		x1 = static_cast<uint8_t>(in[i]);
		x2 = static_cast<uint8_t>(key[m_pos]);
		y = x1 ^ x2;

		out[i] = static_cast<char>(y);

		if (m_pos < 31) m_pos++;
		else m_pos = 0;
	}

	return out;
}

std::string ApiKey::GetProductKey(uint8_t *phyKey, int len)
{
	std::vector<uint8_t> key;
	std::vector<uint8_t> cryptVector;
	std::string Serial;
	//
	uint8_t CryptSerial[MaxPackSize];

	std::vector<uint8_t> out;
	out = usb.GetCryptProductSerial();

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
	CryptoPP::ArraySource as2(phyKey, len, true); //pumpAll

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

void ApiKey::Decrypt(uint8_t * in, int o_size, QByteArray key)
{
	uint8_t x1 = 0;
	uint8_t x2 = 0;
	uint8_t y = 0;
	int m_pos = 0;

	for (int i = 0; i < o_size; i++)
	{
		x1 = in[i];
		x2 = static_cast<uint8_t>(key[m_pos]);
		y = x1 ^ x2;

		in[i] = y;

		if (m_pos < 31) m_pos++;
		else m_pos = 0;
	}
}

void ApiKey::SendPublicKey(QString rsaKeyFileName)
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
	
	usb.SendRSAKey(key);
}

//Возвращает ключ продукта.
std::string ApiKey::ProductSerial(QString fileName)
{
	std::vector<uint8_t> key;
	std::vector<uint8_t> cryptVector;
	std::string Serial;

	uint8_t CryptSerial[MaxPackSize];

	std::vector<uint8_t> out;
	out = usb.GetCryptProductSerial();

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

	for (int i = 0; i < 32; ++i) cryptVector.push_back(out.at(i + 257));

	for (size_t i = 0; i < cryptVector.size(); ++i) CryptSerial[i] = cryptVector.at(i);
	aes128_block_dec(CryptSerial, key.data(), (uint8_t)cryptVector.size());
	for (size_t i = 0; i < cryptVector.size(); ++i) Serial += CryptSerial[i];

	return Serial;
}