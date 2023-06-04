#include "stdafx.h"
#include "LoginForm.h"

LoginForm::LoginForm(QWidget *parent)
	: QDialog(parent)
{
	//Скрываю кнопку подсказки
	Qt::WindowFlags flags = windowFlags();
	Qt::WindowFlags helpFlag =Qt::WindowContextHelpButtonHint | Qt::WindowCloseButtonHint;
	flags = flags & (~helpFlag);
	setWindowFlags(flags);

	ui.setupUi(this);
	//Сохраняю старую позицию кнопки "Отмена" относительно окна.
		QPoint cur = ui.pushButtonCancel->pos();
		posCancelBut.setX(cur.x());
		posCancelBut.setY(cur.y());
	

	connect(ui.pushButtonOk,SIGNAL(pressed()),SLOT(slotPushOk()));
	connect(ui.pushButtonCancel, SIGNAL(pressed()), SLOT(close()));
	StrCoder = QTextCodec::codecForName("Windows-1251"); //Установка кодировки
	ui.labelWarning->setVisible(false);
	ui.lineEditHost->setText("localhost");
	errAuthCnt = 0;//Сбрасываю количество неверных попыток входа.

	 QString styleSheet = "QLabel { font-size:11pt;"
		 "}"
		 "QLineEdit{font-size:11pt; }";
	 setStyleSheet(styleSheet);

     #ifdef INCLUDE_HW_USB_KEY 
	      timerUsbKeyState.setInterval(250); //Интервал проверки наличия USB ключа.
	      usb.InitUsb();
		  if (!usb.isConnected())   //Определяю подключено ли устройство.
			  lockUI(true); //Блокирую интерфейс пользователя.
		  connect(&timerUsbKeyState, SIGNAL(timeout()), this, SLOT(slotTimerUsbKeyStateTimeout()));
		  timerUsbKeyState.start();
     #endif
}

LoginForm::~LoginForm()
{
}


QString LoginForm::getHostName()
{
	return hostName;
}

int LoginForm::getPort()
{
	return srvPort;
}

QString LoginForm::getLoginName()
{
	return loginName; //Имя пользователя.;
}

void LoginForm::setHostName(QString val)
{
	ui.lineEditHost->setText(val);
	hostName = val;
}

void LoginForm::setPort(int val)
{
	ui.lineEditPort->setText(QString::number(val));
	srvPort = val;
}

void LoginForm::setLoginName(QString val)
{
	loginName = val;		
	ui.lineEditUserName->setText(val);
	ui.lineEditPassword->setFocus();
}

bool LoginForm::getIsEmptyDb()
{
	return IsEmptyDb;
}

void LoginForm::lockUI(bool lock)
{
	if (lock)
	{ //Заблокированы.
		ui.lineEditHost->clear();
		ui.lineEditPassword->clear();
		ui.lineEditPort->clear();
		ui.lineEditUserName->clear();

		ui.pushButtonOk->setVisible(false);
		ui.labelWarning->setVisible(true);	
	}
	else
	{
		ui.pushButtonOk->setVisible(true);
		//Восстанавливаю данные.
		ui.lineEditHost->setText(hostName);
		ui.lineEditPort->setText(QString::number(srvPort));
		ui.lineEditUserName->setText(loginName);
		ui.lineEditPassword->setFocus();
		
		ui.labelWarning->setVisible(false);
	}

}

void LoginForm::setWarningMessage(QString m)
{
	ui.labelWarning->setVisible(true);
	ui.labelWarning->setText(m);
}

void LoginForm::checkUsbKey()
{	
	if (!usb.isConnected())   //Определяю подключено ли устройство.
	{
		lockUI(true); //Блокирую интерфейс пользователя. Нет аппаратного ключа.
		return;
	}
	
	timerUsbKeyState.stop(); //Останавливаю таймер поиска аппаратного ключа.Ключ найден.

	//Проверка корректности ключа продукта.
	uint8_t phK[] = usbTLeverVector;
	int lenK = sizeof(phK) / sizeof(phK[0]); 
	int res = 0;
	std::string errMessage = "";

	try 
	{
        //Считываю и проверяю ключ продукта.
		res = usb.cheсkProduckKey(errMessage,phK, lenK);	
	}
	catch (...)
	{
		lockUI(true);
	    //Показать критическое сообщение.
		return;
	}
	   
	//Проблемы при проверке ключа.
	if (res == 0)
	{
		QString mess = QString::fromStdString(errMessage);
		setWarningMessage(mess);
		lockUI(true);
	}

	//Все хорошо.
	if (res == 1)
	{
		lockUI(false); //Разблокирование.	
		//Похоже истекает срок действия ключа.
		if (!errMessage.empty())
		{
			QString mess = QString::fromStdString(errMessage);
			setWarningMessage(mess);
		}
	}

}

void LoginForm::slotTimerUsbKeyStateTimeout()
{
	//Определяю подключено ли устройство.
	if (!usb.isConnected())
	{
		lockUI(true); //Блокирование.	
		return;
	}

	checkUsbKey(); //Взаимодействует с аппаратным ключом.
}

//Нажатие на кнопку ОК.
void LoginForm::slotPushOk()
{	
	QString  hostName_ = ui.lineEditHost->text();
	QString sPort = ui.lineEditPort->text();
	
	bool goodPort;
	 int  SrvPort = sPort.toInt(&goodPort); //Является ли порт числовым значением.	
	if (!goodPort)
	{		
			QMessageBox::critical(this, StrCoder->toUnicode("Ошибка"), StrCoder->toUnicode("Не верный порт сервера."));
			return;
	}

	QString UserName = ui.lineEditUserName->text();
	QString Passwd=ui.lineEditPassword->text();


	//Передаю данные.
	hostName=hostName_; //Хост.
	srvPort= SrvPort; //Порт
	loginName=UserName; //Имя пользователя.

	timerUsbKeyState.stop(); //Останавливаю опрос ключа.
	accept();
}
