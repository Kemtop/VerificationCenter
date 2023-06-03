#include "AboutProgramForm.h"


AboutProgramForm::AboutProgramForm(QWidget *parent)
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
	connect(ui.pushButtonClose, SIGNAL(pressed()), SLOT(close()));
	programIsLicensed=false; //Считаю что программа не лицензированна.
	connect(ui.pushButtonProductKey, SIGNAL(pressed()), this, SLOT(slotClickToLabelSerial()));

}

AboutProgramForm::~AboutProgramForm()
{
}

void AboutProgramForm::setLicensedFlag(bool value)
{
	programIsLicensed=value;
}

void AboutProgramForm::showEvent(QShowEvent * ev)
{
	QDialog::showEvent(ev);
	setInfo();
}


//Слот по клику на кнопку ключ продукта.
void AboutProgramForm::slotClickToLabelSerial()
{
	//Отображаю форму ввода ключа продукта.
	ProductKeyForm form;
	if (form.exec() == QDialog::Accepted)
	{
		int status = form.getStatus();
		//Был введен правильный серийный номер.
		if (status == 1)
		{
			accept(); //Скрывает модальный диалог и устанавливает код возврата в Accepted.
		}
	}
}


void AboutProgramForm::setInfo()
{
	//ui.textEdit->setFontS setFontSize(16);

	std::string Inf[10] =
	{
		"Система защиты информации \"Роза\" MIT License v 158",
		"Автор: Боб, специально для Алисы.",
		"\n Лицензия: Отсутствует.",
		"Использование программы без лицензионного ключа невозможно.",
		"",
		"",
		"2017-2019"
	
	};


	//Программа активирована.
	if (programIsLicensed)
	{
		Inf[2] = "\n Лицензия: Программа активирована.";
		Inf[3] = "";
		ui.pushButtonProductKey->setVisible(false);
	}


	/*
	//Получаю текущий год. Что бы были актуальные данные.
	QDate d = QDate::currentDate();
	QString year = d.toString("yyyy");
	int y = year.toInt();
	if (y < 2019)
		Inf[3] = "2017-2019";
	else
		Inf[3] = "2017-" + year.toStdString();
		*/

	for (int i = 0;i < 7;i++)
	{
		ui.textEdit->append(StrCoder->toUnicode(Inf[i].c_str()));
	}

}
