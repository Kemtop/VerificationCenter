#pragma once

#include <QWidget>
#include "ui_LoginForm.h"
#include "ApiKey/ApiKey.h"
#include "global.h"
#include "uVectors.h"

class LoginForm : public QDialog
{
	Q_OBJECT

public:
	LoginForm(QWidget *parent = Q_NULLPTR);
	~LoginForm();
	
	QString getHostName(); //�������� ��� ���������� �����.
	int getPort(); //�������� ���� �������.
	QString getLoginName(); //�������� ����� ������������.

	void setHostName(QString val);
	void setPort(int val);
	void setLoginName(QString val);
	bool getIsEmptyDb();

	void checkUsbKey();

private:
	Ui::LoginForm ui;
	QTextCodec *StrCoder;
	ApiKey usbKey; //������ ��� ������ � USB ������.

	QTimer timerUsbKeyState; //������ ��� ������ ��������� USB �����.

	QString hostName; //����.
	int srvPort; //����
	QString loginName; //��� ������������.
	bool IsEmptyDb;//���� �������������� ��������� ���� ������.
	QPoint posCancelBut; //������� ������ ������.

	//���������� ����������� ����� � ����� ��������� �� ���������� ����������� �����.
	void lockUI(bool lock);

	//������������� ���������� ���������.
	void setWarningMessage(QString m);

	int errAuthCnt; //���������� �� ������ ������� �����.

          private slots:
		  void slotPushOk(); //������� �� ������ ��.
		  void slotTimerUsbKeyStateTimeout();//������������ ������� ������ Usb �����.

};
