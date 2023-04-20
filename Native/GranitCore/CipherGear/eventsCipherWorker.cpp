#include "eventsCipherWorker.h"



void eventsCipherWorker::setAllProcessVal(int val)
{
	allprocess = val;
}

int eventsCipherWorker::getAllProcessVal()  const
{
	return allprocess;
}

void eventsCipherWorker::setCurFileProcess(int val)
{
	ñurfileprocess = val;
}

int eventsCipherWorker::getCurFileProcess() const
{
	return ñurfileprocess;
}

void eventsCipherWorker::setMaxFileProcess(int val)
{
	maxfileprocess = val;
}

int eventsCipherWorker::getMaxFileProcess() const
{
	return maxfileprocess;
}

int eventsCipherWorker::getThreadStatus() const
{
	return threadStatus;
}

void eventsCipherWorker::setThreadStatus(int val)
{
	threadStatus = val;
}

string eventsCipherWorker::getCurfileprocessInfo() const
{
	return curfileprocessInfo;
}

void eventsCipherWorker::setCurfileprocessInfo(string val)
{
	curfileprocessInfo = val;
	needshowcurfileprocessInfo = true; //Äëÿ áûñòğîãî âûâîäà ñîîáùåíèÿ.
}

bool eventsCipherWorker::getNeedshowcurfileprocessInfo() const
{
	return needshowcurfileprocessInfo;
}

string eventsCipherWorker::getLastError() const
{
	return lasterror;
}

void eventsCipherWorker::setLastError(string val)
{
	lasterror=val;
}
