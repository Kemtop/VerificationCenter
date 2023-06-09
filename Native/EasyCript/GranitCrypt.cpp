#include "GranitCrypt.h"
#include "RequirePswForm.h"


GranitCrypt::GranitCrypt(QWidget *parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);
	lockUi = false; //���� ���������� ��������� ��� ������.

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

	//����� �������� ���������� � ����������� ����� ���� 
	ui.progressBarAll->setVisible(0);
	ui.progressBarFile->setVisible(0);
	ui.labelProgressFile->setVisible(0);
	ui.labelProgressAll->setVisible(0);
	ui.plainTextEditFiles->resize(ui.plainTextEditFiles->width(), 540);

	StrCodec = QTextCodec::codecForName("Windows-1251"); //��������� ���������

	ui.pushButtonCrypt->setEnabled(0);
	ui.pushButtonDecrypt->setEnabled(0);

	UserPassword = ""; //������ ���������� ��� �������� ������
	typeProcessWorking = 0; //��� �� ������ ������ ������.


	Param.PwdIsChecked = 0; //������ ��� ���������� ����� �� ��� ��������
	Param.ReceiverId = -1; //�� ���������� �� �������
	ui.pushButtonSelectKey->setVisible(0); //��� �����!
	//����������� ������� �������.
	ui.plainTextEditFiles->viewport()->installEventFilter(this);
		
	string helloMassage = HelloMessage();

	QTextCharFormat tf;
	tf = ui.plainTextEditFiles->currentCharFormat();
	tf.setForeground(QBrush(QColor("#008000")));
	ui.plainTextEditFiles->setCurrentCharFormat(tf);
	ui.plainTextEditFiles->appendPlainText(StrCodec->toUnicode(helloMassage.c_str()));
		
	badAttempPswd = 0;
	hasThrObj = false; //������� �� ����������.
}

GranitCrypt::~GranitCrypt()
{
	deletechW();
}

void GranitCrypt::customEvent(QEvent * pe)
{
	/*
	  ��������� ��� ����������� �������,
	  � ���� �� ������������� ���� ������ �������,�� �� �������� ��� � ������ �������.
	*/
	if ((int)pe->type() == eventsCipherWorker::eventCipherWorkerType)
	{
		eventsCipherWorker *ev;
		ev = (eventsCipherWorker*)pe;

		int curAll = ev->getAllProcessVal(); //������� �������� ���� �������������� ������.
		int maxCurFile = ev->getMaxFileProcess(); //������������ �������� �������������� �������� � �����.
		int curFileProgress = ev->getCurFileProcess();//������� ��������.
		int thredStatus = ev->getThreadStatus();
		bool needShowPFi = ev->getNeedshowcurfileprocessInfo(); //����� ������� ��������� � �������� ���������� �����.


		//������� �� ��������� ������ ���������� �������� ��������� ������.
		if (curAll > 0)
		{
			string mess_progress = "";
			//�������� ��������� ������.
			mess_progress = " ���� " + std::to_string(curAll + 1) + " �� " + std::to_string(allCurProcessFiles);
			ui.labelProgressAll->setText(StrCodec->toUnicode(mess_progress.c_str()));

			ui.progressBarAll->setValue(curAll); //������ �������� ���.
			paintProcessFile(curAll);
		}

		//������� ��������� ������������� ��������.
		if (maxCurFile > 0)
		{
			ui.progressBarFile->setValue(0);
			ui.progressBarFile->setMaximum(maxCurFile);
		}

		//������� �������� ��������.
		if (curFileProgress > -1)
		{

			ui.progressBarFile->setValue(curFileProgress);//������ �������� ���
		}

		if (needShowPFi)
		{
			string mess = ev->getCurfileprocessInfo();
			ui.labelProgressFile->setText(StrCodec->toUnicode(mess.c_str()));
		}

		//C������ � ��������� ������.
		if (thredStatus > 0)
		{
			 
			string errMess = "";
			string mess_progress1 = "";
			switch (thredStatus)
			{
				//����� �������.
			case 1:
				//������ �������� ����������, ������������ ��� ����������� ����������� �������� ���������� ������
				showWorkComponent(allCurProcessFiles);
				paintProcessFile(0);//���������� ���������� � ������� �����.
				//�������� ��������� ������.
				mess_progress1 = " ���� " + std::to_string(1) + " �� " + std::to_string(allCurProcessFiles);
				ui.labelProgressAll->setText(StrCodec->toUnicode(mess_progress1.c_str()));
				break;
				//����� ������� ��������.
			case 2:
				ui.progressBarAll->setValue(ui.progressBarAll->maximum()); //������������ ������������ ��������.
				//����� ��������� � PlainTextEdit �� ����������, ������ progress bar
				writeEndMessage();
				

				break;
				//�������� ������.
			case 3:
				errMess = ev->getLastError(); //������� ������.
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
	if (hasThrObj) delete chW; //������ ��� ������ ������ ��� ������� ������.
	chW = NULL;

	chW = new CipherWorker(this);
	hasThrObj = true;
}

void GranitCrypt::deletechW()
{
	if (hasThrObj) delete chW; //������ ��� ������ ������.
	hasThrObj = false;
	chW = NULL;
}


bool GranitCrypt::eventFilter(QObject * target, QEvent * event)
{
	//�������� ������� �� ������� ��� ����������.
	if ((target == ui.plainTextEditFiles || target == ui.plainTextEditFiles->viewport()) &&
		event->type() == QEvent::MouseButtonDblClick)
	{
		//��������� �������� ������ ������.
		doOpenFiles();

		return true; //����� �������� ��������� ���������� �����.
	}

	return QWidget::eventFilter(target, event);
}


//���� � ���� ����
void GranitCrypt::slotMenuFileClicked(QAction *action)
{
	QString value = action->text();

	if (StrCodec->toUnicode("�����") == value)
	{
		close();
	}

	//
	if (StrCodec->toUnicode("�����") == value)
	{
		close();
	}



	if (StrCodec->toUnicode("�������") == value)
	{
		doOpenFiles(); //�������� �������� �� ������ ������.
	}

}

bool GranitCrypt::LoadSettings(PathSettings &S)
{
	QString ExePath = QDir::currentPath(); // /
	QString PathToIniFile = ExePath + "/" + Ini_file_name;

	//�������� ������������� �����
	QFileInfo file_info(PathToIniFile);
	if (!file_info.exists()) return 0;

	QSettings settings(PathToIniFile, QSettings::IniFormat);

	QString PathSK = settings.value("SK_Path", "").toString(); //Db_Path
		
	S.SetPathSK(PathSK);


	return true;
}

//������� ������ ������ ���������� �����. ���� ������������ ���������,��� �� ������� �����-���������� false.
bool GranitCrypt::SelectPathSK(PathSettings &S)
{
	//����� ������ ���������� �����
	SelectKeyForm form_secret_key;
	
	if (form_secret_key.exec() == QDialog::Accepted)
	{
		QString SK_path = form_secret_key.KeyPath; //���� � ���������� �����

		typedef unsigned char(*CheckUserKey)(char *, char *);
		CheckUserKey checkUserKey = (CheckUserKey)GetProcAddress(hChipperDll, "CheckUserKey");
		
		char* pathToKey = SK_path.toLocal8Bit().data();
		char *error = (char*)malloc(1000);

	   //�������� ����� ��
		if (checkUserKey(pathToKey, error) == 0)
		{			
			QMessageBox::critical(this, StrCodec->toUnicode("������"), StrCodec->toUnicode(error));
			free(error);
			return false;
		}
		
		free(error);
		S.SetPathSK(SK_path);

		return true;
	}

	return false;
}


void GranitCrypt::ClearSettings()
{
	if (!ShowYNDialog("�� ������������� ������ �������� ��������� ���������? ")) return;

	//�������� ��� �����
	QString ExePath = QDir::currentPath(); // 
	QString PathToIniFile = ExePath + "/" + Ini_file_name;

	//�������� ������������� �����
	QFileInfo file_info(PathToIniFile);
	if (!file_info.exists())
	{
		QMessageBox::information(this, StrCodec->toUnicode("����������"),
			StrCodec->toUnicode("��������� ��������� ���� ��������"));
		return;
	}

	//�������� �������.
	if (!QFile::remove(PathToIniFile))
	{
		QMessageBox::critical(this, StrCodec->toUnicode("������"),
			StrCodec->toUnicode("�� ������� ������� ���� ") + PathToIniFile);
		return;
	}



	QMessageBox::information(this, StrCodec->toUnicode("����������"),
		StrCodec->toUnicode("��������� ��������� ���� ��������"));

}

bool GranitCrypt::ShowPasswordDialog(QString &Password)
{
	//����� ����� ������
	RequirePswForm pswd_form;

	if (pswd_form.exec() == QDialog::Accepted)
	{
		//�������� ������
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

	//��������� ���� � ���������� �����.
	if ((param & 0x01) == 0x01)
		settings.setValue("SK_Path", S.GetPathSK());
}
string GranitCrypt::HelloMessage()
{
	string HelloMassage = "��� ������ ������ �������� ��� ���� ���� � ����. \r\n ��� \r\n"
		"� ���� \"����\" �������� \"�������\",�������� ����� ��������� ���������.";

	return HelloMassage;
}
//���� � ���� "�������"
void GranitCrypt::slotMenuHelpClicked(QAction *action)
{
	QString value = action->text();

	if (StrCodec->toUnicode("� ���������") == value)
	{
		bool value = ui.actionOpenFile->isEnabled();
		AboutProgramForm form;

		form.setLicensedFlag(value); //������� �������� � ��������� �������� ���������.

		//���� �������� ���������� �������� �����-����������� Accepted � ��������� ����� �������.
		if (form.exec() == QDialog::Accepted)
		{
			close();
		}
	}

}
void GranitCrypt::slotMenuSettings(QAction * action)
{
	QString value = action->text();

	if (StrCodec->toUnicode("������������� ����") == value)
	{		
		 genKey.show();
				
		return;
	}

	if (StrCodec->toUnicode("����� ��������") == value)
	{
		ClearSettings();
		return;
	}

}


//�������� ������ � ���� �� ���������� ����.
void GranitCrypt::saveToFile(QByteArray &Ba, QString path)
{
	QFile file(path);
	file.open(QIODevice::WriteOnly);
	file.write(Ba);
	file.close();
}

void GranitCrypt::doOpenFiles()
{
	if (ui.actionOpenFile->isEnabled() == false) return; //�������� � ���������.

	if (lockUi)
	{
		string errMess = "� �������� ������ ��������� ��������� ������. ������� ������������� ���������.";
		QMessageBox::warning(this, "Info", StrCodec->toUnicode(errMess.c_str()));
		return; //������ ��� ������ ���������.
	}


	//����� ������ ������ ���������� ���������� ��� ���������������		
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
		string mess = "�� ��� ���� ����� �� ������ ������. ��������� ������������ ������, � ���������� �����  5 �����.\r\n"
			"��������� ����� �������.";
		QMessageBox::critical(this, StrCodec->toUnicode("������"), StrCodec->toUnicode(mess.c_str()));
		return true;
	}

	badAttempPswd++;

	return false;
}

// ������� �� ������ �����������
void GranitCrypt::slotpushButtonCrypt()
{
	QString password;
	if (!InitSecretKey(password)) return;
		
	//������  ��� �����
	CryptFiles(ProcessingFiles, password, 1);

	FreeChipperDll();
}

void GranitCrypt::slotpushButtonDecrypt()
{
	QString password;
	if (!InitSecretKey(password)) return;

	CryptFiles(ProcessingFiles, password, 2);

	FreeChipperDll();		
}

void GranitCrypt::slotpushButtonSelectKey()
{
	slotpushButtonCrypt();
}



//��������� ��������� ���� ������� ������
QString GranitCrypt::FillTextEditFilesName(QStringList &paths_to_file)
{
	try
	{
		QTextCharFormat tf;
		ui.plainTextEditFiles->clear();
		//����������� ��������� �����
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
		return StrCodec->toUnicode("� ������ FillTextEditFilesName() �������� ����������");
	}
	return "";
}

//������ �������� ����������, ������������ ��� ����������� ����������� �������� ���������� ������
void GranitCrypt::showWorkComponent(int f_count)
{
	//����������� �����
	ui.plainTextEditFiles->resize(ui.plainTextEditFiles->width(), 420);

	//����� �������� ���������� 

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

//����� ��������� � PlainTextEdit �� ����������, ������ progress bar 
void GranitCrypt::writeEndMessage()
{
	//������� �������
	ui.labelProgressFile->setText(StrCodec->toUnicode("��� ����� ����������"));
	//������� �������� ���
	ui.progressBarFile->setMaximum(10);
	ui.progressBarFile->setValue(10);

	QTextCharFormat tf; //��������� �����
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

	string endMsg = "�� ��������� ��������. � ������ �� ������ ���.";
	if (typeProcessWorking == 1)endMsg = "*** ���������� ��������� ������� ***";
	if (typeProcessWorking == 2)endMsg = "*** ����������� ��������� ������� ***";

	ui.plainTextEditFiles->appendPlainText(StrCodec->toUnicode(endMsg.c_str()));
	//������������ ������ � �����
	int end_l = ui.plainTextEditFiles->document()->lineCount();
	cursor = QTextCursor(ui.plainTextEditFiles->document()->findBlockByLineNumber(end_l - 1));
	ui.plainTextEditFiles->setTextCursor(cursor); //������ ������
	//������� label 
	ui.labelProgressAll->setText(StrCodec->toUnicode("��������� ������ ���������"));
}

void GranitCrypt::paintShowError(string msg)
{
	/*
	 �������� ����������. ���������� ����� ��� �������(����� ������������ ������������ ���������) ������� ��������� � ������ ���������.
	 ��� �� ����� ������ ������ ������� ������ ���������, ����� ��������� ������� �� ������
	 �������� � �� ���������� �������� ������, ��� �������� � ������!
	*/
	ui.pushButtonDecrypt->setEnabled(false);
	ui.pushButtonCrypt->setEnabled(false);
	lockUi = true; //������� �� ������ � ���������.

	QTextCharFormat tf; //��������� �����
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
	string str = "������: " + msg + "\r\n �������� ��������� � ���������� ��� ���.";
	ui.plainTextEditFiles->appendPlainText(StrCodec->toUnicode(str.c_str()));
	//������������ ������ � �����
	int end_l = ui.plainTextEditFiles->document()->lineCount();
	cursor = QTextCursor(ui.plainTextEditFiles->document()->findBlockByLineNumber(end_l - 1));
	ui.plainTextEditFiles->setTextCursor(cursor); //������ ������


}


void GranitCrypt::coloringTextLine(int &line_pos, QTextCharFormat &tf, QTextEdit::ExtraSelection &selection, QTextCursor &cursor)
{
	//��������� ������-���� ���� ��������������� �����
	cursor = QTextCursor(ui.plainTextEditFiles->document()->findBlockByLineNumber(line_pos));
	ui.plainTextEditFiles->setTextCursor(cursor); //������ ������
	cursor.select(QTextCursor::LineUnderCursor);
	tf.setForeground(QBrush(QColor("#D35400")));
	selection.cursor = cursor;
	selection.format = tf;
	ui.plainTextEditFiles->setExtraSelections(QList<QTextEdit::ExtraSelection>() << selection);
}

void GranitCrypt::CryptFiles(QStringList &paths_to_file, QString Password, int mode)
{
	typedef string(*GetDhOpenKey)();
	typedef PkContainer(*GetKeyContainer)();

	GetDhOpenKey getDhOpenKey = (GetDhOpenKey)GetProcAddress(hChipperDll, "GetDhOpenKey");
	GetKeyContainer keyContainer = (GetKeyContainer)GetProcAddress(hChipperDll, "GetKeyContainer");

	int f_count = paths_to_file.count(); //���������� ������
	allCurProcessFiles = f_count;
	typeProcessWorking = mode;

	initchW(); //������������� ������� ����������.
	   
	chW->setRAsimOpenKey(getDhOpenKey());
	chW->setKeyContainer(keyContainer());
	chW->setProcessMode(mode);//����� ������-���������� ������.
	chW->setPassword(Password);
	chW->setProcessFiles(paths_to_file);
	chW->setSignerIndex("003");//�� ������������.
	chW->setSignatureDH("abc");
	chW->setDbPath("");
	chW->setRoseMode(true);
		
	chW->start();//�������� �����.
}

void GranitCrypt::paintProcessFile(int ix)
{
	//��������� �����		
	QTextCharFormat tf; //��������� �����
	QTextEdit::ExtraSelection selection;
	QTextCursor cursor;

	//��������� ������ � ��������� ��������������� �����.
	coloringTextLine(ix, tf, selection, cursor);

}


//�� ���� ������ �������� ������������ �������� ��������� ����� ���������� 
bool GranitCrypt::GetrHashOkDH(GranitDbI &DBI)
{
	string  rHashOkDH;
	string  Dh_OpenKey;
	DBI.getDHOpenKey(rHashOkDH, Dh_OpenKey, Param.ReceiverId); //������� �������� ���� �� � ��� CRC.

	Param.rHashOkDH = rHashOkDH;
	//�������� ��������� ����� ��
	Param.Dh_OpenKey = Dh_OpenKey;

	return true;
}

//��������� ������������� ����� �� ���������� ����.
bool GranitCrypt::HasFile(QString path)
{
	QFileInfo zfile(path);
	//�������� ���������� �� ����. ���� ���������� ��������-��� ���� � �� ����������.
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

//������� ���� � ���������������.
void GranitCrypt::ShowWarning(string str)
{
	QMessageBox::warning(this, toUtf("��������"), toUtf(str));
}

//������� ���� � ���������������.
void GranitCrypt::ShowError(string str)
{
	QMessageBox::critical(this, toUtf("������"), toUtf(str));
}

bool GranitCrypt::ShowYNDialog(string str)
{
	QString WarnMess = StrCodec->toUnicode(str.c_str());

	//��������� ������������ �� ����� ����� ������� ������
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(StrCodec->toUnicode("��������������"));
	msgBox.setText(WarnMess);
	msgBox.setStandardButtons(QMessageBox::Yes);
	msgBox.addButton(QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
	msgBox.setIcon(QMessageBox::Question);
	//������������ ��������� �������
	if (msgBox.exec() == QMessageBox::No) return false;
	return true;
}

//��������� �������� ���������.
//��� ����������� �� �� ����������� ���-������� ����� �� �����.
bool GranitCrypt::InitSecretKey(QString &password)
{
	//��������� ��������� �� ini �����
	PathSettings S; //��������� �����.
	LoadSettings(S);

	//�������� ���������� ���������� ��� �������.
	if (!LoadChipperDll()) return false;

	//��������� ��������� �� ���� � �� � �����������.
	if (S.GetPathSK().isEmpty() == 1)
	{
		if (!SelectPathSK(S))
			return false; //����� ���� � ���������� ����� � ��� �����������.
		SavePaths(3, S);//�������� ���� � ����� � �����������.
	}

	typedef unsigned char(*LoadSecretKey)(char *lastError, char * pathToKey);
	typedef unsigned char(*CheckPasswordForSecretKey)(char*, char*);
	typedef unsigned char(*GetKeyContainerStatus)();

	//�������� ��������� ���������� �����,���� �� �� ��� ��������
	GetKeyContainerStatus containerStatus = (GetKeyContainerStatus)GetProcAddress(hChipperDll, "GetKeyContainerStatus");
	LoadSecretKey loadKey = (LoadSecretKey)GetProcAddress(hChipperDll, "LoadSecretKey");

	if (containerStatus() == 0)
	{
		char* pathToKey = S.GetPathSK().toLocal8Bit().data();
		char *error = (char*)malloc(2000);

		if (!loadKey(error, pathToKey))
		{			
			ui.plainTextEditFiles->appendPlainText(StrCodec->toUnicode(error));
			free(error);
			return false;
		}

		free(error);		
	}

	//������� ������ ���� �� �������.
	if (UserPassword.isEmpty())
	{
		if (!ShowPasswordDialog(password)) //��������� ������
			return false; //������ �� �������
	}
	
	//�������� ������������ ���������� ������
	if (Param.PwdIsChecked == 0)
	{
		CheckPasswordForSecretKey checkPassword = (CheckPasswordForSecretKey)GetProcAddress(hChipperDll, "CheckPasswordForSecretKey");

		char *password_ = password.toLocal8Bit().data();
		char *error = (char*)malloc(2000);

		if (checkPassword(password_, error) == 0)
		{
			QMessageBox::critical(this, StrCodec->toUnicode("������"), StrCodec->toUnicode(error));
			free(error);
			UserPassword = ""; //������� ������
			if (maxBadAttempPswd()) close();
			return false;
		}

		free(error);
	}

	Param.PwdIsChecked = 1; //������ ��� ���������� ����� ������� ��������
	UserPassword = password;
	return true;
}

//��������� dll ���������� ����������
bool GranitCrypt::LoadChipperDll()
{
	hChipperDll = LoadLibraryA("NativeDll.dll");
	if (hChipperDll == 0)
	{
		ui.plainTextEditFiles->appendPlainText("NativeDll.dll not found!");
		return false;
	}
    
	//������ ��� ������� � ��� �����.
	typedef unsigned char(*SetSingleModeChipper)();
	SetSingleModeChipper chipperMode = (SetSingleModeChipper)GetProcAddress(hChipperDll, "SetSingleModeChipper");
	unsigned char result = chipperMode(); //����� ������ ������� ����������.
	if (result == 0)
	{
		ui.plainTextEditFiles->appendPlainText("Can't set cipher mode!");
		return false;
	}

	return true;
}

//��������� dll ���������� ����������
void GranitCrypt::FreeChipperDll() 
{
	if (hChipperDll == 0)
		FreeLibrary(hChipperDll);
}