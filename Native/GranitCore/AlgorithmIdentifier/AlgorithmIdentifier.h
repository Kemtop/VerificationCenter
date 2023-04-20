#pragma once
#include <string>

using std::string;
//Идентификатор алгоритма подписи
class _declspec(dllexport) AlgorithmIdentifier
{
public:
	AlgorithmIdentifier();
	~AlgorithmIdentifier();
	//Алгоритм открытого ключа
	static const string PublicKeyAlgoritm[2][2];
	//Количество строк в массиве PublicKeyAlgoritm
	static const int PublicKeyAlgoritmCount; 
	//Алгоритм подписи сертификата
	static const string SignCertAlgoritm[2][2];
	//Количество строк в массиве SignCertAlgoritm
	static const int SignCertAlgoritmCount;

	//Параметры эллиптической кривой для отображения в выпадающем списке при создания пользователя
	static const string EcParam[2][2];
	static const int EcParamCount;

	//Получает название алгоритма ключа на основе его Oid. Если  не найден возвращает false.
	static bool getPublicKeyAlgoritmName(string Oid, string &Name);

	//Получает название алгоритма подписи сертификата на основе его Oid. Если  не найден возвращает false.
	static bool getSignCertAlgoritm(string Oid, string &Name);

	//Получает название эллиптической кривой на основе ее Oid. Если кривая не найдена возвращает false.
	static bool getECName(string Oid,string &Name);


};

