#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QFile>
#include <stdio.h>
#include "ui_KeyProger.h"
#include <QTextCodec>
#include <QFileInfo>
#include "ApiKey/ApiKey.h"
#include "configKey.h"

class KeyProger : public QMainWindow
{
    Q_OBJECT

public:
    KeyProger(QWidget *parent = Q_NULLPTR);

private:
    Ui::KeyProgerClass ui;
	ApiKey key;
	QTimer *timer;
	QFile file;

	QTextCodec *StrCodec; //Для преобразования кодировки.
	int _ledStarus; //Состояние светодиода на плате, которым будем моргать.
	void LedBlinking(); //Мигание светодиодом.

private slots:
	void GetSerial();
	void SetSerial();
	void GetHWSerial();
	void GetLastDate();
	void SetLastDate();
	void TimerTick();
	void SaveKey();
};