#pragma once

#include <QTextCodec>

#include <QFileDialog>
#include <QDomDocument>
#include <QMetaProperty>
#include "PkRequest.h"
#include <QTimer> //Для работы таймера
#include <QKeyEvent> //Для возможности узнавать код клавиши
#include <QRandomGenerator>
#include "mpirxx.h"
#include <qdebug.h>
#include <hash3411\hash3411.h>
#include "Gost3412\Utility.h"
#include "Gost3412\Sib3412.h"
#include "Point.h"
#include "PkContainer.h"
#include <QXmlStreamWriter>
#include "rsa.h"
#include "osrng.h"
#include "CreateSign.h"
#include <Windows.h>
#include "Models\mRequest.h"
#include "SerializableTools\SerializableTools.h"
#include "VanishChest\VanishChest.h"

#include <string>

using std::string;

using namespace CryptoPP;
using CryptoPP::AutoSeededRandomPool;

#define KeyGenVersion "1.4.2.7"
//Размер Заголовока файла-служебная информация [7-байт служебной информации][Заголовок 47байт][Хэш контента 256бит]
//[Длина контейнера ключа 4байта][[iv 16байт][4байта длина контейнера]Контейнер]
#define Key_title_len 90

//Содержит логику для создания закрытого ключа.
class SecretKeyMaker : public QObject
{
	Q_OBJECT
private:
	static const QString  G_publicKeyAlgoritmOid;//Алгоритм открытого ключа подписи.
	static const QString G_EcOid; //Идентификатор эллиптической кривой на основании  которой создается ключ.
	/*
	Создает ключевую пару  ГОСТ Р 34.10-2018 для формирования подписи.
	Генерирует 17 байтную соль.
	Формирует строку для функции хэширования "пароль+соль"
	Вычисляет значение K= (256бит)хэш функции по ГОСТ Р 34.11-2018.
	Формирует начальный вектор iv.
	Используя K,iv шифрует закрытый ключ.
	*/
	bool сreateInvisibleSignKey(unsigned char *Key, QString Password, Point &Q, string &str_iv_, string &salt_, string &crypt_secret_key_);

	/*
	Создает пару ключей  открытый и закрытый для накладывания и проверки подписи;
	//ECname-OID эллиптической кривой;Q-открытый ключ; d-закрытый ключ
	secret_key-закрытый ключ
	uint8_t key_len-размер ключа в байтах 32 или 64
	*/
	bool CreateEcKeyPair(bool BeginSelfTest, string EcOID, uint8_t *secret_key, uint8_t key_len, Point &Q, mpz_class &d);

	//Генерирует 17 байтную соль.
	bool GenerateSalt(string &d);

	//Хэширует пароль с солью, в случае ошибки возвращает сообщение
	bool HashedPassword(QString &Password, string salt, unsigned char *hash);

	//Создает и упаковывает ключевую пару схемы Диффи-Хэлмана.
	bool сreateInvisibleDHkey(QString Password, string &str_iv_, string &salt_, string &crypted_secret_DH_key_, string &DH_public_);

	//Генерирую вектор инициализации
	bool GenerateIV(unsigned char *iv);

	//Шифрую секретный ключ d подписи сообщения, в качестве ключа используется хэш, в случае ошибки возвращает сообщение
	//Режим шифрования-Режим обратной связи по шифротексту
	bool CryptEcSecretKey(string &result, unsigned char *hash, unsigned char *iv, mpz_class &d);

	//Конвертируют байтовый массив в строку
	void ByteArrayToHexString(unsigned char *array, int len, string &str);

	//Конвертирует 16 ричную строку в массив unsigned char
	void HexStr_to_uchar(string str, unsigned char *array);

	//Генерирую ключевую пару для шифрования Диффи-Хэлмана
	bool GenerateDHpair(string & DH_public, uint8_t *private_key, uint32_t &private_key_len);

	//Конвертирует бинарные данные в строке типа string в  16ричное представление QString
	QString str_to_Hex(string & str);

	//Сохраняет контейнер ключа в файл
	bool KeyСontainerToFile(PkContainer &KeyCont, QString PathToSave);

	//Запаковывает контейнер секретного ключа
	bool PackContainer(QByteArray &Container, QByteArray &Packing);

	//Получает значение ключа упаковывания контейнера. В случае ошибки HasErr=1;Err-содержит сообщение об ошибке.
	ByteBlock GetPackKv(string & Err, bool & HasErr);

	//Получает значение ключа упаковывания макета. В случае ошибки HasErr=1;Err-содержит сообщение об ошибке.
	ByteBlock GetPackMf(string & Err, bool & HasErr);

	//Создает файл запроса.
	bool CreateRequestFile(QString pathToSave, mRequest &Req);

	//Шифрую секретный ключ алгоритма Диффи-Хэлмана,в качестве ключа используется хэш, в случае ошибки возвращает сообщение
	bool CryptDHSecretKey(string &crypted_key, uint8_t *private_key, uint32_t &private_key_len, unsigned char *hash, unsigned char *iv);
	
	//Вычисляет хэш256 контейнера
	void GetHashСontainer(uint8_t *container, uint32_t &container_len, uint8_t *hash);

		/*
	   Заполняет массив служебной информацией-фактически формирует заголовок файла
	   [                    arr    54байта           ][  hash   32байта   ] 
	   [7-байт служебной информации][Заголовок 47байт][Хэш контента 256бит][Длина контейнера ключа 4байта] [Контейнер ключа]
	   [7+47+32=86байт][Контейнер ключа] 
	*/
	void SetService_Information(uint8_t *arr, uint8_t *hash,uint32_t &conteiner_len);

	string lastError;

	//Включит режим другой версии ключа.
	bool enableRose;

public:
	SecretKeyMaker();
	~SecretKeyMaker();

	void setRoseMode(); //Включает модифицированную версию ключа.
	

	//Создает файл закрытого ключа и запроса на сертификат.
	bool CreateKeyFile(unsigned char *Key, QString Path, QString &Password, mRequest &Req, QString pathToSaveReq);

	//Создаю файл закрытого ключа, без запроса на сертификат.
	bool CreateKeyFileNoReq(unsigned char *Key, QString Path, QString &Password);

	string getLastError();
};
