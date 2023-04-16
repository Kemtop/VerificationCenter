#pragma once

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
#include "CipherGearData.h"
#include <QThread>
#include "eventsCipherWorker.h"
#include "GranitDbI\GranitDbI.h"
//#include "ASreader\ASreader.h"

using  std::string;
using namespace CryptoPP;
using CryptoPP::AutoSeededRandomPool;

//Управляющий класс запускающий шифрование.
class CipherWorker : public QThread
{

public:
	CipherWorker(QObject *val) :threadEvens(val)
	{
		codec = QTextCodec::codecForName("Windows-1251");
		reportInUI = false;
		enableRose = false;
	};

	//Конструктор используется в dll.
	CipherWorker()
	{
		reportInUI = false;
		enableRose = false;
		IsDisabledEventProcess = true;
	};

	~CipherWorker();

	bool CryptFiles(QStringList &paths_to_file, QString Password, string receiverKey, PkContainer &KeyContainer,
		string SignerIndex, string _rHashOkDH); //Шифрует файлы.
	
	//Выполняет операцию расшифровки выбранных файлов
	bool DecryptFiles(QStringList &paths_to_file, QString DbPath, QString UserPassword,
		PkContainer KeyContainer);

	string getLastError(); //Возвращает ошибку.
	
	void setData(CipherWorkerData data); //Устанавливает параметры работы. 
	void run(); //Переопределение родительского метода.
	//Извлекает ключ подписи из контейнера ключа.
	bool DecryptEc_SK(Point & Q, mpz_class & d, QString Password, PkContainer &KeyContainer);
	//Шифрую 32 байтный сеансовый ключ.
	bool CryptedSessionKey(uint8_t *session_key, string &result);
	//Шифрую файл по указанному пути
	bool CryptFile(QString src_path, QString dst_dir, Cipher3412 &Cipher, uint8_t *Reg, string &CryptedKey,
		mpz_class &user_d, PkContainer &KeyContainer);
	RSA::PublicKey asymmetricKeyR; //Открытый ключ получателя.
	//Отключает генерацию событий обработки файлов.
	bool IsDisabledEventProcess;
private:
	QObject* threadEvens; //Атрибут, указывающий на объект-получатель нашего события.	
	CipherWorkerData baseData; //Основные параметры для запуска шифрования.
	void sendEventAllProcessVal(int val); //Отправляет событие значения обработки всех файлов.
	void sendEventProcessFileInfo(string msg); //Отправляет событие информирования о процессе обработки файла.
	void sendEventThreadStatus(int status);
	void sendEventCurFileProcess(int val);//Отправляет событие прогресса обработки файла.
	void sendMaxFileProcess(int val);//Отправляет событие максимального значения прогресса обработки файла.

	bool initDistDir(QString val, QString &destinationDir);//Инициализирует папку назначения.
	bool enableRose;//Включен альтернативный режим работы.

	QTextCodec *codec;
	//QString destinationDir; //Путь к директории куда будут складываться обработанные файлы.(шифрованный или расшифрованные)
	string lastError;  //Хранит последнюю ошибку.
	string signerIndex;   //Индекс открытого ключа подписанта
	string rHashOkDH; //Значение сигнатуры открытого ключа ДХ.
	bool reportInUI;//Флаг включение вывода отчета в пользовательский интерфейс.

	bool CheckEcPair(Point & Q, mpz_class & d, string  ecOid);
	//Получает результат хэширования пароля и соли
	bool HashedPassword(QString &Passwd, QString &salt_str, uint8_t *hash);
	//Расшифровывает секретный ключ CryptedKey используя key
	bool DecryptSK(uint8_t *key, QString &CryptedKey, QString &iv, string &p_key);
	
	//Формирует заголовок шифруемого файла, записывает его в файл
	bool WriteCryptFileTitle(QFile &file, qint64 file_len, qint64 &file_pos);
	bool SignFile(QFile & file, mpz_class & user_d, PkContainer &KeyContainer); //Подписывает файл.

	//Используя путь к файлу, создает директорию для сохранения расшифрованных файлов, возвращает путь к директории
	bool CreateDstDecryptDir(QString fpath, QString &DirPath);

	//Расшифровываю файл
	bool DecryptingFile(QString src_path, QString dst_dir, QString DbPath,
		QString UserPassword, PkContainer &KeyContainer);

	//Проверяет подпись файла
	bool CheckSign(ASreader &ASr, QFile &file, QString DbPath, PkContainer &KeyContainer);
	//Расшифровка ключа блочного шифра
	bool ASdecryptSecretKey(ASreader &Data, uint8_t *key, QString Password, PkContainer &KeyContainer);

	//Расшифровывает закрытый ключ схемы Диффи-Хэлмана
	bool DecryptDH_SK(RSA::PublicKey &publicKey, RSA::PrivateKey &secret_key,
		QString Password, PkContainer &KeyContainer);

	//Проверяет пару ключей схемы Диффи-Хелмана. Если пара не совпадает возвращает 0
	/*
	Получает закрытый и открытый ключе схемы Диффи-Хелмана
	Тестирование ключевых пар содержащихся в контейнере.
	Грузится открытый ключ, если ключ нельзя загрузить-выводится вообщение возвращается 0
	Грузится закрытый ключ. Если его не удалось расшифровать, то загрузить его не возможно-выводится вообщение возвращается 0
	Сравнивается значение модулей n открытого и закрытого ключа
	*/
	bool GetDhPair(QString &open_key, string &private_key, RSA::PublicKey &publicKey, RSA::PrivateKey &secret_key);
};
