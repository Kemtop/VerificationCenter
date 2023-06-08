#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ProtectionApp.h"
#include <QDialog>
#include <QDomDocument>
#include <exception>
#include "global.h"
#include "qbrush.h"

class ProtectionApp : public QMainWindow
{
    Q_OBJECT

public:
    ProtectionApp(QWidget *parent = Q_NULLPTR);
	~ProtectionApp();
	void setCurentUser(QString login);//������������� �����(���) �������� ������������ ������� ���������� �����������.
	void setDbSrvHostName(QString host);//������������� ��� ����� ������� ��.
	void setDbSrvPort(int port);

private:
    Ui::ProtectionAppClass ui;

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
