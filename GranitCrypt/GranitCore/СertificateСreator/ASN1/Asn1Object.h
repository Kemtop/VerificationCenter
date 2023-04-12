#pragma once
#include <vector>
#include <iostream>
#include <fstream>

//Virtual class
//������� ����� DER ���������  ASN.1 �����

class Asn1Object
{
public:
	Asn1Object();
	~Asn1Object();
	unsigned int ObjLength; //����� 
	unsigned char Tag; //������������� ��� �������

	unsigned char BlockType = 0;//�������� ����� 5-1 ������������� ���� ��� ������� �����;
	unsigned char ClassBlock = 0; // �������� ��� ���������� "�����" �������� �����;
	bool IsConstructedBlock = false;// ������ ���� ���������� � 0 ���� ������� ���� ��������

	//��������� ����� ����� ������ � �����(����� ���������� � ����)
	unsigned int ParentAdress;

     std::vector<unsigned char> ByteArray; //��������� ��� �������� ������
	 unsigned int Array_Pos=0;//��������� ������� ������� ����������

	 virtual void Print() { ; }
	 //����� ���� � ByteArray
	 void WriteByte(unsigned char byte);
	 //�������� ������ �� �������� ������
	 void CopyFromMainBuff(unsigned int start_pos, unsigned int end_pos, std::vector<unsigned char> *DerBuffer);

	 //������  ���� � ������� ������� ����������, ������� ��������� �������
     unsigned char Asn1Object::ReadByte();

	

};

