#pragma once

#include <string>
#include <vector>
using namespace std;
//Класс используется для формирования полей сертификата
class _declspec(dllexport) ExtensionField
{
public:
	
	ExtensionField();
	~ExtensionField();
	string OID;
	unsigned char Boolean=3; //По умолчанию флага нет 

	//Массив данных
	vector <unsigned char> OCTET_STRING_array;

	//Создает и копирует данные в массив OCTET_STRING_array
	void FillArray(unsigned char * exarray,unsigned long len);
};

