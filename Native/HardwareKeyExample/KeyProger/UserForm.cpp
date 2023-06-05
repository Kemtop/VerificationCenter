#include "UserForm.h"
#include "CheckSerial.h"

UserForm::UserForm(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	timerKey.setInterval(300);	

	isLockUi = 0;
	weAreConnect = false;//Флаг успешного подключения к устройству.
	lockUI(true); //Блокирую пока таймер запускается.
	ui.statusBar->showMessage("Инициализация...");

	connect(ui.pushButtonSendProductKey, SIGNAL(pressed()), this, SLOT(slotpushButtonSendProductKey()));
	connect(ui.pushButtonSetRSA, SIGNAL(pressed()), this, SLOT(slotpushButtonSetRSA()));
	connect(&timerKey, SIGNAL(timeout()), this, SLOT(slotTimerTick()));

	key.InitUsb();
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
	if (!key.SaveSerial(serial, "WWWWW"))
	{
		QMessageBox::critical(0, "Ошибка", key.LastError);
		return;
	}
	
	QMessageBox::information(0, "Информация.", "Операция выполнена успешно.");
}

void UserForm::slotpushButtonSetRSA()
{
	QString WarnMess = "Вы действительно хотите установить новый ключ шифрования канального уровня?";
	if (!ShowYNdialog(WarnMess)) return;

	if (!key.SaveRsaModule(RSA_KEY_FILENAME, VECTOR_PATH, CRYPT_ARRAY_KEY_NAME))
	{
		QMessageBox::critical(0, "Ошибка", key.LastError);
		return;
	}

	QMessageBox::information(0, "Информация.", "Операция выполнена успешно.");
}

void UserForm::slotTimerTick()
{
	//Определяю подключено ли устройство.
	if (key.DeviceIsConnected())
	{
		if (weAreConnect) return; //Мы уже все считали и показали.

		QString mess = "Устройство подключено. ";
		QString hwSerial = QString::fromStdString(key.HWSerial());
		QDate lastDt = key.LastDate();
		QString txtLastDate = lastDt.toString("dd.MM.yyyy");
		
		mess = mess + " HwSerial: " + hwSerial+". LastDate: " + txtLastDate + ".";

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