#pragma once
#include <qbytearray>
#include <string>

using std::string;
/*
 ����� ����������� ����� ������������ ��� �������� ������������� ����������
 ��������� �������� ����� ������� ���������� � ���� ����� ������������ ���������.
 ��������� ������ �������������� ������ � �������� ������
*/
//����� ����������� ����� ������������ ��� �������� ������������� ���������� 

//������� ����� ������ Release.
class AsInterpretator
{
public:
	AsInterpretator();
	~AsInterpretator();
	//[114]
	void Dh_Hash(QByteArray &Block,string &K_hash); //��������� ������������ �������� ���� ���������� 
	//[115] 
	//��������� ����������� ��������� ����
	void CryptedBlockKey(QByteArray &Block, string &K);
	//[116]
	void Sign_r(QByteArray &Block, string &r);
	//[117]
	void Sign_s(QByteArray &Block, string &s);
	//[118] ������ ����� �������� ������� � ����
	void SetSignedIndex(QByteArray &Block, string &index);
	//���������� � Block ����� len
	void write_len(QByteArray &Block,int len );


};

/*
 ������
 [��� ����� 1 ����] [����� ����� 4 ����] [������]

*/