#pragma once

#include <string>
#include <vector>
using namespace std;
//����� ������������ ��� ������������ ����� �����������
class _declspec(dllexport) ExtensionField
{
public:
	
	ExtensionField();
	~ExtensionField();
	string OID;
	unsigned char Boolean=3; //�� ��������� ����� ��� 

	//������ ������
	vector <unsigned char> OCTET_STRING_array;

	//������� � �������� ������ � ������ OCTET_STRING_array
	void FillArray(unsigned char * exarray,unsigned long len);
};

