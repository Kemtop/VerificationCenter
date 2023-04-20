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
	QString Egrul;
	QString Podrazdelenie;
	QString Rayon;
	QString Gorod;
	QString Adress;
	QString mail;
	int lastStatementNum; //��������� ����� ������ �� ��������� �����������.  


	//��������� ������� ����� ��������. ���� ����=true.
	bool hasSetingFile();

	//������ ���������. ���������� false ���� ��� ����� ��������.
	bool loadSettings();

	//��������� ���������.����� ���������� ������ ������.
	void saveSettings();

	//��������� ��������� ����� ������ �� ��������� �����������.  
	void savelastStatementNum();
};
