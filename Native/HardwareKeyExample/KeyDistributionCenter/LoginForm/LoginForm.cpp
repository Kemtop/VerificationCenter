#include "stdafx.h"
#include "LoginForm.h"

LoginForm::LoginForm(QWidget *parent)
	: QDialog(parent)
{
	//������� ������ ���������
	Qt::WindowFlags flags = windowFlags();
	Qt::WindowFlags helpFlag =Qt::WindowContextHelpButtonHint | Qt::WindowCloseButtonHint;
	flags = flags & (~helpFlag);
	setWindowFlags(flags);

	ui.setupUi(this);
	//�������� ������ ������� ������ "������" ������������ ����.
		QPoint cur = ui.pushButtonCancel->pos();
		posCancelBut.setX(cur.x());
		posCancelBut.setY(cur.y());
	

	connect(ui.pushButtonOk,SIGNAL(pressed()),SLOT(slotPushOk()));
	connect(ui.pushButtonCancel, SIGNAL(pressed()), SLOT(close()));
	StrCoder = QTextCodec::codecForName("Windows-1251"); //��������� ���������
	ui.labelWarning->setVisible(false);
	ui.lineEditHost->setText("localhost");
	errAuthCnt = 0;//��������� ���������� �������� ������� �����.

	 QString styleSheet = "QLabel { font-size:11pt;"
		 "}"
		 "QLineEdit{font-size:11pt; }";
	 setStyleSheet(styleSheet);

     #ifdef INCLUDE_HW_USB_KEY 
	      timerUsbKeyState.setInterval(250); //�������� �������� ������� USB �����.
	      usb.InitUsb();
		  if (!usb.isConnected())   //��������� ���������� �� ����������.
			  lockUI(true); //�������� ��������� ������������.
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
	return loginName; //��� ������������.;
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
	{ //�������������.
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
		//�������������� ������.
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
	if (!usb.isConnected())   //��������� ���������� �� ����������.
	{
		lockUI(true); //�������� ��������� ������������. ��� ����������� �����.
		return;
	}
	
	timerUsbKeyState.stop(); //������������ ������ ������ ����������� �����.���� ������.

	//�������� ������������ ����� ��������.
	uint8_t phK[] = usbTLeverVector;
	int lenK = sizeof(phK) / sizeof(phK[0]); 
	int res = 0;
	std::string errMessage = "";

	try 
	{
        //�������� � �������� ���� ��������.
		res = usb.che�kProduckKey(errMessage,phK, lenK);	
	}
	catch (...)
	{
		lockUI(true);
	    //�������� ����������� ���������.
		return;
	}
	   
	//�������� ��� �������� �����.
	if (res == 0)
	{
		QString mess = QString::fromStdString(errMessage);
		setWarningMessage(mess);
		lockUI(true);
	}

	//��� ������.
	if (res == 1)
	{
		lockUI(false); //���������������.	
		//������ �������� ���� �������� �����.
		if (!errMessage.empty())
		{
			QString mess = QString::fromStdString(errMessage);
			setWarningMessage(mess);
		}
	}

}

void LoginForm::slotTimerUsbKeyStateTimeout()
{
	//��������� ���������� �� ����������.
	if (!usb.isConnected())
	{
		lockUI(true); //������������.	
		return;
	}

	checkUsbKey(); //��������������� � ���������� ������.
}

//������� �� ������ ��.
void LoginForm::slotPushOk()
{	
	QString  hostName_ = ui.lineEditHost->text();
	QString sPort = ui.lineEditPort->text();
	
	bool goodPort;
	 int  SrvPort = sPort.toInt(&goodPort); //�������� �� ���� �������� ���������.	
	if (!goodPort)
	{		
			QMessageBox::critical(this, StrCoder->toUnicode("������"), StrCoder->toUnicode("�� ������ ���� �������."));
			return;
	}

	QString UserName = ui.lineEditUserName->text();
	QString Passwd=ui.lineEditPassword->text();


	//������� ������.
	hostName=hostName_; //����.
	srvPort= SrvPort; //����
	loginName=UserName; //��� ������������.

	timerUsbKeyState.stop(); //������������ ����� �����.
	accept();
}
