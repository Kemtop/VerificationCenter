#include "AboutProgramForm.h"


AboutProgramForm::AboutProgramForm(QWidget *parent)
	: QDialog(parent)
{
	//������� ������ ���������
	Qt::WindowFlags flags = windowFlags();
	Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
	flags = flags & (~helpFlag);
	setWindowFlags(flags);
	ui.setupUi(this);
	QIcon icon("Image/app.png");
	setWindowIcon(icon);
	StrCoder = QTextCodec::codecForName("Windows-1251"); //��������� ���������
	connect(ui.pushButtonClose, SIGNAL(pressed()), SLOT(close()));
	programIsLicensed=false; //������ ��� ��������� �� ��������������.
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


//���� �� ����� �� ������ ���� ��������.
void AboutProgramForm::slotClickToLabelSerial()
{
	//��������� ����� ����� ����� ��������.
	ProductKeyForm form;
	if (form.exec() == QDialog::Accepted)
	{
		int status = form.getStatus();
		//��� ������ ���������� �������� �����.
		if (status == 1)
		{
			accept(); //�������� ��������� ������ � ������������� ��� �������� � Accepted.
		}
	}
}


void AboutProgramForm::setInfo()
{
	//ui.textEdit->setFontS setFontSize(16);

	std::string Inf[10] =
	{
		"������� ������ ���������� \"����\" MIT License v 158",
		"�����: ���, ���������� ��� �����.",
		"\n ��������: �����������.",
		"������������� ��������� ��� ������������� ����� ����������.",
		"",
		"",
		"2017-2019"
	
	};


	//��������� ������������.
	if (programIsLicensed)
	{
		Inf[2] = "\n ��������: ��������� ������������.";
		Inf[3] = "";
		ui.pushButtonProductKey->setVisible(false);
	}


	/*
	//������� ������� ���. ��� �� ���� ���������� ������.
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
