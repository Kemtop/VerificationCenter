#pragma once

#include <QObject>
#include <string>
#include "hash3411\hash3411.h"
#include "Gost3412\Utility.h"
#include "Gost3412\Sib3412.h"
#include <QDomDocument>
#include "PkContainer.h"
#include "rsa.h"
#include "osrng.h"
#include "mpirxx.h"
#include "SignLib\Point.h"
#include "SignLib\CreateSign.h"
#include "�ertificate�reator\TBSCertificate.h"
#include "X509Reader\X509Reader.h"
#include <qsqldatabase.h>
#include <qsqlerror.h>
#include <QSqlQuery>
#include <QSqlRecord>
//#include "SysParam\SysParam.h"
//#include "ScKeeper\ScKeeper.h"
#include "AsInterpretator\AsInterpretator.h"
#include "ASreader\ASreader.h"
#include "KeyService\KeyService.h"
#include "rfcSignedData.h"
#include "binASN1SignedData.h"
#include "X509Reader\X509Reader.h"

using std::string;

class SingleSign : public QObject
{
	Q_OBJECT

  public:
	SingleSign();
	~SingleSign();

	//�������� ������ ��� ������ � ������.
	void setKeyService(KeyService &KS);

	//�������� ������ ��� ������ � �� �������.
	void setDb(GranitDbI &Db);

	/*
	��������� ������� ������ ������������ � ������� Message ������ Mlen.
	�������� ��������� ����� ���������� � KeyStamp.
	�������� �������� �������(������� r � s ) � SignStr,����� ������� � Slen.
	� ������ ������ ���������� ���������. ���� ������� ������������ �������-���������� ������ ������.
	������������ ����� ��������� ������ �������� 2��, ��� ������� ����� ��������� ������ �� ���������)
	*/
	bool GrKSignData(uint8_t *Message, int Mlen,string &SignStr, uint32_t &Slen);


	/*
	��������� �������� �������.
	uint8_t *Message-���������
	int Mlen-����� ���������
	string KeyStamp-��������� ����� ����������.
	string SignValue-�������
	string &FIOp-��� ����������
	*/
	bool GrKCheckSign(uint8_t *Message, int Mlen, string SignValue, string &FIOp);
	
	//������ ������, ������� ��� ��� �����.
	void ClearRAM();

	//���������� ��������� �� ������.
	string getLastError();

   private:
	   KeyService *ks;
	   GranitDbI *dBi;  //������ ��� ������ � �� ������.

	   string lastError;
};
