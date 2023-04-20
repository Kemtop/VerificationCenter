#pragma once
#include <qsqldatabase.h>
#include <qmessagebox.h>
#include <qsqlerror.h>
#include "VanishChest\VanishChest.h"
#include <qsqlquery.h>
#include <qvariant.h>
#include <QTextCodec>
#include <QDateTime>
#include <QStandardItemModel>
#include "modelOpenSignKey.h"
#include "PkContainer.h"
#include <QCoreApplication>

using std::string;

//Интерфейс для работы с БД СКЗИ
class GranitDbI
{

public:
	GranitDbI(QString PathDb);
	GranitDbI();
	~GranitDbI();

	//Задает путь к базе данных.
	void setDbPath(QString PathDb);

	//Открывает соединение с БД по указанному пути.
	bool OpenDb();

	//Открывает соединение с базой данных.
	bool OpenDb(string PathPlugins, string PathDb, string &Error);

	//Отладочная функция для вывода списка драйверов QSqlDatabase. Не удалять!
	void ShowDriversList();

	//Получает сертификат пользователя на основании данных открытого ключа подписи.--
	//Так же сохраняется индекс ключа-UserQIndex;
	//Выполняет необходимые проверки.
	bool GetUserCert(QString Qx, QString Qy, QString F, QString I, QString O);

	//Возвращает индекс подписанта.
	static bool getSignerIndex(QString Qx, QString Qy, string &Qindex);

	
	//Получает сертификат пользователя на основании данных открытого ключа подписи, с минимумом проверок.
	//Используется для сохранения сертификата пользователя из БД.
	bool GetUserCert(QString Qx, QString Qy, QString F, QString I, QString O, QByteArray &uCert_);

	bool GetUserCert(PkContainer &KeyContainer,QByteArray &uCert);

	//Возвращает сертификат считанные функцией  GetUserCert. Если его получилось считать.
	QByteArray getCert();

	//Получает сертификат CA. В случае ошибки выводит сообщение. Возвращает false если ошибка.
	bool getCACert(QByteArray &CertCA);

	//Получает сертификат CA. В случае ошибки выводит сообщение. Возвращает false если ошибка.
	bool getRootCert(QByteArray &CertRoot);

	//Получаю список открытых ключей пользователей. Используется для выбора пользователя которому отправлять сообщение.
	//В случае ошибок выводи сообщение и возвращает false.
	bool getOpenKeyList(QStandardItemModel *model);
	
	//Получает открытый ключ схемы Д-Х для данного пользователя. В случае ошибок выводи сообщение и возвращает false.
	bool getDHOpenKey(string &rHashOkDH,string &Dh_OpenKey,int UserId);	

	//Возвращает индекс открытого ключа.
	string getQIndex();

	//Используя индекс пользователя, получает данные открытого ключа(ключа проверки подписи).
	bool  getSignOpenKey(string & Qindex, string & Qx, string & Qy, QString & FIO);

	//На основании отпечатка сертификата и его номера ищет сам сертификат в БД системы.
	bool findUserCert(string digest,string num,QByteArray &cert);


	//Возвращает дату окончания действия БД.
	bool getDbDateEnd(QString &dtEnd); 

	//Проверяет срок действия БД используя текущую дату.
	bool checkDbDateEnd();

	//Получаю сообщение об ошибке.
	string getLastError();
	
	//Закрытие БД.
	void CloseDb();

private:
	QString PathToDb;//Путь к базе данных.
	QSqlDatabase Db; //База данных
	QByteArray UserCert; //Сертификат пользователя.
	int UserId; //Ид пользователя.
	string UserQIndex; //Индекс открытого ключа текущего пользователя.

	string lastError; //Последнее сообщение об ошибке.

	//Хранит кэшированные данные сведений об открытом ключе проверки подписи.
	QList<modelOpenSignKey> ListSignOpenKey;
	//Получает данные открытого ключа в ListSignOpenKey  по указанному индексу. Если позиция не найдена возвращает false.
	bool  getCachSignOK(string &index,string & Qx, string & Qy, QString & FIO);
	

	//Проверяет контрольную сумму сертификата полученного из бд на основание crc и его длины. В случае ошибок выводит сообщение
	//возвращает false.
	bool CheckCertCrc(QByteArray &Cert,int Len,QString &crc);

	//Проверяет строковые значения периода действия сертификата. Выдает сообщения. В случае проблем возвращает false.
	/*
		QString DtBegin; //Дата начала действия сертификата
	QString DtEnd; //Дата окончания действия сертификата
	*/
	bool CheckCertUsePeriod(QString DtBegin, QString DtEnd);

	//Удалить!!!
	QString GetSignOkFromDb(string & index, string & Qx, string & Qy, QString & FIO);

	//Выводит сообщение об ошибке.
	void ShowError(string message);
};


