#pragma once

#include <QWidget>
#include <QTimer>
#include <QProcess>
#include <QDate>
#include <QMessageBox>
#include "ui_UserForm.h"
#include "ApiKey/ApiKey.h"
#include "configKey.h"

class UserForm : public QWidget
{
	Q_OBJECT

public:
	UserForm(QWidget *parent = Q_NULLPTR);
	~UserForm();

private:
	Ui::UserForm ui;
	QTimer timerKey;
	ApiKey key;
	int isLockUi; //��������� ��� ������������.
	bool weAreConnect;//���� ��������� ����������� � ����������.

	void lockUI(bool mode); //����������/������������� UI.
	bool ShowYNdialog(QString Message); //���������� ������ � �������� ���/ ����.

	private slots:
	void slotTimerTick();
	void slotpushButtonSendProductKey(); //��������� ���� ��������.
	void slotpushButtonSetRSA();
};
