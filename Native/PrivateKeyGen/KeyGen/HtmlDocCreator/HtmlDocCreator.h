#pragma once

#include <QObject>
#include "mRequest.h"

class HtmlDocCreator : public QObject
{
	Q_OBJECT

public:
	HtmlDocCreator();
	~HtmlDocCreator();

	//��������� ��������� �� ������	����������� ����� �������� ����������� �������.
	QString CreateZayavka(mRequest Req);
};
