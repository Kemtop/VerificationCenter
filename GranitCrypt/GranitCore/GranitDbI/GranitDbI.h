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

//��������� ��� ������ � �� ����
class GranitDbI
{

public:
	GranitDbI(QString PathDb);
	GranitDbI();
	~GranitDbI();

	//������ ���� � ���� ������.
	void setDbPath(QString PathDb);

	//��������� ���������� � �� �� ���������� ����.
	bool OpenDb();

	//��������� ���������� � ����� ������.
	bool OpenDb(string PathPlugins, string PathDb, string &Error);

	//���������� ������� ��� ������ ������ ��������� QSqlDatabase. �� �������!
	void ShowDriversList();

	//�������� ���������� ������������ �� ��������� ������ ��������� ����� �������.--
	//��� �� ����������� ������ �����-UserQIndex;
	//��������� ����������� ��������.
	bool GetUserCert(QString Qx, QString Qy, QString F, QString I, QString O);

	//���������� ������ ����������.
	static bool getSignerIndex(QString Qx, QString Qy, string &Qindex);

	
	//�������� ���������� ������������ �� ��������� ������ ��������� ����� �������, � ��������� ��������.
	//������������ ��� ���������� ����������� ������������ �� ��.
	bool GetUserCert(QString Qx, QString Qy, QString F, QString I, QString O, QByteArray &uCert_);

	bool GetUserCert(PkContainer &KeyContainer,QByteArray &uCert);

	//���������� ���������� ��������� ��������  GetUserCert. ���� ��� ���������� �������.
	QByteArray getCert();

	//�������� ���������� CA. � ������ ������ ������� ���������. ���������� false ���� ������.
	bool getCACert(QByteArray &CertCA);

	//�������� ���������� CA. � ������ ������ ������� ���������. ���������� false ���� ������.
	bool getRootCert(QByteArray &CertRoot);

	//������� ������ �������� ������ �������������. ������������ ��� ������ ������������ �������� ���������� ���������.
	//� ������ ������ ������ ��������� � ���������� false.
	bool getOpenKeyList(QStandardItemModel *model);
	
	//�������� �������� ���� ����� �-� ��� ������� ������������. � ������ ������ ������ ��������� � ���������� false.
	bool getDHOpenKey(string &rHashOkDH,string &Dh_OpenKey,int UserId);	

	//���������� ������ ��������� �����.
	string getQIndex();

	//��������� ������ ������������, �������� ������ ��������� �����(����� �������� �������).
	bool  getSignOpenKey(string & Qindex, string & Qx, string & Qy, QString & FIO);

	//�� ��������� ��������� ����������� � ��� ������ ���� ��� ���������� � �� �������.
	bool findUserCert(string digest,string num,QByteArray &cert);


	//���������� ���� ��������� �������� ��.
	bool getDbDateEnd(QString &dtEnd); 

	//��������� ���� �������� �� ��������� ������� ����.
	bool checkDbDateEnd();

	//������� ��������� �� ������.
	string getLastError();
	
	//�������� ��.
	void CloseDb();

private:
	QString PathToDb;//���� � ���� ������.
	QSqlDatabase Db; //���� ������
	QByteArray UserCert; //���������� ������������.
	int UserId; //�� ������������.
	string UserQIndex; //������ ��������� ����� �������� ������������.

	string lastError; //��������� ��������� �� ������.

	//������ ������������ ������ �������� �� �������� ����� �������� �������.
	QList<modelOpenSignKey> ListSignOpenKey;
	//�������� ������ ��������� ����� � ListSignOpenKey  �� ���������� �������. ���� ������� �� ������� ���������� false.
	bool  getCachSignOK(string &index,string & Qx, string & Qy, QString & FIO);
	

	//��������� ����������� ����� ����������� ����������� �� �� �� ��������� crc � ��� �����. � ������ ������ ������� ���������
	//���������� false.
	bool CheckCertCrc(QByteArray &Cert,int Len,QString &crc);

	//��������� ��������� �������� ������� �������� �����������. ������ ���������. � ������ ������� ���������� false.
	/*
		QString DtBegin; //���� ������ �������� �����������
	QString DtEnd; //���� ��������� �������� �����������
	*/
	bool CheckCertUsePeriod(QString DtBegin, QString DtEnd);

	//�������!!!
	QString GetSignOkFromDb(string & index, string & Qx, string & Qy, QString & FIO);

	//������� ��������� �� ������.
	void ShowError(string message);
};


