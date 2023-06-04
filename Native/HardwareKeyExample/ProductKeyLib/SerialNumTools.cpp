#include "SerialNumTools.h"


SerialNumTools::SerialNumTools()
{
}


SerialNumTools::~SerialNumTools()
{
}


int SerialNumTools::CreateSnContainer()
{
	//libapr_tsvn32.dll
	QString ExePath = QDir::currentPath(); // /
	QString PathToFile = ExePath + "/" + productKeyContainerName;

	//�������� ������������� �����
	QFileInfo file_info(PathToFile);
	//if (file_info.exists()) return 0;


	QByteArray Ba;
	XorShift Rnd;
	unsigned int tmp = 0;
	uint8_t b0, b1, b2, b3;

	
	for (unsigned int i = 0;i < 37313;i++)
	{
		tmp = Rnd.GetValue();
		b0 = (uint8_t)(tmp & 0xff);
		tmp = tmp >> 8;
		b1 = (uint8_t)(tmp & 0xff);
		tmp = tmp >> 8;
		b2 = (uint8_t)(tmp & 0xff);
		tmp = tmp >> 8;
		b3 = (uint8_t)(tmp & 0xff);
		Ba.append(b0);
		Ba.append(b1);
		Ba.append(b2);
		Ba.append(b3);
	}


	//������� ����������� ����� ����� ������.
	 QByteArray baCrc = getCrc(Ba);
	 
	//�������� ����������� ����� � �����(��������� 16 ����).
	 putCrc(Ba,baCrc);

	QFile file(PathToFile);
	file.open(QIODevice::WriteOnly);
	file.write(Ba);
	file.close();

	return 1;
}



int SerialNumTools::GetSerialDataBlock(QByteArray &Ba)
{
	//libapr_tsvn32.dll
	QString ExePath = QDir::currentPath(); // /
	QString PathToFile = ExePath + "/" + productKeyContainerName;

	//�������� ������������� �����
	QFileInfo file_info(PathToFile);
	if (!file_info.exists())
	{
		// "Error BN251: Corrupted structure program.";
		return -251;
	}

	//�������� ������� �����.
	qint64 f_size = file_info.size();

	if (f_size != 149268)
	{
		//"Error BN252: Corrupted structure program.";
		return -252;
	}
		
	//�������� ����������� ����� �����.
	QFile file(PathToFile);
	if (!file.open(QIODevice::ReadOnly ))
	{
		//"Error BN253: Corrupted structure program.";
		return -253;
	}

	QByteArray Fd = file.readAll(); //��������� ��� ������ � ����� � ������ data
	file.close();

	if (Fd.size() != 149268)
	{	
		//"Error BN254: Corrupted structure program.";
		return -254;
	}

	//�������� ����������� ����� �����.
	if (!CheckCrc(Fd))
	{
		// "Error BN255: Corrupted structure program.";
		return -255;
	}

	//149268/32=4664
	quint64 sizeOfRead = 0; //���������� ��������� ���� �� ���.


	//��������� ������� ����� ��������� ������ � �����. ���=524.
	unsigned int a;
	unsigned int x0 = 0;
	unsigned int x1 = 71;
	a = get512( x0,  x1); //����� �������� ������� ������� ����� 512, ��� ������ �� ������.
	 //��� ��������� �������� ���������� 12, ����� ��� ����������� �������������� ��� ������� ����.
	x0 = 3; x1 = x1 & 2;
	unsigned int a1 = getDui(x0, x1);
	x0 = a + a1; //=524

	int Rpos = 0;
	uint8_t tmp1 = 0;

	for (int b = 0;b < 4660;b++)
	{
     	//������� � 525 �����.
		if ((b>static_cast<int>(x0)) && (Rpos<128))
		{
			//������� ����� �� ������� ������.
				tmp1 = Fd[b];
				Ba[Rpos] = tmp1;
				Rpos ++;
		}
	}


	return 1;
}


//��� ��������� �������� ���������� 512, ����� ��� ����������� �������������� ��� ������� ����.
unsigned int SerialNumTools::get512(unsigned int x0, unsigned int x1)
{
	unsigned int y = 0;
	y = x0 + 1;

	if ((x1 & 0x00) == 0) y = y << 1;
	if ((x1 & 0x00) == 0) y = y << 1;
	if ((x1 & 0x00) == 0) y = y << 1;
	if ((x1 & 0x00) == 0) y = y << 1;

	if ((x1 & 0x00) == 0) y = y << 1;
	if ((x1 & 0x00) == 0) y = y << 1;
	if ((x1 & 0x00) == 0) y = y << 1;
	if ((x1 & 0x00) == 0) y = y << 1;

	if ((x1 & 0x00) == 0) y = y << 1;

	return y;
}

//��� ��������� �������� ���������� 12, ����� ��� ����������� �������������� ��� ������� ����.
unsigned int SerialNumTools::getDui(unsigned int x0, unsigned int x1)
{
	unsigned int a = 0; //���� ���������.
	a = x0; //������ ���� 3;
	unsigned int tmp = 0;
	unsigned int b = 0;

	//x1=���������� ��������� �� 2 � ��������;
	for (unsigned int i = 0;i < x1;i++)
	{
		b = a << 1; //�������� �� 2.
		tmp = tmp + b;
	}

	return tmp;
}

QByteArray SerialNumTools::modifyProductKey(string ProductKey)
{
	//���� �������� �������� 5 ������ �� 5 �������� � 4 �����������, �.�. 29  ��������.
	if(ProductKey.length() != 29) return QByteArray();

	QByteArray baProductKey(ProductKey.c_str(), ProductKey.length()); //������ � ������.
	
	QByteArray baR;
	unsigned char tmp;

	//��������� ��������� �������� ���.
	for (uint i = 70;i < 100;i++)
	{
		tmp = static_cast<unsigned char>(i);
		tmp = (tmp + 8)>>2;
		baR.append(tmp);
	}

	//�������� ������.
	unsigned char tmp1;
	unsigned char tmp2;

	QByteArray baRet;
	baRet.resize(29);

	for (uint i = 0;i < 30;i++)
	{
		tmp = static_cast<unsigned char>(baProductKey[i]);
		tmp1 = static_cast<unsigned char>(baR[i]);
		tmp2 = tmp1^tmp;
		baRet[i] = tmp2;
	}


	return baRet;
}

QByteArray SerialNumTools::getCrc(QByteArray Ba)
{
	//������������ ������ ��� ���������� crc.
	/*
	   ��� ���������� ��� ����������� ��������������.
	   �������� �� ����� �������� ������������ ����������� ��������� ��� ������� ����������� ����� -
	   �� �� ���� �� ����������� ���������� �� ������ ���������� ��������� crc, ��� ��� ������ ���������� �����������
	   �������������� ����������.
	   ����������� �������� ������ ������������������� ����.
	*/
	QByteArray Ba1(Ba);
	uint8_t b0, b1, b2, b3,a;

	a = 0x37;

	b0 = Ba1[3];
	b1 = Ba1[4];
	b2 = Ba1[5];
	b3 = Ba1[6];

	b0 = b0^a;
	b1 = b1^a;
	b2 = b2^a;
	b3 = b3^a;

	Ba1[3] = b0;
	Ba1[4] = b1;
	Ba1[5] = b2;
	Ba1[6] = b3;

	//������ ��� ���������� ��� ����� �� ��������� SHA-3 512.
	QCryptographicHash   Crc(QCryptographicHash::Md5);
	Crc.addData(Ba1);
	QByteArray baCrc = Crc.result(); //���.
	
	return baCrc;
}

QByteArray SerialNumTools::trim16Ba(QByteArray Ba)
{
	QByteArray outBa;
	int l = Ba.size()-16;
	//�������� ������ ������� �� ������������.
	outBa.resize(l);

	//�������� ������ ��� ������.
	for (int i = 0;i < l;i++) outBa[i] = Ba[i];


	return outBa;
}

void SerialNumTools::putProductKey(QByteArray & Ba, QByteArray ProductKey)
{
	//��������� ������� ����� ��������� ������ � �����. ���=524.
	unsigned int a;
	unsigned int x0 = 0;
	unsigned int x1 = 71;
	a = get512(x0, x1); //����� �������� ������� ������� ����� 512, ��� ������ �� ������.
						//��� ��������� �������� ���������� 12, ����� ��� ����������� �������������� ��� ������� ����.
	x0 = 3; x1 = x1 & 2;
	unsigned int a1 = getDui(x0, x1);
	x0 = a + a1; //=524

	x1 = 8;
	a = x1<<1; //8 *2=16
	x0 = x0 + a; //524+16=540
	uint8_t tmp1 = 0;


	int pos = 0;

	//��������� � ��������� ���������������� ���� ��������.
	for (unsigned int i = x0;i < x0 + 30;i++)
	{
		Ba[i] = ProductKey[pos];
		pos++;
	}

}

void SerialNumTools::putCrc(QByteArray & Ba, QByteArray baCrc)
{
	for (int i = 0;i < 16;i++)
	{
		Ba.append(baCrc[i]);
	}

}

bool SerialNumTools::writeBaToFile(QByteArray & Ba, QFile & file)
{
	//�������� ������ � �����.
	if (file.open(QIODevice::WriteOnly))
	{
		file.seek(0);
		file.write(Ba);
		file.close();
	}
	else
		return false;

	return true;
}

//��������� ��������� �������������� �������� ���������� � ��������� ����� 29.
int SerialNumTools::getDig30()
{
	int a, b, c;
	c = -1;
	a = 1;b = 1;

	a = a*c;
	b = b*c;
	a = 1;
	b = a << 5; //����������� 32
	a = a << 1;
	c = (b - a); // 30

	return c;
}

string SerialNumTools::getPK(QByteArray prk)
{
	int lastP = 0;//�������

    //���������� ��������������, �������� ����� 15.
	for (int i = 0; i < 15; i++)
	{
         incrementDig(lastP);  //���������� ��������������,������ ��������� ����������. 
	}

	//���������� ��������������, �������� ����� 29.
	int z = getDig30()+ lastP;

	//��������� ���������� ������ � ���������������� ������ ��������.
	QByteArray baPk;
	baPk.resize(29);
	
	//������� 29���� ��������� ������. 
	//������� � lastP=15 � �� z=45
	int pos = 0;
	for (int i = lastP;i < z;i++)
	{
		baPk[pos] = prk[i];
		pos++;
	}

	//��������� �������� ��������������.
	QByteArray realPrk = unModifyProductKey(baPk);

	string retStr = "green fir";
	retStr = realPrk.toStdString();

	return retStr;
}

void SerialNumTools::incrementDig(int & d)
{
	d = d + 1;
}

QByteArray SerialNumTools::unModifyProductKey(QByteArray In)
{

	QByteArray baR;
	unsigned char tmp;

	//��������� ��������� �������� ���.
	for (uint i = 70; i < 100;i++)
	{
		tmp = static_cast<unsigned char>(i);
		tmp = (tmp + 8) >> 2;
		baR.append(tmp);
	}

	//�������� ������.
	unsigned char tmp1;
	unsigned char tmp2;

	QByteArray baRet;
	baRet.resize(29);

	for (uint i = 0;i < 30;i++)
	{
		tmp = static_cast<unsigned char>(In[i]);
		tmp1 = static_cast<unsigned char>(baR[i]);
		tmp2 = tmp1^tmp;
		baRet[i] = tmp2;
	}


	return baRet;
}

qint64 SerialNumTools::daysEnd(string pkDateEnd)
{
	//��������� ����, � �������� ��������������.
	//�������������� ������.
	QString time_format = "dd.MM.yyyy";
	QString t_str = QString::fromStdString(pkDateEnd);
	QDateTime dtEnd = QDateTime::fromString(t_str, time_format);

	QDate curDate = QDateTime::currentDateTime().date(); //������� ����.
	QDate dateEnd = dtEnd.date(); //���� ��������� �������� �����.

	 //�������� ����� ���� �� ������� ���� �� ���� dateEnd. ���� ������� ���� ������-��� ����� �������������.
	qint64 daysEnd = curDate.daysTo(dateEnd);


	return daysEnd;
}

int SerialNumTools::dayAnalys(qint64 d)
{
	//������ ��� ���� �������� ����� ����������.
	if (d == 0)  return 15;

	if(d<0) return 15; //���� �������� ���������� �����.
	
	//�������� ������ ��� ������ �� ��������� ����� �������� �����.
	if(d<=7) return 14;

	
	//���� �������� � ��� ���� �������� �� �������� � �����.
	return 1;
}

bool SerialNumTools::che�kInt(int x, qint64 d)
{
	//�������� ��� ����� ��� ����������� ��������������, ������� ���������� ���������.

	bool ret = false;
	qint64 a = abs(d); 

	int t = 1;
	int cnt = 0;
	for (int i = 0;i < a;i++)
	{
		t = t << 1;

		if (cnt >= 10)
		{
			if (x == 2) ret = true;
		    if (x == 1) ret = true;
			if (x == 21) ret = true;
			if (x == 0) ret = false;
			if (x == 8) ret = true;
			if (x == 15) ret = true;
			break;
		}
		else
		{
			if (x == 15) ret = true;
			if (x == 2) ret = true;
			if (x == 0) ret = false;
			if(x==1) ret = true;
			if (x == 21) ret = true;
			if (x == 8) ret = true;						
		}

		if (t < 0) break;
	}
	
	return ret;
}

//��������� ����������� ����� �����.
bool SerialNumTools::CheckCrc(QByteArray Fd)
{	
		//������� CRC.
	uint8_t a, b, c, d;
	a = 0x37;
	b = 0xca;
	c = 0x09;
	d = 0x71;

	uint8_t b0, b1, b2, b3;
		
	//������� ��� ����� ��������� 16 ����.
	QByteArray Ba;
	for (int i = 0;i < Fd.size() - 16;i++)
	{
		Ba.append(Fd[i]);
	}


	//������������ ������ ��� ���������� crc.
	QByteArray Ba1(Ba);

	b0 = Ba1[3];
	b1 = Ba1[4];
	b2 = Ba1[5];
	b3 = Ba1[6];

	b0 = b0^a;
	b1 = b1^a;
	b2 = b2^a;
	b3 = b3^a;

	Ba1[3] = b0;
	Ba1[4] = b1;
	Ba1[5] = b2;
	Ba1[6] = b3;
		
	//������ ��� ���������� ��� ����� �� ��������� SHA-3 512.
	QCryptographicHash   Crc(QCryptographicHash::Md5);
	Crc.addData(Ba1);
	QByteArray baCrc = Crc.result(); //���.
	
	//��������� ����������� �����.
	bool CrcIsGood = true; //����� ��� ����������� ����� �����.
	int sz = Fd.size(); 
	uint8_t op1 = 0, op2 = 0;
	for (int i = 0;i < 16;i++)
	{
		op1 = Fd[sz - 16+i];
		op2 = baCrc[i];
		 
		if (op1 != op2)
		{
			CrcIsGood = false;
			break;
		}
	}


	return CrcIsGood;
}

//�������� �������� ���,����������� �������� ������������������, ������������ ���� ����������.
int SerialNumTools::GetZeroNumberSeries(QByteArray &Ba)
{
	HMODULE hLib;
	LPCWSTR dllName = L"libVr5b.dll";

	//������������� dll �����������! ��� ������� ��������� ���������, � � ���� ���������� ����������������� ����.
	//��������� ������ ��������������.
	hLib = LoadLibrary(dllName);

	//�� ������� ��������� dll.
	if (hLib==0)
	{
		//"Error BN256: Corrupted structure program.";
		return -256;
	}

	/*  This is test.
	typedef int (*DllTest1)(void);
	DllTest1 F1 = (DllTest1)GetProcAddress(hLib,"DllTest");
	if (F1 == 0)
	{
		string str = "Error BN257: Corrupted structure program.";
		ShowError(str);
		return 0;
	}

	int k = F1();
	*/

	//������� ������ � ������� ������� ������ �������� ������� ���������� �����.
	QByteArray ChekBlock;
	ChekBlock.resize(128); //���������� �����.
	

	char *d_ptr = ChekBlock.data(); //������� ��������� �� ������ ������.

	//int Curdeprecon(int a, int b, char *c)
	typedef int(*FuncCur)(unsigned int a, unsigned int b, QByteArray &c);
	FuncCur FCrd = (FuncCur)GetProcAddress(hLib, "Curdeprecon");

	if (FCrd == 0)
	{
		string str = "Error BN257: Corrupted structure program.";
		return -257;
	}

	//��� ���������� ���� ��� ������� ���������������.
	unsigned int a = 275, b = 329, c = 0;
	c = a + b;
	
	//������� ��� ������� ���������� � ������ �����.
	int kuks = FCrd(a,c,ChekBlock);
	c = c + kuks;

	//��� ���������� ���� ��� ������ ��������� ���������������.
	if (c != 4885)
	{
		string str = "Error BN258: Corrupted structure program.";
		return -258;
	}

	//��� ���������� ���� ��� ������ ��������� ���������������.
	for (int i = 0;i < 128;i++) Ba[i] = ChekBlock[i];
	
	
	return 1;
}

//��������� �������� libVr5b.dll. ���������� �������� ��������� ���� ������� ������ ������ ����������� � ����������
//� ������������ ����������. � ������ ������ =1, ��� ������=0;
int SerialNumTools::CheckZeroNumberSeries(QByteArray &Ba)
{
	uint8_t arr[10];
	int a = 0;
	int b = 0;
	int c = 0;

	//168
	a = 7;
	a = 7 * 8; //56
	b = 112;
	c = a + b; //168
	arr[0] = c;

	//246
	a = a<<1; //56*2=112
	c = a + b; //=224
	b = 11;
	b = b << 1; //22
	c = c + b;
	arr[1] = c;

	//128
	a = arr[1];
	b = 1 << 4;
	c = a + b;

	//������������ ���������� 1, ��� ���������� ������� ���������������.
	if ((c & 1) == 0) c = 1;
	else
	{
		c = c & 1;
	}

	//��������� ����� 128
	a = 3;
	b = 4;
	a = a + b;

	b = 0;
	for (int x = 0;x < a;x++)
	{
		c = c << 1;
		b = c;
	}


	arr[2] = b;
	
	bool IsGood = true; //����� ��� ��� ���������.

	//��������.
	if (arr[0] != (uint8_t)(Ba[0])) IsGood = false;
	if (arr[1] != (uint8_t)(Ba[1])) IsGood = false;
	if (arr[2] != (uint8_t)(Ba[2])) IsGood = false;

	//�������� � �������� �����.
	if (!IsGood) return 0;

	return 1;
}


/*
�������� ������ ����� ��������, ������������ � ����������.
=0-��������� ���� � �� �������� ��������� ������.
=1-��������� �������� �������� �����.
=-1 -�������� ������.
�� ���� ��������� ����������� ��������������� ���, � ���� ������������ � �����.
*/
int SerialNumTools::GetSnStatusArr(QByteArray NumSeries, QByteArray BlockFromFile)
{
	int lastP = 0;//�������

	//��������� ����������� ����1. 0-14;
	for (int i = 0; i < 15; i++)
	{
		if (NumSeries[i] != BlockFromFile[i])
		{
			string mess = "Error BN258: Corrupted structure program."; //�������� ��������������, ���� ���������� �� �������.
			return -258;
		}

		lastP++;
	}

	//���������� ��������������, �������� ����� 29.
	int z = getDig30();
	z = lastP + z;//15+30
	
	int t = 1;
	int end = z + (t << 5); // z+32


	//[44 - 77] - 32 �����, �� �������� ������ ��������� �����.������ ��� �������� ������������ �����.
	for (int k =z; k < end; k++)
	{
		if (NumSeries[k] != BlockFromFile[k])
		{
			string mess = "Error BN259: Corrupted structure program.";
			return -259;
		}
	}


	//�������� ������� ���� �� �������� �� �������� ����� � ����������.
	bool IsCompare = true; //���� ���� ��� � ���� ���������� ������ ��������� ����� � ��� ���������.
  //	[15 - 40] - 25 �������� ������������� ��������� ������, ���� ������������ ���� � ��������� �������� �����.�����
	//	������ ��������� ����� ����������� ���������� ������� ��������� ������.
	for (int r = 15; r < 41; r++)
	{
		if (NumSeries[r] != BlockFromFile[r])
		{
			IsCompare = false;
			break;
		}
	}
	

	if (!IsCompare)  return 1;  //� ���������� ���������� �������� �����.
	
	//=0-��������� ���� � �� �������� ��������� ������.

	return 0;
}

/*
�������� ������ ��������� ������.
=0-���  ��������� ������.
=1-���� �������� �����.
=-1 -�������� ������.
*/
int SerialNumTools::AnalysProductKey()
{
	QByteArray BaPrk; //������ ������� �����, ������� ������ �������� � �������� ������ �� ����� ����������.
	BaPrk.resize(128); //��������� ��������� �� ������� 128.

	int errFlag = 0;
	//������� �� ���������� ���� �������� �������� � ����� ��������.
	errFlag = GetSerialDataBlock(BaPrk);
	if (errFlag == 0) return -1; //�������� ������.

	QByteArray ControlSeries; //����������� �������� ������������������, ������������ ���� ����������.
	ControlSeries.resize(128);

	errFlag = GetZeroNumberSeries(ControlSeries); //������� ����������� ������������������.
	if (errFlag == 0) return -1; //�������� ������.

	errFlag = CheckZeroNumberSeries(ControlSeries); //�������� ���.
	if (errFlag == 0) return -1; //�������� ������.

	//���������� ������� ����� �������� � ����������, � ��� �� ��������� ��� ���� ������.
	int flagPk = GetSnStatusArr(ControlSeries, BaPrk);

	if (flagPk < 0)  //������.
	{
		return -1;
	}

	//���� ��������� �������� ���� ��������.
	if (flagPk==1)
	{
	    //������� �� ���������� ���� ��������.
	     string strPk=getPK(BaPrk); 
		 //������� �� �������� �������� ������� ���������� ����!!!
		 
		//�������� ����� ��������.
		 CheckSerial CPk;
		 std::string pkDateEnd = "01.01.1905"; //���� ��������� �������� ����� ��������.

		 int xReti = 12;
		 QString ProduktKey = QString::fromStdString(strPk);
		 xReti = CPk.CheckProductKey(ProduktKey, pkDateEnd);

		 //��������� ����, � �������� ��������������.

		 //���������� ���������� ���� �� ��������� �������� �����.
		 //���� ���� �������� ���������� ���������� ����� � �������.
		 qint64 days = daysEnd(pkDateEnd);

		 //���� �������� ��� �������� ����� ��������, ��� �� ������ ����� ��������, ������� � ��� ��� ��� ����� ��������.
		 if (!che�kInt(xReti, days)) return 0;

		 //���������� ���������� ���� � ��������� ������ ����.
		 return dayAnalys(days);

	}

	return flagPk;
}

int SerialNumTools::PutProdukKeyToConteiner(string ProductKey)
{

	//���� ������ �������� ���������� ��� ������ �� ��������������, ������� �������� ��� ������ ������������ � ���������
	//�������� ���������.

	//libapr_tsvn32.dll
	QString ExePath = QDir::currentPath(); // /
	QString PathToFile = ExePath + "/" + productKeyContainerName;

	//�������� ������������� ����� ����������.
	QFileInfo file_info(PathToFile);
	if (!file_info.exists()) return -15;

	//�������� ������� �����.
	qint64 f_size = file_info.size();

	if (f_size != 149268)
	{
		//"Error BN252: Corrupted structure program.";
		return -252;
	}

	//�������� ����������� ����� �����.
	QFile file(PathToFile);
	if (!file.open(QIODevice::ReadOnly))
	{
		//"Error BN253: Corrupted structure program.";
		return -253;
	}

	QByteArray Fd = file.readAll(); //��������� ��� ������ � ����� � ������ data
	file.close();

	if (Fd.size() != 149268)
	{
		//"Error BN254: Corrupted structure program.";
		return -254;
	}

	//�������� ����������� ����� �����.
	if (!CheckCrc(Fd))
	{
		// "Error BN255: Corrupted structure program.";
		return -255;
	}

	//�������������� � ������������� ����� ��������.
	QByteArray  mPrKey=modifyProductKey(ProductKey);
	int pos = 0;

	//��������� � ���������(������� ��������� � ������) ���������������� ���� ��������.
	putProductKey(Fd,mPrKey);

	//������� ����������� �����
	QByteArray newFile = trim16Ba(Fd);

	//���������� ����������� ����� ��� ���������� ����� ������.
 	QByteArray baCrc=getCrc(newFile);
		
	//�������� ����������� ����� � �����(��������� 16 ����).
	putCrc(newFile, baCrc);

	//�������� ������ � �����.
	if (!writeBaToFile(newFile,file))
		return -51;


	return 0;
}

int SerialNumTools::lockAnalys(int Stts)
{
	/*
	����������� ��������� ����� ��������.
	=0-��� ����� �������� � ����������.
	=1-���� �������� ��������, � ��� ���� �������� �� �������� � �����.
	=14-���� �������� ��������,�� ��� ���� �������� �������� ����� ��� ����� ������. �������� �������� ������������ �����
	�� ����� �������� ����� �������� ������ ��� ������.
	=15 ���� �������� ����� ����������.
	=-1-�������� ������.
	*/

	//�������� ������.
	if (Stts<0)
	{
		emit lockControls(); //������� ������ ���������� �����������.
		return -10;
	}

	//��� ������.
	if (Stts == 1) return 5;
	if (Stts == 14) return 6; //�������� � ����� ���� �������� �����.


	//�������� ��� ����� ��� ����������� ��������������.
	switch (Stts)
	{
	     case -1:
		emit lockControls();
		break;


	   case 0:  
		  emit lockControls();
	   break;

	   case 14:
		   emit lockControls();
	   break;

	default:
		emit lockControls();
		break;
	}


	return -10;
}

string SerialNumTools::getAlarmLicenseMessage()
{
	string str=" �� ����������� �� ������������������ ������ ���������. \n"
		" ���������� � ������������ �� ���������� ���������� � ����������� ���������.";

	return str;
}

string SerialNumTools::getAlarmErrorMessage(int errCode)
{
	string AlarmMessage = "��� ������ ��������� �������� ����������� ������.\n";
	int eNum = std::abs(errCode);
	//Error BN258: Corrupted structure program.
	string er = "������ BN" + std::to_string(errCode) + ": ����������� ��������� ���������.";
	AlarmMessage = AlarmMessage + er;
	
	return AlarmMessage;
}

string SerialNumTools::getAlarmMessageEndDays()
{
	string str = "";
	str = " ��������! ���� �������� ����� �������� �������� ����� ��� ����� ������. ";

	return str;
}



//��������� ������� ��� �������� ������� ��������� ������. ���������� ������ haveSerialNumber.
void SerialNumTools::analysisProductKey()
{
	//����������� ��������� ����� ��������.
	int pkStatus = AnalysProductKey();
	
	emit productKeyStatus(pkStatus); //��������� ������ � ��������� ����� ��������.

	lockAnalys(pkStatus);//�� ��������� �������� � ������� ����� ��������, ����� ������������� ������ ���������� ��������� emit lockControls().

	emit finished(); //��������� �����.
	
}

