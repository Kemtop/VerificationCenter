#include "SelectKeyForm.h"

SelectKeyForm::SelectKeyForm(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.pushButtonOk,SIGNAL(pressed()),this,SLOT(slotpushButtonOk()) );
	connect(ui.pushButtonSelectKeyPath, SIGNAL(pressed()), this, SLOT(slotpushButtonSelectKeyPath()));
	connect(ui.pushButtonCancel,SIGNAL(pressed()),this,SLOT(close()));
	StrCodec= QTextCodec::codecForName("Windows-1251"); //Установка кодировки
	
	QIcon icon("Image/app.png");
	this->setWindowIcon(icon);

}

SelectKeyForm::~SelectKeyForm()
{
}


//Проверяет существование файла по указанному пути.
bool SelectKeyForm::HasFile(QString path)
{
		QFileInfo zfile(path);
		//Проверяю существует ли файл. Если существует проверяю-это файл а не директория.
		if (zfile.exists() && zfile.isFile())
			return true;
		else
			return false;
}

void SelectKeyForm::slotpushButtonSelectKeyPath()
{
	QString fileName = QFileDialog::getOpenFileName(this,StrCodec->toUnicode("Выбор файла секретного ключа"),
		"", StrCodec->toUnicode("Файлы ключей (*.grk)"));
	ui.lineEditKeyPath->setText(fileName);

}



//Нажатие на кнопку ОК
void SelectKeyForm::slotpushButtonOk()
{
	//Сделать проверку существования файла
	//Сделать проверку правильности файла ключа
	QString path = ui.lineEditKeyPath->text();
	if (path.isEmpty())
	{
		QMessageBox::warning(this, StrCodec->toUnicode("Внимание"), StrCodec->toUnicode("Не выбран путь к файлу секретного ключа"));
		return;
	}

	//Проверяю существование файлов.
	if (!HasFile(path))
	{
		QMessageBox::warning(this, StrCodec->toUnicode("Внимание"),
			StrCodec->toUnicode("Не обнаружен файл секретного ключа по указанному пути."));
		return;
	}

	KeyPath = path;
	accept();
}