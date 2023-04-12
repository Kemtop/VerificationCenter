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
	//������������� ���� �������������� ������ � �������������� ���������.
	void setLicensedFlag(bool value);

protected:
	//������� �� ������ �����.
	void showEvent(QShowEvent *ev);

private:
	Ui::AboutProgramForm ui;
	void setInfo(); //��������� ���� �������.
	QTextCodec *StrCoder;
	bool programIsLicensed; //���� ���� ��� ��������� ������������.
			

	private slots:
			void slotClickToLabelSerial(); //���� �� ����� �� ������ ���� ��������.
};
