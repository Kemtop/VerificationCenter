#pragma once
//Хранит настройки программы. Читает и сохраняет в файл.
class AppConfig
{
private:

	//Конфигурационные параметры
	QString ServerHost; //Имя сервера
	int ServerPort; //Порт
	QString ServerLogin;//Логин.
	bool  hasConfFile; //Флаг существования  файла конфигурации.
	QString PathToSignKey; //Путь к ключу(CA) подписи сертификатов.

public:
	void setServerHost(QString _ServerHost); //Устанавливает параметры.
	void setServerPort(int _ServerPort);
	void setServerLogin(QString _ServerLogin);
	void setPathToSignKey(QString val);//Путь к ключу(CA) подписи сертификатов.


	QString getServerHost();
	int getServerPort();
	QString getServerLogin();
	QString getPathToSignKey();//Путь к ключу(CA) подписи сертификатов.

	bool HasConfFile(); //Существует ли файл конфигурации.

	void Read(); //Читает конфиг из файла
	void Write(); //Сохраняет настройки в файл.
	AppConfig();
	~AppConfig();


};

