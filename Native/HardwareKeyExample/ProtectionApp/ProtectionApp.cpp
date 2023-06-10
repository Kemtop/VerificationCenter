#include "stdafx.h"
#include "ProtectionApp.h"

ProtectionApp::ProtectionApp(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	StrCodec = QTextCodec::codecForName("Windows-1251"); //Установка кодировки
	
	QLabel* m_plblX; //Надпись в низу окна.
	m_plblX = new QLabel(this);
	m_plblX->setText(getDataInfo());
	ui.statusBar->addWidget(m_plblX);
	
	//Нажатие в пункте меню Файл->Выход
	connect(ui.actionExit, SIGNAL(triggered(bool)), this, SLOT(slotActionExit()));
}

ProtectionApp::~ProtectionApp()
{
}

void ProtectionApp::setCurentUser(QString login)
{
	CurentUser = login;
}

void ProtectionApp::setDbSrvHostName(QString host)
{
	DbSrvHostName = host;
}

void ProtectionApp::setDbSrvPort(int port)
{
	dbSrvPort = port;
}

QString ProtectionApp::getDataInfo()
{
	QDate d = QDate::currentDate();

	QString dayName = d.toString("dddd");
	dayName = dayName[0].toUpper() + dayName.mid(1, dayName.size() - 1);
	QString x = " " + dayName + " " + d.toString("dd.MM.yyyy");

	return x;
}

void ProtectionApp::showEvent(QShowEvent *ev)
{
	QWidget::showEvent(ev);
	emit sigShowEvent();
}

//Нажатие на пункт меню Файл->Выход
void ProtectionApp::slotActionExit()
{
	close();
}
