#include "stdafx.h"
#include "VanishChest.h"


VanishChest::VanishChest()
{
}


VanishChest::~VanishChest()
{
}

//Модифицирует открытый ключ  схемы Диффи-Хэлмана. Массив может иметь любую размерность
void VanishChest::ModifyDHKey(QByteArray In, QByteArray &Out)
{
	uint8_t Lk[14]; //Правый операнд.

	Lk[0] = 0x7b;
	Lk[1] = 0xa7;
	Lk[2] = 0xa0;
	Lk[3] = 0xc8;
	Lk[4] = 0x72;
	Lk[5] = 0x10;
	Lk[6] = 0xe8;
	Lk[7] = 0x8c;

	Lk[8] = 0xaf;
	Lk[9] = 0x7b;
	Lk[10] = 0xd7;
	Lk[11] = 0x2a;
	Lk[12] = 0x78;
	Lk[13] = 0xa9;


	//Циклическое сложение по модулю 2 данных в In и Lk.

	uint8_t y, x1, x2;
	int pos = 0;
	for (int l = 0;l < In.size();l++)
	{
		x1 = In[l]; //Текущее значение.

		if (pos >= 14) pos = 0; //Циклически меняем значение Lk.

		x2 = Lk[pos];
		pos++;
		y = x1^x2;
		Out[l] = y;
	}

	//Перестановка значений. 
	//Таблица перестановки. [0]-позиция массиве которая переставляется на позицию [1]. [1] переставляется на позицию [0].
	uint8_t transposition[10][2] =
	{
		{ 0,25 },  //0
		{ 1,17 }, //1
		{ 2,31 }, //2
		{ 3,64 }, //3
		{ 4,37 }, //4
		{ 5,29 }, //5
		{ 6,36 }, //6
	    { 7,54 }, //7
	    { 8,71 }, //8
	    { 9,85 }, //9
	};

	int pos1 = 0; //Число в позиции которое меняется. 
	int pos2 = 0; //На какую позицию меняется.
	uint8_t val1 = 0;
	uint8_t val2 = 0;

	for (int i = 0;i < 10;i++)
	{
		pos1 = transposition[i][0];
		pos2 = transposition[i][1];

		val1 = Out[pos1];
		val2 = Out[pos2];
		//Замена
		 Out[pos1]= val2;
		 Out[pos2]= val1;
	}

}



//Из модифицированного состояния преобразовывает в нормальный вид открытый ключ  схемы Диффи-Хэлмана. 
void VanishChest::UnModifyDHKey(QByteArray In, QByteArray &Out)
{
	//Обратная перестановка значений. 
	//Таблица перестановки. [0]-позиция массиве которая переставляется на позицию [1]. [1] переставляется на позицию [0].
	uint8_t transposition[10][2] =
	{
		{ 0,25 },  //0
		{ 1,17 }, //1
		{ 2,31 }, //2
		{ 3,64 }, //3
		{ 4,37 }, //4
		{ 5,29 }, //5
		{ 6,36 }, //6
		{ 7,54 }, //7
		{ 8,71 }, //8
		{ 9,85 }, //9
	};

	int pos1 = 0; //Число в позиции которое меняется. 
	int pos2 = 0; //На какую позицию меняется.
	uint8_t val1 = 0;
	uint8_t val2 = 0;

	for (int i = 0;i < 10;i++)
	{
		pos1 = transposition[i][0];
		pos2 = transposition[i][1];

		val1 = In[pos1];
		val2 = In[pos2];
		//Замена
		In[pos1] = val2;
		In[pos2] = val1;
	}

	uint8_t Lk[14]; //Правый операнд.

	Lk[0] = 0x7b;
	Lk[1] = 0xa7;
	Lk[2] = 0xa0;
	Lk[3] = 0xc8;
	Lk[4] = 0x72;
	Lk[5] = 0x10;
	Lk[6] = 0xe8;
	Lk[7] = 0x8c;

	Lk[8] = 0xaf;
	Lk[9] = 0x7b;
	Lk[10] = 0xd7;
	Lk[11] = 0x2a;
	Lk[12] = 0x78;
	Lk[13] = 0xa9;


	//Циклическое сложение по модулю 2 данных в In и Lk.

	uint8_t y, x1, x2;
	int pos = 0;
	for (int l = 0;l < In.size();l++)
	{
		x1 = In[l]; //Текущее значение.

		if (pos >= 14) pos = 0; //Циклически меняем значение Lk.

		x2 = Lk[pos];
		pos++;
		y = x1^x2;
		Out[l] = y;
	}

}

//Из модифицированного состояния преобразовывает в нормальный вид открытый ключ  схемы Диффи-Хэлмана.
void VanishChest::UnModifyDHKey(QString & In, QString & Out)
{
	QByteArray baIn = QByteArray::fromHex(In.toUtf8());
	QByteArray baOut;
	baOut.reserve(baIn.size());
	UnModifyDHKey(baIn, baOut); //Выполняю обратное преобразование ключа.

	Out = baOut.toHex(); //Преобразовываю в шестнадцатеричную строку.
}

//Формирует специальную строку для проверки DHKey. Что то вроде контрольной суммы.
void VanishChest::CrcDHKey(QString &hash_str, QString &DHKey)
{
	//Преобразовываю строку в байт массив
	string str = DHKey.toLocal8Bit().constData();
	uint32_t bytes_count = str.length();


	uint8_t *arr = new uint8_t[bytes_count];
	//Преобразовываю строку в массив
	for (uint32_t i = 0;i < bytes_count;i++) arr[i] = static_cast<uint8_t>(str[i]);

	//Вычисляю хэш
	Hash3411 H;
	unsigned int mess_len;
	mess_len = bytes_count * 8;
	unsigned char hash256[32];

	H.hash_256(arr, (unsigned long long)mess_len, hash256);
	delete[]arr; arr = NULL;

	//Выполняю перестановку некоторых байтов.

	//Таблица перестановки. [0]-позиция массиве которая переставляется на позицию [1]. [1] переставляется на позицию [0].
	uint8_t transposition[9][2] =
	{
		{ 3,12 },  //0
		{ 1,30 }, //1
		{ 8,17 }, //2
		{ 0,28 }, //3
		{ 4,16 }, //4
		{ 7,22 }, //5
		{ 6,14 }, //6
		{ 5,18 }, //7
		{ 2,20 }, //8
	};

	int pos1 = 0; //Число в позиции которое меняется. 
	int pos2 = 0; //На какую позицию меняется.
	uint8_t val1 = 0;
	uint8_t val2 = 0;

	for (int i = 0;i < 9;i++)
	{
		pos1 = transposition[i][0];
		pos2 = transposition[i][1];

		val1 = hash256[pos1];
		val2 = hash256[pos2];
		//Замена
		hash256[pos1] = val2;
		hash256[pos2] = val1;
	}


	mpz_class hash2;
	mpz_import(hash2.get_mpz_t(), 32, 1, sizeof(unsigned char), 0, 0, hash256);
	string hash_string = hash2.get_str(16);

	hash_str = QString::fromStdString(hash_string);
}

//Формирует контрольную сумму для проверки ключа ЭЦП. Также используется как индекс для быстрого поиска строки  с необходимым
//сертификатом используя открытый ключ ЭЦП.
QString VanishChest::CrcECKey(QString x, QString y)
{
	QByteArray Qx = QByteArray::fromHex(x.toUtf8());
	QByteArray Qy = QByteArray::fromHex(y.toUtf8());

	QByteArray Qz; //Общий массив
	int Qxsize = Qx.size(); //Размеры массивов
	int Qysize = Qy.size();
	int Qzsize = Qxsize+ Qysize;
	Qz.resize(Qzsize);

	//Собираю оба массива вместе.
	for (int i = 0;i < Qysize;i++)
		Qz[i] = Qy[i];

	int pos = Qysize;
	for (int i = 0;i < Qxsize;i++)
	{
		Qz[pos] = Qx[i];
		pos++;
	}
		
	//Выполняю легкую перестановку данных.
	TranspECQ(Qz);
		
	//Объект для вычисления хэш файла по алгоритму SHA-3 512.
	QCryptographicHash   Crc(QCryptographicHash::Md5);
	Crc.addData(Qz);
	QByteArray crc_bA = Crc.result(); //Хэш.
	//Представляю содержимое как 16ричное значение.
	QString res = crc_bA.toHex(); //Преобразовываю в шестнадцатеричную строку.
	
	return res;
}


//Выполняет перестановку открытого ключа ЭЦП. Вспомогательная для CrcECKey.
void VanishChest::TranspECQ(QByteArray Qz)
{
	//Таблица перестановки. [0]-позиция массиве которая переставляется на позицию [1]. [1] переставляется на позицию [0].
	uint8_t transposition[3][2] =
	{
		{ 17,3 },  //0
		{ 28,12 }, //1
		{ 33,7 } //2
	};


	int pos1 = 0; //Число в позиции которое меняется. 
	int pos2 = 0; //На какую позицию меняется.
	uint8_t val1 = 0;
	uint8_t val2 = 0;

	for (int i = 0;i < 3;i++)
	{
		pos1 = transposition[i][0];
		pos2 = transposition[i][1];

		val1 = Qz[pos1];
		val2 = Qz[pos2];
		//Замена
		Qz[pos1] = val2;
		Qz[pos2] = val1;
	}
	
}


//Вычисляет значение хэш фушкции с использованием алгоритма MD5 на основании значений строк x,y.
string  VanishChest::QkeyMD5(string x, string y)
{
	string toHash = x + "b03af5" + y;
	QByteArray bA = QByteArray::fromHex(toHash.c_str());

	//Объект для вычисления хэш файла по алгоритму SHA-3 512.
	QCryptographicHash   Crc(QCryptographicHash::Md5);
	Crc.addData(bA);
	QByteArray crc_bA = Crc.result(); //Хэш.
									  //Представляю содержимое как 16ричное значение.
	QByteArray ba_as_hex_string = crc_bA.toHex();
	string result = ba_as_hex_string.toStdString();

	return result;
}

//Возвращает контрольную сумму пользовательского сертификата.
QString VanishChest::CrcUserCertificate(QByteArray cert)
{
	//Объект для вычисления хэш файла по алгоритму SHA-3 512.
	QCryptographicHash   Crc(QCryptographicHash::Sha3_512);
	Crc.addData(cert);
	QByteArray crc_bA = Crc.result(); //Хэш файла.
	QString HashHexStr(crc_bA.toHex()); //Представление массива в шестнадцатеричной форме.
	
	return HashHexStr;
}

//Возвращает контрольную сумму системного сертификата.
QString VanishChest::CrcSystemCertificate(QByteArray cert)
{
	//Объект для вычисления хэш файла по алгоритму SHA-3 512.
	QCryptographicHash   Crc(QCryptographicHash::Sha3_512);
	Crc.addData(cert);
	QByteArray crc_bA = Crc.result(); //Хэш файла.
	QString HashHexStr(crc_bA.toHex()); //Представление массива в шестнадцатеричной форме.

	return HashHexStr;
}

//Модифицирует строку с данными Ф И О пользователя в группе.
QString VanishChest::ModifyExUserFIO(QString & in)
{

	uint8_t Lk[17]; //C чем будет складываться.

	Lk[0] = 0xb3;
	Lk[1] = 0xc7;
	Lk[2] = 0x99;
	Lk[3] = 0xf3;
	Lk[4] = 0x15;
	Lk[5] = 0x36;
	Lk[6] = 0x31;
	Lk[7] = 0xcc;

	Lk[8] = 0x01;
	Lk[9] = 0x89;
	Lk[10] = 0xa2;
	Lk[11] = 0x12;
	Lk[12] = 0x3d;
	Lk[13] = 0x99;
	Lk[14] = 0x36;
	Lk[15] = 0x3f;
	Lk[16] = 0x57;


	//Циклическое сложение по модулю 2.
	QByteArray _In = in.toUtf8();
	QByteArray Out;
	Out.resize(_In.size());

	uint8_t y, x1, x2;
	int pos = 0;
	for (int l = 0;l < _In.size();l++)
	{
		x1 = _In[l]; //Текущее значение.

		if (pos >= 17) pos = 0; //Циклически меняем значение Lk.

		x2 = Lk[pos];
		pos++;
		y = x1^x2;
		Out[l] = y;
	}

	QString OutHexStr(Out.toHex()); //Представление массива в шестнадцатеричной форме.

	return OutHexStr;
}

//Превращаю  строку с данными Ф И О пользователя в группе в нормальный вид.
QString VanishChest::UnModifyExUserFIO(QString & in)
{
	QByteArray _In = QByteArray::fromHex(in.toUtf8());
	uint8_t Lk[17]; //C чем будет складываться.

	Lk[0] = 0xb3;
	Lk[1] = 0xc7;
	Lk[2] = 0x99;
	Lk[3] = 0xf3;
	Lk[4] = 0x15;
	Lk[5] = 0x36;
	Lk[6] = 0x31;
	Lk[7] = 0xcc;

	Lk[8] = 0x01;
	Lk[9] = 0x89;
	Lk[10] = 0xa2;
	Lk[11] = 0x12;
	Lk[12] = 0x3d;
	Lk[13] = 0x99;
	Lk[14] = 0x36;
	Lk[15] = 0x3f;
	Lk[16] = 0x57;


	//Циклическое сложение по модулю 2.
	QByteArray Out;
	Out.resize(_In.size());

	uint8_t y, x1, x2;
	int pos = 0;
	for (int l = 0;l < _In.size();l++)
	{
		x1 = _In[l]; //Текущее значение.

		if (pos >= 17) pos = 0; //Циклически меняем значение Lk.

		x2 = Lk[pos];
		pos++;
		y = x1^x2;
		Out[l] = y;
	}

	QString OutStr(Out);

	return OutStr;
}

QString VanishChest::ModifyEndDbDate(QString in)
{
   //Строка содержит 10 символов.
	QByteArray _In = in.toUtf8();

	//Массив биекции. [0]-исходный ряд,[1]-конечный ряд.
	uint8_t biekArr [10][2]=
	{
		{0,4},
		{ 1,8 },
		{ 2,7 },
		{ 4,9 },
		{ 7,6 },
		{ 6,0 },
		{ 5,3 },
		{ 9,1 },
		{ 8,2 },
		{ 3,5 }
	};

	uint8_t ar1[10]; //Ряд 2.

	uint8_t tmp1 = 14;

	for (int i = 0;i < 10;i++)
	{
		tmp1 = static_cast<uint8_t>(_In[biekArr[i][0]]);
			ar1[biekArr[i][1]] = tmp1;
	}

		
	uint8_t choper[10]=
	{
		15,
		18,
		123,
		47,
		56,
		99,
		11,
		23,
		48,
		3
	};


	QByteArray outBa;
	outBa.resize(10);
	uint8_t tmp2;
	for (int i = 0;i < 10;i++)
	{
		tmp1 = ar1[i];
		tmp2 = tmp1^choper[i];
		outBa[i]=tmp2;
	}

	QString OutHexStr(outBa.toHex());

	return OutHexStr;
}

QString VanishChest::UnModifyEndDbDate(QString in,bool &err)
{
	err = false;
	QByteArray _In = QByteArray::fromHex(in.toUtf8());
	if (_In.size() != 10) //Строка должна содержать 10 символов.
	{
		err = true;
		return "";
	}



	uint8_t ar1[10]; //Ряд 2.

	uint8_t choper[10] =
	{
		15,
		18,
		123,
		47,
		56,
		99,
		11,
		23,
		48,
		3
	};


	uint8_t tmp1 = 14;
	uint8_t tmp2;
	for (int i = 0;i < 10;i++)
	{
		tmp1 = _In[i];
		tmp2 = tmp1^choper[i];
		ar1[i] = tmp2;
	}
	

	// Массив биекции.[0] - исходный ряд, [1] - конечный ряд.
		uint8_t biekArr[10][2] =
	{
		{ 0,4 },
		{ 1,8 },
		{ 2,7 },
		{ 4,9 },
		{ 7,6 },
		{ 6,0 },
		{ 5,3 },
		{ 9,1 },
		{ 8,2 },
		{ 3,5 }
	};
	
	QByteArray outBa;
	outBa.resize(10);
	for (int i = 0;i < 10;i++)
	{
		tmp1=ar1[biekArr[i][1]];
		outBa[biekArr[i][0]]=static_cast<uint8_t>(tmp1);	
	}


	QString OutStr(outBa);

	return OutStr;
}

bool VanishChest::TestVanichEndDbDate()
{
	QString in = "abcdefghj9";
	QString modStr = ModifyEndDbDate(in);
	bool err;
	QString out = UnModifyEndDbDate(modStr,err);
	if (!err) return false;

	if(in!=out) return false;

	return true;
}

QString VanishChest::getCrcEndDbDate(QString in)
{
	QByteArray _In = in.toUtf8();
	_In.append(33);

	//Объект для вычисления хэш файла по алгоритму.
	QCryptographicHash   Crc(QCryptographicHash::Keccak_224);
	Crc.addData(_In);
	QByteArray crc_bA = Crc.result(); //Хэш файла.
	QString HashHexStr(crc_bA.toHex()); //Представление массива в шестнадцатеричной форме.

	return HashHexStr;
}

QString VanishChest::algHashSignKey(QString x, QString y)
{
	QString toHashStr = x +"werden"+ y+"coil";
	QByteArray Ba = toHashStr.toUtf8();

	unsigned long  Ba_len = static_cast<unsigned long>(Ba.length());  //Длина строки подлежащей хешированию.
														
	unsigned char *toHash = new unsigned char[Ba_len]; //Данные подлежащие хэшированию.
	//Копирование.
	for (int i = 0;i < Ba_len;i++)
	{
		toHash[i] = static_cast<unsigned char>(Ba[i]);
	}


	Hash3411 H;
	unsigned char hash[32];
	unsigned long  mess_len;
	mess_len = Ba_len * 8; //Длина сообщения в битах.
	H.hash_256(toHash,(unsigned long long)mess_len, hash);
	
	delete[]toHash;
	toHash = NULL;

	//Представление массива в шестнадцатеричной форме.
	QByteArray buf = QByteArray(reinterpret_cast<char*>(hash),32);
	QString HashHexStr(buf.toHex()); 

	return HashHexStr;
}



