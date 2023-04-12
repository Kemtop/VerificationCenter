#include "KeyGen.h"


KeyGen::KeyGen(QWidget *parent):QMainWindow(parent)
{
	ui.setupUi(this);

	QIcon icon("Image/app.png");
	setWindowIcon(icon);

	StrCodec = QTextCodec::codecForName("Windows-1251"); //Установка кодировки

	//Нажатие на кнопку "Далее" в первом виджете
	connect(ui.pushButtonW1Next, SIGNAL(pressed()), SLOT(slotpushButtonW1Next()));
	connect(ui.pushButtonW3Next, SIGNAL(pressed()), SLOT(slotpushButtonW3Next()));
	

	connect(ui.pushButtonW1Cancel, SIGNAL(pressed()), SLOT(close()));

	//Нажатие "закрыть" в последнем виджете
	connect(ui.pushButtonW5PrintZayavka, SIGNAL(pressed()), SLOT(close()));

	//Выбор пути к секретному файлу
	connect(ui.pushButtonSelectKeyPath, SIGNAL(pressed()), SLOT(slotpushButtonSelectKeyPath()));
	

	//Сигнал того что был сгенерирован ключ
	connect(this, SIGNAL(KeyIsGenarated()), SLOT(slotKeyIsGenarated()));
		
	ui.stackedWidget->setCurrentIndex(0);
		

	ui.lineEditKeyPath->clear();

	EnableGeneratedKey = 0; //Генерация ключа не производиться


	ui.labelAnimation->setStyleSheet("QLabel::{background-color:#F0F0F0;color:blue;}");
	//statusBar()->setFixedHeight(1);
//	statusBar()->hide();
}

//Нажатие на кнопку "Далее" в первом виджете
void KeyGen::slotpushButtonW1Next()
{

	//Выбран ли путь к ключу
	if (ui.lineEditKeyPath->text().isEmpty() == true)
	{
			QMessageBox::warning(this, StrCodec->toUnicode("Внимание"), StrCodec->toUnicode("Не выбран каталог хранения секретного ключа"));
			return;
	}
		
	
	initBioRng();
}

//Нажатие на кнопку "Назад" во втором виджете
void KeyGen::slotpushButtonW2Back()
{
	ui.stackedWidget->setCurrentIndex(0);
}


void KeyGen::initBioRng()
{
	//Генерация ключа
	//Настройка прогресс бара
	ui.progressBarW3->setRange(0, 64);
	ui.progressBarW3->setValue(0);
	//Инициализация таймеров
	KeyTimer = new QTimer(this);
	ClockTimer = new QTimer(this);
	KeyBetweenTimer = new QTimer(this);


	connect(KeyTimer, SIGNAL(timeout()), SLOT(slotKeyTimerTick()));
	connect(ClockTimer, SIGNAL(timeout()), SLOT(slotClockTimerTick()));
	connect(KeyBetweenTimer, SIGNAL(timeout()), SLOT(slotKeyBetweenTimerTick()));

	//Установка часов
	Clock.min = 5;
	Clock.sec = 0;
	ui.pushButtonW2Next->setEnabled(false); //Делаю кнопку "Далее" не активной
	ui.stackedWidget->setCurrentIndex(1); //Сменяю виджет
	enableAnimation(); //Разрешаю анимацию.
	TimeKeyPress = 0; //Время нажатия клавиши
	TimeBetweenKeyPress = 0; //Время между нажатиями клавиш
	ClearArrays();

	//Позиция в массиве времени нажатой клавиши
	pos_PeriodKeyPress = 0;
	//Позиция в массиве времени между нажатием клавиши
	pos_PeriodBetWeenKeyPush = 0;

	NowKeyCode = 0; //Очищение кодов нажатых клавиш
	LastKeyCode = 0;//Очищение кодов нажатых клавиш

	
	ClockTimer->start(1000);
	EnableGeneratedKey = 1;//Разрешаю генерацию ключа
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


//Выбор пути к секретному файлу
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



//Отображает сообщение об ошибке
void KeyGen::ShowError(QString Message)
{
	QMessageBox::warning(this, "Error", Message);
}

KeyGen::~KeyGen()
{
}

//Реализует часы обратного отсчета, в случае истечения времени возвращает 1 и останавливает отсчет
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

	//Проверяю истекло ли установленное время
	if ((Clock.sec == 0) && (Clock.min == 0)) end_time = true;

	return end_time;
}

//Возвращает строку для отображения часов
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
	//int PeriodKeyPress[100];//Время нажатия клавиши
	//int PeriodBetWeenKeyPush[100]; 
	for (int i = 0;i < 100;i++)
	{
		PeriodKeyPress[i] = 0;
		PeriodBetWeenKeyPush[i] = 0;
	}
}


//Преобразовывает число. Если число больше 255 возвращает младшие 8 бит. Сдвигает исходное число в лево на 8 бит, 
//полученное значение помещает в массив  
unsigned char KeyGen::ToUCDiget(int x, QVector<unsigned char> &Excess)
{
	int a, b, c;
	b = 0xFF;
	a = x&b;
	//Если число больше 8 бит,сдвигаю в право
	while (x > 0xff)
	{
		x >>= 8;
		c = static_cast<unsigned char>(x&b);
		if(a==0) return (unsigned char)x; //Есть только старшая часть.

		Excess.append(c);
	}

	return static_cast<unsigned char>(a);
}


void KeyGen::processRawData(unsigned char *Key)
{
	unsigned char b1 = 0, b2 = 0;
	int pos = 0;

	QVector<unsigned char> Excess; //Старшая часть если существует.
	unsigned char Raw1[64];

	for (int i = 0;i < 64;i++)
	{
		b1 = ToUCDiget(PeriodKeyPress[i], Excess); //Время нажатия клавиши
		b2 = ToUCDiget(PeriodBetWeenKeyPush[i],Excess); //Период между нажатиями клавиш
		if (b2 > 3)
		{
			b1 = b1^b2;
		}

		Raw1[pos] = b1;
		pos++;
	}

	//Вектор не  пуст. Складываю по модулю остатки.
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

	
	//Сглаживаю распределение нулей и единиц.
	Hash3411 H;
	unsigned long long mess_len=512;//Длина сообщения в битах
	H.hash_512(Raw1,mess_len,Key);

}


//Нажатие на кнопку "Далее" в  виджете 3 --введен пароль
void KeyGen::slotpushButtonW3Next()
{
	QString password = ui.lineEditW4Password->text();
	QString passwordReply = ui.lineEditW4PasswordReply->text();
	//Не заполнено поле пароль
	if (password.isEmpty() == true)
	{
		QMessageBox::warning(this, StrCodec->toUnicode("Внимание"), StrCodec->toUnicode("Введите пароль"));
		return;
	}

	if (passwordReply.isEmpty() == true)
	{
		QMessageBox::warning(this, StrCodec->toUnicode("Внимание"), StrCodec->toUnicode("Введите подтверждение пароля"));
		return;
	}

	//Подтверждение не совпадает с паролем
	if (password != passwordReply)
	{
		QMessageBox::warning(this, StrCodec->toUnicode("Внимание"), StrCodec->toUnicode("Подтверждение не совпадает с паролем"));
		return;
	}

	//Пароль не может быть менее 8 символов
	if (password.length()<8)
	{
		QMessageBox::warning(this, StrCodec->toUnicode("Внимание"), 
			StrCodec->toUnicode("Пароль не может быть менее 8-ми символов"));
		return;
	}

	//Пароль не может быть более 64 символов
	if (password.length() > 64)
	{
		QMessageBox::warning(this, StrCodec->toUnicode("Внимание"),
			StrCodec->toUnicode("Пароль не может быть более 64 символов."));
		return;
	}

	ui.pushButtonW3Next->setEnabled(false);
	 ui.lineEditW4Password->setText(""); //Затирание данных.
	ui.lineEditW4PasswordReply->setText("");

	QDateTime now = QDateTime::currentDateTime();
	QString strNow = now.toString("MM-dd-yyyy-hh-mm-ss");

	//Путь к каталогу для сохранения секретного ключа
	QString FileName = "Key" + strNow + ".grk";
	QString Path = ui.lineEditKeyPath->text() + "\\" + FileName;

	//Создает контейнер хранения ключа, помещает ключ
	SecretKeyMaker Skm;
	Skm.setRoseMode(); 

	unsigned char Key[64];
	processRawData(Key);
	
	
	QString pathToSaveReq=""; //Путь сохранения запроса.

	bool errFlag=Skm.CreateKeyFileNoReq(Key,Path,password);
	password = "x1";//Затираю пароль.
	ClearArrays();
	
	for (int i = 0;i < 64;i++)  Key[i] = 0;
		
	if (!errFlag)
	{
		string emess=Skm.getLastError();
		QMessageBox::critical(this, StrCodec->toUnicode("Ошибка"),
			StrCodec->toUnicode(emess.c_str()));
		return;
	}

	ui.stackedWidget->setCurrentIndex(3);
	//Выводит сообщение об успешной генерации ключа.
	setHappyMassege();
}


void KeyGen::setHappyMassege()
{
	//Заполняю сообщение в последнем виджете
	QString HappyMessage;
	string ms1 = "";

	ms1 = "<b>Уважаемый пользователь!</b><br><br> "
		"Программа создала секретный ключ и сохранила его в файл по пути: <br>" +
		StrCodec->fromUnicode(ui.lineEditKeyPath->text()) +
		"<br>Храните в тайне пароль. Не передавайте секретный ключ другим людям.<br>"
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
	QMessageBox::warning(this, StrCodec->toUnicode("Внимание."), StrCodec->toUnicode(mess.c_str()));
}

//Событие по нажатию кнопки
//Если кнопка удерживается некоторое время событие повторно вызывается
void KeyGen::keyPressEvent(QKeyEvent* event) {
	if (EnableGeneratedKey == 0) return;
	TimeKeyPress = 0;
	//int KeyCode = event->key(); //Код нажатой клавиши в юникоде
	KeyTimer->start(1);
	//Если таймер измеряющий интервал между нажатиями отключен-включаем его
	if (EnableKeyBetweenTimer == 0)
	{
		KeyBetweenTimer->start(1);
	}
	animationTimer.start(300);
}

//События отпускания клавиши. Повторяется если клавишу зажали и держут
void KeyGen::keyReleaseEvent(QKeyEvent* event)
{
	if (EnableGeneratedKey == 0) return;
	KeyTimer->stop(); //Останавливаю таймер

	if (pos_PeriodKeyPress == 63) return; //Последовательность необходимой длины была сгенерирована.


	NowKeyCode = event->key(); //Код нажатой клавиши в юникоде						 

							   //Код предыдущей отпущенной клавиши отличен от кода текущей 
	if (NowKeyCode != LastKeyCode)
	{
		//Клавиша уже отпускалась, произошла смена клавиш
		if (LastKeyCode != 0)
		{
			pos_PeriodKeyPress++; //Сдвигаю позицию периода нажатия клавиш
								  //Сохраняю время между нажатиями клавиш	
			PeriodBetWeenKeyPush[pos_PeriodBetWeenKeyPush] = TimeBetweenKeyPress;
			TimeBetweenKeyPress = 0;
			pos_PeriodBetWeenKeyPush++;
		}
		LastKeyCode = NowKeyCode;
	}
	//Сохраняю время нажатия клавиши
	PeriodKeyPress[pos_PeriodKeyPress] = TimeKeyPress;

}

//Срабатывание таймера измерения времени нажатия клавиши
void KeyGen::slotKeyTimerTick()
{
	TimeKeyPress++;
}
//Срабатывание таймера часов, и работа часов реального времени
void KeyGen::slotClockTimerTick()
{

	//Часы
	//Если разрешена генерация ключа
	if (EnableGeneratedKey == 1)
	{
		ClockTimer->stop();
		bool EndTime = ClockProcess();
		QString str = StrCodec->toUnicode("Осталось: ");
		QString str1 = str + GetStrClock();
		ui.labelW3EndTime->setText(str1);
		//Время генерации истекло
		if (EndTime == 1)
		{
			QMessageBox::critical(this, StrCodec->toUnicode("Ошибка"),
				StrCodec->toUnicode("Истекло время генерации ключа. Программа будет закрыта."));
			close();
			return;
		}

		int curProgress = pos_PeriodKeyPress + 1;
		if(curProgress>1)//Что бы не показывал, пока пользователь не чего не сделал.
		ui.progressBarW3->setValue(curProgress); //Текущий прогресс.

		ClockTimer->start(1000);
												 //Если ключ создан
		if (curProgress == 64)
		{
			emit(KeyIsGenarated()); //Сигнал того что ключ сгенерированн
		}
	}

}

//Срабатывание таймера замера периода между нажатием клавиш
void KeyGen::slotKeyBetweenTimerTick()
{
	TimeBetweenKeyPress++;
}

//Слот того что ключ сгенерированн
void KeyGen::slotKeyIsGenarated()
{
	EnableGeneratedKey = 0;//Запрещаю генерацию ключа
	ui.pushButtonW3Next->setEnabled(true);
	ui.stackedWidget->setCurrentIndex(2);
}

void KeyGen::slotAnimationTimerTick()
{
	rotateAnime();
	animationTimer.stop();
}


