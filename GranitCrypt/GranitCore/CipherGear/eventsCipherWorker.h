#pragma once

#include <QObject>
#include <QEvent>
#include <string>


using std::string;

//C������ ������.
class eventsCipherWorker :public QEvent
{
public:
	enum { eventCipherWorkerType = User+1 };

	// � ������������ �� ������ ��� ������ �������, ��������� ��� ������������� ������������� � ����������� ������ QEvent().
	eventsCipherWorker():QEvent((Type)eventCipherWorkerType)
	{
		allprocess=0; //�������� ��������� ���� ������.
		�urfileprocess=-1; //������� ������� ��������� �����.
		maxfileprocess=0; //������������ �������� �������� ��������� �����.
		needshowcurfileprocessInfo = false;
		threadStatus = 0;
	}

	void setAllProcessVal(int val);
	int getAllProcessVal() const;

	void setCurFileProcess(int val);
	int getCurFileProcess() const;

	void setMaxFileProcess(int val);
	int getMaxFileProcess() const;

	int getThreadStatus() const; //��������� ������.
	void setThreadStatus(int val);

	string getCurfileprocessInfo() const;
	void setCurfileprocessInfo(string val);

	bool getNeedshowcurfileprocessInfo() const;

	string getLastError() const; //��������� �� ������.
	void setLastError(string val);

private:
	int allprocess; //�������� ��������� ���� ������.
	int �urfileprocess; //������� ������� ��������� �����.
	int maxfileprocess; //������������ �������� �������� ��������� �����.
	string curfileprocessInfo; //���������� �� �������� ��������������� �����.
	bool needshowcurfileprocessInfo; //��� ������� ��������� ���������.
	int threadStatus; //������ ������. 0-�� ��������, 1 -�������. 2-������� ��������.3-������.
	string lasterror; //��������� �� ������.
};
