#include "stdafx.h"
#include "AppConfig.h"

void AppConfig::setServerHost(QString _ServerHost)
{
	ServerHost= _ServerHost;
}

void AppConfig::setServerPort(int _ServerPort)
{
	 ServerPort= _ServerPort;
}

void AppConfig::setServerLogin(QString _ServerLogin)
{
	ServerLogin=_ServerLogin;
}

QString AppConfig::getServerHost()
{
	return ServerHost; //��� �������
}

int AppConfig::getServerPort()
{
	return ServerPort;
}

QString AppConfig::getServerLogin()
{
	return ServerLogin;//�����.
}

bool AppConfig::HasConfFile()
{
	return hasConfFile;
}

void AppConfig::Read()
{
	QString ExePath = QDir::currentPath(); // 
	QString PathToIniFile = ExePath + "/config.app";

	//�������� ������������� �����
	QFileInfo file_info(PathToIniFile);
	if (!file_info.exists()) return ; //���� �� ����������

	QFile loadFile(PathToIniFile);

	//�� ������� ������� ���������������� ����-������ ����� ��� ���.
	if (!loadFile.open(QIODevice::ReadOnly))
	{
		QMessageBox::critical(0, "Error", "Error to open configure file");
		return;
	}
	
	QByteArray saveData = loadFile.readAll();
	QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
	QJsonObject cObj = loadDoc.object();

	if (cObj.contains("ServerConfig") && cObj["ServerConfig"].isObject())
	{
		QJsonObject ServerConfig = cObj["ServerConfig"].toObject();

		if (ServerConfig.contains("Host") && ServerConfig["Host"].isString())
			ServerHost= ServerConfig["Host"].toString(); //��� �����

		if (ServerConfig.contains("Port") && ServerConfig["Port"].isDouble())
			ServerPort= ServerConfig["Port"].toInt();

		if (ServerConfig.contains("ServerLogin") && ServerConfig["ServerLogin"].isString())
			ServerLogin = ServerConfig["ServerLogin"].toString();
	}

	hasConfFile = 1; //���� ������������ ����������.
}

void AppConfig::Write()
{

	QString ExePath = QDir::currentPath(); // /
	QString PathToIniFile = ExePath + "/config.app";

	QFile saveFile(PathToIniFile);

	if (!saveFile.open(QIODevice::WriteOnly)) 
	{
		QMessageBox::critical(0,"Error","Error to save configure file");
		return;
	}

	QJsonObject ServerConf;
	ServerConf["Host"] = ServerHost; 
	ServerConf["Port"] = ServerPort;
	ServerConf["ServerLogin"] = ServerLogin;

	QJsonObject AppConf;
	   
	QJsonObject configObject;
	configObject["ServerConfig"] = ServerConf;
	configObject["AppConfig"] = AppConf;
	
	QJsonDocument saveDoc(configObject);
	saveFile.write(saveDoc.toJson());
	saveFile.close();
}


AppConfig::AppConfig()
{
	hasConfFile = 0; //�� ���������� ����� ������������
	ServerHost="127.0.0.1"; //��� �������
	ServerPort= 5432; //����
	ServerLogin="postgres";//�����.
}

AppConfig::~AppConfig()
{
}
