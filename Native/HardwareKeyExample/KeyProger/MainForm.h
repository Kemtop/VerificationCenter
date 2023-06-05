#pragma once

#include <QWidget>
#include <QDialog>
#include <qmessagebox.h>
#include "ui_MainForm.h"

class MainForm : public QDialog
{
	Q_OBJECT

public:
	MainForm(QWidget *parent = Q_NULLPTR);
	~MainForm();
	int getMode();

private:
	Ui::MainForm ui;

	int mode; //Режим работы - простой пользователь/"эксперт".
	int attemp; //Количество попыток ввода пароля для входа в режим "эксперт".
	private slots:
	   void slotToggledExpert(bool checked);
	   void slotpushButtonOK();
};
