#include "ApiKey.h"

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
		LastError = "����������� ���� rsa.";
		return false;
	}

	if (!usb.EncodeRsaKey(keyFileName, vectorPath, cryptArrayKeyName))
	{
		LastError = usb.LastError;
		return false;
	}

	usb.SendPublicKey(keyFileName);

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

//���������� ���� ��������.
std::string ApiKey::Serial(QString keyName) {
	return usb.GetSerial(keyName);
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