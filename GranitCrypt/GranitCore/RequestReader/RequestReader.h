#pragma once

#include <QObject>
#include "Models\mRequest.h"
#include <string>
#include <QFile>
#include "hash3411\hash3411.h"
#include "Gost3412\Sib3412.h"
#include "windows.h"
#include "SerializableTools\SerializableTools.h"

using std::string;

class RequestReader : public QObject
{
	Q_OBJECT
private:
	//Распаковывает макет ключа из src в dst
	bool UnpackMaket(QByteArray &src, QByteArray &dst);

	//Получает значение ключа упаковывания макета. В случае ошибки HasErr=1;Err-содержит сообщение об ошибке.
	ByteBlock GetPackMf(string &Err, bool &HasErr);
	
	string lastError;


public:
	RequestReader();
	~RequestReader();

	//Читает запрос в модель.
	bool Read(QString path, mRequest &Req);

	//Проверяет модель запроса на корректность.
	bool checkRequest(mRequest &Req);

	string getLastError();
};
