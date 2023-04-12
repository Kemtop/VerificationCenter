#include "ProductKeyForm.h"


ProductKeyForm::ProductKeyForm(QWidget *parent)
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

	badAttempts = 0;// �� ���� �� ������ ������� ������ ����.
	statusValue = 0; //������� ����.

	connect(ui.pushButtonOk, SIGNAL(pressed()), this, SLOT(slotPushButtonOk()));
	connect(ui.pushButtonCancel, SIGNAL(pressed()), this, SLOT(slotPushButtonCancel())); //���� �� ������� �� ������ "������".

}

ProductKeyForm::~ProductKeyForm()
{
}

void ProductKeyForm::setSerialtoControl(QString text)
{
	bool BadString = false; //���� ������� � �������� �������.

	QStringList list = text.split('-');

	if (list.size() < 5) return; //���� �������� �� ����� ��������� ����� 4 ����.
		//�������� ������ �� ���� �������� 5 ��������.
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
	//������������ ���������� �������� ����� �� ����� � �������� ��� �������� � ������ �������.
	if (str.length() == 29)
	{
		setSerialtoControl(str);
		return;
	}


	if(str.length()>5) str = str.mid(0,5); //������� 5 ��������

	ui.lineEdit_1->setText(str.toUpper());
}


bool ProductKeyForm::CheckNoAsciiCharInString(QString str)
{
	ushort utf16Code = 0;

	bool HasBadChar = false;
	int posBad = 0;
	for (int i = 0;i < str.length();i++)
	{
		utf16Code = str[i].unicode(); //������� ��� �������.
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

		std::string str1 = "���� �������� ����� ��������� ������ ��������� �����.\n"
			"�������� ������ ";

		QString mess = StrCoder->toUnicode(str1.c_str());
		str1 = " � �����";
		QString str2= StrCoder->toUnicode(str1.c_str());

		mess = mess + badChar + "  " + str2 + "  [" + str + "].\n ";
		str1=" �������� �� ����� ������ �� ������� �����.";
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


//���� �� ������� �� ������ "��".
void ProductKeyForm::slotPushButtonOk()
{
	//�������� �������� �� ������� �� ASCII �������� � ����������� �����. �������� ������� ����.
	if (!CheckNoAsciiCharInString(ui.lineEdit_1->text())) return;
	if (!CheckNoAsciiCharInString(ui.lineEdit_2->text())) return;
	if (!CheckNoAsciiCharInString(ui.lineEdit_3->text())) return;
	if (!CheckNoAsciiCharInString(ui.lineEdit_4->text())) return;
	if (!CheckNoAsciiCharInString(ui.lineEdit_5->text())) return;


	//�������� ������ ������.
	QString ProduktKey =
		ui.lineEdit_1->text() + "-" +
		ui.lineEdit_2->text() + "-" +
		ui.lineEdit_3->text() + "-" +
		ui.lineEdit_4->text() + "-" +
		ui.lineEdit_5->text();
		
	//�������� ����� ��������.
	CheckSerial CPk;
	std::string pkDateEnd = "01.01.1905"; //���� ��������� �������� ����� ��������.
	int xReti = 12;
	xReti=CPk.CheckProductKey(ProduktKey,pkDateEnd);
	
	std::string str1 = "";
	QString str2 = "";
	//�� ������ �������� �����.
	if (xReti == 0)
	{
		//��� ���� ����� �� ������ ���� ��������.
		if (badAttempts >= 3)
		{
			str1 = "�� 3 ���� ����� �� ������ ���� ��������. ��������� ����� �������������.\n ���������� ����� 2 ����.";
			str2 = StrCoder->toUnicode(str1.c_str());
			ui.labelShowError->setText(str2);

			//�������� ����������.
			ui.pushButtonOk->setVisible(false);

			str1 = "�������";
			str2 = StrCoder->toUnicode(str1.c_str());

			ui.pushButtonCancel->setText(str2);


			return;
		}

        str1 = "�� ������ ���� ��������.";
        str2=StrCoder->toUnicode(str1.c_str());
		ui.labelShowError->setText(str2);

		if (badAttempts < 3) badAttempts++; //��������� ����� ���������� ������� ������ �� ������ ����
		return;
	}
	
	
	    str1 = "��������� ������������. ���� ��������� �������� ����� ��������: "+ pkDateEnd+"\n"
		" ������� \"�������\" � �������� ��������� ���������.\n";

		 str2 = StrCoder->toUnicode(str1.c_str());

		 //����� ����.
		 QString css =
			 "font-family:Times New Roman;"
			 "font-style: normal;"
			 "font-weight: bold;"
			 "font-size:16px;"
			 "color:#008000;";
	 
		ui.labelShowError->setStyleSheet(css);
		ui.labelShowError->setText(str2);
		ui.pushButtonOk->setVisible(false);

		str1 = "�������";
		str2 = StrCoder->toUnicode(str1.c_str());

		ui.pushButtonCancel->setText(str2);
		statusValue = 1; //������� ����-��� ������ ������ �������� �����.

		//C�������� �������� ����� � ���������.
		SerialNumTools St;
		string pk_ = ProduktKey.toStdString();
		St.PutProdukKeyToConteiner(pk_);

	

}

//���� �� ������� �� ������ "������".
void ProductKeyForm::slotPushButtonCancel()
{
	//������ ���������� ���� ��������.
	if (statusValue == 1)
	{
		accept();
		return;
	}

	close();
}