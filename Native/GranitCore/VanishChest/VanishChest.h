#pragma once
#include "hash3411\hash3411.h"
#include "mpirxx.h"
#include <QByteArray>
#include <QString>
#include <QCryptographicHash>

//����� ��� �������� ������ �������� �� ����������� ����. �������� ������� ��������������.
class VanishChest
{
public:
	VanishChest();
	~VanishChest();
	
	//������������ �������� ����  ����� �����-�������. ������ ����� ����� ����� �����������
	static void ModifyDHKey(QByteArray In, QByteArray &Out);

	//�� ����������������� ��������� ��������������� � ���������� ��� �������� ����  ����� �����-�������. 
	static void UnModifyDHKey(QByteArray In, QByteArray &Out);

	//�� ����������������� ��������� ��������������� � ���������� ��� �������� ����  ����� �����-�������. 
	static void UnModifyDHKey(QString &In, QString &Out);


	//��������� ����������� ������ ��� �������� DHKey. ��� �� ����� ����������� �����.
	static void CrcDHKey(QString &hash_str, QString &DHKey);
	//��������� ����������� ����� ��� �������� ����� ���. ����� ������������ ��� ������ ��� �������� ������ ������  � �����������
	//������������ ��������� �������� ���� ���.
	static QString CrcECKey(QString x,  QString y);

	//��������� �������� ��� ������� � �������������� ��������� MD5 �� ��������� �������� ����� x,y.
	static string  QkeyMD5(string x, string y);

	//���������� ����������� ����� ����������������� �����������.
	static QString CrcUserCertificate(QByteArray cert);

	//���������� ����������� ����� ���������� �����������.
	static QString CrcSystemCertificate(QByteArray cert);

	//������������ ������ � ������� � � � ������������ � ������.
	static QString ModifyExUserFIO(QString &in);

	//���������  ������ � ������� � � � ������������ � ������ � ���������� ���.
	static QString UnModifyExUserFIO(QString &in);

	//������������ ���� ��������� ����� �������� ���� ������. ������� ������ ������ ��������� 10 ��������. 
	static QString ModifyEndDbDate(QString in);

	//��������������� ���� ��������� ����� �������� ���� ������. ������� ������ ������ ��������� 20 ��������. 
	static QString UnModifyEndDbDate(QString in,bool &err);

	//���� �������������� ���� ��������� ����� �������� �� �������.
	bool TestVanichEndDbDate();

	//���������� ����������� ����� ���� ��������� �������� �� �������.
	static QString getCrcEndDbDate(QString in);

	//��������� ������, ������� ���������� "��� �����" � ������(WORD ��������).
	static QString algHashSignKey(QString x, QString y);

private:
	//��������� ������������ ��������� ����� ���. ��������������� ��� CrcECKey.
	static void TranspECQ(QByteArray Qz);

};

