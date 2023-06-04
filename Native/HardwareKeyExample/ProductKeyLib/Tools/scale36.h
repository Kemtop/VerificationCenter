#pragma once

#include "qstring.h"
#include "qlist.h"
#include "qbitarray.h"

class Scale36 : QList<uint8_t>
{
public:
	Scale36();
	Scale36(QList<uint8_t>);
	Scale36 operator+ (Scale36);
	Scale36 operator- (Scale36);
	bool operator< (Scale36);
	void resize(int);
	void FromBitArray(QBitArray);
	void FromString(QString);
	QString ToString();
	QBitArray ToBitArray();
};

