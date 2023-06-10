#include "KeyProger.h"

KeyProger::KeyProger(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	timer = new QTimer();
	timer->setInterval(1000);

	//Нажатие на кнопку GetSerial.
	connect(ui.BtnGetSerial, SIGNAL(clicked()), this, SLOT(GetSerial()));
	//Нажатие на кнопку SetSerial.
	connect(ui.BtnSetSerial, SIGNAL(clicked()), this, SLOT(SetSerial()));
	//connect(ui->BtnGetHWSerial, SIGNAL(clicked()), this, SLOT(GetHWSerial()));
	//Нажатие на кнопку Get Last Date.
	connect(ui.BtnGetDate, SIGNAL(clicked()), this, SLOT(GetLastDate()));
	//Нажатие на кнопку Set Last Date.
	connect(ui.BtnSetDate, SIGNAL(clicked()), this, SLOT(SetLastDate()));
	//Нажатие на кнопку Load New RSA Key.
	connect(ui.BtnKeyGen, SIGNAL(clicked()), this, SLOT(SaveKey()));
	//Нажатие на кнопку  Get HW Serial.
	connect(ui.BtnGetHWSerial, SIGNAL(clicked()), this, SLOT(GetHWSerial()));
	connect(timer, SIGNAL(timeout()), this, SLOT(TimerTick()));

	StrCodec = QTextCodec::codecForName("Windows-1251"); //Установка кодировки

	timer->start();
	key.InitUsb();
	_ledStarus = 0;
}

//Получаем серийный номер из устройства.
void KeyProger::GetSerial()
{
	QString serial = QString::fromStdString(key.ProductSerial(RSA_KEY_FILENAME));
	ui.TbSerial->setText(serial);
}

//Установка серийного номера.
void KeyProger::SetSerial()
{	
	QString serial = ui.TbSerial->text();
	if (!key.SaveSerial(serial, "00000"))
	{
		QMessageBox::critical(0, "Ошибка", key.LastError);
		return;
	}

	QMessageBox::information(0, "Информация.", "Операция выполнена успешно.");
}

void KeyProger::GetHWSerial()
{
	ui.TbHWSerial->setText(QString::fromStdString(key.HWSerial()));
}

void KeyProger::GetLastDate()
{
	ui.dateEdit->setDate(key.LastDate());
}

void KeyProger::SetLastDate()
{
	key.SetLastDate(ui.dateEdit->date());
}

void KeyProger::TimerTick()
{
	//Определяю подключено ли устройство.
	if (key.DeviceIsConnected())
	{
		LedBlinking();
		uint8_t btnStatus = key.InputStatus();
		ui.statusBar->showMessage("Подключено. BtnValue=" + QString::number(btnStatus));
		//ui.statusBar->showMessage("Устройство подключено");
		ui.statusBar->setStyleSheet("color: green");
		ui.TbHWSerial->setText(QString::fromStdString(key.HWSerial()));
	}
	else
	{
		ui.statusBar->showMessage("Устройство отключено");
		ui.statusBar->setStyleSheet("color: red");
	}
}

void KeyProger::SaveKey()
{
	if (!key.SaveRsaModule(RSA_KEY_FILENAME, VECTOR_PATH, CRYPT_ARRAY_KEY_NAME))
	{
		QMessageBox::critical(0, "Ошибка", key.LastError);
		return;
	}

	QMessageBox::information(0, "Информация.", "Операция выполнена успешно.");
}

//Мигание светодиодом.
void KeyProger::LedBlinking()
{
	if (!_ledStarus)
	{
		key.SetOutputs(0x01);
		_ledStarus = 1;
	}
	else
	{
		key.SetOutputs(0);
		_ledStarus = 0;
	}
}
