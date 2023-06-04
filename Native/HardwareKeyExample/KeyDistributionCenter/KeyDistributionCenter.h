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
	void setCurentUser(QString login);//Устанавливает Логин(имя) текущего пользователя успешно прошедшего авторизацию.
	void setDbSrvHostName(QString host);//Устанавливает имя хоста сервера БД.
	void setDbSrvPort(int port);

private:
    Ui::KeyDistributionCenterClass ui;

	QString CurentUser; //Логин(имя) текущего пользователя успешно прошедшего авторизацию.
	QString DbSrvHostName; //Имя хоста сервера БД.
	int dbSrvPort; //Порт сервера БД
	QString getDataInfo(); //Получает информационные данные которые отображаются внизу окна.

	QTextCodec *StrCodec; //Для преобразования кодировки

protected:
	void showEvent(QShowEvent *ev);

	private slots:
	
	void slotActionExit(); //Меню файл-выйти из программы

	public slots:
	
signals:
	void sigShowEvent();
	void pushOk(); //bool =1 вставка =0 обновление
};
