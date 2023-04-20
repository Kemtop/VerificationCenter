#include "RequirePswForm.h"

RequirePswForm::RequirePswForm(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	StrCodec = QTextCodec::codecForName("Windows-1251"); //Установка кодировки
	QIcon icon("Image/app.png");
	setWindowIcon(icon);
	setWindowTitle(StrCodec->toUnicode("Ввод пароля"));

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
		QMessageBox::warning(this, StrCodec->toUnicode("Внимание"), StrCodec->toUnicode("Введите пароль."));
		return;
	}
	//Пароль не может быть менее 8 символов
	if (psw.length()<8)
	{
		QMessageBox::warning(this, StrCodec->toUnicode("Внимание"), StrCodec->toUnicode("Пароль не может быть менее 8-ми символов."));
		return;
	}

	pswd = psw;
	accept();
}