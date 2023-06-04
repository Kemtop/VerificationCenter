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
	QString PathToSignKey; //���� � �����(CA) ������� ������������.

public:
	void setServerHost(QString _ServerHost); //������������� ���������.
	void setServerPort(int _ServerPort);
	void setServerLogin(QString _ServerLogin);
	void setPathToSignKey(QString val);//���� � �����(CA) ������� ������������.


	QString getServerHost();
	int getServerPort();
	QString getServerLogin();
	QString getPathToSignKey();//���� � �����(CA) ������� ������������.

	bool HasConfFile(); //���������� �� ���� ������������.

	void Read(); //������ ������ �� �����
	void Write(); //��������� ��������� � ����.
	AppConfig();
	~AppConfig();


};

