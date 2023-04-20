#include "Gost3413.h"


Gost3413::Gost3413()
{
}


Gost3413::~Gost3413()
{
}


//Преобразовывает длину файла в массив из 16 ти байт
void Gost3413::FileLengthToArray(qint64 len, unsigned char *arr)
{
	for (int i = 0;i < 16;i++)
	{
		arr[i] = len & 0xFF;
		len = len >> 8;
	}
}

//Выполняет обратное преобразование длины
void Gost3413::ArrayToFileLength(qint64 &len, uint8_t *arr)
{
	len = 0;
	quint64 tmp = 0;
	quint64 tmp1 = 0;

	for (uint8_t i = 0;i < 8;i++)
	{
		tmp = 0;
		tmp = tmp | arr[i];
		tmp = tmp << i * 8;
		tmp1 = tmp1 | tmp;
	}
	len = static_cast<qint64>(tmp1);
}

//Копирует 16 старших бит из массива R в массив MSB
void Gost3413::getMSBfromR(uint8_t *R, uint8_t *MSB)
{
	/*
	for (uint8_t i = 0;i < 16;i++)
	{
	MSB[i] = R[i];
	}
	Старший значащий байт находится в нулевой ячейки регистра R
	т.е. R[0]- на самом деле 31 байт
	*/
	//Полагаю так будет работать быстрее

	MSB[0] = R[0];
	MSB[1] = R[1];
	MSB[2] = R[2];
	MSB[3] = R[3];
	MSB[4] = R[4];
	MSB[5] = R[5];
	MSB[6] = R[6];
	MSB[7] = R[7];

	MSB[8] = R[8];
	MSB[9] = R[9];
	MSB[10] = R[10];
	MSB[11] = R[11];
	MSB[12] = R[12];
	MSB[13] = R[13];
	MSB[14] = R[14];
	MSB[15] = R[15];

}

//Складывает блоки длиной 16 байт по модулю 2
void Gost3413::xor_blocks(uint8_t *block1, uint8_t *block2, uint8_t *result)
{
	for (uint8_t i = 0;i < 16;i++)
	{
		result[i] = block1[i] ^ block2[i];
	}
}

//P-открытый текст,MSB-значением n разрядов регистра сдвига с большими номерами,C-результирующий шифртекст
void Gost3413::сryptIterationCBC(Cipher3412 *Cipher, uint8_t *P, uint8_t  *MSB, uint8_t *C)
{
	xor_blocks(P, MSB, C);//Складываю  блоки
	Cipher->encryptBlock(C);//шифрую блок
}


void Gost3413::deсryptIterationCBC(Cipher3412 *Cipher, uint8_t *P, uint8_t  *MSB, uint8_t *C)
{
	Cipher->decryptBlock(C);//расшифровываю блок
	xor_blocks(C, MSB, P);//Складываю  блоки
}

//Сдвигаю регистр R влево на 16 байт, младшие ячейки заполняю значением шифротекста
void Gost3413::RegLeftShift(uint8_t *R, uint8_t *Ci)
{
	//Сдвиг
	R[0] = R[16];
	R[1] = R[17];
	R[2] = R[18];
	R[3] = R[19];
	R[4] = R[20];
	R[5] = R[21];
	R[6] = R[22];
	R[7] = R[23];
	R[8] = R[24];
	R[9] = R[25];
	R[10] = R[26];
	R[11] = R[27];
	R[12] = R[28];
	R[13] = R[29];
	R[14] = R[30];
	R[15] = R[31];


	//Заполнение младших бит значением блока шифротекста
	for (uint8_t i = 0;i < 16;i++)
	{
		R[16 + i] = Ci[i];
	}

}


//Записывает блок результирующего шифротекста С в файл, начиная с указанной позиции, в случае ошибки возвращет 0
bool Gost3413::writeBlockC(qint64 &pos, uint8_t *C, QFile &file)
{
	//Устанавливаю позицию в pos
	if (!file.seek(pos)) return 0;

	uint8_t l = (uint8_t)file.write(reinterpret_cast<char *>(C), 16);
	if (l != 16) return 0; //Проблемы с записью
	pos = pos + 16;

	return 1;
}

//Записываю блок указанной длины
bool Gost3413::writeBlock(qint64 &pos, uint8_t *C, QFile &file, uint8_t len)
{
	//Устанавливаю позицию в pos
	if (!file.seek(pos)) return 0;

	uint8_t l = (uint8_t)file.write(reinterpret_cast<char *>(C), len);
	if (l != len) return 0; //Проблемы с записью

	return 1;
}

//Записывает начальный вектор(32байта) начиная с позиции, в случае ошибки возвращет 0
bool Gost3413::writeBlockIV(qint64 &pos, uint8_t *IV, QFile &file)
{
	//Устанавливаю позицию в pos
	if (!file.seek(pos)) return 0;

	uint8_t l = (uint8_t)file.write(reinterpret_cast<char *>(IV), 32);
	if (l != 32) return 0; //Проблемы с записью

	return 1;
}

//Копирует 16 байт типа chat в uint8_t, используется для чтения файла
void Gost3413::copyFtoC(char *F_block, uint8_t *C_block)
{
	for (uint8_t i = 0;i < 16;i++)
	{
		C_block[i] = static_cast<uint8_t>(F_block[i]);
	}
}

//Копирует len байт типа chat в uint8_t, используется для чтения файла
void Gost3413::copyFtoC(char *F_block, uint8_t *C_block, uint8_t len)
{
	for (uint8_t i = 0;i < len;i++)
	{
		C_block[i] = static_cast<uint8_t>(F_block[i]);
	}
}

//Дополняю сообщение согласно ГОСТ 34.13-2015 4.1.2 Процедура 2
void Gost3413::paddingMessage(uint8_t *C_block, uint8_t r_len)
{
	//Ситуация когда длина оставшегося сообщения =15
	if (r_len == 15)
	{
		C_block[15] = 1;//Дополняю блок 1
		return;
	}

	//Сообщение не равно 15
	C_block[r_len] = 1;

	for (uint8_t i = r_len + 1;i < 16;i++)
	{
		C_block[i] = 0;
	}


}

//Считывает значение вектора iv 
bool Gost3413::readIV(uint8_t *IV, QFile &file)
{
	//Устанавливаю текущую позицию(считываемый файл) на начало определения размера блока данных
	if (!file.seek(118)) return 0;

	//Считывание размера блока данных
	uint8_t arr_len[8];
	char F_block[32];

	if (file.read(F_block,8) != 8)	return 0;
	for (uint8_t i = 0;i < 8;i++)  arr_len[i] = static_cast<uint8_t>(F_block[i]);

	//Преобразовываю массив в длину
	quint64 data_len = 0;
	quint64 tmp = 0;

	for (uint8_t i = 0;i < 8;i++)
	{
		tmp = 0;
		tmp = tmp | arr_len[i];
		tmp = tmp << i * 8;
		data_len = data_len | tmp;
	}
	
	//Вычисляю позицию в которой заканчиваются данные и начинается iv
	data_len = data_len + 118 + 8; //Позиция конца блока данных
	data_len = data_len - 32;//Начало блока iv
	//Устанавливаю текущую позицию(считываемый файл) на начало iv
	if (!file.seek(data_len)) return 0;

	//Считываю 32 байт из файла
	if (file.read(F_block, 32) != 32) return 0;
	
	//Конвертирую
	for (uint8_t i = 0;i < 32;i++)
	{
		IV[i] = static_cast<uint8_t>(F_block[i]);
	}

	return 1;
}
//Считывает блок по указанной позиции
bool Gost3413::readFblock(qint64 &pos, char *F_block, QFile &file)
{
	//Устанавливаю текущую позицию(считываемый файл)
	if (!file.seek(pos))
	{
		std::cout << "can t seek curpos" << std::endl;
		return 0;
	}

	if (file.read(F_block, 16) != 16)
	{
		std::cout << "can t read" << std::endl;
		return 0;
	}

	pos = pos + 16;
	return 1;
}
//Создает начальный вектор. Вектор должен являться случайной величиной
bool Gost3413::generateIV(uint8_t * IV)
{
	/*
	  iv 32 байта , за один раз ГСЧ генерирует 8 байт, нужно 32/8=4 итерации
	*/
	quint64 rnd = 0;//Значение полученное из ГСЧ
	uint8_t pos = 0; //Позиция в массиве
	for (uint8_t i = 0;i < 4;i++)
	{
		rnd = QRandomGenerator::system()->generate64();
		//Раскладываю число на байты
		for (uint8_t b = 0;b < 8;b++)
		{
			IV[pos] = rnd & 0xff;
			rnd >>= 8;
			pos++;
		}
		//QThread::msleep(i*3);
	}
	
	return true;
}

//Выполняет тестирование режима простой замены с зацеплением, в случае ошибки возвращет 0
bool Gost3413::TestCryptCBC()
{
	/* Старший бит находится в нулевой ячейке
	[0] [1] [2]-массив
	12   34 56 7890abcef0a1b2c3d4e5f0011223344556677889901213141516171819
	по факту старший значащий бит идет первым  т.е.
	[31] [30] [29]-реальное значение байт
	12   34 56 78
	*/
	//Начальный вектор 2*n	
	const uint8_t iv[32] =
	{
		0x12,0x34,0x56,0x78,0x90,0xab,0xce,0xf0,0xa1,0xb2,0xc3,0xd4,0xe5,0xf0,0x01,0x12,
		0x23,0x34,0x45,0x56,0x67,0x78,0x89,0x90,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19
	};


	//Секретный ключ
	const uint8_t secretKey[32] =
	{
		0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
		0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
	};
	//Открытый текст – четыре 128 - битных блока :
	uint8_t text[4][16] =
	{
		{ 0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x00,0xff,0xee,0xdd,0xcc,0xbb,0xaa,0x99,0x88 },
		{ 0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xee,0xff,0x0a },
		{ 0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xee,0xff,0x0a,0x00 },
		{ 0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xee,0xff,0x0a,0x00,0x11 }

	};

	//Значения выходного блока для проверки итераций
	const uint8_t etalon_values[4][16] =
	{
		{ 0x68,0x99,0x72,0xd4,0xa0,0x85,0xfa,0x4d,0x90,0xe5,0x2e,0x3d,0x6d,0x7d,0xcc,0x27 },
		{ 0x28,0x26,0xe6,0x61,0xb4,0x78,0xec,0xa6,0xaf,0x1e,0x8e,0x44,0x8d,0x5e,0xa5,0xac },
		{ 0xfe,0x7b,0xab,0xf1,0xe9,0x19,0x99,0xe8,0x56,0x40,0xe8,0xb0,0xf4,0x9d,0x90,0xd0 },
		{ 0x16,0x76,0x88,0x06,0x5a,0x89,0x5c,0x63,0x1a,0x2d,0x9a,0x15,0x60,0xb6,0x39,0x70 }
	};



	//Регистр размером m=kn=2*16;k=2
	uint8_t Register[32];
	uint8_t P[16]; //Блок сообщения подлежащего шифрованию
	uint8_t MSB[16];//значением n разрядов регистра сдвига с большими номерами
	uint8_t C_block[16];

	Cipher3412 Cipher; //Шифр
					   //Развертывание ключа
	Cipher.deploymentEncryptionRoundKeys(secretKey);

	//Заполнение регистра данными IV
	for (int i = 0;i < 32;i++)
	{
		Register[i] = iv[i];
	}

	//Шифрование 4-x блоков
	for (int i = 0;i < 4;i++)
	{
		getMSBfromR(Register, MSB);//Получаю старшие байты MSB из регистра
		сryptIterationCBC(&Cipher, text[i], MSB, C_block); //Обработка блока в режиме простой замены с зацеплением
														   //Проверка результата
		for (uint8_t c = 0;c < 16;c++)
		{
			if (C_block[c] != etalon_values[i][c]) return 0;
		}
		//Сдвиг регистра влево
		RegLeftShift(Register, C_block);

	}

	return 1;
}


bool Gost3413::TestDecryptCBC()
{
	/* Старший бит находится в нулевой ячейке
	[0] [1] [2]-массив
	12   34 56 7890abcef0a1b2c3d4e5f0011223344556677889901213141516171819
	по факту старший значащий бит идет первым  т.е.
	[31] [30] [29]-реальное значение байт
	12   34 56 78
	*/
	//Начальный вектор 2*n	
	const uint8_t iv[32] =
	{
		0x12,0x34,0x56,0x78,0x90,0xab,0xce,0xf0,0xa1,0xb2,0xc3,0xd4,0xe5,0xf0,0x01,0x12,
		0x23,0x34,0x45,0x56,0x67,0x78,0x89,0x90,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19
	};


	//Секретный ключ
	const uint8_t secretKey[32] =
	{
		0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
		0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
	};


	//Значения выходного блока для проверки итераций
	uint8_t crypted_text[4][16] =
	{
		{ 0x68,0x99,0x72,0xd4,0xa0,0x85,0xfa,0x4d,0x90,0xe5,0x2e,0x3d,0x6d,0x7d,0xcc,0x27 },
		{ 0x28,0x26,0xe6,0x61,0xb4,0x78,0xec,0xa6,0xaf,0x1e,0x8e,0x44,0x8d,0x5e,0xa5,0xac },
		{ 0xfe,0x7b,0xab,0xf1,0xe9,0x19,0x99,0xe8,0x56,0x40,0xe8,0xb0,0xf4,0x9d,0x90,0xd0 },
		{ 0x16,0x76,0x88,0x06,0x5a,0x89,0x5c,0x63,0x1a,0x2d,0x9a,0x15,0x60,0xb6,0x39,0x70 }
	};

	//Открытый текст – четыре 128 - битных блока :
	uint8_t text[4][16] =
	{
		{ 0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x00,0xff,0xee,0xdd,0xcc,0xbb,0xaa,0x99,0x88 },
		{ 0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xee,0xff,0x0a },
		{ 0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xee,0xff,0x0a,0x00 },
		{ 0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xee,0xff,0x0a,0x00,0x11 } //0x11

	};


	//Регистр размером m=kn=2*16;k=2
	uint8_t Reg[32];
	uint8_t P[16]; //Блок сообщения подлежащего шифрованию
	uint8_t MSB[16];//значением n разрядов регистра сдвига с большими номерами
	uint8_t C_block[16];

	Cipher3412 Cipher; //Шифр					
	Cipher.deploymentDecryptionRoundKeys(secretKey);   //Развертывание ключа

													   //Заполнение регистра данными IV
	for (int i = 0;i < 32;i++)Reg[i] = iv[i];

	for (int i = 0;i < 4;i++)
	{
		getMSBfromR(Reg, MSB);//Получаю старшие байты MSB из регистра
							  //Копирую блок данных, так как измениться содержимое C_block
		memcpy(C_block, crypted_text[i], 16);
		deсryptIterationCBC(&Cipher, P, MSB, C_block);//Расшифровываю

													  //Проверяю расшифрованное сообщение
		for (int c = 0;c < 16;c++)
		{
			if (P[c] != text[i][c]) return 0;
		}

		//Сдвиг регистра влево, помещая в регистр шифрованное сообщение
		RegLeftShift(Reg, crypted_text[i]);
	}

	return 1;
}



void Gost3413::CryptFileCBC(QString src_path, QString dst_path, const uint8_t *iv, const uint8_t *key)
{
	/*
	Для работы блочного шифра файл должен быть разбит на блоки. Но не факт что размер файл вложиться в нужное количество блоков.
	Поэтому определяем длину файла в байтах.
	Первые 64 байта шифрованного сообщения содержат длину реального сообщения.
	Если для последнего блока не хватает байт, он дополняется до нужной длины нулями.
	*/
	QFileInfo file_info(src_path);
	qint64 file_len = file_info.size(); //Размер исходного файла в байтах

										//!!!!!!!!!!!!!!Предусмотреть ситуацию когда размер файла менее 16 байт!!!!!!!!!!!!!!!!1
	qint64 block_count = file_len / 16; //Количество блоков содержащихся в сообщении

										//Открыть файл для чтения,откуда будут браться данные
	QFile src_file(src_path);
	if (!src_file.open(QIODevice::ReadOnly))
	{
		//return StrCodec->toUnicode("Не удалось открыть файл макета по пути :") + FilePath;
		std::cout << "can t open src" << std::endl;
		return;
	}


	//Открыть файл для записи, куда будет записываться шифрованный файл
	QFile fileOut(dst_path);
	if (!fileOut.open(QIODevice::WriteOnly))
	{
		//return StrCodec->toUnicode("Не удалось сохранить файл");
		std::cout << "can t open dst" << std::endl;
		return;
	}

	//Блок подлежащий шифрованию
	uint8_t P[16]; //Блок сообщения подлежащего шифрованию
	uint8_t MSB[16];//значением n разрядов регистра сдвига с большими номерами
	uint8_t C_block[16]; //результирующий шифротекст
	uint8_t Reg[32]; // Регистр размером m = kn =  2*16
	uint8_t Flen[16];//Заголовок-размер файла в виде 16 байт
	char input_block[16]; //Блок данных считываемых из файла подлежащего шифрованию

	qint64 readf_pos = 0;//Текущая позиция считываемого блока из файла
	qint64 writef_pos = 0;//Текущая записываемого блока в файл
	bool GoodOpFlag = 0;//Флаг успешности выполнения операции

						
						//Массив для временного хранения данных
	//uint8_t tmp_data[16];
	/*
	down vote accepted
	Loading whole large files in memory, be it QVector or QByteArray is probably not a good solution.
	Assuming the files have some kind of structure, you should use QFile::seek to position yourself at
	the start of a "record" and use qint64 QIODevice::read ( char * data, qint64 maxSize ) to read
	one record at a time in a buffer of your choice.
	*/


	//Устанавливаю позицию в считываемом файле в 0
	if (!src_file.seek(0))
	{
		std::cout << "can t seek 0" << std::endl;
		return;
	}

	//Заполнение регистра данными IV
	for (uint8_t i = 0;i < 32;i++) Reg[i] = iv[i];


	Cipher3412 Cipher;
	//Развертывание секретного ключа
	Cipher.deploymentEncryptionRoundKeys(key);

	//DeL!!!!
	//for (int i = 0;i < 16;i++)C_block[i] = 0;
	//Заполнение заголовка длиной файла
	FileLengthToArray(file_len, Flen);

	//Шифрование размера файла
	getMSBfromR(Reg, MSB);//Получаю MSB из регистра
	сryptIterationCBC(&Cipher, Flen, MSB, C_block);
	//Сохраняю блок шифротекста в файл
	GoodOpFlag = writeBlockC(writef_pos, C_block, fileOut);
	if (!GoodOpFlag)
	{
		std::cout << "can t write block" << std::endl;
		return;
	}

	//Сдвиг регистра влево
	RegLeftShift(Reg, C_block);


	for (qint64 i = 0;i < block_count;i++)
	{
		//Устанавливаю текущую позицию(считываемый файл)
		if (!src_file.seek(readf_pos))
		{
			std::cout << "can t seek curpos" << std::endl;
			return;
		}
		//Считываю 16 байт из файла
		if (src_file.read(input_block, 16) != 16)
		{
			std::cout << "can t read" << std::endl;
			return;
		}

		//Преобразоываю char в uint8_t
		copyFtoC(input_block, P);
		//Итерация шифрования
		getMSBfromR(Reg, MSB);//Получаю MSB из регистра
		сryptIterationCBC(&Cipher, P, MSB, C_block);//шифрую блок
													//Запись шифротектса
		GoodOpFlag = writeBlockC(writef_pos, C_block, fileOut);
		if (!GoodOpFlag)
		{
			std::cout << "can t write block" << std::endl;
			return;
		}

		//Сдвиг регистра влево
		RegLeftShift(Reg, C_block);

		readf_pos += 16;

		//std::cout << i << std::endl;
	}

	quint64 last_bytes = file_len % 16; //Количество оставшихся байт в файле
										//Если требуется дополнение сообщения
	if (last_bytes > 0)
	{
		//Читаю значение оставшихся байт
		//Считываю x байт из файла
		if (src_file.read(input_block, last_bytes) != last_bytes)
		{
			std::cout << "can t read" << std::endl;
			return;
		}

		uint8_t r_len = static_cast<uint8_t>(last_bytes);//Длина оставшегося сообщения
														 //Копирую оставшиеся байты
		copyFtoC(input_block, P, r_len);
		//Дополнение сообщения
		paddingMessage(P, r_len);
		//Шифрование последнего блока
		//Итерация шифрования
		getMSBfromR(Reg, MSB);//Получаю MSB из регистра
		сryptIterationCBC(&Cipher, P, MSB, C_block);//шифрую блок
													//Запись шифротектса
		GoodOpFlag = writeBlockC(writef_pos, C_block, fileOut);
		if (!GoodOpFlag)
		{
			std::cout << "can t write block" << std::endl;
			return;
		}
	}
	//Запись вектора iv
	for (uint8_t v = 0;v < 32;v++) Reg[v] = iv[v];

	GoodOpFlag = writeBlockIV(writef_pos, Reg, fileOut);
	if (!GoodOpFlag)
	{
		std::cout << "can t write block" << std::endl;
		return;
	}
	src_file.close();
	fileOut.close();
}

void Gost3413::DecryptFileCBC(QString src_path, QString dst_path, const uint8_t *key)
{
	//Открыть файл для чтения,откуда будут браться данные
	QFile src_file(src_path);
	if (!src_file.open(QIODevice::ReadOnly))
	{
		//return StrCodec->toUnicode("Не удалось открыть файл макета по пути :") + FilePath;
		std::cout << "can t open src" << std::endl;
		return;
	}


	//Открыть файл для записи, куда будет записываться расшифрованный файл
	QFile fileOut(dst_path);
	if (!fileOut.open(QIODevice::WriteOnly))
	{
		//return StrCodec->toUnicode("Не удалось сохранить файл");
		std::cout << "can t open dst" << std::endl;
		return;
	}


	uint8_t P[16]; //Блок сообщения подлежащего шифрованию
	uint8_t MSB[16];//значением n разрядов регистра сдвига с большими номерами
	uint8_t C_block[16]; //результирующий шифротекст
	uint8_t tmp_block[16]; //хранит временные данные
	uint8_t Reg[32]; // Регистр размером m = kn =  2*16
	uint8_t Flen[16];//Заголовок-размер файла в виде 16 байт
	char input_block[16]; //Блок данных считываемых из файла подлежащего расшифрованию
	uint8_t iv[32]; //Начальный вектор считанный из файла
	qint64 block_count;// = file_len / 16; //Количество блоков содержащихся в сообщении

	qint64 readf_pos = 0;//Текущая позиция считываемого блока из файла
	qint64 writef_pos = 0;//Текущая записываемого блока в файл
	bool GoodOpFlag = 0;//Флаг успешности выполнения операции
	qint64 src_file_len = 0;//Длина расшифровуемого сообщения
	qint64 data_len = 0;//Длина блока данных 
	uint8_t  r_len = 0;//Остаток в последнем блоке



					   //Считать значение начального вектора IV
	QFileInfo f1info(src_path);
	src_file_len = f1info.size(); //Размер файла
	readf_pos = src_file_len - 32;//Позиция где начинается iv

								  //Считывает значение вектора iv по указанной позиции
//	GoodOpFlag = readIV(readf_pos, iv, src_file);
	if (!GoodOpFlag)
	{
		std::cout << "can t read iv" << std::endl;
		return;
	}
	//Cчитываю первые 16 байт, длина сообщения
	readf_pos = 0;
	GoodOpFlag = readFblock(readf_pos, input_block, src_file);
	if (!GoodOpFlag)
	{
		std::cout << "can t read file" << std::endl;
		return;
	}
	//Преобразовываю данные
	copyFtoC(input_block, C_block);


	Cipher3412 Cipher; //Шифр					
	Cipher.deploymentDecryptionRoundKeys(key);   //Развертывание ключа

												 //Заполнение регистра данными IV
	for (int i = 0;i < 32;i++)Reg[i] = iv[i];

	//Пытаюсь расшифровать длину сообщения
	getMSBfromR(Reg, MSB);//Получаю старшие байты MSB из регистра
	memcpy(tmp_block, C_block, 16);
	deсryptIterationCBC(&Cipher, P, MSB, C_block);//Расшифровываю
												  //Сдвиг регистра влево, помещая в регистр шифрованное сообщение
	RegLeftShift(Reg, tmp_block);


	ArrayToFileLength(data_len, P);
	//Добавить проверку размера данных!!!!!!

	//Длину файла мы обработали 16, IV считали IV
	block_count = (src_file_len - 48) / 16; //Количество блоков в сообщении

											//Кратна ли длина сообщения размеру блока шифра?
	r_len = static_cast<uint8_t>(data_len % 16);
	//Не кратна
	if (r_len != 0) block_count = block_count - 1; //Выполняю все итерации кроме последней


												   /*
												   Цикл расшифровывания блоков
												   */
	for (qint64 i = 0;i<block_count;i++)
	{
		//Считываю очередной блок
		GoodOpFlag = readFblock(readf_pos, input_block, src_file);
		if (!GoodOpFlag)
		{
			std::cout << "can t read file" << std::endl;
			return;
		}

		copyFtoC(input_block, C_block); //Преобразовываю данные

		getMSBfromR(Reg, MSB);//Получаю старшие байты MSB из регистра
							  //Копирую блок данных, так как измениться содержимое C_block
		memcpy(tmp_block, C_block, 16);
		deсryptIterationCBC(&Cipher, P, MSB, C_block);//Расшифровываю

		writeBlockC(writef_pos, P, fileOut); //Записываю расшифрованный блок в файл

											 //Сдвиг регистра влево, помещая в регистр шифрованное сообщение
		RegLeftShift(Reg, tmp_block);

	}

	//Длина сообщения не кратна размеру блока шифра
	if (r_len != 0)
	{
		//Считываю очередной блок
		GoodOpFlag = readFblock(readf_pos, input_block, src_file);
		if (!GoodOpFlag)
		{
			std::cout << "can t read file" << std::endl;
			return;
		}

		copyFtoC(input_block, C_block); //Преобразовываю данные
		getMSBfromR(Reg, MSB);//Получаю старшие байты MSB из регистра				  
		memcpy(tmp_block, C_block, 16); //Копирую блок данных, так как измениться содержимое C_block
		deсryptIterationCBC(&Cipher, P, MSB, C_block);//Расшифровываю

		writeBlock(writef_pos, P, fileOut, r_len);//Записываю расшифрованный блок в файл
	}

	src_file.close();
	fileOut.close();

}