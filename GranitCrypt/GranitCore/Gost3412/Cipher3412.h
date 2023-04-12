#pragma once
#include <QtCore/QCoreApplication>
#include <iostream>

using std::string;


union qword_t 
{
	uint8_t asBytes[8];
	uint64_t asQWord;
};
//������� ���� ����34.12
class Cipher3412
{

private:
	static const union qword_t roundConstantsLeft[32];
	static const union qword_t roundConstantsRight[32];
	static const union qword_t precomputedLSTableLeft[16][256];
	static const union qword_t precomputedLSTableRight[16][256];
	static const union qword_t precomputedInversedLSTableLeft[16][256];
	static const union qword_t precomputedInversedLSTableRight[16][256];
	
	static const uint8_t roundConstants[512];
	//������� ��������� (��������� = 0x02 � GF(256) �� ������ g(x) = 111000011).
	static const uint8_t exponentialTable[256];
	//������� ���������� ���������� (��������� = 0x02 � GF(256) �� ������ g(x) = 111000011).  
	static const uint8_t logarithmicTable[256];
	//���� ������ �� ��� ���������� ����������� ����������� �������������� ���� � 34.12-2015 ���. 3. 
	static const uint8_t Pi[256];
	//�������� ���� ��. 
	static const uint8_t InversedPi[256];
	//L ������� �������������� ��� ����� ����� GF(256).
	static const uint8_t LTransformationMatrix[16][16];
	//�������� ������� �������������� L ���  ����� ����� GF(256).
	static const uint8_t inversedLTransformationMatrix[16][16];

	//������� ��� ������������� ��������� ������
	uint8_t KroundKeys[160];
	uint8_t Kmemory[32];

	//���������� ���������� �������������� (�������������� S)
	void STransformation(unsigned char *block);
	//�������� ���������� ���������� ��������������(�������� ��������������	S)
	void InversedSTransformation(unsigned char *block);

	void swapBlocks(uint64_t *__restrict left, uint64_t *__restrict right);
	void LSTransformation(const unsigned char *input, uint64_t *output);
	void InversedLSTransformation(const unsigned char *input, uint64_t *output);
	void FTransformation(int constantIndex,uint64_t *__restrict left,uint64_t *__restrict right,
		uint64_t *__restrict temp1,	uint64_t *__restrict temp2);


		
public:
	Cipher3412();
	~Cipher3412();

	//��������� �������� ����������� � ���� � 34.12�2015 ������� ���� � ���������� � ���������� ����������
	//� ������ ������ ���������� 1, � ������ ������ 0
	bool TestEncryption();
	bool TestEncryption1(); //���������� ����
	//��������� �������� ����������� � ���� � 34.12�2015 �������������� ���� � ���������� � ���������� ����������
	//� ������ ������ ���������� 1, � ������ ������ 0
	bool TestDecryption();	
	
	//������������� �������� ������������� ������������ ������ �� ������ ������-�����(key)
	void deploymentEncryptionRoundKeys(void *__restrict roundKeys, const void *__restrict key, void *__restrict memory);
	void deploymentEncryptionRoundKeys(const void *__restrict key);


	//������������� �������� ������������� ������������ ������ �� ������ ������-�����(key)
	void deploymentDecryptionRoundKeys(void *__restrict roundKeys, const void *__restrict key, void *__restrict memory);
	void deploymentDecryptionRoundKeys(const void *__restrict key);

	//������ ����
	void encryptBlock(const void *__restrict roundKeys, void *__restrict data);
	void encryptBlock(void *__restrict data);

	void decryptBlock(const void *__restrict roundKeys, void *__restrict data);
	void decryptBlock(void *__restrict data);
};

