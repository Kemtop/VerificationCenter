#include "GranitCrypt.h"
#include "RequirePswForm.h"


GranitCrypt::GranitCrypt(QWidget *parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);
	lockUi = false; //Флаг блокировки контролов при ошибке.

	ui.actionOpenFile->setIcon(QIcon("Image/openMenu.png"));
	ui.actionExit->setIcon(QIcon("Image/exitMenu.png"));
	ui.actionHelp->setIcon(QIcon("Image/infoMenu.png"));
	ui.actionResetSetings->setIcon(QIcon("Image/resetMenu.png"));
	ui.actionDbPath->setIcon(QIcon("Image/key.png"));
	

	connect(ui.menu_File, SIGNAL(triggered(QAction *)), this, SLOT(slotMenuFileClicked(QAction*)));
	connect(ui.menuHelp, SIGNAL(triggered(QAction *)), this, SLOT(slotMenuHelpClicked(QAction*)));
	connect(ui.menuSettings, SIGNAL(triggered(QAction *)), this, SLOT(slotMenuSettings(QAction *)));

	connect(ui.pushButtonCrypt, SIGNAL(pressed()), this, SLOT(slotpushButtonCrypt()));
	connect(ui.pushButtonDecrypt, SIGNAL(pressed()), this, SLOT(slotpushButtonDecrypt()));
	connect(ui.pushButtonSelectKey, SIGNAL(pressed()), this, SLOT(slotpushButtonSelectKey()));

	//Делаю скрытыми компоненты и масштабирую текст эдит 
	ui.progressBarAll->setVisible(0);
	ui.progressBarFile->setVisible(0);
	ui.labelProgressFile->setVisible(0);
	ui.labelProgressAll->setVisible(0);
	ui.plainTextEditFiles->resize(ui.plainTextEditFiles->width(), 540);

	StrCodec = QTextCodec::codecForName("Windows-1251"); //Установка кодировки

	ui.pushButtonCrypt->setEnabled(0);
	ui.pushButtonDecrypt->setEnabled(0);

	UserPassword = ""; //Очищаю переменную для хранения пароля
	typeProcessWorking = 0; //Нет ни какого режима работы.


	Param.PwdIsChecked = 0; //Пароль для секретного ключа не был проверен
	Param.ReceiverId = -1; //Ид получателя не выбрано
	ui.pushButtonSelectKey->setVisible(0); //Для теста!
	//Регистрация фильтра событий.
	ui.plainTextEditFiles->viewport()->installEventFilter(this);
		
	string helloMassage = HelloMessage();

	QTextCharFormat tf;
	tf = ui.plainTextEditFiles->currentCharFormat();
	tf.setForeground(QBrush(QColor("#008000")));
	ui.plainTextEditFiles->setCurrentCharFormat(tf);
	ui.plainTextEditFiles->appendPlainText(StrCodec->toUnicode(helloMassage.c_str()));

	//Код не выносить в отдельный метод! Это снизит защиту приложения от взлома!
	HMODULE hLib;
	hLib = LoadLibraryA("libCn3q.dll");
	if (hLib == 0) return; //Не удалось загрузить библиотеку.

	typedef unsigned char(*F0)();
	typedef unsigned char(*setTpPointA)(unsigned int val);
	typedef unsigned char(*envirGradient)(unsigned int val);

	F0 f0 = (F0)GetProcAddress(hLib, "IMA_MAP_ANPOINT");//Злоумышленник не сможет вызвать другие функции, так как они будут давать исключения, из-за попытки обратится 
														//к внутри класовой переменной. Исключение ошибки чтения.
	setTpPointA f1 = (setTpPointA)GetProcAddress(hLib, "setTpPointA");
	envirGradient f2 = (envirGradient)GetProcAddress(hLib, "environmentGradient");

	unsigned int p = 36;
	unsigned int *ptrP = &p; //Адрес в памяти.
	*ptrP++;

	//Добавить проверки на пустоту функций в dll.


	unsigned int retX = f1(p);

	if (retX < p)
	{
		FreeLibrary(hLib);
		return; //Ошибка.
	}

	p = 128;
	retX = f2(p);

	if (retX != 1)
	{
		FreeLibrary(hLib);
		return; //Ошибка.
	}

	FreeLibrary(hLib);
	badAttempPswd = 0;
	hasThrObj = false; //Объекта не существует.
}

GranitCrypt::~GranitCrypt()
{
	deletechW();
}

void GranitCrypt::customEvent(QEvent * pe)
{
	/*
	  Проверяем тип полученного события,
	  и если он соответствует типу нашего события,то мы приводим его к классу события.
	*/
	if ((int)pe->type() == eventsCipherWorker::eventCipherWorkerType)
	{
		eventsCipherWorker *ev;
		ev = (eventsCipherWorker*)pe;

		int curAll = ev->getAllProcessVal(); //Текущее значение всех обрабатываемых файлов.
		int maxCurFile = ev->getMaxFileProcess(); //Максимальное значение обрабатываемых итераций в файле.
		int curFileProgress = ev->getCurFileProcess();//Текущее значение.
		int thredStatus = ev->getThreadStatus();
		bool needShowPFi = ev->getNeedshowcurfileprocessInfo(); //Нужно вывести сообщение о процессе шифрования файла.


		//Событие об изменении общего количества процесса шифруемых файлов.
		if (curAll > 0)
		{
			string mess_progress = "";
			//Прогресс обработки файлов.
			mess_progress = " Файл " + std::to_string(curAll + 1) + " из " + std::to_string(allCurProcessFiles);
			ui.labelProgressAll->setText(StrCodec->toUnicode(mess_progress.c_str()));

			ui.progressBarAll->setValue(curAll); //Двигаю прогресс бар.
			paintProcessFile(curAll);
		}

		//Событие установки максимального значения.
		if (maxCurFile > 0)
		{
			ui.progressBarFile->setValue(0);
			ui.progressBarFile->setMaximum(maxCurFile);
		}

		//Событие текущего значения.
		if (curFileProgress > -1)
		{

			ui.progressBarFile->setValue(curFileProgress);//Двигаю прогресс бар
		}

		if (needShowPFi)
		{
			string mess = ev->getCurfileprocessInfo();
			ui.labelProgressFile->setText(StrCodec->toUnicode(mess.c_str()));
		}

		//Cобытие о состоянии потока.
		if (thredStatus > 0)
		{
			 
			string errMess = "";
			string mess_progress1 = "";
			switch (thredStatus)
			{
				//Поток запущен.
			case 1:
				//Делает видимыми компоненты, используемые для визуального отображения процесса шифрования файлов
				showWorkComponent(allCurProcessFiles);
				paintProcessFile(0);//Шифрование начинается с первого файла.
				//Прогресс обработки файлов.
				mess_progress1 = " Файл " + std::to_string(1) + " из " + std::to_string(allCurProcessFiles);
				ui.labelProgressAll->setText(StrCodec->toUnicode(mess_progress1.c_str()));
				break;
				//Поток успешно завершен.
			case 2:
				ui.progressBarAll->setValue(ui.progressBarAll->maximum()); //Устанавливаю максимальное значение.
				//Пишет сообщение в PlainTextEdit об успешности, меняет progress bar
				writeEndMessage();
				

				break;
				//Возникла ошибка.
			case 3:
				errMess = ev->getLastError(); //Получаю ошибку.
				paintShowError(errMess);
				typeProcessWorking = 0;
		
				break;
			default:
	
				
				break;
			}
		}

		return;
	}
	QWidget::customEvent(pe);
}

void GranitCrypt::initchW()
{
	if (hasThrObj) delete chW; //Объект был создан удаляю для очистки данных.
	chW = NULL;

	chW = new CipherWorker(this);
	hasThrObj = true;
}

void GranitCrypt::deletechW()
{
	if (hasThrObj) delete chW; //Объект был создан удаляю.
	hasThrObj = false;
	chW = NULL;
}


bool GranitCrypt::eventFilter(QObject * target, QEvent * event)
{
	//Фильтрую события от нужного мне компонента.
	if ((target == ui.plainTextEditFiles || target == ui.plainTextEditFiles->viewport()) &&
		event->type() == QEvent::MouseButtonDblClick)
	{
		//Выполняет действия выбора файлов.
		doOpenFiles();

		return true; //Решаю проблему выделения расширения файла.
	}

	return QWidget::eventFilter(target, event);
}


//Клик в меню файл
void GranitCrypt::slotMenuFileClicked(QAction *action)
{
	QString value = action->text();

	if (StrCodec->toUnicode("Выход") == value)
	{
		close();
	}

	//
	if (StrCodec->toUnicode("Выход") == value)
	{
		close();
	}



	if (StrCodec->toUnicode("Открыть") == value)
	{
		doOpenFiles(); //Выполняю действия по выбору файлов.
	}

}

bool GranitCrypt::LoadSettings(PathSettings &S)
{
	QString ExePath = QDir::currentPath(); // /
	QString PathToIniFile = ExePath + "/" + Ini_file_name;

	//Проверка существования файла
	QFileInfo file_info(PathToIniFile);
	if (!file_info.exists()) return 0;

	QSettings settings(PathToIniFile, QSettings::IniFormat);

	QString PathSK = settings.value("SK_Path", "").toString(); //Db_Path
		
	S.SetPathSK(PathSK);


	return true;
}

//Выводит диалог выбора секретного ключа. Если пользователь отказался,или не найдено ключа-возвращает false.
bool GranitCrypt::SelectPathSK(PathSettings &S)
{
	//Форма выбора секретного ключа
	SelectKeyForm form_secret_key;


	if (form_secret_key.exec() == QDialog::Accepted)
	{
		QString SK_path = form_secret_key.KeyPath; //Путь к секретному ключу

	   //Проверка файла СК
		uint32_t l;
		if (!Ks.CheckSK(SK_path, l))
		{
			QMessageBox::critical(this, StrCodec->toUnicode("Ошибка"), StrCodec->toUnicode(Ks.getLastError().c_str()));
			return false;
		}
			
		S.SetPathSK(SK_path);

		return true;
	}

	return false;
}


void GranitCrypt::ClearSettings()
{
	if (!ShowYNDialog("Вы действительно хотите сбросить настройки программы? ")) return;

	//Удаление ини файла
	QString ExePath = QDir::currentPath(); // 
	QString PathToIniFile = ExePath + "/" + Ini_file_name;

	//Проверка существования файла
	QFileInfo file_info(PathToIniFile);
	if (!file_info.exists())
	{
		QMessageBox::information(this, StrCodec->toUnicode("Информация"),
			StrCodec->toUnicode("Настройки программы были сброшены"));
		return;
	}

	//Удаление конфига.
	if (!QFile::remove(PathToIniFile))
	{
		QMessageBox::critical(this, StrCodec->toUnicode("Ошибка"),
			StrCodec->toUnicode("Не удалось удалить файл ") + PathToIniFile);
		return;
	}



	QMessageBox::information(this, StrCodec->toUnicode("Информация"),
		StrCodec->toUnicode("Настройки программы были сброшены"));

}

bool GranitCrypt::ShowPasswordDialog(QString &Password)
{
	//форма ввода пароля
	RequirePswForm pswd_form;

	if (pswd_form.exec() == QDialog::Accepted)
	{
		//Сохраняю пароль
		Password = pswd_form.GetPsw();
		return true;
	}
	return false;
}


void GranitCrypt::SavePaths(uint8_t param, PathSettings S)
{
	QString ExePath = QDir::currentPath();
	QString PathToIniFile = ExePath + "/" + Ini_file_name;

	QSettings settings(PathToIniFile, QSettings::IniFormat);

	//Сохранять путь к секретному ключу.
	if ((param & 0x01) == 0x01)
		settings.setValue("SK_Path", S.GetPathSK());
}
string GranitCrypt::HelloMessage()
{
	string HelloMassage = "Для начала работы щелкните два раза мыши в окне. \r\n или \r\n"
		"В меню \"Файл\" выберите \"Открыть\",выделите файлы требующие обработки.";

	return HelloMassage;
}
//Клик в меню "Справка"
void GranitCrypt::slotMenuHelpClicked(QAction *action)
{
	QString value = action->text();

	if (StrCodec->toUnicode("О программе") == value)
	{
		bool value = ui.actionOpenFile->isEnabled();
		AboutProgramForm form;

		form.setLicensedFlag(value); //Передаю сведения о состоянии лицензии программы.

		//Если вводился правильный серийный номер-вовращается Accepted и программу нужно закрыть.
		if (form.exec() == QDialog::Accepted)
		{
			close();
		}
	}

}
void GranitCrypt::slotMenuSettings(QAction * action)
{
	QString value = action->text();

	if (StrCodec->toUnicode("Сгенерировать ключ") == value)
	{		
		 genKey.show();
				
		return;
	}

	if (StrCodec->toUnicode("Сброс настроек") == value)
	{
		ClearSettings();
		return;
	}

}


//Сохраняю массив в файл по указанному пути.
void GranitCrypt::saveToFile(QByteArray &Ba, QString path)
{
	QFile file(path);
	file.open(QIODevice::WriteOnly);
	file.write(Ba);
	file.close();
}

void GranitCrypt::doOpenFiles()
{
	if (ui.actionOpenFile->isEnabled() == false) return; //Проблема с лицензией.

	if (lockUi)
	{
		string errMess = "В процессе работы программы произошла ошибка. Сначала перезапустите программу.";
		QMessageBox::warning(this, "Info", StrCodec->toUnicode(errMess.c_str()));
		return; //Ошибка при работе программы.
	}


	//Выбор списка файлов подлежащих шифрованию или расшифровыванию		
	ProcessingFiles = QFileDialog::getOpenFileNames(this, tr(""), QDir::currentPath(), tr("Crypt files (*.*);"));//;All files (*.*)
	if (ProcessingFiles.isEmpty()) return;

	FillTextEditFilesName(ProcessingFiles);
	ui.pushButtonCrypt->setEnabled(1);
	ui.pushButtonDecrypt->setEnabled(1);
}

bool GranitCrypt::maxBadAttempPswd()
{
	if (badAttempPswd >= 2)
	{
		string mess = "Вы три раза ввели не верный пароль. Проверьте правильность пароля, и попробуйте через  5 минут.\r\n"
			"Программа будет закрыта.";
		QMessageBox::critical(this, StrCodec->toUnicode("Ошибка"), StrCodec->toUnicode(mess.c_str()));
		return true;
	}

	badAttempPswd++;

	return false;
}


// Нажатие на кнопку зашифровать
void GranitCrypt::slotpushButtonCrypt()
{
	//Загружает настройки из ini файла
	PathSettings S; //Настройки путей.
	LoadSettings(S);
	Ks.setRoseMode(); //Модифицированная версия криптосистемы.

	//Проверяет настроены ли пути к ск и сертификату.
	if (S.GetPathSK().isEmpty() == 1)
	{
		if (!SelectPathSK(S))			
			return; //Выбор пути к секретному ключу и его сертификату.
		SavePaths(3, S);//Сохраняю пути к ключу и сертификату.
	}


	//Загружаю контейнер секретного ключа,если он не был загружен
	if (Ks.getKeyContainerStatus() == 0)
		if (!Ks.LoadKey(S.GetPathSK()))
		{
			QMessageBox::critical(this, StrCodec->toUnicode("Ошибка"), StrCodec->toUnicode(Ks.getLastError().c_str()));
			return;
		}

	//Получаю пароль если не получен.
	QString Password;
	if (UserPassword.isEmpty())
	{
		if (!ShowPasswordDialog(Password)) //Отображаю диалог
			return; //Пароль не получен
	}


	//Проверка правильности введенного пароля
	if (Param.PwdIsChecked == 0)
	{
		if (!Ks.CheckPassword(Password))
		{
			QMessageBox::critical(this, StrCodec->toUnicode("Ошибка"), StrCodec->toUnicode(Ks.getLastError().c_str()));
			UserPassword = ""; //Очистка пароля
			if (maxBadAttempPswd()) close();
			return;
		}
	}
		
	Param.PwdIsChecked = 1; //Пароль для секретного ключа успешно проверен
		

	//Шифрую  все файлы
	CryptFiles(ProcessingFiles, Password);
}

void GranitCrypt::slotpushButtonDecrypt()
{
	//Загружает настройки из ini файла
	PathSettings S; //Настройки путей.
	LoadSettings(S);

	Ks.setRoseMode(); //Модифицированная версия криптосистемы.

	//Проверяет настроены ли пути к ск и сертификату.
	if (S.GetPathSK().isEmpty() == 1)
	{
		if (!SelectPathSK(S)) return; //Выбор пути к секретному ключу и его сертификату.
		SavePaths(3, S);//Сохраняю пути к ключу и сертификату.
	}


	//Загружаю контейнер секретного ключа,если он не был загружен
	if (Ks.getKeyContainerStatus() == 0)
		if (!Ks.LoadKey(S.GetPathSK()))
		{
			QMessageBox::critical(this, StrCodec->toUnicode("Ошибка"), StrCodec->toUnicode(Ks.getLastError().c_str()));
			return;
		}

	//Получаю пароль если не получен.
	QString Password;//Пароль
	if (UserPassword.isEmpty())
	{
		if (!ShowPasswordDialog(Password)) //Отображаю диалог
			return; //Пароль не получен
	}


	//Проверка правильности введенного пароля
	if (Param.PwdIsChecked == 0)
	{
		if (!Ks.CheckPassword(Password))
		{
			QMessageBox::critical(this, StrCodec->toUnicode("Ошибка"), StrCodec->toUnicode(Ks.getLastError().c_str()));
			UserPassword = ""; //Очистка пароля
			if (maxBadAttempPswd()) close();
			return;
		}
	}

	Param.PwdIsChecked = 1; //Пароль для секреного ключа успешно проверен
	UserPassword = Password;

	CipherWorkerData cgDate; //Данные для механизма дешифрования. 
	cgDate.setProcessFiles(ProcessingFiles);
	cgDate.setPassword(Password);
	cgDate.setKeyContainer(Ks.getContainer());
	cgDate.setDbPath("");
	cgDate.setProcessMode(2);//Режим работы-дешифрование файлов.
	cgDate.setRoseMode(true);//Включаю режим упрощенной работы.


	int f_count = ProcessingFiles.count(); //Количество файлов
	allCurProcessFiles = f_count;

	typeProcessWorking = 2;
	initchW(); //Инициализация объекта шифрования.	
	chW->setData(cgDate);
	chW->start();//Запускаю поток.	
}

void GranitCrypt::slotpushButtonSelectKey()
{
	slotpushButtonCrypt();
}



//Заполняет текстовое поле именами файлов
QString GranitCrypt::FillTextEditFilesName(QStringList &paths_to_file)
{
	try
	{
		QTextCharFormat tf;
		ui.plainTextEditFiles->clear();
		//Раскрашиваю выбранные файлы
		for (int i = 0;i < paths_to_file.count();i++)
		{
			tf = ui.plainTextEditFiles->currentCharFormat();
			tf.setForeground(QBrush(QColor("#000000")));
			ui.plainTextEditFiles->setCurrentCharFormat(tf);
			ui.plainTextEditFiles->appendPlainText(paths_to_file[i]);
		}
	}
	catch (...)
	{
		return StrCodec->toUnicode("В методе FillTextEditFilesName() возникло исключение");
	}
	return "";
}

//Делает видимыми компоненты, используемые для визуального отображения процесса шифрования файлов
void GranitCrypt::showWorkComponent(int f_count)
{
	//Масштабирую форму
	ui.plainTextEditFiles->resize(ui.plainTextEditFiles->width(), 420);

	//Делаю видимыми компоненты 

	ui.progressBarAll->setVisible(1);
	ui.progressBarFile->setVisible(1);

	ui.labelProgressFile->clear();
	ui.labelProgressAll->clear();

	ui.labelProgressFile->setVisible(1);
	ui.labelProgressAll->setVisible(1);

	ui.progressBarAll->setValue(0);
	ui.progressBarAll->setMaximum(f_count);
	ui.progressBarFile->setValue(0);

}

//Пишет сообщение в PlainTextEdit об успешности, меняет progress bar 
void GranitCrypt::writeEndMessage()
{
	//Изменяю надписи
	ui.labelProgressFile->setText(StrCodec->toUnicode("Все файлы обработаны"));
	//Изменяю прогресс бар
	ui.progressBarFile->setMaximum(10);
	ui.progressBarFile->setValue(10);

	QTextCharFormat tf; //Изменение стиля
	QTextEdit::ExtraSelection selection;
	QTextCursor cursor = QTextCursor(ui.plainTextEditFiles->document());

	cursor = QTextCursor(ui.plainTextEditFiles->document());
	cursor.movePosition(QTextCursor::Start);
	cursor.select(QTextCursor::LineUnderCursor);
	tf.setForeground(QBrush(QColor("#000000")));

	selection.cursor = cursor;
	selection.format = tf;
	ui.plainTextEditFiles->setExtraSelections(QList<QTextEdit::ExtraSelection>() << selection);

	tf = ui.plainTextEditFiles->currentCharFormat();
	tf.setForeground(QBrush(QColor("#239B56")));
	ui.plainTextEditFiles->setCurrentCharFormat(tf);

	string endMsg = "Не известная операция. В методе не указан код.";
	if (typeProcessWorking == 1)endMsg = "*** Шифрование завершено успешно ***";
	if (typeProcessWorking == 2)endMsg = "*** Расшифровка завершена успешно ***";

	ui.plainTextEditFiles->appendPlainText(StrCodec->toUnicode(endMsg.c_str()));
	//Устанавливаю курсор в конец
	int end_l = ui.plainTextEditFiles->document()->lineCount();
	cursor = QTextCursor(ui.plainTextEditFiles->document()->findBlockByLineNumber(end_l - 1));
	ui.plainTextEditFiles->setTextCursor(cursor); //Двигаю курсор
	//Скрываю label 
	ui.labelProgressAll->setText(StrCodec->toUnicode("Обработка файлов завершена"));
}

void GranitCrypt::paintShowError(string msg)
{
	/*
	 Блокирую компоненты. Блокировка нужна для очистки(когда пользователь перезапустит программу) объекта обработки и данных программы.
	 Без неё нужно писать методы очистки данных программы, иначе повторное нажатие на кнопки
	 приведет к не корректной отправки данных, что приведет к ошибке!
	*/
	ui.pushButtonDecrypt->setEnabled(false);
	ui.pushButtonCrypt->setEnabled(false);
	lockUi = true; //Сообщаю об ошибке в программе.

	QTextCharFormat tf; //Изменение стиля
	QTextEdit::ExtraSelection selection;
	QTextCursor cursor = QTextCursor(ui.plainTextEditFiles->document());

	cursor = QTextCursor(ui.plainTextEditFiles->document());
	cursor.movePosition(QTextCursor::Start);
	cursor.select(QTextCursor::LineUnderCursor);
	tf.setForeground(QBrush(QColor("#000000")));

	selection.cursor = cursor;
	selection.format = tf;
	ui.plainTextEditFiles->setExtraSelections(QList<QTextEdit::ExtraSelection>() << selection);

	tf = ui.plainTextEditFiles->currentCharFormat();
	tf.setForeground(QBrush(QColor(Qt::red)));
	ui.plainTextEditFiles->setCurrentCharFormat(tf);
	string str = "Ошибка: " + msg + "\r\n Закройте программу и попробуйте еще раз.";
	ui.plainTextEditFiles->appendPlainText(StrCodec->toUnicode(str.c_str()));
	//Устанавливаю курсор в конец
	int end_l = ui.plainTextEditFiles->document()->lineCount();
	cursor = QTextCursor(ui.plainTextEditFiles->document()->findBlockByLineNumber(end_l - 1));
	ui.plainTextEditFiles->setTextCursor(cursor); //Двигаю курсор


}


void GranitCrypt::coloringTextLine(int &line_pos, QTextCharFormat &tf, QTextEdit::ExtraSelection &selection, QTextCursor &cursor)
{
	//Раскраска строки-меня цвет обрабатываемого файла
	cursor = QTextCursor(ui.plainTextEditFiles->document()->findBlockByLineNumber(line_pos));
	ui.plainTextEditFiles->setTextCursor(cursor); //Двигаю курсор
	cursor.select(QTextCursor::LineUnderCursor);
	tf.setForeground(QBrush(QColor("#D35400")));
	selection.cursor = cursor;
	selection.format = tf;
	ui.plainTextEditFiles->setExtraSelections(QList<QTextEdit::ExtraSelection>() << selection);
}


void GranitCrypt::CryptFiles(QStringList &paths_to_file, QString Password)
{
	string Dh_OpenKey = Ks.getOpenDHkey();//Получаю открытый ключ схемы Диффи-Хэлмана.

	CipherWorkerData cgDate; //Данные для механизма шифрования. 
	cgDate.setRoseMode(true); //Включает альтернативный режим шифрования.
	cgDate.setProcessFiles(paths_to_file);
	cgDate.setPassword(Password);
	cgDate.setRAsimOpenKey(Dh_OpenKey);
	cgDate.setKeyContainer(Ks.getContainer());
	cgDate.setSignerIndex("003");//Не используется.
	cgDate.setSignatureDH("abc");
	cgDate.setProcessMode(1);//Режим работы-шифрование файлов.

	int f_count = paths_to_file.count(); //Количество файлов
	allCurProcessFiles = f_count;

	initchW(); //Инициализация объекта шифрования.
	chW->setData(cgDate);
	chW->start();//Запускаю поток.
	typeProcessWorking = 1;
}



void GranitCrypt::paintProcessFile(int ix)
{
	//Раскраска строк		
	QTextCharFormat tf; //Изменение стиля
	QTextEdit::ExtraSelection selection;
	QTextCursor cursor;

	//Раскраска строки с названием обрабатываемого файла.
	coloringTextLine(ix, tf, selection, cursor);

}


//Из базы ключей получает хэшированное значение открытого ключа получателя 
bool GranitCrypt::GetrHashOkDH(GranitDbI &DBI)
{
	string  rHashOkDH;
	string  Dh_OpenKey;
	DBI.getDHOpenKey(rHashOkDH, Dh_OpenKey, Param.ReceiverId); //Получаю открытый ключ ДХ и его CRC.

	Param.rHashOkDH = rHashOkDH;
	//значение открытого ключа ДХ
	Param.Dh_OpenKey = Dh_OpenKey;

	return true;
}

//Проверяет существование файла по указанному пути.
bool GranitCrypt::HasFile(QString path)
{
	QFileInfo zfile(path);
	//Проверяю существует ли файл. Если существует проверяю-это файл а не директория.
	if (zfile.exists() && zfile.isFile())
		return true;
	else
		return false;
}

QString GranitCrypt::toUtf(string str)
{
	QString x = StrCodec->toUnicode(str.c_str());
	return x;
}

//Выводит окно с предупреждением.
void GranitCrypt::ShowWarning(string str)
{
	QMessageBox::warning(this, toUtf("Внимание"), toUtf(str));
}

//Выводит окно с предупреждением.
void GranitCrypt::ShowError(string str)
{
	QMessageBox::critical(this, toUtf("Ошибка"), toUtf(str));
}

bool GranitCrypt::ShowYNDialog(string str)
{
	QString WarnMess = StrCodec->toUnicode(str.c_str());

	//Спрашиваю пользователя он точно хочет удалить запись
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(StrCodec->toUnicode("Предупреждение"));
	msgBox.setText(WarnMess);
	msgBox.setStandardButtons(QMessageBox::Yes);
	msgBox.addButton(QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
	msgBox.setIcon(QMessageBox::Question);
	//Пользователь отказался удалять
	if (msgBox.exec() == QMessageBox::No) return false;
	return true;
}
