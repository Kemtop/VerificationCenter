#pragma once
//������ ��������� ���������. ������ � ��������� � ����.
class AppConfig
{
private:

	//���������������� ���������
	QString ServerHost; //��� �������
	int ServerPort; //����
	QString ServerLogin;//�����.
	bool  hasConfFile; //���� �������������  ����� ������������.

public:
	void setServerHost(QString _ServerHost); //������������� ���������.
	void setServerPort(int _ServerPort);
	void setServerLogin(QString _ServerLogin);
	
	QString getServerHost();
	int getServerPort();
	QString getServerLogin();

	bool HasConfFile(); //���������� �� ���� ������������.

	void Read(); //������ ������ �� �����
	void Write(); //��������� ��������� � ����.
	AppConfig();
	~AppConfig();
};

