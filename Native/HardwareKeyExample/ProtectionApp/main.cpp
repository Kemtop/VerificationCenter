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
	//Cтиль для QMessageBox
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
		

	//Для того что бы кнопки в диалоговых окнах отображались на русском(Да Нет) а не Yes No
	//set translator for qhelpsearchquerywidget
	QTranslator qtHelpTranslator;
	qtHelpTranslator.load("qt_help_ru", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	a.installTranslator(&qtHelpTranslator);

	//set translator for default widget's text (for example: QMessageBox's buttons)
	QTranslator qtTranslator;
	qtTranslator.load("qt_ru", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	a.installTranslator(&qtTranslator);
	

	 //Кодек для преобразования.
	 QTextCodec *StrCodec;
	 StrCodec = QTextCodec::codecForName("Windows-1251"); //Установка кодировки

	 //Форма авторизации.
	LoginForm loginForm;
	loginForm.setWindowIcon(icon);
	loginForm.setWindowTitle(StrCodec->toUnicode("Авторизация в системе."));

	AppConfig sConf;
	sConf.Read(); //Читаю конфигурационный файл если он есть

	//Передаю данный форме аутентификации.
	loginForm.setHostName(sConf.getServerHost());
	loginForm.setPort(sConf.getServerPort());
	loginForm.setLoginName(sConf.getServerLogin());

#ifdef INCLUDE_HW_USB_KEY 
	loginForm.checkUsbKey();
#endif
	
	if (loginForm.exec() != QDialog::Accepted) //Пользователь отказался.
		return 0;
	  
	sConf.setServerHost(loginForm.getHostName()); //Передаю данные из формы.
	sConf.setServerPort(loginForm.getPort());
	sConf.setServerLogin(loginForm.getLoginName());
	sConf.Write(); //Сохраняю настройки.
		
	//Передача данных учетной записи. Может потребоваться при смене пароля для открытия другого соединения.
	w.setDbSrvHostName(loginForm.getHostName());
	w.setDbSrvPort(loginForm.getPort());
	w.setCurentUser(loginForm.getLoginName());

	w.show();
	return a.exec();
}
