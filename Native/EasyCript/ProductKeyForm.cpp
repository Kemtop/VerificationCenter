#include "ProductKeyForm.h"


ProductKeyForm::ProductKeyForm(QWidget *parent)
	: QDialog(parent)
{
	//Скрываю кнопку подсказки
	Qt::WindowFlags flags = windowFlags();
	Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
	flags = flags & (~helpFlag);
	setWindowFlags(flags);
	ui.setupUi(this);
	QIcon icon("Image/app.png");
	setWindowIcon(icon);
	StrCoder = QTextCodec::codecForName("Windows-1251"); //Установка кодировки



	ui.lineEdit_1->clear();
	ui.lineEdit_2->clear();
	ui.lineEdit_3->clear();
	ui.lineEdit_4->clear();
	ui.lineEdit_5->clear();

	connect(ui.lineEdit_1, SIGNAL(textChanged(const QString &)), this, SLOT(slotTextChangeEdit1(const QString &)));
	connect(ui.lineEdit_2, SIGNAL(textChanged(const QString &)), this, SLOT(slotTextChangeEdit2(const QString &)));
	connect(ui.lineEdit_3, SIGNAL(textChanged(const QString &)), this, SLOT(slotTextChangeEdit3(const QString &)));
	connect(ui.lineEdit_4, SIGNAL(textChanged(const QString &)), this, SLOT(slotTextChangeEdit4(const QString &)));
	connect(ui.lineEdit_5, SIGNAL(textChanged(const QString &)), this, SLOT(slotTextChangeEdit5(const QString &)));

	badAttempts = 0;// Не было не верных попыток ввести ключ.
	statusValue = 0; //Обнуляю флаг.

	connect(ui.pushButtonOk, SIGNAL(pressed()), this, SLOT(slotPushButtonOk()));
	connect(ui.pushButtonCancel, SIGNAL(pressed()), this, SLOT(slotPushButtonCancel())); //Слот по нажатие на кнопку "Отмена".

}

ProductKeyForm::~ProductKeyForm()
{
}

void ProductKeyForm::setSerialtoControl(QString text)
{
	bool BadString = false; //Флаг проблем с серийным номером.

	QStringList list = text.split('-');

	if (list.size() < 5) return; //Ключ продукта не может содержать менее 4 тире.
		//Проверяю каждый ли блок содержит 5 символов.
		for (int i = 0;i < 5;i++)
		{
			if (list.at(i).length() != 5) return;
		}


    ui.lineEdit_1->setText(list.at(0));
	ui.lineEdit_2->setText(list.at(1));
	ui.lineEdit_3->setText(list.at(2));
	ui.lineEdit_4->setText(list.at(3));
	ui.lineEdit_5->setText(list.at(4));

}

int ProductKeyForm::getStatus()
{
	return statusValue;
}


void ProductKeyForm::slotTextChangeEdit1(const QString &text)
{
	QString str = text;
	//Пользователь скопировал серийный номер из файла и пытается его вставить в первый контрол.
	if (str.length() == 29)
	{
		setSerialtoControl(str);
		return;
	}


	if(str.length()>5) str = str.mid(0,5); //Обрезаю 5 символов

	ui.lineEdit_1->setText(str.toUpper());
}


bool ProductKeyForm::CheckNoAsciiCharInString(QString str)
{
	ushort utf16Code = 0;

	bool HasBadChar = false;
	int posBad = 0;
	for (int i = 0;i < str.length();i++)
	{
		utf16Code = str[i].unicode(); //Получаю код символа.
		if (utf16Code > 0x7f)
		{
			HasBadChar = true;
			posBad = i;
			break;
		}
	}
	

	if (HasBadChar)
	{		
		QString badChar;
		badChar = "["+str[posBad]+"]";

		std::string str1 = "Ключ продукта может содержать только латинские буквы.\n"
			"Неверный символ ";

		QString mess = StrCoder->toUnicode(str1.c_str());
		str1 = " в блоке";
		QString str2= StrCoder->toUnicode(str1.c_str());

		mess = mess + badChar + "  " + str2 + "  [" + str + "].\n ";
		str1=" Возможно вы ввели символ на русском языке.";
		mess=mess+ StrCoder->toUnicode(str1.c_str());

		ui.labelShowError->setText(mess);

		return false;
	}

	return true;
}

void ProductKeyForm::slotTextChangeEdit2(const QString &text)
{
	ui.lineEdit_2->setText(text.toUpper());
}


void ProductKeyForm::slotTextChangeEdit3(const QString &text)
{
	ui.lineEdit_3->setText(text.toUpper());
}



void ProductKeyForm::slotTextChangeEdit4(const QString &text)
{
	ui.lineEdit_4->setText(text.toUpper());
}



void ProductKeyForm::slotTextChangeEdit5(const QString &text)
{
	ui.lineEdit_5->setText(text.toUpper());
}


//Слот по нажатие на кнопку "Ок".
void ProductKeyForm::slotPushButtonOk()
{
	
}

//Слот по нажатие на кнопку "Отмена".
void ProductKeyForm::slotPushButtonCancel()
{
	//Введен правильный ключ продукта.
	if (statusValue == 1)
	{
		accept();
		return;
	}

	close();
}