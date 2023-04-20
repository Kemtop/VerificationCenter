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

	//��������� �������� ����� �� ��� ��������. 
	void setSerialtoControl(QString text);
	//�������� �������� ����� statusValue.
	int  getStatus();


private:
	Ui::ProductKeyForm ui;
	QTextCodec *StrCoder;

	//��������� ������� ������� ���� � ������, ������� ���������.
	bool CheckNoAsciiCharInString(QString str);

	int badAttempts; //���� ���������� ������� ������ �� ������ ���� ��������.
		//���� ���������� ������ �����. =0-�� ���� �� ���������-������������ ������ ������ �����. 
	   //=1 ��� ������ ������ �������� �����.
	   //=2 ��� ��� ���� ������ �� ������ �������� �����.
	int statusValue; 


	private slots:
	void slotTextChangeEdit1(const QString &text); //����� �� ��������� ������ � ���������.
	void slotTextChangeEdit2(const QString &text);
	void slotTextChangeEdit3(const QString &text);
	void slotTextChangeEdit4(const QString &text);
	void slotTextChangeEdit5(const QString &text);

	void slotPushButtonOk(); //���� �� ������� �� ������ "��".
	void slotPushButtonCancel();//���� �� ������� �� ������ "������".

};
