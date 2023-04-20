#pragma once

#include <QObject>
#include <QEvent>
#include <string>


using std::string;

//Cобытия потока.
class eventsCipherWorker :public QEvent
{
public:
	enum { eventCipherWorkerType = User+1 };

	// В конструкторе мы задаем тип нашего события, передавая его целочисленный идентификатор в конструктор класса QEvent().
	eventsCipherWorker():QEvent((Type)eventCipherWorkerType)
	{
		allprocess=0; //Значение обработки всех файлов.
		сurfileprocess=-1; //Текущий процесс обработки файла.
		maxfileprocess=0; //Максимальное значения процесса обработки файла.
		needshowcurfileprocessInfo = false;
		threadStatus = 0;
	}

	void setAllProcessVal(int val);
	int getAllProcessVal() const;

	void setCurFileProcess(int val);
	int getCurFileProcess() const;

	void setMaxFileProcess(int val);
	int getMaxFileProcess() const;

	int getThreadStatus() const; //Состояние потока.
	void setThreadStatus(int val);

	string getCurfileprocessInfo() const;
	void setCurfileprocessInfo(string val);

	bool getNeedshowcurfileprocessInfo() const;

	string getLastError() const; //Сообщение об ошибке.
	void setLastError(string val);

private:
	int allprocess; //Значение обработки всех файлов.
	int сurfileprocess; //Текущий процесс обработки файла.
	int maxfileprocess; //Максимальное значения процесса обработки файла.
	string curfileprocessInfo; //Информация по текущему обрабатываемому файлу.
	bool needshowcurfileprocessInfo; //Для быстрой обработки сообщения.
	int threadStatus; //Статус потока. 0-не работает, 1 -запущен. 2-успешно выполнен.3-ошибка.
	string lasterror; //Сообщение об ошибке.
};
