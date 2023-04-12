#pragma once
#include "ASN1\DerCoder\DerCoder.h"
//Класс для кодирования типа Integer
class DerInteger:public DerCoder
{
public:
	DerInteger();
	~DerInteger();
};

