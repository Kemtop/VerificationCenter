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
	
	QString getHostName(); //Получает имя введенного хоста.
	int getPort(); //Получает порт сервера.
	QString getLoginName(); //Получает логин пользователя.

	void setHostName(QString val);
	void setPort(int val);
	void setLoginName(QString val);
	bool getIsEmptyDb();

	void checkUsbKey();

private:
	Ui::LoginForm ui;
	QTextCodec *StrCoder;
	ApiKey usbKey; //Объект для работы с USB ключом.

	QTimer timerUsbKeyState; //Таймер для опроса состояния USB ключа.

	QString hostName; //Хост.
	int srvPort; //Порт
	QString loginName; //Имя пользователя.
	bool IsEmptyDb;//Флаг первоначальной установки базы данных.
	QPoint posCancelBut; //Позиция кнопки отмена.

	//Блокировка компонентов ввода и вывод сообщения об отсутствии аппаратного ключа.
	void lockUI(bool lock);

	//Устанавливает компоненту сообщение.
	void setWarningMessage(QString m);

	int errAuthCnt; //Количество не верных попыток входа.

          private slots:
		  void slotPushOk(); //Нажатие на кнопку ОК.
		  void slotTimerUsbKeyStateTimeout();//Срабатывание таймера опроса Usb ключа.

};
