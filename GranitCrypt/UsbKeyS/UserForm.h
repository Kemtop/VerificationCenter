#pragma once

#include <QWidget>
#include <QTimer>
#include <QProcess>
#include <QDate>
#include <QMessageBox>
#include "ui_UserForm.h"
#include "UsbKey.h"

class UserForm : public QWidget
{
	Q_OBJECT

public:
	UserForm(QWidget *parent = Q_NULLPTR);
	~UserForm();

private:
	Ui::UserForm ui;
	QTimer timerKey;
	UsbKey usb;
	int isLockUi; //��������� ��� ������������.
	bool weAreConnect;//���� ��������� ����������� � ����������.

	void lockUI(bool mode); //����������/������������� UI.
	bool ShowYNdialog(QString Message);

	bool existCanalKey();


	private slots:
	void slotTimerTick();
	void slotpushButtonSendProductKey(); //��������� ���� ��������.
	void slotpushButtonSetRSA();
};
