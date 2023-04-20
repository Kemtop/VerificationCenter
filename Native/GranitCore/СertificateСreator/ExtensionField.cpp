#include "stdafx.h"
#include "ExtensionField.h"


ExtensionField::ExtensionField()
{

}


ExtensionField::~ExtensionField()
{
}

//Создает и копирует данные в массив OCTET_STRING_array
void ExtensionField::FillArray(unsigned char * exarray, unsigned long len)
{
  for (unsigned long i = 0;i < len;i++)
	{
		OCTET_STRING_array.push_back(exarray[i]);
	}
}