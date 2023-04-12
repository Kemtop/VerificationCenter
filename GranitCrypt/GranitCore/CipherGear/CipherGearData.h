#pragma once

#include <QObject>
#include "PkContainer.h"

class CipherWorkerData : public QObject
{
	Q_OBJECT

public:
	CipherWorkerData(QObject *parent);
	CipherWorkerData();
	~CipherWorkerData();

	//����������� �����������.
	CipherWorkerData(const CipherWorkerData&val);
	
	void setProcessFiles(QStringList val); //������������� ������ ������ ����� ������ ���������� ���������.
	QStringList getProcessFiles() const;

	void setPassword(QString val); //������ � ��������� �����.
	QString  getPassword() const; //������ � ��������� �����.

	void setRAsimOpenKey(std::string val); //������������� ���� ����������.
	string getRAsimOpenKey() const;

	void setKeyContainer(PkContainer val); //��������� �����.
	PkContainer getKeyContainer() const;

	void setSignerIndex(string val); //������ ����������.
	string getSignerIndex() const;

	void setSignatureDH(string val);
	string getSignatureDH() const;

	void setProcessMode(int Val); //����� ������.
	int getProcessMode() const;

	void setDbPath(QString val); //������������� ���� � �� �������.
	QString getDbPath()const;

	void setRoseMode(bool val); //�������� ���������� ����� ������.
	bool getRoseMode()const;

	//�������� ������������.
	CipherWorkerData& operator=(const CipherWorkerData &val);

private:
	QStringList filesToProcess; //������ ������ ����� ������ ���������� ���������.	
	QString password; //������ ��� �������� �����.
	std::string receiverKey; //�������� ���� ����������.
	PkContainer keyContainer; //����.
	string signerIndex; //������ ����������.
	string signatureDH; //��������� ��������� ����� ����������.
	int processmode; //����� ������-���������� 1, ����������� 2.
	QString DbPath; //���� � ��.
	bool enableRose; //������� ���������� ����� ������.
};
