#pragma once
class Biektion
{
public:
	Biektion();
	~Biektion();

	// ��������� ��� ������� ������������ len. ���� ���� �� ���� ��� ���������� � ��������-��������� false.
	bool CompireArray(bool a[], bool b[], int len);

	//void Biektiv0(bool *Bits, bool block1[]);
	// ��������� ������������ �������� ������� Y, �������� ������� ���� ����������� ��� � �������� ���������.
	bool ChekBiektiv0(bool  Y[],bool *ControlBits);

	// ��������� ������������ �������� ������� Y, �������� ������� ���� ����������� ��� � �������� ���������.
	bool ChekBiektiv1(bool  Y[], bool *ControlBits);

	// ��������� ������������ �������� ������� Y, �������� ������� ���� ����������� ��� � �������� ���������.
	bool ChekBiektiv2(bool  Y[], bool *ControlBits);

	// ��������� ������������ �������� ������� Y, �������� ������� ���� ����������� ��� � �������� ���������.
	bool ChekBiektiv3(bool  Y[], bool *ControlBits);

	// ��������� ������������ �������� ������� Y, �������� ������� ���� ����������� ��� � �������� ���������.
	bool ChekBiektiv4(bool  Y[], bool *ControlBits);

	// ��������� ������������ �������� ������� Y, �������� ������� ���� ����������� ��� � �������� ���������.
	bool ChekBiektiv5(bool  Y[], bool *ControlBits);

	// ��������� ������������ �������� ������� Y, �������� ������� ���� ����������� ��� � �������� ���������.
	bool ChekBiektiv6(bool  Y[], bool *ControlBits);

	// ��������� ������������ �������� ������� Y, �������� ������� ���� ����������� ��� � �������� ���������.
	bool ChekBiektiv7(bool  Y[], bool *ControlBits);

	// ��������� ������������ �������� ������� Y, �������� ������� ���� ����������� ��� � �������� ���������.
	bool ChekBiektiv8(bool  Y[], bool *ControlBits);

};

