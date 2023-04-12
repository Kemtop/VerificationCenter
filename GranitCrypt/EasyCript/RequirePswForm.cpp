#include "RequirePswForm.h"

RequirePswForm::RequirePswForm(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	StrCodec = QTextCodec::codecForName("Windows-1251"); //��������� ���������
	QIcon icon("Image/app.png");
	setWindowIcon(icon);
	setWindowTitle(StrCodec->toUnicode("���� ������"));

	connect(ui.pushButtonOk, SIGNAL(pressed()), this, SLOT(slotpushButtonOk()));
	connect(ui.pushButtonCancel, SIGNAL(pressed()), this, SLOT(close()));	
}

RequirePswForm::~RequirePswForm()
{
}

QString RequirePswForm::GetPsw()
{
	return pswd;
}


void RequirePswForm::slotpushButtonOk()
{
	QString psw = ui.lineEditPswd->text();
	
	if (psw.isEmpty())
	{
		QMessageBox::warning(this, StrCodec->toUnicode("��������"), StrCodec->toUnicode("������� ������."));
		return;
	}
	//������ �� ����� ���� ����� 8 ��������
	if (psw.length()<8)
	{
		QMessageBox::warning(this, StrCodec->toUnicode("��������"), StrCodec->toUnicode("������ �� ����� ���� ����� 8-�� ��������."));
		return;
	}

	pswd = psw;
	accept();
}