#include "dllHwKey.h"
#include "CheckSerial.h"


//�������� ���� ������� �������� ��������� ������.
__declspec(dllexport)
int verify(char * dateEnd)
{
	try
	{
		UsbKey usb;
		usb.InitUsb();

		if (!usb.IsConnected()) return -1; //���� �� ������.

		std::string productKey = usb.GetSerial();

		//�������� ����� ��������. � ������ ������� ���������� 0, ���� ��� �� ���������� 1.
		CheckSerial CPk;
		std::string pkDateEnd = "01.01.1905"; //���� ��������� �������� ����� ��������.
		int xReti = 12;
		QString str = QString::fromStdString(productKey);
		xReti = CPk.CheckProductKey(str, pkDateEnd);

		if (xReti == 0) return 0;
		int len = pkDateEnd.length() + 1; //+1 ��� ������������ �������.

		strcpy_s(dateEnd, len, pkDateEnd.c_str()); //������� ������.

		return 1;
	}
	catch(...)
	{
		return -14;
	}
}
