#pragma once

#include <QObject>
#include <string>
#include <QDateTime>

using std::string;

//����� ������ ���������� ��� ������������ ASN1 ������ ��� ������������ 
//���������� ����������������� ���������, �������� ��������� RFC 5652. � ������ ������ ��� ������������ �������� �������.
class rfcSignedData 
{
public:
	rfcSignedData();
	~rfcSignedData();

	int CMSVersion; //����� ������ ����������.
	string digestAlgorithms; //������������� ���������� ������� ��������� ���������.
	string commonName;
	int certNum;//����� ����������� ����������.
	QDateTime signingTime; //����� �������.
	std::vector <unsigned char> digest; //�������� ���������.

	string signingCertDigestAlg; //�������� ��������� ����������� ����������.
	std::vector <unsigned char> signingCertDigest;   //��������� ����������� ����������.
	
	string signatureAlgorithm;//�������� ���.
	std::vector<unsigned char> signVal; //���������� �������� �������.

};
