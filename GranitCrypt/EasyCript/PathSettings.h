#pragma once
#include <qstring.h>
//Хранит пути, необходимые для работы программы.
class PathSettings
{
public:
	PathSettings();
	~PathSettings();

	QString GetPathDb(); //Путь к базе данных системы.
	QString GetPathSK(); //Путь к секретному ключу.


	void SetPathDb(QString val); //Путь к базе данных системы.
	void SetPathSK(QString val); //Путь к секретному ключу.

private:
	 QString PathDb; //Путь к базе данных системы.
	 QString PathSK; //Путь к секретному ключу.
};

