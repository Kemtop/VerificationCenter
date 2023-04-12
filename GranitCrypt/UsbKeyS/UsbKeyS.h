#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QFile>
#include <stdio.h>
#include "UsbKey.h"
#include "ui_UsbKeyS.h"
#include <QTextCodec>
#include <QFileInfo>

class UsbKeyS : public QMainWindow
{
    Q_OBJECT

public:
    UsbKeyS(QWidget *parent = Q_NULLPTR);

private:
    Ui::UsbKeySClass ui;
	UsbKey usb;
	QTimer *timer;
	QFile file;

	QTextCodec *StrCodec; //��� �������������� ���������.

	bool ShowYNdialog(QString Message);
	bool existCanalKey(); //��������� ������� ����� ���������� ���������� ������.

private slots:
	void GetSerial();
	void SetSerial();
	void GetHWSerial();
	void GetLastDate();
	void SetLastDate();
	void TimerTick();
	void GenKey();
};









/*
1. ������ ����� � ������.
2. ������������� ���� RSA ��� ���������� ���������� �����, ������������� ������(bat ���� � �����).
� ��� ����� �������:
��������� RSA ����� ������ 2048���, � ��������� ��� � .pem ���������.
������������ ���� �� ������� .pem � pkc8.
������� ���� � ������� pem.
��� ���� ����������� ����� GitBash, ������ ��� �� ����� ��� ����� openss.
cd /D/LocalRepository/VerificationCenter/STM32UsbKey/UsbKeyS/UsbKeyS
./genkey.bat

3. ��������� �������� ���� (������ n �� ���������������� ��������� �����)
���������� (host, ��������� ������� �������� � ���������� ������). ��� ����� ��������  LoadNewRSAKey.

*/