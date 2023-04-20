#include "SelectKeyForm.h"

SelectKeyForm::SelectKeyForm(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.pushButtonOk,SIGNAL(pressed()),this,SLOT(slotpushButtonOk()) );
	connect(ui.pushButtonSelectKeyPath, SIGNAL(pressed()), this, SLOT(slotpushButtonSelectKeyPath()));
	connect(ui.pushButtonCancel,SIGNAL(pressed()),this,SLOT(close()));
	StrCodec= QTextCodec::codecForName("Windows-1251"); //��������� ���������
	
	QIcon icon("Image/app.png");
	this->setWindowIcon(icon);

}

SelectKeyForm::~SelectKeyForm()
{
}


//��������� ������������� ����� �� ���������� ����.
bool SelectKeyForm::HasFile(QString path)
{
		QFileInfo zfile(path);
		//�������� ���������� �� ����. ���� ���������� ��������-��� ���� � �� ����������.
		if (zfile.exists() && zfile.isFile())
			return true;
		else
			return false;
}

void SelectKeyForm::slotpushButtonSelectKeyPath()
{
	QString fileName = QFileDialog::getOpenFileName(this,StrCodec->toUnicode("����� ����� ���������� �����"),
		"", StrCodec->toUnicode("����� ������ (*.grk)"));
	ui.lineEditKeyPath->setText(fileName);

}



//������� �� ������ ��
void SelectKeyForm::slotpushButtonOk()
{
	//������� �������� ������������� �����
	//������� �������� ������������ ����� �����
	QString path = ui.lineEditKeyPath->text();
	if (path.isEmpty())
	{
		QMessageBox::warning(this, StrCodec->toUnicode("��������"), StrCodec->toUnicode("�� ������ ���� � ����� ���������� �����"));
		return;
	}

	//�������� ������������� ������.
	if (!HasFile(path))
	{
		QMessageBox::warning(this, StrCodec->toUnicode("��������"),
			StrCodec->toUnicode("�� ��������� ���� ���������� ����� �� ���������� ����."));
		return;
	}

	KeyPath = path;
	accept();
}