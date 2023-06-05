#include "dllHwKey.h"
#include "CheckSerial.h"


//Выполняю весь процесс проверки серийного номера.
__declspec(dllexport)
int verify(char * dateEnd)
{
	try
	{
		UsbKey usb;
		usb.InitUsb();

		if (!usb.IsConnected()) return -1; //Ключ не найден.

		std::string productKey = usb.GetSerial();

		//Проверка ключа продукта. В случае проблем возвращает 0, если все ок возвращает 1.
		CheckSerial CPk;
		std::string pkDateEnd = "01.01.1905"; //Дата окончания действия ключа продукта.
		int xReti = 12;
		QString str = QString::fromStdString(productKey);
		xReti = CPk.CheckProductKey(str, pkDateEnd);

		if (xReti == 0) return 0;
		int len = pkDateEnd.length() + 1; //+1 для завершающего символа.

		strcpy_s(dateEnd, len, pkDateEnd.c_str()); //Передаю строку.

		return 1;
	}
	catch(...)
	{
		return -14;
	}
}
