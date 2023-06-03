#pragma once

#include <QObject>
#include <QDir>
#include <QSettings>

#define settingFileName "settings.ini"


//Инструмент для сохранения настроек в  .ini файл.
class SettingTools 
{

private:


public:
	SettingTools();
	~SettingTools();

	QString OrgName; //Наименование оргнизации.
	QString OrgCode;
	QString Podrazdelenie;
	QString Rayon;
	QString Gorod;
	QString Adress;
	QString mail; 
	
	//Проверяет наличие файла настроек. Есть файл=true.
	bool hasSetingFile();

	//Читает настройки. Возвращает false если нет файла настроек.
	bool loadSettings();

	//Сохраняет настройки.Кроме последнего номера заявки.
	void saveSettings();

	//Сохраняет последний номер заявки на получение сертификата.  
	void savelastStatementNum();
};
