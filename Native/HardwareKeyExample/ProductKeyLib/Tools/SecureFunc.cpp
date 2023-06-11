#include "SecureFunc.h"

void SecureFunc::SfCRC(QByteArray &xi)
{
	unsigned char  k[11]; 

	int a, b, c, d, t;
	int pos = 0;

	//Формирую некоторый секретный ряд. Ряд был протестирован. Дрогой ряд "отбалды" использовать нельзя!
	for (int i = 5; i < 60; i = i + 5)
	{
		a = -1;
		b = a * a * i;

		c = 2 * i - 1;
		d = (c - b) * (3 * i);
		t = d & 0xff;//Обрезаю байт.
		k[pos] = static_cast<unsigned char>(t);
		pos++;
	}

	unsigned char x1 = 0;
	unsigned char x2 = 0;
	unsigned char y = 0;
	//Замешиваю байты.
	for (int i = 0;i<11;i++)
	{
		x1 = xi[i];
		x2 = k[i];
		y = x1 ^ x2;
		xi[i] = static_cast<unsigned char>(y);
	}
}

//Вращения 32 битного кольца. Сдвиг с обратной связью-"Вращение кольца".
void SecureFunc::TwistTheRing(int rotate, bool * r)
{
	bool tmp1 = false;
	bool tmp2 = false;

	//Вращение кольца n-раз.
	for (int i = 0; rotate < 5; i++)
	{
		//Замкнутый сдвиг блока размером 32 бита.
		tmp1 = r[31]; //Копирую последний бит который станет первым.              
		for (int j = 0; j < 32; j++)
		{
			tmp2 = r[j]; //Сохраняю текущее значение бита.
			r[j] = tmp1;//Сдвигаю
			tmp1 = tmp2;//Подготавливаю для следующего сдвига.
		}
	}
}

//Подсчитывает количество единиц в массиве длиной len
int SecureFunc::CountUnits(bool val[], int len)
{
	int cnt = 0;

	for (int i = 0; i < len; i++)
	{
		if (val[i] == true) cnt++;
	}

	return cnt;
}

//Заполняет массив ArSF0 данными некоторой функции. 
void SecureFunc::FillArSF0()
{
	qint64 y, x;
	quint64 ret;

	for (int i = 1; i < SF_LEN; i++)
	{
		x = i * (-15200);
		y = (715 * x) + 25300;
		ret = (static_cast<quint64>(y)) & 0xffffffff;
		ret = ret << 16;
		ArSF0[i - 1] = ret;
	}
}

//Конвертирует битовый массив в UInt64.
void SecureFunc::ConvertBaseNumUint(bool *baseNum, quint64 & result)
{
	bool In[32]; //Временный массив.
	//Очищаю массив.
	for (int i = 0;i < 32;i++) In[i] = false;

	//Копирую базовое число во временный массив.
	for (int i = 0; i < 26; i++)
	{
		In[i] = baseNum[i];
	}

	quint32 bVAL[32]; //Значения 2ки в соответствующей степени.

	bVAL[0] = 0x00000001;//1
	bVAL[1] = 0x00000002;//2
	bVAL[2] = 0x00000004;//4
	bVAL[3] = 0x00000008;//8
	bVAL[4] = 0x00000010;//16
	bVAL[5] = 0x00000020;//32
	bVAL[6] = 0x00000040;//64
	bVAL[7] = 0x00000080;//128
	bVAL[8] = 0x00000100;//256
	bVAL[9] = 0x00000200;//512
	bVAL[10] = 0x00000400;//1024
	bVAL[11] = 0x00000800;//2048
	bVAL[12] = 0x00001000;//4096
	bVAL[13] = 0x00002000;//8192
	bVAL[14] = 0x00004000;//16384
	bVAL[15] = 0x00008000;//32768
	bVAL[16] = 0x00010000;//65536
	bVAL[17] = 0x00020000;//131072
	bVAL[18] = 0x00040000;//262144
	bVAL[19] = 0x00080000;//524288
	bVAL[20] = 0x00100000;//1048576
	bVAL[21] = 0x00200000;
	bVAL[22] = 0x00400000;
	bVAL[23] = 0x00800000;
	bVAL[24] = 0x01000000;
	bVAL[25] = 0x02000000;
	bVAL[26] = 0x04000000;
	bVAL[27] = 0x08000000;
	bVAL[28] = 0x10000000;
	bVAL[29] = 0x20000000;
	bVAL[30] = 0x40000000;
	bVAL[31] = 0x80000000;//2147483648
	
	quint64 Res = 0;//Результат преобразования.

	for (int i = 0; i < 32; i++)
	{
		if (In[i] == true) //1 в разряде
		{
			Res = Res + bVAL[i]; //Преобразовываем.
		}
	}

	result = Res;
}

void SecureFunc::ConvertBaseNumInt(bool *BaseNum, qint64 & result)
{
	bool In[32]; //Временный массив.
				 //Очищаю массив.
	for (int i = 0;i < 32;i++) In[i] = false;

							  //Копирую базовое число во временный массив.
	for (int i = 0; i < 26; i++)
	{
		In[i] = BaseNum[i];
	}

	qint64 tmp = 0;
	result = 0;

	for (int i = 0; i < 32; i++)
	{
		if (In[i] == true)
		{
			tmp = 1;
			tmp = tmp << i; //2 в соответсвующей степени.
			result = result + tmp;
		}

	}

}

void SecureFunc::ConvertU64ToBool(quint64 d, bool * Result)
{
	//Обнуляю массив. 
	for (int k = 0; k < 32; k++) Result[k] = 0;
	

	quint64 tmp = 0;
	quint64 PowOF2 = 0;//Степень двойки.

	for (int i = 0; i < 32; i++)
	{
		PowOF2 = 1;
		PowOF2 = PowOF2 << i; //Возведение в степень 2ки.

		tmp = d & PowOF2; //Вырезание не нужных бит.
						  //Сравнение
		if (tmp == PowOF2) //Присутствует 1 текущем разряде.
			Result[i] = true;
	}

}

void SecureFunc::ConvertI64ToBool(qint64 d, bool * Result)
{
	for (int i = 0; i < 32; i++) Result[i] = 0; //Обнуляю массив.

	qint64 tmp = 0;
	qint64 PowOF2 = 0;//Степень двойки.

	for (int i = 0; i < 32; i++)
	{
		PowOF2 = 1;
		PowOF2 = PowOF2 << i; //Возведение в степень 2ки.

		tmp = d & PowOF2; //Вырезание не нужных бит.
						  //Сравнение
		if (tmp == PowOF2) //Присутствует 1 текущем разряде.
			Result[i] = true;
		else
			Result[i] = false;

	}

}

void SecureFunc::SF0(bool * BaseNum, bool * Dst)
{
	//Вращаю  базовое число.
	TwistTheRing(48,BaseNum);

	//Количество единиц в числе.
	int UnitsCnt = CountUnits(BaseNum, 32);

	FillArSF0();//Формирую значения некоторой функции.
	quint64 a = ArSF0[UnitsCnt]; //Получаю значение соответствующее текущему количеству.
	quint64 tmp1 = 0; //хранит промежуточное значение
	quint64 mask = 1;//Маска.
	bool tmp = false;//хранит промежуточное значение

					 //Выполняю  побитовое сложение по модулю 2.
	for (int i = 0; i < 26; i++)
	{
		mask = 1; //Готовлю бит для сдвига.
		mask = mask << i;//Устанавливает 1 в нужном бите.
		tmp1 = a & mask; //Вырезаем нужный бит.

						 //Если в бите единица тогда результат равен маске, иначе в бите не единица.
		if (tmp1 == mask) tmp = true;
		else tmp = false;

		Dst[i] = BaseNum[i] ^ tmp;
	}
}

void SecureFunc::SF1(bool * BaseNum, bool * Dst)
{
	quint64 x = 0;
	ConvertBaseNumUint(BaseNum,x); //Конвертируем базовое число в виде байт массива в нормальное число.

	quint64 y;
	y = (x * x) + 3578;

	//Обратное преобразование
	ConvertU64ToBool(y,Dst);
}

void SecureFunc::SF2(bool * BaseNum, bool * Dst)
{
	quint64 x = 0;
	//Конвертируем базовое число в виде байт массива в нормальное число.
	ConvertBaseNumUint(BaseNum,x);

	quint64 y;
	y = (x * x) + (x * 2) - 2354;

	//Обратное преобразование
	ConvertI64ToBool(y,Dst);
}

void SecureFunc::SF3(bool * BaseNum, bool * Dst)
{
	quint64 x = 0;
	//Конвертируем базовое число в виде байт массива в нормальное число.
	ConvertBaseNumUint(BaseNum,x);

	quint64 y;

	y = (715 * x) + 25300;

	//Обратное преобразование
	ConvertI64ToBool(y,Dst);
}

void SecureFunc::SF4(bool * BaseNum, bool * Dst)
{
	quint64 x = 0;
	//Конвертируем базовое число в виде байт массива в нормальное число.
	ConvertBaseNumUint(BaseNum,x);

	quint64 y;
	y = 2793967291 - ((x * x) - (x * 7) - 5718);

	//Обратное преобразование
	ConvertI64ToBool(y,Dst);
}

void SecureFunc::SF5(bool * BaseNum, bool * Dst)
{
	quint64 x = 0;
	//Конвертируем базовое число в виде байт массива в нормальное число.
	ConvertBaseNumUint(BaseNum,x);

	quint64 y, t;

	y = ((x * x) - x * 7 - 5718);
	t = 2 * (x * x) - x * 7 - 10287;
	y = (y + t) + 78;

	//Обратное преобразование
	ConvertI64ToBool(y,Dst);
}

void SecureFunc::SF6(bool * BaseNum, bool * Dst)
{
	quint64 x = 0;
	//Конвертируем базовое число в виде байт массива в нормальное число.
	ConvertBaseNumUint(BaseNum,x);

	quint64 y;
	y = 31979673410 - ((x * 7 + 57158) / 80);

	//Обратное преобразование
	ConvertI64ToBool(y,Dst);
}

void SecureFunc::SF7(bool * BaseNum, bool * Dst)
{
	quint64 x = 0;
	//Конвертируем базовое число в виде байт массива в нормальное число.
	ConvertBaseNumUint(BaseNum, x);

	quint64 y, t;

	y = (x * x * 5) + (x * 3) - 15324;
	t = y >> 2;
	y = y | t;

	//Обратное преобразование
	ConvertI64ToBool(y,Dst);
}

void SecureFunc::SF8(bool * BaseNum, bool * Dst)
{
	quint64 x = 0;
	//Конвертируем базовое число в виде байт массива в нормальное число.
	ConvertBaseNumUint(BaseNum,x);

	quint64 y, t;

	y = (x * x * 3) + x * 2 - 215324;
	t = (y >> 4) + (x * x);
	y = y & t;

	//Обратное преобразование
	ConvertI64ToBool(y, Dst);
}
