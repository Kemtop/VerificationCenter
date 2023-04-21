#pragma once

#include <string>
#include <iostream>

#include "KeyService.h"
#include "SingleSign.h"
#include "CipherWorker.h"
#include <QObject>
#include <QFileInfo>
#include <QDir>
#include "Gost3412\Cipher3412.h"
#include "Gost3413\Gost3413.h"
#include "rsa.h"
#include "osrng.h"
#include "PkContainer.h"
#include "Point.h"
#include "CreateSign.h"
#include <QByteArray>
#include <QTextCodec>
#include "Gost3412\Utility.h"
#include "Gost3412\Sib3412.h"
#include "AsInterpretator\AsInterpretator.h"
#include "ASreader\ASreader.h"
#include "VerifySign.h"
#include <QThread>
#include "eventsCipherWorker.h"
#include "GranitDbI\GranitDbI.h"

using std::string;
//#include "libgranitk_global.h"

KeyService keyService; //Объект для работы с секретным ключом.	
GranitDbI dBi;
SingleSign Sg;
//Открытый ключ получателя. Им кодируется сеансовый ключ.
RSA::PublicKey asymmetricKeyR;
//Закрытый ключ подписи согласно ГОСТ.
mpz_class user_d;
//Методы шифрования.
CipherWorker *cipherWorker;
