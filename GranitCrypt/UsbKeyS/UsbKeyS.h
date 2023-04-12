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

	QTextCodec *StrCodec; //Для преобразования кодировки.

	bool ShowYNdialog(QString Message);
	bool existCanalKey(); //Проверяет наличие ключа шифрования канального уровня.

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
1. Прошил плату с ключом.
2. Сгенерировать ключ RSA для шифрования сеансового ключа, генерируемого платой(bat файл в папке).
В бат файле указано:
Генерация RSA ключа длиной 2048бит, и помещение его в .pem контейнер.
Конвертируем ключ из формата .pem в pkc8.
Удаляем ключ в формате pem.
Бат файл запускается через GitBash, потому как он знает где лежит openss.
cd /D/LocalRepository/VerificationCenter/STM32UsbKey/UsbKeyS/UsbKeyS
./genkey.bat

3. Загружаем открытый ключ (вектор n из сгенерированного закрытого ключа)
получателя (host, программа которая работает с аппаратным ключом). Для этого нажимаем  LoadNewRSAKey.

*/