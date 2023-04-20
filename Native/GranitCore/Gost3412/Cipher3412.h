#pragma once
#include <QtCore/QCoreApplication>
#include <iostream>

using std::string;


union qword_t 
{
	uint8_t asBytes[8];
	uint64_t asQWord;
};
//Блочный шифр ГОСТ34.12
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
	//Таблица экспонент (генератор = 0x02 в GF(256) по модулю g(x) = 111000011).
	static const uint8_t exponentialTable[256];
	//Таблица дискретных логарифмов (генератор = 0x02 в GF(256) по модулю g(x) = 111000011).  
	static const uint8_t logarithmicTable[256];
	//Поле замены Пи для выполнения нелинейного биективного преобразования ГОСТ Р 34.12-2015 стр. 3. 
	static const uint8_t Pi[256];
	//Инверсия поля Пи. 
	static const uint8_t InversedPi[256];
	//L матрица преобразования над полем Галуа GF(256).
	static const uint8_t LTransformationMatrix[16][16];
	//Обратная матрица преобразования L над  полем Галуа GF(256).
	static const uint8_t inversedLTransformationMatrix[16][16];

	//Массивы для развертывания раундовых ключей
	uint8_t KroundKeys[160];
	uint8_t Kmemory[32];

	//Нелинейное биективное преобразование (преобразование S)
	void STransformation(unsigned char *block);
	//Обратное нелинейное биективное преобразование(обратное преобразование	S)
	void InversedSTransformation(unsigned char *block);

	void swapBlocks(uint64_t *__restrict left, uint64_t *__restrict right);
	void LSTransformation(const unsigned char *input, uint64_t *output);
	void InversedLSTransformation(const unsigned char *input, uint64_t *output);
	void FTransformation(int constantIndex,uint64_t *__restrict left,uint64_t *__restrict right,
		uint64_t *__restrict temp1,	uint64_t *__restrict temp2);


		
public:
	Cipher3412();
	~Cipher3412();

	//Используя значения приведенные в ГОСТ Р 34.12—2015 шифрует блок и сравнивает с эталонными значениями
	//В случае успеха возвращает 1, в случае ошибки 0
	bool TestEncryption();
	bool TestEncryption1(); //Улучшенный тест
	//Используя значения приведенные в ГОСТ Р 34.12—2015 расшифровывает блок и сравнивает с эталонными значениями
	//В случае успеха возвращает 1, в случае ошибки 0
	bool TestDecryption();	
	
	//Реализовывает алгоритм развертывания итерационных ключей на основе мастер-ключа(key)
	void deploymentEncryptionRoundKeys(void *__restrict roundKeys, const void *__restrict key, void *__restrict memory);
	void deploymentEncryptionRoundKeys(const void *__restrict key);


	//Реализовывает алгоритм развертывания итерационных ключей на основе мастер-ключа(key)
	void deploymentDecryptionRoundKeys(void *__restrict roundKeys, const void *__restrict key, void *__restrict memory);
	void deploymentDecryptionRoundKeys(const void *__restrict key);

	//Шифрую блок
	void encryptBlock(const void *__restrict roundKeys, void *__restrict data);
	void encryptBlock(void *__restrict data);

	void decryptBlock(const void *__restrict roundKeys, void *__restrict data);
	void decryptBlock(void *__restrict data);
};

