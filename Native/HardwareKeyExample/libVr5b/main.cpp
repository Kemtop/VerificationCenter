#include "main.h"

#include <QtCore/QCoreApplication>
#include "windows.h"
#include "XorShift\XorShift.h" 


__declspec(dllexport)
int DllTest()
{
	MessageBoxA(GetActiveWindow(), "libVr5b.dll", "Dll test", MB_ICONERROR);
	return 1;
}



/*
 ���������� ������� ���� �����.
 �������� �������� � ������� � Dll ���� ��� ����������� ���������������.
*/
__declspec(dllexport)
uint8_t CubicWatchMan(unsigned int val)
{
	uint8_t b = 0;
	b = (uint8_t)(val & 0xff);
	return b;
}


/*
������� ���������� ������� �� ��� �� ������������.
����� ��� ����� �������������� � �����������.
*/
__declspec(dllexport)
uint8_t CubicWatchManVx(unsigned int val, uint8_t a)
{
	uint8_t b = 0;
	b = (uint8_t)(val & a);
	return b;
}

/*
������� ���������� ������� �� ��� �� ������������.
����� ��� ����� �������������� � �����������.
*/
__declspec(dllexport)
uint8_t CubicWatchManLoS(unsigned int val, uint8_t a)
{
	unsigned int c = 1;
	for (int i = 0;i < a;i++)
	{
		c = c + c << i;
	}

	uint8_t b = 0;
	b = (uint8_t)(val & c);
	return b;
}

/*
������� ���������� ������� �� ��� �� ������������.
����� ��� ����� �������������� � �����������.
*/
__declspec(dllexport)
uint8_t CubicWatchManWoW64(unsigned int val, uint8_t a)
{
	unsigned int c = 1;
	for (int i = 0;i < a;i++)
	{
		c = c + (c << i) + 1;
	}

	uint8_t b = 0;
	b = (uint8_t)(val & c);
	return b;
}


//������ ������� �������� ����� 50,��� �������� � ���������� ��������������.
//x1=5  x2=2; x3=20
unsigned int TaoLink(unsigned int x1, unsigned int x2, unsigned int x3)
{
	unsigned int c = 9;
	unsigned int a = 9;
	c = 1;
	a = c << x1; //�������� ��������� 32 .
	c = a - x2; //=30
	a = c + x3; //+20
	return a;
}

//������� ������� ���������� �� �������� ��� � �����. ��� ����� � ����������� ��������������.
__declspec(dllexport)
bool IsToi(unsigned int a, unsigned int b)
{
	if (a < b)
		return false;

	return true;
}


/*
   ��������� ��� "���������" �����, �������� �������� ��������� ��������.
   ���������� ���� ������ � ������ ������� � ���������� ������.
*/
__declspec(dllexport)
int Curdeprecon(unsigned int a, unsigned int b, QByteArray &ExtArr)
{
	XorShift Rnd;
	unsigned int tmp = 0;
	uint8_t b0, b1, b2, b3;
	int fakeVar = 0; //��� �� �������� ��������������.

	//������� �������� ������� ����� � ����. ��� ��� ��������� ����� ��� �����������������.
	unsigned int xA = 1, xB = 15, xC = 36, xD = 28;
	xA = 1 << 2; //=4
	//a=275 b=604 -���������� ������� �����������.
	xB = xA + xB + a - 3; //4+15+275-3  ����� ���������� ��� ���������. =291 

	//����� �������� 524
	xC = b - xD; //604-28=576;
	xA = xC - xB; //576-291=285;


	if (xA < 128) return 1526; //������ ��� ��� �� ������ ��� �� ������. ���� �������� ���������.
	IsToi(xA, xC); //������ ����� ������.
	//xA=285.

	//�������� �� 2.
	xB = xA << 1; //570

	//���������� 4.
	xD = 1;
	xD = xD << 2;// �������� ����� 4.
	xC = xB + xD; //�������� �������� 574.

	//������ ������� �������� ����� 50,��� �������� � ���������� ��������������.
	xA = TaoLink(5, 2, 20);
	//������� 524.
	xB = xC - xA; //574-50=524.

	//149 248 ���� ������ �����.  149 248/4=37312
	int CopyBlock = 0;

	QByteArray Ba;
	//Ba.resize(300);
	//37313

	for (int i = 0;i < 3531;i++)
	{
		tmp = Rnd.GetValue();
		b0 = CubicWatchMan(tmp);
		tmp = tmp >> 8;
		b1 = CubicWatchMan(tmp);
		tmp = tmp >> 8;
		b2 = CubicWatchMan(tmp);
		tmp = tmp >> 8;
		b3 = CubicWatchMan(tmp);
		
		//����������� ����, ��� ������� ���������� ������� �� ��� ������������� ������� ������ ���������.
			Ba.append(static_cast<char>(b0));
			Ba.append(static_cast<char>(b1));
			Ba.append(static_cast<char>(b2));
			Ba.append(static_cast<char>(b3));	
	}

	//fakeVar = i + a + b+b0;

	//������ ����� �������� ��������� ������.
	//(i>524)&& xB
	//if ((i>524)&&(CopyBlock<128))

	for (int i = 0;i < 128;i++)
	{
		ExtArr[i] = Ba[i+xB+1];//i+525
	}

	return 4281;
}