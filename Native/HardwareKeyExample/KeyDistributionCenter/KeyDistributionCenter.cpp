#include "stdafx.h"
#include "KeyDistributionCenter.h"

KeyDistributionCenter::KeyDistributionCenter(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//ui.actionChangePswd->setIcon(QIcon("Image//Menu//password.png"));
	//ui.actionCheckCert->setIcon(QIcon("Image//Menu//certdoc.png"));
	//ui.actionGiveSysCert->setIcon(QIcon("Image//Menu//sysCert.png"));
	//ui.actionExit->setIcon(QIcon("Image//Menu//exit.png"));
	//ui.actionAboutProgram->setIcon(QIcon("Image//Menu//about_program.png"));

	StrCodec = QTextCodec::codecForName("Windows-1251"); //��������� ���������

	QMenu *certMenu = new QMenu();
	//StrCodec->toUnicode("��������� ���������� ������������.")
	QString hy = StrCodec->toUnicode("��������� ���������� ������������.");
	QAction *saveUserCertAction = new QAction(hy, this);
	certMenu->addAction(saveUserCertAction);

	ui.toolButtonGiveCert->setMenu(certMenu);


	QLabel* m_plblX; //������� � ���� ����.
	m_plblX = new QLabel(this);
	m_plblX->setText(getDataInfo());
	ui.statusBar->addWidget(m_plblX);
	
	//������� � ������ ���� ����->�����
	connect(ui.actionExit, SIGNAL(triggered(bool)), this, SLOT(slotActionExit()));
}

KeyDistributionCenter::~KeyDistributionCenter()
{
}

void KeyDistributionCenter::setCurentUser(QString login)
{
	CurentUser = login;
}

void KeyDistributionCenter::setDbSrvHostName(QString host)
{
	DbSrvHostName = host;
}

void KeyDistributionCenter::setDbSrvPort(int port)
{
	dbSrvPort = port;
}

QString KeyDistributionCenter::getDataInfo()
{
	QDate d = QDate::currentDate();

	QString dayName = d.toString("dddd");
	dayName = dayName[0].toUpper() + dayName.mid(1, dayName.size() - 1);
	QString x = " " + dayName + " " + d.toString("dd.MM.yyyy");

	return x;
}

void KeyDistributionCenter::showEvent(QShowEvent *ev)
{
	QWidget::showEvent(ev);
	emit sigShowEvent();
}

//������� �� ����� ���� ����->�����
void KeyDistributionCenter::slotActionExit()
{
	close();
}
