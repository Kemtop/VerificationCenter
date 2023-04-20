#pragma once

#include <QWidget>
#include <qdialog.h>
#include <QTextCodec>
#include <QDate>
#include <QMessageBox>
#include "ui_AboutProgramForm.h"
#include "ProductKeyForm.h"

class AboutProgramForm : public QDialog
{
	Q_OBJECT

public:
	AboutProgramForm(QWidget *parent = Q_NULLPTR);
	~AboutProgramForm();
	//Устанавливает флаг информирования класса о лицензионности программы.
	void setLicensedFlag(bool value);

protected:
	//Событие по показу формы.
	void showEvent(QShowEvent *ev);

private:
	Ui::AboutProgramForm ui;
	void setInfo(); //Заполняет окно данными.
	QTextCodec *StrCoder;
	bool programIsLicensed; //Флаг того что программа лицензионная.
			

	private slots:
			void slotClickToLabelSerial(); //Слот по клику на кнопку ключ продукта.
};
