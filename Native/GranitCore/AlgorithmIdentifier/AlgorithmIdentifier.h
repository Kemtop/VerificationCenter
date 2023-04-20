#pragma once
#include <string>

using std::string;
//������������� ��������� �������
class _declspec(dllexport) AlgorithmIdentifier
{
public:
	AlgorithmIdentifier();
	~AlgorithmIdentifier();
	//�������� ��������� �����
	static const string PublicKeyAlgoritm[2][2];
	//���������� ����� � ������� PublicKeyAlgoritm
	static const int PublicKeyAlgoritmCount; 
	//�������� ������� �����������
	static const string SignCertAlgoritm[2][2];
	//���������� ����� � ������� SignCertAlgoritm
	static const int SignCertAlgoritmCount;

	//��������� ������������� ������ ��� ����������� � ���������� ������ ��� �������� ������������
	static const string EcParam[2][2];
	static const int EcParamCount;

	//�������� �������� ��������� ����� �� ������ ��� Oid. ����  �� ������ ���������� false.
	static bool getPublicKeyAlgoritmName(string Oid, string &Name);

	//�������� �������� ��������� ������� ����������� �� ������ ��� Oid. ����  �� ������ ���������� false.
	static bool getSignCertAlgoritm(string Oid, string &Name);

	//�������� �������� ������������� ������ �� ������ �� Oid. ���� ������ �� ������� ���������� false.
	static bool getECName(string Oid,string &Name);


};

