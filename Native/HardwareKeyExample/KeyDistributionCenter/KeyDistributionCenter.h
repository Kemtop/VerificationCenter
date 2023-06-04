#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_KeyDistributionCenter.h"
#include <QDialog>
#include <QDomDocument>
#include <exception>
#include "global.h"
#include "qbrush.h"

class KeyDistributionCenter : public QMainWindow
{
    Q_OBJECT

public:
    KeyDistributionCenter(QWidget *parent = Q_NULLPTR);
	~KeyDistributionCenter();
	void setCurentUser(QString login);//������������� �����(���) �������� ������������ ������� ���������� �����������.
	void setDbSrvHostName(QString host);//������������� ��� ����� ������� ��.
	void setDbSrvPort(int port);

private:
    Ui::KeyDistributionCenterClass ui;

	QString CurentUser; //�����(���) �������� ������������ ������� ���������� �����������.
	QString DbSrvHostName; //��� ����� ������� ��.
	int dbSrvPort; //���� ������� ��
	QString getDataInfo(); //�������� �������������� ������ ������� ������������ ����� ����.

	QTextCodec *StrCodec; //��� �������������� ���������

protected:
	void showEvent(QShowEvent *ev);

	private slots:
	
	void slotActionExit(); //���� ����-����� �� ���������

	public slots:
	
signals:
	void sigShowEvent();
	void pushOk(); //bool =1 ������� =0 ����������
};
