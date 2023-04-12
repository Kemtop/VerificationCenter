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
	//������������� ����� ����� �� src � dst
	bool UnpackMaket(QByteArray &src, QByteArray &dst);

	//�������� �������� ����� ������������ ������. � ������ ������ HasErr=1;Err-�������� ��������� �� ������.
	ByteBlock GetPackMf(string &Err, bool &HasErr);
	
	string lastError;


public:
	RequestReader();
	~RequestReader();

	//������ ������ � ������.
	bool Read(QString path, mRequest &Req);

	//��������� ������ ������� �� ������������.
	bool checkRequest(mRequest &Req);

	string getLastError();
};
