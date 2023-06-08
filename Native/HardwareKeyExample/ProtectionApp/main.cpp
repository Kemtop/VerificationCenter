#include "stdafx.h"
#include "ProtectionApp.h"
#include <QtWidgets/QApplication>
#include <LoginForm.h>
#include <AppConfig/AppConfig.h>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
	QIcon icon("Image/app.ico");
	ProtectionApp w;
	w.move(QApplication::desktop()->screen()->rect().center() - w.rect().center());
	w.setWindowIcon(icon);
	//C���� ��� QMessageBox
	w.setStyleSheet(
		"QMessageBox{"
		"font-family:\"Georgia\";"
		"font-size:14pt;"
		"}"
		"QDialog{"
		"font-family:\"Georgia\";"
		"font-size:14pt;"
		"}"
	    
	);
		

	//��� ���� ��� �� ������ � ���������� ����� ������������ �� �������(�� ���) � �� Yes No
	//set translator for qhelpsearchquerywidget
	QTranslator qtHelpTranslator;
	qtHelpTranslator.load("qt_help_ru", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	a.installTranslator(&qtHelpTranslator);

	//set translator for default widget's text (for example: QMessageBox's buttons)
	QTranslator qtTranslator;
	qtTranslator.load("qt_ru", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	a.installTranslator(&qtTranslator);
	

	 //����� ��� ��������������.
	 QTextCodec *StrCodec;
	 StrCodec = QTextCodec::codecForName("Windows-1251"); //��������� ���������

	 //����� �����������.
	LoginForm loginForm;
	loginForm.setWindowIcon(icon);
	loginForm.setWindowTitle(StrCodec->toUnicode("����������� � �������."));

	AppConfig sConf;
	sConf.Read(); //����� ���������������� ���� ���� �� ����

	//������� ������ ����� ��������������.
	loginForm.setHostName(sConf.getServerHost());
	loginForm.setPort(sConf.getServerPort());
	loginForm.setLoginName(sConf.getServerLogin());

#ifdef INCLUDE_HW_USB_KEY 
	loginForm.checkUsbKey();
#endif
	
	if (loginForm.exec() != QDialog::Accepted) //������������ ���������.
		return 0;
	  
	sConf.setServerHost(loginForm.getHostName()); //������� ������ �� �����.
	sConf.setServerPort(loginForm.getPort());
	sConf.setServerLogin(loginForm.getLoginName());
	sConf.Write(); //�������� ���������.
		
	//�������� ������ ������� ������. ����� ������������� ��� ����� ������ ��� �������� ������� ����������.
	w.setDbSrvHostName(loginForm.getHostName());
	w.setDbSrvPort(loginForm.getPort());
	w.setCurentUser(loginForm.getLoginName());

	w.show();
	return a.exec();
}
