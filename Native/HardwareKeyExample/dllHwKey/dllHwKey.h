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
	//���������� ���������� �� ����������.
	int DeviceConnected(); 
	//������������� �������� ���� RSA, ��� ���������� ��������� ������.
	void  setTransportPubKey();

	//��������������� �����, ��������� ���� ��������.� ������ ������ ���������� 0.
    //ProductKey - ���� ��������;
	//dateEnd - ���� ��������� ����� �������� ����� ��������.
	int VerifyProductKey(char *ProductKey, char *dateEnd);

private:
	UsbKey usb;
};
*/