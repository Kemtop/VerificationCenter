#pragma once
#include <string>
//��������������� ����� ��� �������������� � OBJECT_IDENTIFIER
class OidTokenizer
{
private:
	std::string  oid;
	std::size_t  index;
public:
	OidTokenizer(std::string Oid);
	~OidTokenizer();

	std::string NextToken();
	bool HasMoreTokens();
};

