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
		LastError = "Rsa key not found.";
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
		errMessage = "Fatal error:: Code:507. Please contact support.";
		return 0;
	}

	CheckSerial chkS; //Объект для проверки ключа продукта.
	std::string dtEnd = "";
	QString prk = QString::fromStdString(productKey);

	int  res = 0;

	//Проверяю правильность ключа продукта и получаю дату окончания действия ключа продукта.
	try
	{
		res = chkS.CheckProductKey(prk, dtEnd);
	}
	catch (...)
	{
		errMessage = "Fatal error:: Code:508. Please contact support.";
		return 0;
	}

	//Проблемы с ключом продукта.
	if (res != NO_SERIAL_ERROR)
	{
		errMessage = "Fatal error:: Code:509. Please contact support..";
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
			errMessage = "Fatal error:: Code:510. Please contact support.";
			return 0;
		}

		//Вычисляю разность текущей даты и последней даты записанной в аппаратный ключ.
		quint64 daysPassed = LastDate.daysTo(curDate);

		if (daysPassed > UPDATE_DATE_PERIOD) //Обновляю последнюю дату.
		{
			SetLastDate(curDate); //Пишу дату в аппаратный блок.
		}

		//До окончания действия ключа продукта вшитого в аппаратный блок осталось менее 7дней.
		quint64 daysToEnd = curDate.daysTo(endDatePk); //Сколько дней осталось до окончания действия ключа.

		if ((daysToEnd > -1) && (daysToEnd < UPDATE_DATE_PERIOD)) //Меньше 7ми или текущая(=0), но не меньше текущей.
		{
			SetLastDate(curDate); //Пишу дату в аппаратный блок.
		}

		if ((daysToEnd < 0) || (daysToEnd == 0)) //Перевели время, или текущая дата.
		{
			curDate.addDays(1);
			SetLastDate(curDate); //Пишу дату в аппаратный блок.
			//После следующего запуска программа перестанет работать.
		}
	}
	catch (...)
	{
		errMessage = "Fatal error:: Code:506. Please contact support.";
		return 0;
	}

	return 1;
}

//Шифрует ключ RSA.
bool ApiKey::EncodeRsaKey(QString rsaKeyFileName, QString protectProjectName, QString arrayName)
{
	QFileInfo fileInfo(rsaKeyFileName);
	qint64 fileSize = fileInfo.size();

	//Размер файла не удалось получить или размер файла меньше заголовка
	if (fileSize == 0)
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

	if (fileSize != aR.size())
	{
		LastError = "Не удается считать файл ключа.";
		return false;
	}

	QByteArray cipherKey; //Получаю ключ шифрования.
	GetCipherKey(cipherKey);
	QByteArray out = Encrypt(aR, cipherKey);

	QByteArray hex = out.toHex();
	//Проверить длину массива должна быть кратна 2!.

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

	QFile resultFile(outDir);
	if (!resultFile.open(QIODevice::WriteOnly))
	{
		LastError = "Не удается сохранить массив.";
		return false;
	}

	resultFile.write(str.toUtf8());
	resultFile.close();

	return true;
}

//Получает 32  числа ряда функции a(n) = 7^n + 8^n + 9^n. Используется как ключ шифрования контейнера rsa. 
//Чем экзотичнее тут код – тем сложнее будет взломать защиту.
void ApiKey::GetCipherKey(QByteArray & ba)
{
	//a(n) = 7 ^ n + 8 ^ n + 9 ^ n
	quint64 add1 = 0;
	quint64 add2 = 0;
	quint64 add3 = 0;
	quint64 a = 0;

	quint64 data[SESSION_KEY_LEN];
	int pos = 0;
	for (int i = 2; i < SESSION_KEY_LEN + 2; i++)
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
	for (int i = 0; i < SESSION_KEY_LEN; i++)
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

		if (m_pos < CIPHER_PRODUCT_KEY_LEN - 1) m_pos++;
		else m_pos = 0;
	}

	return out;
}

//Получает ключ продукта из аппаратного ключа, использует ключ шифрования rsa (phyKey). Вшитый в защищаемую программу.
std::string ApiKey::GetProductKey(uint8_t *phyKey, int len)
{
	QByteArray keyCipher;
	GetCipherKey(keyCipher);
	Decrypt(phyKey, len, keyCipher); //Расшифровка закрытого ключа rsa.
	CryptoPP::ArraySource arraySource(phyKey, len, true); //pumpAll
	CryptoPP::RSA::PrivateKey privateKey;
	privateKey.Load(arraySource);

	return GetProductSerial(privateKey);
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

		if (m_pos < CIPHER_PRODUCT_KEY_LEN - 1) m_pos++;
		else m_pos = 0;
	}
}

void ApiKey::SendPublicKey(QString rsaKeyFileName)
{
	CryptoPP::RSA::PrivateKey privateKey;

	CryptoPP::FileSource in(rsaKeyFileName.toStdString().c_str(), true);
	privateKey.Load(in);

	CryptoPP::Integer n = privateKey.GetModulus();
	uint8_t key[CIPHER_SESSION_KEY_LEN];
	for (int i = 0; i < CIPHER_SESSION_KEY_LEN; ++i)
	{
		key[i] = n.GetByte(CIPHER_SESSION_KEY_LEN - 1 - i);
	}
	
	usb.SendRSAKey(key);
}

//Возвращает ключ продукта. Используя закрытый ключ rsa из файла.
std::string ApiKey::ProductSerial(QString fileName)
{
	CryptoPP::RSA::PrivateKey privateKey;
	CryptoPP::FileSource inkey(fileName.toStdString().c_str(), true); //Добавить проверку исключений!
	privateKey.Load(inkey);

	return GetProductSerial(privateKey);
}

//Читает и расшифровывает ключ (серийный номер) продукта.
std::string ApiKey::GetProductSerial(CryptoPP::RSA::PrivateKey privateKey) {

	std::vector<uint8_t> data;
	data = usb.GetCryptProductSerial(); //Получаем [шифрованный rsa сеансовый ключ, 256 байт ][данные, 32 байта]

	uint8_t cryptSessionKey[CIPHER_SESSION_KEY_LEN];
	memcpy(cryptSessionKey, data.data(), data.size() - CIPHER_PRODUCT_KEY_LEN);
	   
	uint8_t sessionKey[SESSION_KEY_LEN];
	DecryptSessionKey(privateKey, cryptSessionKey, CIPHER_SESSION_KEY_LEN, sessionKey);

	return DecryptProductSerial(data, sessionKey);
}

//Расшифровывает сеансовый ключ.
void ApiKey::DecryptSessionKey(CryptoPP::RSA::PrivateKey privateKey, uint8_t *cryptSessionKey, int keyLen, uint8_t *sessionKey) {

	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::Integer cryptRsaData = CryptoPP::Integer((const byte *)cryptSessionKey, keyLen);
	CryptoPP::Integer decryptResult = privateKey.CalculateInverse(rng, cryptRsaData); //Расшифровка сеансового ключа.

	for (int i = 0; i < SESSION_KEY_LEN; ++i)
	{
		sessionKey[i] = decryptResult.GetByte(SESSION_KEY_LEN - 1 - i);
	}
}

//Расшифровывает серийный номер продукта.
std::string ApiKey::DecryptProductSerial(std::vector<uint8_t> data, uint8_t *sessionKey) {

	uint8_t cryptSerial[MaxPackSize];
	memcpy(cryptSerial, &data.at(CIPHER_SESSION_KEY_LEN), CIPHER_PRODUCT_KEY_LEN);
	aes128_block_dec(cryptSerial, sessionKey, CIPHER_PRODUCT_KEY_LEN);
	
	std::string productSerial;
	for (size_t i = 0; i < CIPHER_PRODUCT_KEY_LEN; ++i) productSerial += cryptSerial[i];
	
	return productSerial;
}

//Задает состояние выходам на плате. Возвращает текущее состояние.
uint8_t ApiKey::SetOutputs(uint8_t data) {
	std::vector<uint8_t> result = usb.SetOutputs(data);
	return result[1];
}

//Возвращает состояние входов.
uint8_t ApiKey::InputStatus() {
	std::vector<uint8_t> result = usb.InputStatus();
	return result[1];
}