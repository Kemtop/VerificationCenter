#pragma once
#include "hash3411\hash3411.h"
#include "Gost3412\Utility.h"
#include "Gost3412\Sib3412.h"
#include <QDomDocument>
#include "PkContainer.h"
#include "rsa.h" //CryptLib
#include "osrng.h"
#include "Point.h"
#include "CreateSign.h"
#include <QFileInfo>
#include <QObject>
#include <QByteArray>
#include <Windows.h>
#include <QMetaProperty>
#include <QTextCodec>
#include <QVariant>
#include "GranitDbI\GranitDbI.h"
#include "TBSCertificate.h"
#include "X509Reader\X509Reader.h"
#include "SingleSign\ScKeeper.h"

//Размер Заголовока файла-служебная информация [7-байт служебной информации][Заголовок 47байт][Хэш контента 256бит][Длина контейнера ключа 4байта]
#define Key_title_len 90


using std::string;
using namespace CryptoPP;

//Класс для взаимодействия с закрытыми ключами
class KeyService
{
public:
	KeyService();
	~KeyService();

	/*
	Проверяет секретный ключ, в случае ошибки возвращает 0
	Проверка поверхностная. Проверяется соответствие заголовка и контрольной суммы контейнера ключа
	Возращает размер контейнера ключа container_length
	*/
	bool CheckSK(QString PathToSK, uint32_t &container_length);

	//Считывает контейнер ключа из файла. Расшифровывает его, помещает в объект ключа KeyContainer. 
	bool LoadKey(QString PathToSecretKey);
	//Проверяет пароль
	bool KeyService::CheckPassword(QString psw);
	//Возвращает открытый ключ
	Point GetPk();
	//Возвращает закрытый ключ
	mpz_class GetPrvEcKey();

	string GetCertAlgorithmIdentifier();//Возвращает алгоритм подписи сертификата данным ключем.
	string GetPublicKeyAlgoritm();//Возвращает алгоритм открытого ключа
	string GetEcParam();//Возвращает OID эллиптической кривой, на основании которой создан ключ.

	//Возвращает параметры кривой. Если не загружен ключ, возвращает false.
	bool getEcParam(string &Value);

	string getLastError(); //Возвращает сообщение об ошибке.
	bool getKeyContainerStatus(); //Возвращает статус загрузки секретного ключа.

	PkContainer getContainer() const; //Возвращает объект ключа.

	 //Проверяет валидность срока действия ключа.
	bool  CheckKeyDateValidity();

	//Проверяет соответствие закрытого ключа и сертификата находящегося в БД системы для данного ключа.
	bool CheckKeyCert(GranitDbI &Dbi);

	//Получает информацию о владельце секретного ключа.
	bool GetSkInfo(string &Familia, string &Imia, string &Otchestvo, string &DateBegin, string &DateEnd, string &DaysLeft);

	
	void enableCachedSk(); //Включаю режим кеширования секретного ключа. Данные ключа кешируются при проверке пароля.
	bool isCacheSk(); //Был ли закеширован ключ подписи?

	//Возвращает индекс подписанта.
	void getSignerIndex(string &Value); 
	
	//Возвращает закрытый ключ подписи.
	mpz_class getSignPk();

	//Возвращает данные открытого ключа подписи.
	void getSignOpenKey(string &Qx, string &Qy);

	//Очищает секретный ключ подписи, если он был загружен.
	void Clear();

	//Возвращает пользовательский сертификат.
	TBSCertificate getUserCert();
	void setRoseMode(); //Включает модифицированную версию ключа.	

	//Возвращает открытый ключ схемы ДиффиХэлмана.
	string getOpenDHkey();

private:
	//Включит режим другой версии ключа.
	bool enableRose;
	
	
	string lastError; //Ошибка.
	bool KeyContainerIsLoad; //Флаг того что контейнер ключа был загружен из файла
	bool cachedSk; //Разрешить кеширование секретного ключа.
	//Содержимое пользовательского сертификата.
	TBSCertificate  tbsUserCertificate; 

	SkKeeper Skd; //Хранит секретный ключ, если это требуется.

	typedef int (*AreApisANSI)(char *Linear, int *Len, char *Value); //Тип данных для упаковки контейнера ключа.
	
   //Получает значение ключа упаковывания контейнера. В случае ошибки HasErr=1;Err-содержит сообщение об ошибке.
	ByteBlock  KeyService::GetPackKv(string & Err, bool & HasErr);

	//Расшифровывает контейнер секретного ключа и проверяю его заголовок, все хорошо =1
	/*
	в случает ошибки:
	=2; //Не удалось загрузить библиотеку libCn3q.dll.
	=3; //Не удалось загрузить функцию.
	= 4; //Странная функция в dll
	=5 //Не верный контейнер ключа
	*/
	int UnpackSKContainer(QByteArray &C, QByteArray &Result);
	
	PkContainer KeyContainer; //Контейнер секретного ключа
	//Десериализует контейнер
	bool DeserializeKeyContainer(QByteArray *Maket, QObject *object);
	//Расшифровывает закрытый ключ схемы Диффи-Хэлмана
	bool DecryptDH_SK(QString Password,RSA::PublicKey &publicKey, RSA::PrivateKey &secret_key);
	//Получает результат хэширования пароля и соли
	bool HashedPassword(QString &Passwd, QString &salt_str, uint8_t *hash);

	//Расшифровывает секретный ключ CryptedKey используя key
	bool KeyService::DecryptSK(uint8_t *key, QString &CryptedKey, QString &iv, string &p_key);
	//Проверяет пару ключей схемы Диффи-Хелмана. Если пара не совпадает возвращает 0
    bool KeyService::GetDhPair(QString &open_key, string &private_key, RSA::PublicKey &publicKey, RSA::PrivateKey &secret_key);
	/*
	Расшифровывает закрытый ключ подписи.
	Проверяет соответствие закрытого и открытого ключа. В случае не соответствия возвращает 0;
	Point &Q-открытый ключ ;mpz_class &d -закрытый ключ
	*/
	bool DecryptEc_SK(QString Password, Point &Q, mpz_class &d);
	/*
	Проверяет соответствие открытого и закрытого ключа
	ec_name-имя кривой
	*/
	bool CheckEcPair(Point &Q, mpz_class &d, string ec_oid);
	//Открытый ключ для эллиптической криптографии
	Point pubECkey;
	//Закрытый ключ для эллиптической криптографии
	mpz_class prvEcKey;

	//Сравнивает открытый ключ сертификата и ключ в контейнере.
	bool CompareKeys(TBSCertificate  &tbsUser);

	//Проверяет срок действия сертификата. В случае проблем выдает сообщение. Возвращает false. Успех =true.
	bool CheckValidityCert(TBSCertificate cert);
};

