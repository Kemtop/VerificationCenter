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

	//Проверка существования файла
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


	//Получаю контрольную сумму блока данных.
	 QByteArray baCrc = getCrc(Ba);
	 
	//Добавляю контрольную сумму к файлу(последние 16 байт).
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

	//Проверка существования файла
	QFileInfo file_info(PathToFile);
	if (!file_info.exists())
	{
		// "Error BN251: Corrupted structure program.";
		return -251;
	}

	//Проверка размера файла.
	qint64 f_size = file_info.size();

	if (f_size != 149268)
	{
		//"Error BN252: Corrupted structure program.";
		return -252;
	}
		
	//Проверка контрольной суммы файла.
	QFile file(PathToFile);
	if (!file.open(QIODevice::ReadOnly ))
	{
		//"Error BN253: Corrupted structure program.";
		return -253;
	}

	QByteArray Fd = file.readAll(); //считываем все данные с файла в объект data
	file.close();

	if (Fd.size() != 149268)
	{	
		//"Error BN254: Corrupted structure program.";
		return -254;
	}

	//Проверяю контрольную сумму файла.
	if (!CheckCrc(Fd))
	{
		// "Error BN255: Corrupted structure program.";
		return -255;
	}

	//149268/32=4664
	quint64 sizeOfRead = 0; //Количество считанных байт за раз.


	//Вычисляем позицию блока серийного номера в файле. Она=524.
	unsigned int a;
	unsigned int x0 = 0;
	unsigned int x1 = 71;
	a = get512( x0,  x1); //Особо странным образом получаю число 512, для защиты от взлома.
	 //При некоторых условиях возвращает 12, нужна для запутывания злоумышленника при анализе кода.
	x0 = 3; x1 = x1 & 2;
	unsigned int a1 = getDui(x0, x1);
	x0 = a + a1; //=524

	int Rpos = 0;
	uint8_t tmp1 = 0;

	for (int b = 0;b < 4660;b++)
	{
     	//Начиная с 525 блока.
		if ((b>static_cast<int>(x0)) && (Rpos<128))
		{
			//Копирую блоки во внешний массив.
				tmp1 = Fd[b];
				Ba[Rpos] = tmp1;
				Rpos ++;
		}
	}


	return 1;
}


//При некоторых условиях возвращает 512, нужна для запутывания злоумышленника при анализе кода.
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

//При некоторых условиях возвращает 12, нужна для запутывания злоумышленника при анализе кода.
unsigned int SerialNumTools::getDui(unsigned int x0, unsigned int x1)
{
	unsigned int a = 0; //Типо основание.
	a = x0; //Должно быть 3;
	unsigned int tmp = 0;
	unsigned int b = 0;

	//x1=Количество умножения на 2 и сложения;
	for (unsigned int i = 0;i < x1;i++)
	{
		b = a << 1; //умножаем на 2.
		tmp = tmp + b;
	}

	return tmp;
}

QByteArray SerialNumTools::modifyProductKey(string ProductKey)
{
	//Ключ продукта содержит 5 блоков по 5 символов и 4 разделителя, т.е. 29  символов.
	if(ProductKey.length() != 29) return QByteArray();

	QByteArray baProductKey(ProductKey.c_str(), ProductKey.length()); //Строка в массив.
	
	QByteArray baR;
	unsigned char tmp;

	//Генерирую некоторый числовой ряд.
	for (uint i = 70;i < 100;i++)
	{
		tmp = static_cast<unsigned char>(i);
		tmp = (tmp + 8)>>2;
		baR.append(tmp);
	}

	//Мутируем данные.
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
	//Модифицируем данные для вычисления crc.
	/*
	   Это необходимо для запутывания злоумышленника.
	   Очевидно он будет пытаться использовать стандартные алгоритмы для расчета контрольной суммы -
	   но ни один из стандартных алгоритмов не сможет рассчитать правильно crc, так как данные подлежащие хешированию
	   предварительно изменяются.
	   Потребуется глубокий анализ дизасемблированного кода.
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

	//Объект для вычисления хэш файла по алгоритму SHA-3 512.
	QCryptographicHash   Crc(QCryptographicHash::Md5);
	Crc.addData(Ba1);
	QByteArray baCrc = Crc.result(); //Хэш.
	
	return baCrc;
}

QByteArray SerialNumTools::trim16Ba(QByteArray Ba)
{
	QByteArray outBa;
	int l = Ba.size()-16;
	//Изменяем размер массива до необходимого.
	outBa.resize(l);

	//Копируем нужные нам данные.
	for (int i = 0;i < l;i++) outBa[i] = Ba[i];


	return outBa;
}

void SerialNumTools::putProductKey(QByteArray & Ba, QByteArray ProductKey)
{
	//Вычисляем позицию блока серийного номера в файле. Она=524.
	unsigned int a;
	unsigned int x0 = 0;
	unsigned int x1 = 71;
	a = get512(x0, x1); //Особо странным образом получаю число 512, для защиты от взлома.
						//При некоторых условиях возвращает 12, нужна для запутывания злоумышленника при анализе кода.
	x0 = 3; x1 = x1 & 2;
	unsigned int a1 = getDui(x0, x1);
	x0 = a + a1; //=524

	x1 = 8;
	a = x1<<1; //8 *2=16
	x0 = x0 + a; //524+16=540
	uint8_t tmp1 = 0;


	int pos = 0;

	//Сохраняем в контейнер модифицированный ключ продукта.
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
	//Обновляю данные в файле.
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

//Выполняет некоторые математические операции приводящие к получению числа 29.
int SerialNumTools::getDig30()
{
	int a, b, c;
	c = -1;
	a = 1;b = 1;

	a = a*c;
	b = b*c;
	a = 1;
	b = a << 5; //Присваиваем 32
	a = a << 1;
	c = (b - a); // 30

	return c;
}

string SerialNumTools::getPK(QByteArray prk)
{
	int lastP = 0;//Позиция

    //Запутываем злоумышленника, получаем число 15.
	for (int i = 0; i < 15; i++)
	{
         incrementDig(lastP);  //Запутываем злоумышленника,делаем инкримент переменной. 
	}

	//Запутываем злоумышленника, получаем число 29.
	int z = getDig30()+ lastP;

	//Контейнер содержащий строку с модифицированным ключем продукта.
	QByteArray baPk;
	baPk.resize(29);
	
	//Копирую 29байт серийного номера. 
	//Начиная с lastP=15 и до z=45
	int pos = 0;
	for (int i = lastP;i < z;i++)
	{
		baPk[pos] = prk[i];
		pos++;
	}

	//Выполняем обратное преобразование.
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

	//Генерирую некоторый числовой ряд.
	for (uint i = 70; i < 100;i++)
	{
		tmp = static_cast<unsigned char>(i);
		tmp = (tmp + 8) >> 2;
		baR.append(tmp);
	}

	//Мутируем данные.
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
	//Проверить даты, и выводить предупреждение.
	//Преобразование данных.
	QString time_format = "dd.MM.yyyy";
	QString t_str = QString::fromStdString(pkDateEnd);
	QDateTime dtEnd = QDateTime::fromString(t_str, time_format);

	QDate curDate = QDateTime::currentDateTime().date(); //Текущая дата.
	QDate dateEnd = dtEnd.date(); //Дата окончания действия ключа.

	 //Получаем число дней от текущей даты до даты dateEnd. Если текущая дата меньше-дни будут отрицательные.
	qint64 daysEnd = curDate.daysTo(dateEnd);


	return daysEnd;
}

int SerialNumTools::dayAnalys(qint64 d)
{
	//Ошибка или срок действия ключа закончился.
	if (d == 0)  return 15;

	if(d<0) return 15; //Срок действия закончился давно.
	
	//Осталась неделя или меньше до окончания срока действия ключа.
	if(d<=7) return 14;

	
	//Ключ валидный и его срок действия не подходит к концу.
	return 1;
}

bool SerialNumTools::cheсkInt(int x, qint64 d)
{
	//Странный код нужен для запутывания злоумышленника, который взламывает программу.

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

//Проверяет контрольную сумму файла.
bool SerialNumTools::CheckCrc(QByteArray Fd)
{	
		//Считаем CRC.
	uint8_t a, b, c, d;
	a = 0x37;
	b = 0xca;
	c = 0x09;
	d = 0x71;

	uint8_t b0, b1, b2, b3;
		
	//Копирую все кроме последних 16 байт.
	QByteArray Ba;
	for (int i = 0;i < Fd.size() - 16;i++)
	{
		Ba.append(Fd[i]);
	}


	//Модифицируем данные для вычисления crc.
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
		
	//Объект для вычисления хэш файла по алгоритму SHA-3 512.
	QCryptographicHash   Crc(QCryptographicHash::Md5);
	Crc.addData(Ba1);
	QByteArray baCrc = Crc.result(); //Хэш.
	
	//Сравниваю контрольную сумму.
	bool CrcIsGood = true; //Думаю что контрольная сумма верна.
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

//Получает числовой ряд,контрольная числовая последовательность, генерируемая спец алгоритмом.
int SerialNumTools::GetZeroNumberSeries(QByteArray &Ba)
{
	HMODULE hLib;
	LPCWSTR dllName = L"libVr5b.dll";

	//Использование dll обязательно! Она убивает некоторые отладчики, и в разы затрудняет дизасемблирование кода.
	//Затрудняя работу злоумышленнику.
	hLib = LoadLibrary(dllName);

	//Не удалось загрузить dll.
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

	//Область память в которую запишем чистое значение области контейнера ключа.
	QByteArray ChekBlock;
	ChekBlock.resize(128); //Увеличиваю объем.
	

	char *d_ptr = ChekBlock.data(); //Получаю указатель на начало памяти.

	//int Curdeprecon(int a, int b, char *c)
	typedef int(*FuncCur)(unsigned int a, unsigned int b, QByteArray &c);
	FuncCur FCrd = (FuncCur)GetProcAddress(hLib, "Curdeprecon");

	if (FCrd == 0)
	{
		string str = "Error BN257: Corrupted structure program.";
		return -257;
	}

	//Для усложнения кода при анализе злоумышленником.
	unsigned int a = 275, b = 329, c = 0;
	c = a + b;
	
	//Получаю ряд который содержится в пустом файле.
	int kuks = FCrd(a,c,ChekBlock);
	c = c + kuks;

	//Для усложнения кода при взломе программы злоумышленником.
	if (c != 4885)
	{
		string str = "Error BN258: Corrupted structure program.";
		return -258;
	}

	//Для усложнения кода при взломе программы злоумышленником.
	for (int i = 0;i < 128;i++) Ba[i] = ChekBlock[i];
	
	
	return 1;
}

//Усложняет подделку libVr5b.dll. Сравнивает значение числового ряда который всегда должен содержаться в контейнере
//с вычисленными значениями. В случае успеха =1, при ошибке=0;
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

	//Замороченное присвоение 1, для усложнения анализа злоумышленником.
	if ((c & 1) == 0) c = 1;
	else
	{
		c = c & 1;
	}

	//Получение числа 128
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
	
	bool IsGood = true; //Думаю что все правильно.

	//Проверяю.
	if (arr[0] != (uint8_t)(Ba[0])) IsGood = false;
	if (arr[1] != (uint8_t)(Ba[1])) IsGood = false;
	if (arr[2] != (uint8_t)(Ba[2])) IsGood = false;

	//Проблемы с цифровым рядом.
	if (!IsGood) return 0;

	return 1;
}


/*
Получает статус ключа продукта, содержащемся в контейнере.
=0-контейнер пуст и не содержит серийного номера.
=1-контейнер содержит серийный номер.
=-1 -возникла ошибка.
На вход принимает контрольный псевдослучайный ряд, и блок содержащийся в файле.
*/
int SerialNumTools::GetSnStatusArr(QByteArray NumSeries, QByteArray BlockFromFile)
{
	int lastP = 0;//Позиция

	//Сравниваю контрольные блок1. 0-14;
	for (int i = 0; i < 15; i++)
	{
		if (NumSeries[i] != BlockFromFile[i])
		{
			string mess = "Error BN258: Corrupted structure program."; //Запутаем злоумышленника, если компилятор не выкинет.
			return -258;
		}

		lastP++;
	}

	//Запутываем злоумышленника, получаем число 29.
	int z = getDig30();
	z = lastP + z;//15+30
	
	int t = 1;
	int end = z + (t << 5); // z+32


	//[44 - 77] - 32 байта, не изменные псевдо случайные числа.Служат для проверки корректности файла.
	for (int k =z; k < end; k++)
	{
		if (NumSeries[k] != BlockFromFile[k])
		{
			string mess = "Error BN259: Corrupted structure program.";
			return -259;
		}
	}


	//Проверка наличия чего то похожего на серийный номер в контейнере.
	bool IsCompare = true; //Флаг того что в файл содержатся псевдо случайные числа и нет серийника.
  //	[15 - 40] - 25 символов кодированного серийного номера, если пользователь ввел в программе серийный номер.Иначе
	//	псевдо случайные числа позволяющие определить наличие серийного номера.
	for (int r = 15; r < 41; r++)
	{
		if (NumSeries[r] != BlockFromFile[r])
		{
			IsCompare = false;
			break;
		}
	}
	

	if (!IsCompare)  return 1;  //В контейнере содержится серийный номер.
	
	//=0-контейнер пуст и не содержит серийного номера.

	return 0;
}

/*
Получает статус серийного номера.
=0-нет  серийного номера.
=1-есть серийный номер.
=-1 -возникла ошибка.
*/
int SerialNumTools::AnalysProductKey()
{
	QByteArray BaPrk; //Данные участка файла, который хранит сведения о серийном номере из файла контейнера.
	BaPrk.resize(128); //Расширяем контейнер до размера 128.

	int errFlag = 0;
	//Получаю из контейнера блок хранящий сведения о ключе продукта.
	errFlag = GetSerialDataBlock(BaPrk);
	if (errFlag == 0) return -1; //Возникла ошибка.

	QByteArray ControlSeries; //Контрольная числовая последовательность, генерируемая спец алгоритмом.
	ControlSeries.resize(128);

	errFlag = GetZeroNumberSeries(ControlSeries); //Получаю контрольную последовательность.
	if (errFlag == 0) return -1; //Возникла ошибка.

	errFlag = CheckZeroNumberSeries(ControlSeries); //Проверяю ряд.
	if (errFlag == 0) return -1; //Возникла ошибка.

	//Определяет наличие ключа продукта в контейнере, а так же проверяет сам блок данных.
	int flagPk = GetSnStatusArr(ControlSeries, BaPrk);

	if (flagPk < 0)  //Ошибка.
	{
		return -1;
	}

	//Если контейнер содержит ключ продукта.
	if (flagPk==1)
	{
	    //Получаю из контейнера ключ продукта.
	     string strPk=getPK(BaPrk); 
		 //Неплохо бы добавить проверку наличия правильных букв!!!
		 
		//Проверка ключа продукта.
		 CheckSerial CPk;
		 std::string pkDateEnd = "01.01.1905"; //Дата окончания действия ключа продукта.

		 int xReti = 12;
		 QString ProduktKey = QString::fromStdString(strPk);
		 xReti = CPk.CheckProductKey(ProduktKey, pkDateEnd);

		 //Проверить даты, и выводить предупреждение.

		 //Возвращает количество дней до окончания действия ключа.
		 //Если срок действия закончился возвращает число с минусом.
		 qint64 days = daysEnd(pkDateEnd);

		 //Если проблемы при проверке ключа продукта, или не верный ключа продукта, говорим о том что нет ключа продукта.
		 if (!cheсkInt(xReti, days)) return 0;

		 //Анализирую количество дней и возвращаю нужный флаг.
		 return dayAnalys(days);

	}

	return flagPk;
}

int SerialNumTools::PutProdukKeyToConteiner(string ProductKey)
{

	//Ниже идущие проверки необходимы для защиты от злоумышленника, который пытается при помощи дизасемблера и отладчика
	//взломать программу.

	//libapr_tsvn32.dll
	QString ExePath = QDir::currentPath(); // /
	QString PathToFile = ExePath + "/" + productKeyContainerName;

	//Проверка существования файла контейнера.
	QFileInfo file_info(PathToFile);
	if (!file_info.exists()) return -15;

	//Проверка размера файла.
	qint64 f_size = file_info.size();

	if (f_size != 149268)
	{
		//"Error BN252: Corrupted structure program.";
		return -252;
	}

	//Проверка контрольной суммы файла.
	QFile file(PathToFile);
	if (!file.open(QIODevice::ReadOnly))
	{
		//"Error BN253: Corrupted structure program.";
		return -253;
	}

	QByteArray Fd = file.readAll(); //считываем все данные с файла в объект data
	file.close();

	if (Fd.size() != 149268)
	{
		//"Error BN254: Corrupted structure program.";
		return -254;
	}

	//Проверяю контрольную сумму файла.
	if (!CheckCrc(Fd))
	{
		// "Error BN255: Corrupted structure program.";
		return -255;
	}

	//Преобразование и трансформация ключа продукта.
	QByteArray  mPrKey=modifyProductKey(ProductKey);
	int pos = 0;

	//Сохраняем в контейнер(который находится в памяти) модифицированный ключ продукта.
	putProductKey(Fd,mPrKey);

	//Обрезаю контрольную сумму
	QByteArray newFile = trim16Ba(Fd);

	//Возвращает контрольную сумму для некоторого блока данных.
 	QByteArray baCrc=getCrc(newFile);
		
	//Добавляю контрольную сумму к файлу(последние 16 байт).
	putCrc(newFile, baCrc);

	//Обновляю данные в файле.
	if (!writeBaToFile(newFile,file))
		return -51;


	return 0;
}

int SerialNumTools::lockAnalys(int Stts)
{
	/*
	Анализирует состояние ключа продукта.
	=0-нет ключа продукта в контейнере.
	=1-ключ продукта валидный, и его срок действия не подходит к концу.
	=14-ключ продукта валидный,но его срок действия истекает менее чем через неделю. Значение начинает возвращаться когда
	до конца действия ключа остается неделя или меньше.
	=15 срок действия ключа закончился.
	=-1-возникла ошибка.
	*/

	//Возникла ошибка.
	if (Stts<0)
	{
		emit lockControls(); //Посылаю сигнал блокировки компонентов.
		return -10;
	}

	//Все хорошо.
	if (Stts == 1) return 5;
	if (Stts == 14) return 6; //Подходит к концу срок действия ключа.


	//Странный код нужен для запутывания злоумышленника.
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
	string str=" Вы используете не зарегистрированную версию программы. \n"
		" Обратитесь к разработчику за получением информации о регистрации программы.";

	return str;
}

string SerialNumTools::getAlarmErrorMessage(int errCode)
{
	string AlarmMessage = "При работе программы возникла критическая ошибка.\n";
	int eNum = std::abs(errCode);
	//Error BN258: Corrupted structure program.
	string er = "Ошибка BN" + std::to_string(errCode) + ": Повреждение структуры программы.";
	AlarmMessage = AlarmMessage + er;
	
	return AlarmMessage;
}

string SerialNumTools::getAlarmMessageEndDays()
{
	string str = "";
	str = " Внимание! Срок действия ключа продукта истекает менее чем через неделю. ";

	return str;
}



//Потоковая обертка для проверки наличия серийного номера. Генерирует сигнал haveSerialNumber.
void SerialNumTools::analysisProductKey()
{
	//Анализирует состояние ключа продукта.
	int pkStatus = AnalysProductKey();
	
	emit productKeyStatus(pkStatus); //Отправляю сигнал о состоянии ключа продукта.

	lockAnalys(pkStatus);//На основании сведений о статусе ключа продукта, может сгенерировать сигнал блокировки контролов emit lockControls().

	emit finished(); //Завершаем поток.
	
}

