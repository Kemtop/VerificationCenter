#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "Gost3412\Cipher3412.h"
#include <QFileInfo>
#include <QRandomGenerator>

#pragma once
//Набор методов для реализации режимов работы блочного шифра согласно ГОСТ 34.13-2015 
class Gost3413
{
public:
	Gost3413();
	~Gost3413();

	//Выполняет тестирование режима простой замены с зацеплением, в случае ошибки возвращет 0
	//ГОСТ 34.13-2015
	bool TestCryptCBC();
	bool TestDecryptCBC();

	//5.4 Режим простой замены с зацеплением
	void CryptFileCBC(QString src_path, QString dst_path, const uint8_t *iv, const uint8_t *key);
	void DecryptFileCBC(QString src_path, QString dst_path, const uint8_t *key);

	//Преобразовывает длину файла в массив из 16 ти байт
	//[15] [...] [1][0]
	void FileLengthToArray(qint64 len, unsigned char *arr);
	//Выполняет обратное преобразование длины
	void ArrayToFileLength(qint64 &len, uint8_t *arr);
	//Складывает блоки длиной 16 байт по модулю 2
	void xor_blocks(uint8_t *block1, uint8_t *block2, uint8_t *result);

	//Итерация в режиме простой замены(ГОСТп5.4стр16)
	//P-открытый текст,MSB-значением n разрядов регистра сдвига с большими номерами,C-результирующий шифртекст
	void сryptIterationCBC(Cipher3412 *Cipher, uint8_t *P, uint8_t  *MSB, uint8_t *C);
	void deсryptIterationCBC(Cipher3412 *Cipher, uint8_t *P, uint8_t  *MSB, uint8_t *C);

	//Cipher3412
	//Копирует 16 старших бит из массива R в массив MSB
	void getMSBfromR(uint8_t *R, uint8_t *MSB);
	//Сдвигаю регистр R влево на 16 байт, младшие ячейки заполняю значением шифротекста
	void RegLeftShift(uint8_t *R, uint8_t *Ci);

	//Записывает блок результирующего шифротекста С в файл, начиная с указанной позиции, в случае ошибки возвращет 0
	//Выполняет инкремент pos на размер блока
	bool writeBlockC(qint64 &pos, uint8_t *C, QFile &file);
	//Записываю блок указанной длины
	bool writeBlock(qint64 &pos, uint8_t *C, QFile &file, uint8_t len);

	//Записывает начальный вектор(32байта) начиная с позиции, в случае ошибки возвращет 0
	bool writeBlockIV(qint64 &pos, uint8_t *IV, QFile &file);

	//Копирует 16 байт типа chat в uint8_t, используется для чтения файла
	void copyFtoC(char *F_block, uint8_t *C_block);
	//Копирует len байт типа chat в uint8_t, используется для чтения файла
	void copyFtoC(char *F_block, uint8_t *C_block, uint8_t len);

	//Дополняю сообщение согласно ГОСТ 34.13-2015 4.1.2 Процедура 2
	void paddingMessage(uint8_t *C_block, uint8_t r_len);
	//Считывает значение вектора iv 
	bool readIV(uint8_t *IV, QFile &file);

	//Считывает блок по указанной позиции
	bool readFblock(qint64 &pos, char *F_block, QFile &file);
	//Создает начальный вектор. Вектор должен являться случайной величиной
	bool generateIV(uint8_t *IV);


};

