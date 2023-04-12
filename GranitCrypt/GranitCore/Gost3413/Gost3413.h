#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "Gost3412\Cipher3412.h"
#include <QFileInfo>
#include <QRandomGenerator>

#pragma once
//����� ������� ��� ���������� ������� ������ �������� ����� �������� ���� 34.13-2015 
class Gost3413
{
public:
	Gost3413();
	~Gost3413();

	//��������� ������������ ������ ������� ������ � �����������, � ������ ������ ��������� 0
	//���� 34.13-2015
	bool TestCryptCBC();
	bool TestDecryptCBC();

	//5.4 ����� ������� ������ � �����������
	void CryptFileCBC(QString src_path, QString dst_path, const uint8_t *iv, const uint8_t *key);
	void DecryptFileCBC(QString src_path, QString dst_path, const uint8_t *key);

	//��������������� ����� ����� � ������ �� 16 �� ����
	//[15] [...] [1][0]
	void FileLengthToArray(qint64 len, unsigned char *arr);
	//��������� �������� �������������� �����
	void ArrayToFileLength(qint64 &len, uint8_t *arr);
	//���������� ����� ������ 16 ���� �� ������ 2
	void xor_blocks(uint8_t *block1, uint8_t *block2, uint8_t *result);

	//�������� � ������ ������� ������(�����5.4���16)
	//P-�������� �����,MSB-��������� n �������� �������� ������ � �������� ��������,C-�������������� ���������
	void �ryptIterationCBC(Cipher3412 *Cipher, uint8_t *P, uint8_t  *MSB, uint8_t *C);
	void de�ryptIterationCBC(Cipher3412 *Cipher, uint8_t *P, uint8_t  *MSB, uint8_t *C);

	//Cipher3412
	//�������� 16 ������� ��� �� ������� R � ������ MSB
	void getMSBfromR(uint8_t *R, uint8_t *MSB);
	//������� ������� R ����� �� 16 ����, ������� ������ �������� ��������� �����������
	void RegLeftShift(uint8_t *R, uint8_t *Ci);

	//���������� ���� ��������������� ����������� � � ����, ������� � ��������� �������, � ������ ������ ��������� 0
	//��������� ��������� pos �� ������ �����
	bool writeBlockC(qint64 &pos, uint8_t *C, QFile &file);
	//��������� ���� ��������� �����
	bool writeBlock(qint64 &pos, uint8_t *C, QFile &file, uint8_t len);

	//���������� ��������� ������(32�����) ������� � �������, � ������ ������ ��������� 0
	bool writeBlockIV(qint64 &pos, uint8_t *IV, QFile &file);

	//�������� 16 ���� ���� chat � uint8_t, ������������ ��� ������ �����
	void copyFtoC(char *F_block, uint8_t *C_block);
	//�������� len ���� ���� chat � uint8_t, ������������ ��� ������ �����
	void copyFtoC(char *F_block, uint8_t *C_block, uint8_t len);

	//�������� ��������� �������� ���� 34.13-2015 4.1.2 ��������� 2
	void paddingMessage(uint8_t *C_block, uint8_t r_len);
	//��������� �������� ������� iv 
	bool readIV(uint8_t *IV, QFile &file);

	//��������� ���� �� ��������� �������
	bool readFblock(qint64 &pos, char *F_block, QFile &file);
	//������� ��������� ������. ������ ������ �������� ��������� ���������
	bool generateIV(uint8_t *IV);


};

