#pragma once

#include <QObject>
#include "mRequest.h"

class HtmlDocCreator : public QObject
{
	Q_OBJECT

public:
	HtmlDocCreator();
	~HtmlDocCreator();

	//Формирует заявление на выдачу	сертификата ключа проверки электронной подписи.
	QString CreateZayavka(mRequest Req);
};
