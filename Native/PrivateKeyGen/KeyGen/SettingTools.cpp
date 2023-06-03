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
	OrgName = settings.value("OrgName", "").toString();
	OrgCode = settings.value("OrgCode", "").toString();
	Podrazdelenie = settings.value("Podrazdelenie", "").toString();
	Rayon = settings.value("Rayon", "").toString();
	Gorod = settings.value("Gorod", "").toString();
	Adress = settings.value("Adress", "").toString();
	mail = settings.value("Mail", "").toString();

	return true;
}
void SettingTools::saveSettings()
{
	QString ExePath = QDir::currentPath();
	QString PathToIniFile = ExePath + "/" + settingFileName;

	QSettings settings(PathToIniFile, QSettings::IniFormat);

	settings.setValue("OrgName", OrgName);
	settings.setValue("OrgCode", OrgCode);
	settings.setValue("Podrazdelenie", Podrazdelenie);
	settings.setValue("Rayon", Rayon);
	settings.setValue("Gorod", Gorod);
	settings.setValue("Adress", Adress);
	settings.setValue("Mail", mail);
}
