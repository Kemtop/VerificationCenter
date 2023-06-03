#pragma once

#include <QObject>
#include <QDir>
#include <QSettings>

#define settingFileName "settings.ini"


//���������� ��� ���������� �������� �  .ini ����.
class SettingTools 
{

private:


public:
	SettingTools();
	~SettingTools();

	QString OrgName; //������������ ����������.
	QString OrgCode;
	QString Podrazdelenie;
	QString Rayon;
	QString Gorod;
	QString Adress;
	QString mail; 
	
	//��������� ������� ����� ��������. ���� ����=true.
	bool hasSetingFile();

	//������ ���������. ���������� false ���� ��� ����� ��������.
	bool loadSettings();

	//��������� ���������.����� ���������� ������ ������.
	void saveSettings();

	//��������� ��������� ����� ������ �� ��������� �����������.  
	void savelastStatementNum();
};
