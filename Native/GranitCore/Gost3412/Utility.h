/*
Вспомогательный класс
   Объекты данного класса (далее «сообщения») повсеместно используются в программе для хранения
информации в двоичном виде. Интерфейс продумал таким образом, чтобы эффективно решались
следующие задачи:
Хранение байтовых строк произвольной длины
Обеспечение «единственности» сообщений в памяти
Обнуление участка памяти перед освобождением памяти
Обеспечение своевременного удаление сообщений
Предоставление удобного доступа к отельным байтам, и последовательностей байтов в сообщении
Единственность обеспечивается тем, что конструктор копирования и копирующий оператор присваивания
запрещены (неявно), так как описаны аналогичные функции перемещения.
Объекты класса ByteBlock всегда владеют памятью, на которую указывают. Даже когда объект
инициализируется с некоторым участком памяти, конструктор копирует его на новое место, и объект
работает с копией исходной информации. В некотором смысле данный класс похож на умный указатель из
STL — std::unique_ptr.
Обнуление памяти обеспечивается функцией memset. Стоит заметить, что при сборке данной программы
не стоит указывать опции оптимизации, так как некоторые компиляторы имеют свойство игнорировать
вызов функции memset, зная, что далее память не будет использована, а вскоре будет удалена.
*/
#pragma once

#include "StdAfx.h"

#include <string>
#include <vector>
using std::string;
//using std::vec;

//typedef unsigned char unsigned char;
typedef unsigned short WORD;
//_declspec(dllexport)
class  ByteBlock
{
	unsigned char * pBlocks;
	size_t amount_of_bytes;
public:
	// Construct block of bytes which contsists of
	// size_ blocks each of them with init_value in it
	ByteBlock(size_t size_ = 0, unsigned char init_value = 0);

	// Construct block with size_ first bytes of pBlocks_
	// The value will be copied, source stays untouchable
	ByteBlock(unsigned char * pBlocks_, size_t size_);

	// Move constructor
	// Copy constructor thus implicitly deleted
	// Object to move turn to null
	ByteBlock(ByteBlock && rhs);

	// Destructor, yeah
	~ByteBlock();


	// Move assigment operator
	// Object to move turn to null
	void operator = (ByteBlock &&rhs)
	{
		if (this == &rhs) return;
		if (pBlocks)
		{
			memset(pBlocks, 0, amount_of_bytes);
			delete[] pBlocks;
		}
		pBlocks = rhs.pBlocks;
		amount_of_bytes = rhs.amount_of_bytes;
		rhs.pBlocks = nullptr;
		rhs.amount_of_bytes = 0;
	};



	// This cast may be convenient to use the ByteBlock
	// in functions which takes raw pointers as argument
	unsigned char * byte_ptr();
	const unsigned char * byte_ptr() const;

	// Indexing operator with evident functionality
	unsigned char & operator [] (size_t index);
	unsigned char operator [] (size_t index) const;

	bool operator == (const ByteBlock & lhs) const;
	bool operator != (const ByteBlock & lhs) const;

	// Replace body of the current block with pBlocks_
	// Old value will be zerod, and then, deleted
	// New value copied into the block,
	// source stays untouchable
	void reset(const unsigned char * pBlocks_, size_t size_);

	// Return amount of bytes in block
	size_t size() const;

	// It'll return deep copy of the block, which
	// points to different place in memory
	ByteBlock deep_copy() const;

	// It'll return slice of current ByteBlock
	ByteBlock operator () (size_t begin, size_t length) const;

	// Changes values between two ByteBlock-s
	friend void swap(ByteBlock & lhs, ByteBlock & rhs);
};

//Некоторые функции которые будут полезны для реализации алгоритма шифрования
class _declspec(dllexport) Utillity
{
	Utillity();
	~Utillity();

public:
	static std::vector<ByteBlock> split_blocks(const ByteBlock & src, size_t length);
	static ByteBlock join_blocks(const std::vector<ByteBlock> & blocks);
	static void xor_blocks(ByteBlock & to_assign, const ByteBlock & lhs, const ByteBlock & rhs);
	// Some I/O functions to work with hex representation of ByteBlock
	static string hex_representation(const ByteBlock & bb);
	static ByteBlock hex_to_bytes(const string & s);
	static char to_hex_literal(unsigned char number);
	static unsigned char from_hex_literal(char symbol);
};











