#pragma once
#include <QByteArray>
#include <QDate>

#define SF_LEN 100

class SecureFunc
{
private:

	// ������ ��� ������ ������ SF0;
	quint64 ArSF0[SF_LEN];
		
	//�������� 32 ������� ������. ����� � �������� ������-"�������� ������".	
	void TwistTheRing(int rotate, bool *r);
		
	//������������ ���������� ������ � ������� ������ len
	int CountUnits(bool val[], int len);
		
	//��������� ������  ArSF0 ������� ��������� �������.  	
	void FillArSF0();
		
	//������������ ������� ������ � UInt64.
	void ConvertBaseNumUint(bool *baseNum, quint64 &result);
		
	//������������ ������� ������ � Int64.
	void ConvertBaseNumInt(bool *BaseNum, qint64 &result);
		
	//��������������� 64������ ��� �������� ����� � ���� ������.
	void ConvertU64ToBool(quint64 d, bool *Result);

	//��������������� 64������ ��� �������� ����� � ���� 32 ������ ���� ������. 
	void ConvertI64ToBool(qint64 d, bool *Result);

public:

	//������� ��� ���������� ����������� �����.
	void SfCRC(QByteArray &xi);

	//������� ��� ��������� ������� ����� ��������� �����.
	void SF0(bool *BaseNum, bool *Dst);

	//������� ��� ��������� ������� ����� ��������� �����.
	void SF1(bool *BaseNum, bool *Dst);

	//������� ��� ��������� �������� ����� ��������� �����.
	void SF2(bool *BaseNum, bool *Dst);

    //������� ��� ��������� ���������� ����� ��������� �����.
	void SF3(bool *BaseNum, bool *Dst);

	void SF4(bool *BaseNum, bool *Dst);
	void SF5(bool *BaseNum, bool *Dst);
	void SF6(bool *BaseNum, bool *Dst);
	void SF7(bool *BaseNum, bool *Dst);
	void SF8(bool *BaseNum, bool *Dst);
};

