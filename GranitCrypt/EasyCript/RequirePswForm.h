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
	 QString GetPsw(); //���������� ��������� ������.
private:
	Ui::RequirePswForm ui;
	QTextCodec *StrCodec;
	QString pswd; //��������� ������.
protected:
	protected slots:
		   void slotpushButtonOk();

};
