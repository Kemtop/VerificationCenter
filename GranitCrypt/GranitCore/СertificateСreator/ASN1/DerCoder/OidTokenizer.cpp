#include "stdafx.h"
#include "OidTokenizer.h"


OidTokenizer::OidTokenizer(std::string Oid)
{
	oid = Oid;
	index = 0;
}


OidTokenizer::~OidTokenizer()
{
}

std::string OidTokenizer::NextToken()
{
	//≈сли индекс вышел за размер строки
	if (index == std::string::npos) return "";
	
	std::size_t end=oid.find(".",index); //ѕозици€ точки в строке,начина€ от текущего индекса
	
	//¬ строке больше нет разделител€ -возвращаю оставшеюс€ часть
	if (end == std::string::npos)
	{
		std::string lastToken = oid.substr(index);
	    index = std::string::npos;
		return lastToken;
	}


	std::string nextToken;
	nextToken = oid.substr(index, end-index);
	index = end + 1;

	return nextToken;
}

bool OidTokenizer::HasMoreTokens()
{
	if (index != std::string::npos) return true;

	return false;
}

