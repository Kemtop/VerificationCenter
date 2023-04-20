#include "SettingTools.h"

SettingTools::SettingTools()
{
}

SettingTools::~SettingTools()
{
}


bool SettingTools::hasSetingFile()
{
	QString ExePath = QDir::currentPath(); // /
	QString PathToIniFile = ExePath + "/" + settingFileName;

	//Проверка существования файла
	QFileInfo file_info(PathToIniFile);
	if (!file_info.exists()) return false;

	return true;
}

bool SettingTools::loadSettings()
{
	QString ExePath = QDir::currentPath(); // /
	QString PathToIniFile = ExePath + "/" + settingFileName;

	//Проверка существования файла
	QFileInfo file_info(PathToIniFile);
	if (!file_info.exists()) return false;

	QSettings settings(PathToIniFile, QSettings::IniFormat);
	//Считываю параметры
	OrgName = settings.value("D1", "").toString();
	Egrul = settings.value("D2", "").toString();
	Podrazdelenie = settings.value("D3", "").toString();
	Rayon = settings.value("D4", "").toString();
	Gorod = settings.value("D5", "").toString();
	Adress = settings.value("D6", "").toString();
	mail = settings.value("D7", "").toString();
	lastStatementNum = settings.value("D8", "").toInt();

	return true;
}
void SettingTools::saveSettings()
{
	QString ExePath = QDir::currentPath();
	QString PathToIniFile = ExePath + "/" + settingFileName;

	QSettings settings(PathToIniFile, QSettings::IniFormat);

	settings.setValue("D1", OrgName);
	settings.setValue("D2", Egrul);
	settings.setValue("D3", Podrazdelenie);
	settings.setValue("D4", Rayon);
	settings.setValue("D5", Gorod);
	settings.setValue("D6", Adress);
	settings.setValue("D7", mail);
}

void SettingTools::savelastStatementNum()
{
	QString ExePath = QDir::currentPath();
	QString PathToIniFile = ExePath + "/" + settingFileName;

	QSettings settings(PathToIniFile, QSettings::IniFormat);

	settings.setValue("D8", lastStatementNum);	
}
