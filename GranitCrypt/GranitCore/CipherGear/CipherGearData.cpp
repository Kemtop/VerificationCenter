#include "CipherGearData.h"

CipherWorkerData::CipherWorkerData(QObject *parent)
	: QObject(parent)
{
	processmode = 0;
	enableRose = false;
}

CipherWorkerData::CipherWorkerData()
{
	processmode = 0;
	enableRose = false;
}

CipherWorkerData::~CipherWorkerData()
{
}

// онструктор копировани€.
CipherWorkerData::CipherWorkerData(const CipherWorkerData &val)
{
	setProcessFiles(val.getProcessFiles());
	setPassword(val.getPassword());
	setRAsimOpenKey(val.getRAsimOpenKey());
	setKeyContainer(val.getKeyContainer());
	setSignerIndex(val.getSignerIndex());
	setSignatureDH(val.getSignatureDH());
	setProcessMode(val.getProcessMode());
	setDbPath(val.getDbPath());
	setRoseMode(val.getRoseMode());
}



//ќператор присваивани€.
CipherWorkerData& CipherWorkerData::operator=(const CipherWorkerData &val)
{
	//проверка на самоприсваивание
	if (this == &val) return *this;
	
	setProcessFiles(val.getProcessFiles());
	setPassword(val.getPassword());
	setRAsimOpenKey(val.getRAsimOpenKey());
	setKeyContainer(val.getKeyContainer());
	setSignerIndex(val.getSignerIndex());
	setSignatureDH(val.getSignatureDH());
	setProcessMode(val.getProcessMode());
	setDbPath(val.getDbPath());
	setRoseMode(val.getRoseMode());
	
	return *this;
}



void CipherWorkerData::setProcessFiles(QStringList val)
{
	filesToProcess = val;
}

QStringList CipherWorkerData::getProcessFiles() const
{
	return filesToProcess;
}

void CipherWorkerData::setPassword(QString val) 
{
	password = val;
}

QString CipherWorkerData::getPassword() const
{
	return password;
}

void CipherWorkerData::setRAsimOpenKey(std::string val)
{
	receiverKey = val;
}

string CipherWorkerData::getRAsimOpenKey() const
{
	return receiverKey;
}

void CipherWorkerData::setKeyContainer(PkContainer val)
{
	keyContainer = val;
}

PkContainer CipherWorkerData::getKeyContainer() const
{
	return keyContainer;
}

void CipherWorkerData::setSignerIndex(string val)
{
	signerIndex=val;
}

string CipherWorkerData::getSignerIndex() const
{
	return signerIndex;
}

void CipherWorkerData::setSignatureDH(string val)
{
	signatureDH = val;
}

string CipherWorkerData::getSignatureDH() const
{
	return signatureDH;
}

void CipherWorkerData::setProcessMode(int Val)
{
	processmode = Val;
}

int CipherWorkerData::getProcessMode() const
{
	return processmode;
}

void CipherWorkerData::setDbPath(QString val)
{
	DbPath = val;
}

QString CipherWorkerData::getDbPath() const
{
	return DbPath;
}

void CipherWorkerData::setRoseMode(bool val)
{
	enableRose = val;
}

bool CipherWorkerData::getRoseMode() const
{
	return enableRose;
}
