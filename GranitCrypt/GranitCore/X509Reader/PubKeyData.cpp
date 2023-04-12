#include "stdafx.h"
#include "PubKeyData.h"


PubKeyData::PubKeyData()
{
}


PubKeyData::~PubKeyData()
{
}

void PubKeyData::SetAlgoritmOID(OID sigAlgOID)
{
	vector<word32> I;
	I = sigAlgOID.GetValues();
	string str;

	//�����������
	for (int i = 0;i < I.size();i++)
	{
		if (i != 0) str = str + ".";
		str = str + std::to_string(I[i]);
	}

	AlgoritmOID = str;
}

void PubKeyData::SetE�OID(OID EcOID)
{
	vector<word32> I;
	I = EcOID.GetValues();
	string str;

	//�����������
	for (int i = 0;i < I.size();i++)
	{
		if (i != 0) str = str + ".";
		str = str + std::to_string(I[i]);
	}

	E�OID = str;
}

void PubKeyData::SetHashOID(OID HashOID)
{
	vector<word32> I;
	I = HashOID.GetValues();
	string str;

	//�����������
	for (int i = 0;i < I.size();i++)
	{
		if (i != 0) str = str + ".";
		str = str + std::to_string(I[i]);
	}

	HOid =str;
}

void PubKeyData::SetPubKey(SecByteBlock pk)
{
	//� Bit String co�������� OctetString. ���� �� ��������� -�������� ��������.
	//��� ��� ������ ��������c� ������ 127 ��� ����������� �������������� ���� ����������� �����

	byte os_b1 = pk[0]; //������ ���� 04
	byte os_b2 = pk[1]; //81
	byte os_b3 = pk[2]; //80
	byte os_b4 = pk[3];

	//������� �������� ����
	for (int i = 3;i < pk.size();i++)
		PubKey.push_back(pk[i]);
}
