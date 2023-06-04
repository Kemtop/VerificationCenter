#pragma once

#include "dllhwkey_global.h"
#include "UsbKey.h"
#include <QMessageBox>

/*

class DLLHWKEY_EXPORT dllHwKey
{
public:
	dllHwKey();

	void InitUSB();
	
	int  SetSerial(std::string serial);
	std::string GetHWSerial();
	unsigned int GetLastDate();
	int SetLastDate(unsigned int value);
	//Определяет подключено ли устройство.
	int DeviceConnected(); 
	//Устанавливает открытый ключ RSA, для шифрования серийного номера.
	void  setTransportPubKey();

	//Вспомогательный метод, проверяет ключ продукта.В случае ошибки возвращает 0.
    //ProductKey - ключ продукта;
	//dateEnd - дата окончания срока действия ключа продукта.
	int VerifyProductKey(char *ProductKey, char *dateEnd);

private:
	UsbKey usb;
};
*/