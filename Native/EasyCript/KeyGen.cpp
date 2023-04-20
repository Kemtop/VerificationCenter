#include "KeyGen.h"


KeyGen::KeyGen(QWidget *parent):QMainWindow(parent)
{
	ui.setupUi(this);

	QIcon icon("Image/app.png");
	setWindowIcon(icon);

	StrCodec = QTextCodec::codecForName("Windows-1251"); //��������� ���������

	//������� �� ������ "�����" � ������ �������
	connect(ui.pushButtonW1Next, SIGNAL(pressed()), SLOT(slotpushButtonW1Next()));
	connect(ui.pushButtonW3Next, SIGNAL(pressed()), SLOT(slotpushButtonW3Next()));
	

	connect(ui.pushButtonW1Cancel, SIGNAL(pressed()), SLOT(close()));

	//������� "�������" � ��������� �������
	connect(ui.pushButtonW5PrintZayavka, SIGNAL(pressed()), SLOT(close()));

	//����� ���� � ���������� �����
	connect(ui.pushButtonSelectKeyPath, SIGNAL(pressed()), SLOT(slotpushButtonSelectKeyPath()));
	

	//������ ���� ��� ��� ������������ ����
	connect(this, SIGNAL(KeyIsGenarated()), SLOT(slotKeyIsGenarated()));
		
	ui.stackedWidget->setCurrentIndex(0);
		

	ui.lineEditKeyPath->clear();

	EnableGeneratedKey = 0; //��������� ����� �� �������������


	ui.labelAnimation->setStyleSheet("QLabel::{background-color:#F0F0F0;color:blue;}");
	//statusBar()->setFixedHeight(1);
//	statusBar()->hide();
}

//������� �� ������ "�����" � ������ �������
void KeyGen::slotpushButtonW1Next()
{

	//������ �� ���� � �����
	if (ui.lineEditKeyPath->text().isEmpty() == true)
	{
			QMessageBox::warning(this, StrCodec->toUnicode("��������"), StrCodec->toUnicode("�� ������ ������� �������� ���������� �����"));
			return;
	}
		
	
	initBioRng();
}

//������� �� ������ "�����" �� ������ �������
void KeyGen::slotpushButtonW2Back()
{
	ui.stackedWidget->setCurrentIndex(0);
}


void KeyGen::initBioRng()
{
	//��������� �����
	//��������� �������� ����
	ui.progressBarW3->setRange(0, 64);
	ui.progressBarW3->setValue(0);
	//������������� ��������
	KeyTimer = new QTimer(this);
	ClockTimer = new QTimer(this);
	KeyBetweenTimer = new QTimer(this);


	connect(KeyTimer, SIGNAL(timeout()), SLOT(slotKeyTimerTick()));
	connect(ClockTimer, SIGNAL(timeout()), SLOT(slotClockTimerTick()));
	connect(KeyBetweenTimer, SIGNAL(timeout()), SLOT(slotKeyBetweenTimerTick()));

	//��������� �����
	Clock.min = 5;
	Clock.sec = 0;
	ui.pushButtonW2Next->setEnabled(false); //����� ������ "�����" �� ��������
	ui.stackedWidget->setCurrentIndex(1); //������ ������
	enableAnimation(); //�������� ��������.
	TimeKeyPress = 0; //����� ������� �������
	TimeBetweenKeyPress = 0; //����� ����� ��������� ������
	ClearArrays();

	//������� � ������� ������� ������� �������
	pos_PeriodKeyPress = 0;
	//������� � ������� ������� ����� �������� �������
	pos_PeriodBetWeenKeyPush = 0;

	NowKeyCode = 0; //�������� ����� ������� ������
	LastKeyCode = 0;//�������� ����� ������� ������

	
	ClockTimer->start(1000);
	EnableGeneratedKey = 1;//�������� ��������� �����
}

void KeyGen::enableAnimation()
{
	ui.labelAnimation->setPixmap(QPixmap("Image/gear.png"));
	connect(&animationTimer, SIGNAL(timeout()), SLOT(slotAnimationTimerTick()));
	animationValue = 0;

}

void KeyGen::rotateAnime()
{
	QPixmap ship("Image/gear.png");

	QImage image(ship.size(), QImage::Format_ARGB32_Premultiplied);
	image.fill(Qt::transparent);


	QPixmap rotate = QPixmap::fromImage(image);

	QPainter p(&rotate);
	p.setRenderHint(QPainter::Antialiasing);
	p.setRenderHint(QPainter::SmoothPixmapTransform);
	p.setRenderHint(QPainter::HighQualityAntialiasing);
	p.translate(rotate.size().width()/2, rotate.size().height() / 2);
	p.rotate(animationValue);
	p.translate(-rotate.size().width() / 2,- rotate.size().height() / 2);
	p.drawPixmap(0,0,ship);
	p.end();
	ui.labelAnimation->setPixmap(rotate);
	if(animationValue < 1000000)
	animationValue++;
}


//����� ���� � ���������� �����
void KeyGen::slotpushButtonSelectKeyPath()
{
	QString path;	
	
		path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
			"",
			QFileDialog::ShowDirsOnly
			| QFileDialog::DontResolveSymlinks);
	
	

	if (path.isEmpty() == true) return;

	path=path.replace("/", "\\");
	ui.lineEditKeyPath->setText(path);
}



//���������� ��������� �� ������
void KeyGen::ShowError(QString Message)
{
	QMessageBox::warning(this, "Error", Message);
}

KeyGen::~KeyGen()
{
}

//��������� ���� ��������� �������, � ������ ��������� ������� ���������� 1 � ������������� ������
bool KeyGen::ClockProcess()
{
	bool end_time = false;

	if (Clock.sec > 0)
	{
		Clock.sec--;
	}
	else
	{
		if (Clock.min > 0)
		{
			Clock.min--;
			Clock.sec = 59;
		}
	}

	//�������� ������� �� ������������� �����
	if ((Clock.sec == 0) && (Clock.min == 0)) end_time = true;

	return end_time;
}

//���������� ������ ��� ����������� �����
QString KeyGen::GetStrClock()
{
	QString str;

	str = "0" + QString::number(Clock.min) + ":";

	if (Clock.sec < 10)
		str += "0" + QString::number(Clock.sec);
	else
		str += QString::number(Clock.sec);

	return str;
}



void KeyGen::ClearArrays()
{
	//int PeriodKeyPress[100];//����� ������� �������
	//int PeriodBetWeenKeyPush[100]; 
	for (int i = 0;i < 100;i++)
	{
		PeriodKeyPress[i] = 0;
		PeriodBetWeenKeyPush[i] = 0;
	}
}


//��������������� �����. ���� ����� ������ 255 ���������� ������� 8 ���. �������� �������� ����� � ���� �� 8 ���, 
//���������� �������� �������� � ������  
unsigned char KeyGen::ToUCDiget(int x, QVector<unsigned char> &Excess)
{
	int a, b, c;
	b = 0xFF;
	a = x&b;
	//���� ����� ������ 8 ���,������� � �����
	while (x > 0xff)
	{
		x >>= 8;
		c = static_cast<unsigned char>(x&b);
		if(a==0) return (unsigned char)x; //���� ������ ������� �����.

		Excess.append(c);
	}

	return static_cast<unsigned char>(a);
}


void KeyGen::processRawData(unsigned char *Key)
{
	unsigned char b1 = 0, b2 = 0;
	int pos = 0;

	QVector<unsigned char> Excess; //������� ����� ���� ����������.
	unsigned char Raw1[64];

	for (int i = 0;i < 64;i++)
	{
		b1 = ToUCDiget(PeriodKeyPress[i], Excess); //����� ������� �������
		b2 = ToUCDiget(PeriodBetWeenKeyPush[i],Excess); //������ ����� ��������� ������
		if (b2 > 3)
		{
			b1 = b1^b2;
		}

		Raw1[pos] = b1;
		pos++;
	}

	//������ ��  ����. ��������� �� ������ �������.
	if (!Excess.isEmpty())
	{
		unsigned char t = 0;
		pos = 0;
		for (int i = 0; i < Excess.size(); ++i)
		{
			t = Excess.at(i);
			b1 = Raw1[pos];
			b2 = b1^t;
			Raw1[pos] = b2;
			pos++;
		}
	}

	
	//��������� ������������� ����� � ������.
	Hash3411 H;
	unsigned long long mess_len=512;//����� ��������� � �����
	H.hash_512(Raw1,mess_len,Key);

}


//������� �� ������ "�����" �  ������� 3 --������ ������
void KeyGen::slotpushButtonW3Next()
{
	QString password = ui.lineEditW4Password->text();
	QString passwordReply = ui.lineEditW4PasswordReply->text();
	//�� ��������� ���� ������
	if (password.isEmpty() == true)
	{
		QMessageBox::warning(this, StrCodec->toUnicode("��������"), StrCodec->toUnicode("������� ������"));
		return;
	}

	if (passwordReply.isEmpty() == true)
	{
		QMessageBox::warning(this, StrCodec->toUnicode("��������"), StrCodec->toUnicode("������� ������������� ������"));
		return;
	}

	//������������� �� ��������� � �������
	if (password != passwordReply)
	{
		QMessageBox::warning(this, StrCodec->toUnicode("��������"), StrCodec->toUnicode("������������� �� ��������� � �������"));
		return;
	}

	//������ �� ����� ���� ����� 8 ��������
	if (password.length()<8)
	{
		QMessageBox::warning(this, StrCodec->toUnicode("��������"), 
			StrCodec->toUnicode("������ �� ����� ���� ����� 8-�� ��������"));
		return;
	}

	//������ �� ����� ���� ����� 64 ��������
	if (password.length() > 64)
	{
		QMessageBox::warning(this, StrCodec->toUnicode("��������"),
			StrCodec->toUnicode("������ �� ����� ���� ����� 64 ��������."));
		return;
	}

	ui.pushButtonW3Next->setEnabled(false);
	 ui.lineEditW4Password->setText(""); //��������� ������.
	ui.lineEditW4PasswordReply->setText("");

	QDateTime now = QDateTime::currentDateTime();
	QString strNow = now.toString("MM-dd-yyyy-hh-mm-ss");

	//���� � �������� ��� ���������� ���������� �����
	QString FileName = "Key" + strNow + ".grk";
	QString Path = ui.lineEditKeyPath->text() + "\\" + FileName;

	//������� ��������� �������� �����, �������� ����
	SecretKeyMaker Skm;
	Skm.setRoseMode(); 

	unsigned char Key[64];
	processRawData(Key);
	
	
	QString pathToSaveReq=""; //���� ���������� �������.

	bool errFlag=Skm.CreateKeyFileNoReq(Key,Path,password);
	password = "x1";//������� ������.
	ClearArrays();
	
	for (int i = 0;i < 64;i++)  Key[i] = 0;
		
	if (!errFlag)
	{
		string emess=Skm.getLastError();
		QMessageBox::critical(this, StrCodec->toUnicode("������"),
			StrCodec->toUnicode(emess.c_str()));
		return;
	}

	ui.stackedWidget->setCurrentIndex(3);
	//������� ��������� �� �������� ��������� �����.
	setHappyMassege();
}


void KeyGen::setHappyMassege()
{
	//�������� ��������� � ��������� �������
	QString HappyMessage;
	string ms1 = "";

	ms1 = "<b>��������� ������������!</b><br><br> "
		"��������� ������� ��������� ���� � ��������� ��� � ���� �� ����: <br>" +
		StrCodec->fromUnicode(ui.lineEditKeyPath->text()) +
		"<br>������� � ����� ������. �� ����������� ��������� ���� ������ �����.<br>"
		;


	HappyMessage = StrCodec->toUnicode(ms1.c_str());

	ui.textEditW5Message->setText(HappyMessage);
}

void KeyGen::createDebugFile(unsigned char *Key)
{
	QByteArray Ba;

	Ba.resize(64);

	for (int i = 0;i < 64;i++)
	{
		Ba[i] = Key[i];
	}


	QByteArray hba;
	hba.append('[');
	hba.append('0');
	hba.append(']');

	hba.append(Ba.toHex());
	hba.append('\r');
	hba.append('\n');

	QFile file("testSeq");
	if (file.open(QIODevice::Append))
	{
		file.write(hba);
	}
	file.close();
}

void KeyGen::createDebugValue(int v)
{
	QByteArray Ba;
	Ba.append(v);
	QByteArray hba = Ba.toHex();
	hba.append('\r');
	hba.append('\n');

	QFile file("testVal");
	if (file.open(QIODevice::Append))
	{
		file.write(hba);
	}
	file.close();
}

void KeyGen::ShowWarning(string mess)
{
	QMessageBox::warning(this, StrCodec->toUnicode("��������."), StrCodec->toUnicode(mess.c_str()));
}

//������� �� ������� ������
//���� ������ ������������ ��������� ����� ������� �������� ����������
void KeyGen::keyPressEvent(QKeyEvent* event) {
	if (EnableGeneratedKey == 0) return;
	TimeKeyPress = 0;
	//int KeyCode = event->key(); //��� ������� ������� � �������
	KeyTimer->start(1);
	//���� ������ ���������� �������� ����� ��������� ��������-�������� ���
	if (EnableKeyBetweenTimer == 0)
	{
		KeyBetweenTimer->start(1);
	}
	animationTimer.start(300);
}

//������� ���������� �������. ����������� ���� ������� ������ � ������
void KeyGen::keyReleaseEvent(QKeyEvent* event)
{
	if (EnableGeneratedKey == 0) return;
	KeyTimer->stop(); //������������ ������

	if (pos_PeriodKeyPress == 63) return; //������������������ ����������� ����� ���� �������������.


	NowKeyCode = event->key(); //��� ������� ������� � �������						 

							   //��� ���������� ���������� ������� ������� �� ���� ������� 
	if (NowKeyCode != LastKeyCode)
	{
		//������� ��� �����������, ��������� ����� ������
		if (LastKeyCode != 0)
		{
			pos_PeriodKeyPress++; //������� ������� ������� ������� ������
								  //�������� ����� ����� ��������� ������	
			PeriodBetWeenKeyPush[pos_PeriodBetWeenKeyPush] = TimeBetweenKeyPress;
			TimeBetweenKeyPress = 0;
			pos_PeriodBetWeenKeyPush++;
		}
		LastKeyCode = NowKeyCode;
	}
	//�������� ����� ������� �������
	PeriodKeyPress[pos_PeriodKeyPress] = TimeKeyPress;

}

//������������ ������� ��������� ������� ������� �������
void KeyGen::slotKeyTimerTick()
{
	TimeKeyPress++;
}
//������������ ������� �����, � ������ ����� ��������� �������
void KeyGen::slotClockTimerTick()
{

	//����
	//���� ��������� ��������� �����
	if (EnableGeneratedKey == 1)
	{
		ClockTimer->stop();
		bool EndTime = ClockProcess();
		QString str = StrCodec->toUnicode("��������: ");
		QString str1 = str + GetStrClock();
		ui.labelW3EndTime->setText(str1);
		//����� ��������� �������
		if (EndTime == 1)
		{
			QMessageBox::critical(this, StrCodec->toUnicode("������"),
				StrCodec->toUnicode("������� ����� ��������� �����. ��������� ����� �������."));
			close();
			return;
		}

		int curProgress = pos_PeriodKeyPress + 1;
		if(curProgress>1)//��� �� �� ���������, ���� ������������ �� ���� �� ������.
		ui.progressBarW3->setValue(curProgress); //������� ��������.

		ClockTimer->start(1000);
												 //���� ���� ������
		if (curProgress == 64)
		{
			emit(KeyIsGenarated()); //������ ���� ��� ���� �������������
		}
	}

}

//������������ ������� ������ ������� ����� �������� ������
void KeyGen::slotKeyBetweenTimerTick()
{
	TimeBetweenKeyPress++;
}

//���� ���� ��� ���� �������������
void KeyGen::slotKeyIsGenarated()
{
	EnableGeneratedKey = 0;//�������� ��������� �����
	ui.pushButtonW3Next->setEnabled(true);
	ui.stackedWidget->setCurrentIndex(2);
}

void KeyGen::slotAnimationTimerTick()
{
	rotateAnime();
	animationTimer.stop();
}


