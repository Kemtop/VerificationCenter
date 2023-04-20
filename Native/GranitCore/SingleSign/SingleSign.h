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
#include "СertificateСreator\TBSCertificate.h"
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

	//Передает объект для работы с ключом.
	void setKeyService(KeyService &KS);

	//Передает объект для работы с Бд системы.
	void setDb(GranitDbI &Db);

	/*
	Выполняет подпись данных содержащихся в массиве Message длиной Mlen.
	Помещает отпечаток ключа подписанта в KeyStamp.
	Помещает цифровую подпись(вектора r и s ) в SignStr,длину подпись в Slen.
	В случае ошибки возвращает сообщение. Если подпись сформирована успешно-возвращает пустую строку.
	Теоретически можно подписать массив размером 2Гб, при желании можно увеличить размер до пентобайт)
	*/
	bool GrKSignData(uint8_t *Message, int Mlen,string &SignStr, uint32_t &Slen);


	/*
	Выполняет проверку подписи.
	uint8_t *Message-сообщение
	int Mlen-длина сообщения
	string KeyStamp-отпечаток ключа подписанта.
	string SignValue-подпись
	string &FIOp-ФИО подписанта
	*/
	bool GrKCheckSign(uint8_t *Message, int Mlen, string SignValue, string &FIOp);
	
	//Чистит память, вычищая все что можно.
	void ClearRAM();

	//Возвращает сообщение об ошибке.
	string getLastError();

   private:
	   KeyService *ks;
	   GranitDbI *dBi;  //Объект для работы в БД ключей.

	   string lastError;
};
