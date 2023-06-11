#include "ApiKey.h"
#include "AES\inc\aes.h"
#include "AES\inc\aes_blocks_dec.h"
#include "libusb.h"
#include <CheckSerial.h>
#include <sstream>

ApiKey::ApiKey()
{

}

//��������� ������������ ��������� ������.
bool ApiKey::CheckValidSerial(QString serial, QString defaultSerial)
{
	QString curentSerial = serial.mid(0, 5);
	if (curentSerial == defaultSerial)
	{
		LastError = "������ ���������� �������� ����� � �������� ������� ������.";
		return false;
	}
	
	QRegExp reg("[0-9,A-Z]{5}-[0-9,A-Z]{5}-[0-9,A-Z]{5}-[0-9,A-Z]{5}-[0-9,A-Z]{5}");
	int match = reg.indexIn(serial);
	if (match < 0)
	{
		LastError =  "��������� �������� ����� �� ������������� �������";
		return false;
	}

	CheckSerial Chk;
	std::string strDtEnd = "";
	int result = Chk.CheckProductKey(serial, strDtEnd);
	if (result == 0)
	{
		LastError = "������", "������ �� ������ ���� ��������.";
		return false;
	}
	   
	return true;
}

//��������� �������� ����� ��������.
bool ApiKey::SaveSerial(QString serial, QString defaultSerial) {

	if (!CheckValidSerial(serial, defaultSerial)) return false;

	usb.SetSerial((uint8_t *)(serial.toStdString().c_str()));

	return true;
}

//��������� ������ n ����� rsa.
bool ApiKey::SaveRsaModule(QString keyFileName, QString vectorPath, QString cryptArrayKeyName) {

	//�������� ������� ����� RSA � ������� �����.
	QString path = QCoreApplication::applicationDirPath() + "/" + keyFileName;

	QFileInfo zfile(path);
	//�������� ���������� �� ����. ���� ���������� ��������-��� ���� � �� ����������.
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

//�������������� usb.
void ApiKey::InitUsb() {
	usb.InitUsb();
}

//���������� �� ����������.
bool ApiKey::DeviceIsConnected() {
	return usb.IsConnected();
}

//���������� �������� ����� ����������.
std::string ApiKey::HWSerial() {
	return usb.GetHWSerial();
}

QDate ApiKey::LastDate() {
	return usb.GetLastHwDate();
}

//���� � ���������� ���� ��������� ����.
void ApiKey::SetLastDate(QDate days) {
	usb.SetLastDate(days);
}

int ApiKey::Che�kProduckKey(std::string & errMessage, uint8_t * phyKey, int len)
{
	std::string productKey;
	errMessage = "";

	try
	{
		//�������� ���� �������� �� ����������� �����, ���������� ���� ���������� ���������� ������.
		productKey = GetProductKey(phyKey, len);
	}
	catch (...)
	{
		errMessage = "Fatal error:: Code:507. Please contact support.";
		return 0;
	}

	CheckSerial chkS; //������ ��� �������� ����� ��������.
	std::string dtEnd = "";
	QString prk = QString::fromStdString(productKey);

	int  res = 0;

	//�������� ������������ ����� �������� � ������� ���� ��������� �������� ����� ��������.
	try
	{
		res = chkS.CheckProductKey(prk, dtEnd);
	}
	catch (...)
	{
		errMessage = "Fatal error:: Code:508. Please contact support.";
		return 0;
	}

	//�������� � ������ ��������.
	if (res != NO_SERIAL_ERROR)
	{
		errMessage = "Fatal error:: Code:509. Please contact support..";
		return 0; //�������� � ������ ��������.
	}

	//�������� ���.
	//������� ���� ��������� �������� ����� ��������.  
	QString prkDateEnd = QString::fromStdString(dtEnd);

	QDate endDatePk = QDate::fromString(prkDateEnd, "dd.MM.yyyy");
	QDate curDate = QDateTime::currentDateTime().date();

	//�������� ���� ��������� �������� ��������� ������. ������ ������������ �� ����.
	if (endDatePk <= curDate.addDays(10))
	{
		errMessage = "The hardware unit will stop working in less than 10d.";
	}

	//�������� �������� ���������� �������.
	try
	{
		QDate LastDate = usb.GetLastHwDate(); //������� ��������� ���� ���������� �  ���������� ����.
		//���� ������� ���� ������, ��������� ���������� � ���������� ����. ����� �������� ������� �� �� ������������.
		if (curDate < LastDate)
		{
			errMessage = "Fatal error:: Code:510. Please contact support.";
			return 0;
		}

		//�������� �������� ������� ���� � ��������� ���� ���������� � ���������� ����.
		quint64 daysPassed = LastDate.daysTo(curDate);

		if (daysPassed > UPDATE_DATE_PERIOD) //�������� ��������� ����.
		{
			SetLastDate(curDate); //���� ���� � ���������� ����.
		}

		//�� ��������� �������� ����� �������� ������� � ���������� ���� �������� ����� 7����.
		quint64 daysToEnd = curDate.daysTo(endDatePk); //������� ���� �������� �� ��������� �������� �����.

		if ((daysToEnd > -1) && (daysToEnd < UPDATE_DATE_PERIOD)) //������ 7�� ��� �������(=0), �� �� ������ �������.
		{
			SetLastDate(curDate); //���� ���� � ���������� ����.
		}

		if ((daysToEnd < 0) || (daysToEnd == 0)) //�������� �����, ��� ������� ����.
		{
			curDate.addDays(1);
			SetLastDate(curDate); //���� ���� � ���������� ����.
			//����� ���������� ������� ��������� ���������� ��������.
		}
	}
	catch (...)
	{
		errMessage = "Fatal error:: Code:506. Please contact support.";
		return 0;
	}

	return 1;
}

//������� ���� RSA.
bool ApiKey::EncodeRsaKey(QString rsaKeyFileName, QString protectProjectName, QString arrayName)
{
	QFileInfo fileInfo(rsaKeyFileName);
	qint64 fileSize = fileInfo.size();

	//������ ����� �� ������� �������� ��� ������ ����� ������ ���������
	if (fileSize == 0)
	{
		LastError = "������ ����� �� ������� �������� ��� ������ ����� ������ ���������";
		return false;
	}

	QFile file(rsaKeyFileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		LastError = "�� ������� ������� ���� �����.";
		return false;
	}

	QByteArray aR = file.readAll();

	if (fileSize != aR.size())
	{
		LastError = "�� ������� ������� ���� �����.";
		return false;
	}

	QByteArray cipherKey; //������� ���� ����������.
	GetCipherKey(cipherKey);
	QByteArray out = Encrypt(aR, cipherKey);

	QByteArray hex = out.toHex();
	//��������� ����� ������� ������ ���� ������ 2!.

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
		LastError = "�� ������� ��������� ������.";
		return false;
	}

	resultFile.write(str.toUtf8());
	resultFile.close();

	return true;
}

//�������� 32  ����� ���� ������� a(n) = 7^n + 8^n + 9^n. ������������ ��� ���� ���������� ���������� rsa. 
//��� ���������� ��� ��� � ��� ������� ����� �������� ������.
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

		//�������� � �������.
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

	//������ ���� ��� �����. ����� ��.
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

//�������� (������ xor) ������ in, �� ����� key.
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

//�������� ���� �������� �� ����������� �����, ���������� ���� ���������� rsa (phyKey). ������ � ���������� ���������.
std::string ApiKey::GetProductKey(uint8_t *phyKey, int len)
{
	QByteArray keyCipher;
	GetCipherKey(keyCipher);
	Decrypt(phyKey, len, keyCipher); //����������� ��������� ����� rsa.
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

//���������� ���� ��������. ��������� �������� ���� rsa �� �����.
std::string ApiKey::ProductSerial(QString fileName)
{
	CryptoPP::RSA::PrivateKey privateKey;
	CryptoPP::FileSource inkey(fileName.toStdString().c_str(), true); //�������� �������� ����������!
	privateKey.Load(inkey);

	return GetProductSerial(privateKey);
}

//������ � �������������� ���� (�������� �����) ��������.
std::string ApiKey::GetProductSerial(CryptoPP::RSA::PrivateKey privateKey) {

	std::vector<uint8_t> data;
	data = usb.GetCryptProductSerial(); //�������� [����������� rsa ��������� ����, 256 ���� ][������, 32 �����]

	uint8_t cryptSessionKey[CIPHER_SESSION_KEY_LEN];
	memcpy(cryptSessionKey, data.data(), data.size() - CIPHER_PRODUCT_KEY_LEN);
	   
	uint8_t sessionKey[SESSION_KEY_LEN];
	DecryptSessionKey(privateKey, cryptSessionKey, CIPHER_SESSION_KEY_LEN, sessionKey);

	return DecryptProductSerial(data, sessionKey);
}

//�������������� ��������� ����.
void ApiKey::DecryptSessionKey(CryptoPP::RSA::PrivateKey privateKey, uint8_t *cryptSessionKey, int keyLen, uint8_t *sessionKey) {

	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::Integer cryptRsaData = CryptoPP::Integer((const byte *)cryptSessionKey, keyLen);
	CryptoPP::Integer decryptResult = privateKey.CalculateInverse(rng, cryptRsaData); //����������� ���������� �����.

	for (int i = 0; i < SESSION_KEY_LEN; ++i)
	{
		sessionKey[i] = decryptResult.GetByte(SESSION_KEY_LEN - 1 - i);
	}
}

//�������������� �������� ����� ��������.
std::string ApiKey::DecryptProductSerial(std::vector<uint8_t> data, uint8_t *sessionKey) {

	uint8_t cryptSerial[MaxPackSize];
	memcpy(cryptSerial, &data.at(CIPHER_SESSION_KEY_LEN), CIPHER_PRODUCT_KEY_LEN);
	aes128_block_dec(cryptSerial, sessionKey, CIPHER_PRODUCT_KEY_LEN);
	
	std::string productSerial;
	for (size_t i = 0; i < CIPHER_PRODUCT_KEY_LEN; ++i) productSerial += cryptSerial[i];
	
	return productSerial;
}

//������ ��������� ������� �� �����. ���������� ������� ���������.
uint8_t ApiKey::SetOutputs(uint8_t data) {
	std::vector<uint8_t> result = usb.SetOutputs(data);
	return result[1];
}

//���������� ��������� ������.
uint8_t ApiKey::InputStatus() {
	std::vector<uint8_t> result = usb.InputStatus();
	return result[1];
}