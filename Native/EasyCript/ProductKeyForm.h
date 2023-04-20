#pragma once

#include <QWidget>
#include "ui_ProductKeyForm.h"
#include <QDialog>
#include <QTextCodec>
#include <QMessageBox>

class ProductKeyForm : public QDialog
{
	Q_OBJECT

public:
	ProductKeyForm(QWidget *parent = Q_NULLPTR);
	~ProductKeyForm();

	//Вставляет серийный номер во все контролы. 
	void setSerialtoControl(QString text);
	//Получает значение флага statusValue.
	int  getStatus();


private:
	Ui::ProductKeyForm ui;
	QTextCodec *StrCoder;

	//Проверяет наличие русский букв в строке, выводит сообщение.
	bool CheckNoAsciiCharInString(QString str);

	int badAttempts; //Флаг количества попыток ввести не верный ключ продукта.
		//Флаг результата работы формы. =0-ни чего не произошло-пользователь просто закрыл форму. 
	   //=1 был введен верный серийный номер.
	   //=2 был три раза введен не верный серийный номер.
	int statusValue; 


	private slots:
	void slotTextChangeEdit1(const QString &text); //Слоты по изменению текста в контролах.
	void slotTextChangeEdit2(const QString &text);
	void slotTextChangeEdit3(const QString &text);
	void slotTextChangeEdit4(const QString &text);
	void slotTextChangeEdit5(const QString &text);

	void slotPushButtonOk(); //Слот по нажатие на кнопку "Ок".
	void slotPushButtonCancel();//Слот по нажатие на кнопку "Отмена".

};
