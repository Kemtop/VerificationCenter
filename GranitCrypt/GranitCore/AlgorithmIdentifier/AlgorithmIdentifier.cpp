#include "stdafx.h"
#include "AlgorithmIdentifier.h"


AlgorithmIdentifier::AlgorithmIdentifier()
{
}


AlgorithmIdentifier::~AlgorithmIdentifier()
{
}

bool AlgorithmIdentifier::getPublicKeyAlgoritmName(string Oid, string & Name)
{
	string algName = "";	//Алгоритм открытого ключа
	bool find = false;

	for (int i = 0;i < PublicKeyAlgoritmCount;i++)
	{
		if (PublicKeyAlgoritm[i][1] == Oid)
		{
			find = true;
			Name = PublicKeyAlgoritm[i][0];
			break;
		}
	}

	return find;
}

bool AlgorithmIdentifier::getSignCertAlgoritm(string Oid, string & Name)
{
	string algName = "";	
	bool find = false;

	//Алгоритм подписи сертификатов
	for (int i = 0;i <SignCertAlgoritmCount;i++)
	{
		if (SignCertAlgoritm[i][1] == Oid)
		{
			find = true;
			Name = SignCertAlgoritm[i][0];
			break;
		}
	}

	return find;
}

bool AlgorithmIdentifier::getECName(string Oid, string & Name)
{
	//Получаю имя эллиптической кривой.
	string EcName = "";
	bool find = false;

	for (int i = 0;i < EcParamCount;i++)
	{
		if (EcParam[i][1] == Oid)
		{
			Name = EcParam[i][0];
			find = true;
			break;
		}
	}

	return find;
}

 const int AlgorithmIdentifier::PublicKeyAlgoritmCount=2;


 const int AlgorithmIdentifier::SignCertAlgoritmCount=2;


 const string AlgorithmIdentifier::PublicKeyAlgoritm[2][2]=
 {
	 {"ГОСТ Р 34.10-2012 512бит","1.2.643.7.1.1.1.2"},
	 {"ГОСТ Р 34.10-2012 256бит","1.2.643.7.1.1.1.1"}
 };


 const string AlgorithmIdentifier::SignCertAlgoritm[2][2] =
 {
	 { "ГОСТ Р 34.10-2012 512бит","1.2.643.7.1.1.3.3" },
	 { "ГОСТ Р 34.10-2012 256бит","1.2.643.7.1.1.3.2" }
 };


 /*
 id-tc26-gost3410-2012-512 OBJECT IDENTIFIER ::=
{ iso(1) member-body(2) ru(643) rosstandart(7) tc26(1) algorithms(1)
sign(1) gost3410-2012-512(2)
 
 id-tc26-gost3410-2012-256 OBJECT IDENTIFIER ::=
{ iso(1) member-body(2) ru(643) rosstandart(7) tc26(1) algorithms(1)
sign(1) gost3410-2012-256(1)


id-tc26-signwithdigest-gost3410-2012-512 OBJECT IDENTIFIER ::=
{ iso(1) member-body(2) ru(643) rosstandart(7) tc26(1) algorithms(1) signwithdigest(3) gost3410-2012-512(3)

id-tc26-signwithdigest-gost3410-2012-256 OBJECT IDENTIFIER ::=
{ iso(1) member-body(2) ru(643) rosstandart(7) tc26(1) algorithms(1) signwithdigest(3)gost3410-2012-256(2)
 */

 //Параметры эллиптической кривой для отображения в выпадающем списке при создания пользователя
 const int AlgorithmIdentifier::EcParamCount=2;
 const string AlgorithmIdentifier::EcParam[2][2]=
 {
	 { "tc26_gost_3410_12_512_paramSetA","1.2.643.7.1.2.1.2.1"},
	 { "tc26_gost_3410_12_512_paramSetB","1.2.643.7.1.2.1.2.2"}
 };
 