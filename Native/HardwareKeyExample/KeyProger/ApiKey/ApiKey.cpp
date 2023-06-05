#include "ApiKey.h"

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

	if (!usb.EncodeRsaKey(keyFileName, vectorPath, cryptArrayKeyName))
	{
		LastError = usb.LastError;
		return false;
	}

	usb.SendPublicKey(keyFileName);

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

//Возвращает ключ продукта.
std::string ApiKey::Serial(QString keyName) {
	return usb.GetSerial(keyName);
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