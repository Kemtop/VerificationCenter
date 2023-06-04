#pragma once
#include <QByteArray>
#include <QDate>

class SecureFunc
{
private:
	/// <summary>
	/// ������ ��� ������ ������ SF0;
	/// </summary>
	quint64 ArSF0[100];


	/// <summary>
	/// �������� 32 ������� ������. ����� � �������� ������-"�������� ������".
	/// </summary>
	void TwistTheRing(int rotate, bool *R);

	/// <summary>
	/// ������������ ���������� ������ � ������� ������ len
	/// </summary>
	/// <param name="Val"></param>
	/// <returns></returns>
	int CountUnits(bool Val[], int Len);

	/// <summary>
	/// ��������� ������  ArSF0 ������� ��������� �������.  
	/// </summary>
	void FillArSF0();

	/// <summary>
	/// ������������ ������� ������ � UInt64.
	/// </summary>
	/// <param name="BaseNum"></param>
	/// <param name="result"></param>
	void ConvertBaseNumUint(bool *BaseNum, quint64 &result);

	/// <summary>
	/// ������������ ������� ������ � Int64.
	/// </summary>
	/// <param name="BaseNum"></param>
	/// <param name="result"></param>
	void ConvertBaseNumInt(bool *BaseNum, qint64 &result);

	/// <summary>
	/// ��������������� 64������ ��� �������� ����� � ���� ������. 
	/// </summary>
	/// <param name="Arr"></param>
	/// <param name="Result"></param>
	void ConvertU64ToBool(quint64 d, bool *Result);

	/// <summary>
	/// ��������������� 64������ ��� �������� ����� � ���� 32 ������ ���� ������. 
	/// </summary>
	/// <param name="Arr"></param>
	/// <param name="Result"></param>
	void ConvertI64ToBool(qint64 d, bool *Result);

public:
	SecureFunc();
	~SecureFunc();

	// ������� ��� ���������� ����������� �����.
	void SfCRC(QByteArray &Xi);

	/// <summary>
	/// ������� ��� ��������� ������� ����� ��������� �����.
	/// </summary>
	/// <param name="BaseNum"></param>
	/// <param name="Dst"></param>
	 void SF0(bool *BaseNum, bool *Dst);

	 /// <summary>
	 /// ������� ��� ��������� ������� ����� ��������� �����.
	 /// </summary>
	 /// <param name="BaseNum"></param>
	 /// <param name="Dst"></param>
	 void SF1(bool *BaseNum, bool *Dst);


	 /// <summary>
	 /// ������� ��� ��������� �������� ����� ��������� �����.
	 /// </summary>
	 /// <param name="BaseNum"></param>
	 /// <param name="Dst"></param>
	  void SF2(bool *BaseNum, bool *Dst);

	  /// <summary>
	  /// ������� ��� ��������� ���������� ����� ��������� �����.
	  /// </summary>
	  /// <param name="BaseNum"></param>
	  /// <param name="Dst"></param>
	  void SF3(bool *BaseNum, bool *Dst);


      void SF4(bool *BaseNum, bool *Dst);
	  void SF5(bool *BaseNum, bool *Dst);
	  void SF6(bool *BaseNum, bool *Dst);
	  void SF7(bool *BaseNum, bool *Dst);
	  void SF8(bool *BaseNum, bool *Dst);

};

