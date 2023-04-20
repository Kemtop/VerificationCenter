#pragma once

#include <QObject>

class CipherGear : public QObject
{
	Q_OBJECT

public:
	CipherGear(QObject *parent);
	~CipherGear();
};
