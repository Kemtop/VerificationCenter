#pragma once

#include <QWidget>
#include "ui_SelectKeyForm.h"
#include <QFileDialog>
#include <QTextCodec>
#include <QMessageBox>

class SelectKeyForm : public QDialog
{
	Q_OBJECT

public:
	SelectKeyForm(QWidget *parent = Q_NULLPTR);
	~SelectKeyForm();
	QString KeyPath;

private:
	Ui::SelectKeyForm ui;
	QTextCodec *StrCodec;

	 //Проверяет существование файла по указанному пути.
	bool HasFile(QString path);

	private slots:
	void slotpushButtonOk();//Нажатие на кнопку ОК
	void slotpushButtonSelectKeyPath(); //Нажатие на кнопку выбора пути

};
