#pragma once

#include "libhwkey_global.h"
#include "stdint.h"
#include <vector>
#include "files.h"
#include <QFileInfo>
#include <QDate>
#include "libusb.h"

#define DEV_VID 0x0483  //������������� USB ���������� (�����). 
#define DEV_PID 0x5711

#define DEV_INTERFACE 0x00

/*
  ��������� ���������(������� ������� ��. ������ OSI).
  [������� 1 ����][������ n ����]

  ��������� ������(������������ �������)
  [������� 1 ����][����� ������ 1 ����.][������ n(����) = MaxPackSize-6 = 58;][����������� �����, 4 ����� CRC32]
  */

//������������ ������ ���������.
#define MaxMessageSize 0x800  //2048

//������������ ������ ������ ������, 64 �����.
#define MaxPackSize 0x40

#define ENDP_IN 0x81
#define ENDP_OUT 0x01

#define TIMEOUT 200000

#define BEGIN_YEAR 2019 //���� ������������ ������� ��������� ������� ���� (���� ������ ���������� ����).
#define BEGIN_MONTH 1
#define BEGIN_DAY 1

#define GET_HW_ID_COMMAND 0x3C //������������� ������� ��������� �������� ����� ����������.
#define SAVE_SERIAL_COMMAND 0xa0 //��������� �������� ����� ��������.
#define SAVE_DATE_COMMAND 0x15 //��������� ���� (� ����).
#define GET_DATE_COMMAND 0x17 //�������� ���� (� ����).
#define SAVE_RSA_KEY_COMMAND 0xa9 //��������� ������ n ��������� ����� rsa (256 ����).
#define GET_CRYPT_SERIAL_COMMAND 0x07 //�������� ����������� ������ �������� ����� ��������.
#define GET_RANDOM_COMMAND 0x50 //�������� ��������� �����.

#define ANSWER_BAD_CRC 0xFF //�������� CRC
#define ANSWER_UNKNOWN_COMMAND 0xF0

#define TRANSPORT_START_PACKET_TRANSMISSION 0xb0 //������ �������� ��������.
#define TRANSPORT_SENDING_PACKAGE 0xb1 //�������� ������.
#define TRANSPORT_EXECUTE_COMMAND 0xb2 //���������� ��������� �� ������� usb ����������.
#define TRANSPORT_GET_RESULT 0xb3 //�������� ����� �� USB ����������. ����� ��������.

#define SERIAL_NUM_LEN 29 //����� ��������� ������ (����� ��������).

#define CRC_LEN 32 //����� ����������� �����.
#define SESSION_KEY_LEN 16 //����� ���������� �����.
#define CIPHER_SESSION_KEY_LEN 256 //����� ������������ rsa ���������� �����.
#define CIPHER_PRODUCT_KEY_LEN 32 //������ ����� � ������������� ������ ��������.
#define UPDATE_DATE_PERIOD 7 //������ ���������� ���� �����.

enum USBERROR
{
	NOT_CONNECTED = -1,
	NO_GET_INTERFACE = -2,
	ERROR_WRITE = -3,
	ERROR_READ = -4,
	UNKNOWN_COMMAND = -5
};

//��������� ��� �������������� � USB ������.
class  UsbKey
{
public:
	UsbKey();

	//��������� ��������� �� ������.
	QString LastError;

	//��������� ���������� �� ����������.
	bool IsConnected();
	//��������� ������ n rsa �����.
	void SendRSAKey(uint8_t key[CIPHER_SESSION_KEY_LEN]);

	std::string GetHWSerial();
	void SetSerial(uint8_t serial[SERIAL_NUM_LEN]); //��������� �������� ����� ��������.
	void SetLastDate(uint16_t days);
	void SetLastDate(QDate lastDate); 	//���� � ���������� ���� ��������� ����.
		
	uint16_t GetLastDate();
	QDate GetLastHwDate(); //������� ��������� ���� �� ����������� �����.

	std::vector<uint16_t> GetRandData(); //��������� ���������� �����.
	std::vector<uint8_t> GetCryptProductSerial(); //���������� ����������� ���� ��������.

	void InitUsb();

private:	
	/*
	   �������� ������ USB �����, � ��������� ������.
	   command - ������� ������������� ������, 1 ����
	   inBuffer - ������ ��� ��������
	   inLen - ����� ������������ ������
	   outBuffer - ������ ���������� �� ����������
	   outLen - ��������� ������ ������

	   ������� ������������� ������:
	   0xb0 - ������ �������� ��������.
	   0xb1 - �������� ������
	   0xb2 - ���������� ��������� �� ������� usb ����������. ����� �� ���������� �������� ������ ������, ������� ����� ��������
	   ����� ��������� ���������. ����� ��� �����.
	   0xb3 - �������� ����� �� USB ����������. ����� ��������.
	*/
	int Command(uint8_t command, std::vector<uint8_t> inBuffer, int inLen, std::vector<uint8_t>& outBuffer, int outLen);
	
	/*
	  ����� ������� ���������� ��������� USB ����� � ��������� ����� �����. 
	  command - �������;
	  inBuffer - ������ ��� ��������;
	  inLen - ����� ������ ������� ����� �������� USB �����;
	  outBuffer - �������� �����, �������� ����� USB �����.
	*/
	int Packcom(uint8_t command, std::vector<uint8_t> inBuffer, int inLen, std::vector<uint8_t>& outBuffer);

	//���������� ���������������� ������. ������� � ������ 1 ���� � �������.
	std::vector<uint8_t> GetDataFromPacket(std::vector<uint8_t>& outBuffer);
};
