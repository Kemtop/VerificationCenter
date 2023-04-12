#pragma once

#include <QWidget>
#include "ui_RequirePswForm.h"
#include <QMessageBox>
#include <QTextCodec>
class RequirePswForm : public QDialog
{
	Q_OBJECT

public:
	RequirePswForm(QWidget *parent = Q_NULLPTR);
	~RequirePswForm();
	 QString i;
	 QString GetPsw(); //Возвращает введенный пароль.
private:
	Ui::RequirePswForm ui;
	QTextCodec *StrCodec;
	QString pswd; //Введенный пароль.
protected:
	protected slots:
		   void slotpushButtonOk();

};
