#include "UsbKeyS.h"
#include "libHwKey.h"


UsbKeyS::UsbKeyS(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	timer = new QTimer();
	timer->setInterval(50);

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
	connect(ui.BtnKeyGen, SIGNAL(clicked()), this, SLOT(GenKey()));
	//Нажатие на кнопку  Get HW Serial.
	connect(ui.BtnGetHWSerial, SIGNAL(clicked()), this, SLOT(GetHWSerial()));
	connect(timer, SIGNAL(timeout()), this, SLOT(TimerTick()));

	StrCodec = QTextCodec::codecForName("Windows-1251"); //Установка кодировки

	timer->start();

	usb.InitUsb();

}

//Получаем серийный номер из устройства.
void UsbKeyS::GetSerial()
{
	if (!existCanalKey()) //Проверяет наличие ключа шифрования канального уровня.
		return;

	QString serial = QString::fromStdString(usb.getSerial());
	ui.TbSerial->setText(serial);
}

bool UsbKeyS::ShowYNdialog(QString Message)
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle("Внимание");
	msgBox.setText(Message);
	msgBox.setStandardButtons(QMessageBox::Yes);
	msgBox.addButton(QMessageBox::No);

	msgBox.setDefaultButton(QMessageBox::No);
	msgBox.setIcon(QMessageBox::Question);

	//Нажал НЕТ
	if (msgBox.exec() == QMessageBox::No) return false;

	return true;
}

bool UsbKeyS::existCanalKey()
{
	//Проверяю наличие ключа RSA в текущей папке.
	QString path = QCoreApplication::applicationDirPath() + "/prkey";

	QFileInfo zfile(path);
	//Проверяю существует ли файл. Если существует проверяю-это файл а не директория.
	if (!(zfile.exists() && zfile.isFile()))
	{
		QMessageBox::critical(0, "Ошибка", "Отсутствует ключ шифрования канального уровня.");
		return false;
	}

	return true;
}

//Установка серийного номера.
void UsbKeyS::SetSerial()
{
	QString WarnMess = "Вы действительно хотите установить новый серийный номер?";
	if (!ShowYNdialog(WarnMess)) return;
	

	QString serial = ui.TbSerial->text();

	QString a = serial.mid(0, 5);
	if (a == "00000")
	{
		QMessageBox::critical(0, "Ошибка", "Нельзя установить серийный номер с неверным базовым числом.");
		return;
	}


	QRegExp reg("[0-9,A-Z]{5}-[0-9,A-Z]{5}-[0-9,A-Z]{5}-[0-9,A-Z]{5}-[0-9,A-Z]{5}");
	int match = reg.indexIn(serial);
	if (match < 0)
	{
		QMessageBox::critical(0, "Ошибка", "Указанный серийный номер не соответствует формату");
		return;
	}

	usb.setSerial((uint8_t *)(serial.toStdString().c_str()));
}

void UsbKeyS::GetHWSerial()
{
	ui.TbHWSerial->setText(QString::fromStdString(usb.getHWSerial()));
}

void UsbKeyS::GetLastDate()
{
	uint16_t days = usb.getLastDate();
	QDate dat(2019, 01, 01);
	ui.dateEdit->setDate(dat.addDays(days));
}

void UsbKeyS::SetLastDate()
{
	QDate dat(2019, 01, 01);
	uint16_t days = (uint16_t)dat.daysTo(ui.dateEdit->date());
	usb.setLastDate(days);
}

void UsbKeyS::TimerTick()
{
	//Определяю подключено ли устройство.
	if (usb.isConnected())
	{
		ui.statusBar->showMessage("Устройство подключено");
		ui.statusBar->setStyleSheet("color: green");

		//std::vector<uint16_t> rnd = usb.getRandData();
		//QByteArray arr;
		//for (int i = 0; i < rnd.size(); ++i)
		//{
		//	uint16_t word = rnd.at(i);
		//	arr.append((uint8_t)(word >> 8));
		//	arr.append((uint8_t)(word));
		//}
		//file.write(arr);
		//file.flush();
	}
	else
	{
		ui.statusBar->showMessage("Устройство отключено");
		ui.statusBar->setStyleSheet("color: red");
	}
}

void UsbKeyS::GenKey()
{
	QString WarnMess = "Вы действительно хотите установить новый ключ асимметричного шифрования?";
	if (!ShowYNdialog(WarnMess)) return;

	if (!existCanalKey()) //Проверяет наличие ключа шифрования канального уровня.
		return;
	
	usb.translateChannelKey();

	usb.SendPublicKey();
}