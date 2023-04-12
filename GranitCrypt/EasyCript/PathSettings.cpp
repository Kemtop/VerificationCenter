#include "PathSettings.h"



PathSettings::PathSettings()
{
}


PathSettings::~PathSettings()
{
}

QString PathSettings::GetPathDb()
{
	return PathDb;
}

QString PathSettings::GetPathSK()
{
	return PathSK;
}



void PathSettings::SetPathDb(QString val)
{
	PathDb = val;
}

void PathSettings::SetPathSK(QString val)
{
	PathSK = val;
}


