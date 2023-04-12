#pragma once

#include "libhwkey_global.h"
#include "stdint.h"
#include <vector>
#include "rsa.h"
#include "files.h"
#include "osrng.h"
#include <QFileInfo>
#include <streambuf>




#define DEV_VID 0x0483
#define DEV_PID 0x5711

#define DEV_INTERFACE 0x00


//using namespace CryptoPP;

/*
  ��������� ���������(������� ������� ��. ������ OSI).
  [������� 1 ����][������ n ����]

  ��������� ������(������������ �������)
  [������� 1 ����][����� ������ 1 ����.][������ n(����)=MaxPackSize-6=58;][����������� �����,4 ����� CRC32]
  */


//������������ ������ ���������.
#define MaxMessageSize 0x800  //2048

//������������ ������ ������ ������, 64 �����.
#define MaxPackSize 0x40

#define ENDP_IN 0x81
#define ENDP_OUT 0x01

#define TIMEOUT 200000

enum USBERROR
{
	NOT_CONNECTED = -1,
	NO_GET_INTERFACE = -2,
	ERROR_WRITE = -3,
	ERROR_READ = -4,
	UNKNOWN_COMMAND = -5
};

//
//��������� ��� �������������� � USB ������.
//LIBHWKEY_EXPORT
class  UsbKey
{
public:
	UsbKey();
	//��������� ���������� �� ����������.
	bool isConnected();

	void sendRSAKey(uint8_t key[256]);
	std::string getSerial();




	std::string getHWSerial();
	void setSerial(uint8_t serial[29]);
	void setLastDate(uint16_t days);
	//���� � ���������� ���� ��������� ����.
	void setLastDate(QDate lastDate);

	
	uint16_t getLastDate();
	QDate getLastHwDate(); //������� ��������� ���� �� ����������� �����.

	std::vector<uint16_t> getRandData();

	void SendPublicKey();
	void InitUsb();

	//����������� ���� ���������� ���������� ������.
	int translateChannelKey();

	//��������� � ��������� ���� ��������, ���� ��� � ������� ������������ 1, ����� 0 � ��������� �� ������.
	//����� �������� ����� 10 ���� �� ��������� ����� ��������-���������� errMessage.
	int che�kProduckKey(std::string &errMessage, uint8_t *phyKey, int len);


private:
	//�������� ���� �������� �� ����������� �����, ���������� ���� ���������� ���������� ������.
	std::string getProductKey(uint8_t *phyKey, int len);

	/*
	   �������� ������ USB �����, � ��������� ������.
	   command-������� ������������� ������, 1 ����
	   inBuffer-������ ��� ��������
	   inLen-����� ������������ ������
	   outBuffer-������ ���������� �� ����������
	   outLen-��������� ������ ������

	   ������� ������������� ������:
	   0xb0-������ �������� ��������.
	   0xb1-�������� ������
	   0xb2-���������� ��������� �� ������� usb ����������. ����� �� ���������� �������� ������ ������, ������� ����� ��������
	   ����� ��������� ���������. ����� ��� �����.
	   0xb3-�������� ����� �� USB ����������. ����� ��������.
	*/
	int command(uint8_t command, std::vector<uint8_t> inBuffer, int inLen, std::vector<uint8_t>& outBuffer, int outLen);
	/*
	  ����� ������� ���������� ��������� USB ����� � ��������� ����� �����. 
	  command -�������;
	  inBuffer-������ ��� ��������;
	  inLen-����� ������ ������� ����� �������� USB �����;
	  outBuffer-�������� �����, �������� ����� USB �����.
	*/
	int packcom(uint8_t command, std::vector<uint8_t> inBuffer, int inLen, std::vector<uint8_t>& outBuffer);

	/*
	   �������� 32  ����� ����.	a(n) = 7^n + 8^n + 9^n.
	   0        3
	   1		24
	   2		194
	   3		1584
	   4		13058
	   5		108624
	   6		911234
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
	void getTr1(QByteArray &ba);

	//�������� ������ In, ��������� �������� mask.
	QByteArray vanishAlg(QByteArray &In, QByteArray &mask);

	void unVanish(uint8_t *in, int o_size);
};
