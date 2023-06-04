#pragma once

#include <QWidget>
#include "ui_LoginForm.h"
#include "UsbKey.h"
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
	UsbKey usb; //������ ��� ������ � USB ������.

	QTimer timerUsbKeyState; //������ ��� ������ ��������� USB �����.

	QString hostName; //����.
	int srvPort; //����
	QString loginName; //��� ������������.
	bool IsEmptyDb;//���� �������������� ��������� ���� ������.
	QPoint posCancelBut; //������� ������ ������.

	//���������� ����������� ����� � ����� ��������� �� ���������� ����������� �����.
	void lockUI(bool lock);
	
	//������������� ���������� ���������. �� �������!�������� ������ ������������ ���������� �� ������.
	void setCatchErr1();
	//������������� ���������� ���������. �� �������!�������� ������ ������������ ���������� �� ������.
	void setWarningMessage(QString m);




	int errAuthCnt; //���������� �� ������ ������� �����.

          private slots:
		  void slotPushOk(); //������� �� ������ ��.
		  void slotTimerUsbKeyStateTimeout();//������������ ������� ������ Usb �����.

};
