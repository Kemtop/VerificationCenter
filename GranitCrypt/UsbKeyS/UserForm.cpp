#include "UserForm.h"
#include "CheckSerial.h"

UserForm::UserForm(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	timerKey.setInterval(300);	

	isLockUi = 0;
	weAreConnect=false;//Флаг успешного подключения к устройству.
	lockUI(true); //Блокирую пока таймер запускается.
	ui.statusBar->showMessage("Инициализация...");

	connect(ui.pushButtonSendProductKey, SIGNAL(pressed()), this, SLOT(slotpushButtonSendProductKey()));
	connect(ui.pushButtonSetRSA, SIGNAL(pressed()), this, SLOT(slotpushButtonSetRSA()));
	connect(&timerKey, SIGNAL(timeout()), this, SLOT(slotTimerTick()));

	usb.InitUsb();
	timerKey.start();
}

UserForm::~UserForm()
{
}

void UserForm::lockUI(bool mode)
{
	if (mode)
	{
		if (isLockUi == 1) return; //Не блокирую, если заблокирован.
		ui.labelKeyInfo->setEnabled(false);
	    ui.pushButtonSendProductKey->setEnabled(false);
		ui.pushButtonSetRSA->setEnabled(false);
		ui.lineEditProductKey->setEnabled(false);
		isLockUi = 1;
	}
	else
	{
		if (isLockUi == 0) return; //Разблокирован, не разблокирую.
		ui.labelKeyInfo->setEnabled(true);
		ui.pushButtonSendProductKey->setEnabled(true);
		ui.pushButtonSetRSA->setEnabled(true);
		ui.lineEditProductKey->setEnabled(true);
		isLockUi = 0;
	}
}

void UserForm::slotpushButtonSendProductKey()
{
	QString WarnMess = "Вы действительно хотите установить новый ключ продукта?";
	if (!ShowYNdialog(WarnMess)) return;
	
	QString serial = ui.lineEditProductKey->text();
	serial = serial.trimmed();

	QString a = serial.mid(0, 5);
	if (a == "WWWWW")
	{
		QMessageBox::critical(0, "Ошибка", "Нельзя установить серийный номер с неверным базовым числом.");
		return;
	}
	
	QRegExp reg("[0-9,A-Z]{5}-[0-9,A-Z]{5}-[0-9,A-Z]{5}-[0-9,A-Z]{5}-[0-9,A-Z]{5}");
	int match = reg.indexIn(serial);
	if (match < 0)
	{
		QMessageBox::critical(0, "Ошибка", "Указанный серийный номер не соответствует формату.");
		return;
	}

	CheckSerial Chk;
	std::string strDtEnd = "";
	int u = Chk.CheckProductKey(serial, strDtEnd);
	if (u == 0)
	{
		QMessageBox::critical(0, "Ошибка", "Указан не верный ключ продукта.");
		return;
	}

	usb.setSerial((uint8_t *)(serial.toStdString().c_str()));
	QMessageBox::information(0, "Информация.", "Операция выполнена успешно.");
}

void UserForm::slotpushButtonSetRSA()
{
	//QString winDirPath = qEnvironmentVariable("PATH");

	//QStandardPaths::LocateFile
	QString WarnMess = "Вы действительно хотите установить новый ключ шифрования канального уровня?";
	if (!ShowYNdialog(WarnMess)) return;

	if (!existCanalKey()) //Проверяет наличие ключа шифрования канального уровня.
		return;

	usb.translateChannelKey();
	usb.SendPublicKey();
	QMessageBox::information(0, "Информация.", "Операция выполнена успешно.");
}


bool UserForm::ShowYNdialog(QString Message)
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

bool UserForm::existCanalKey()
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

void UserForm::slotTimerTick()
{
	//Определяю подключено ли устройство.
	if (usb.isConnected())
	{
		if (weAreConnect) return; //Мы уже все считали и показали.

		QString mess = "Устройство подключено. ";
		QString hwSerial = QString::fromStdString(usb.getHWSerial());
		QDate lastDt = usb.getLastHwDate();
		QString txtLastDate = lastDt.toString("dd.MM.yyyy");
		
		mess = mess + " HwSerial: " + hwSerial+". LastDate: "+txtLastDate+".";

		ui.statusBar->showMessage(mess);
		ui.statusBar->setStyleSheet("color: green");
		lockUI(false);
		weAreConnect = true;//Флаг успешного подключения к устройству.
	}
	else
	{
		ui.statusBar->showMessage("USB ключ отключен.");
		ui.statusBar->setStyleSheet("color: red");
		lockUI(true);
		weAreConnect = false;//Флаг успешного подключения к устройству.
	}
}



