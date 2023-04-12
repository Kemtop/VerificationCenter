#include "stdafx.h"
#include "CertCreator.h"


CertCreator::CertCreator()
{
}


CertCreator::~CertCreator()
{
}

//Создает сертификат CA
void CertCreator::CreateCA(TBSCertificate Tbs, string  save_path)
{
	DerSequence Certificate; //Корневой блок сертификата
	DerSequence SeqTbsCertificate;

	//----------------------Версия сертификата V3-----------------------
	DerContextSpecific CS_Version;

	//Cоздаю объект Integer
	DerCoder version;
	mpz_class v(2);
	version.IntegerToDer(v);

	CS_Version.Add(version); //Помещаю в контейнер
	CS_Version.CreateBinary(0x00, true); //Формирую контейнер-пишу тег и длину

	SeqTbsCertificate.Add(CS_Version);
	//------------------------------------------------------------------
	//-----------------Серийный номер сертификата-----------------------
	DerCoder  serialNumber;
	serialNumber.IntegerToDer(Tbs.serialNumber);
	SeqTbsCertificate.Add(serialNumber);
	//------------------------------------------------------------------
	//-----------------Signature Это субполе содержит идентификатор алгоритма, используемого УЦ для подписи сертификата-----------------------
	DerSequence Signature;

	DerCoder  OidAlgorithmIdentifier;
	OidAlgorithmIdentifier.to_OBJECT_IDENTIFIER(Tbs.AlgorithmIdentifier);
	Signature.Add(OidAlgorithmIdentifier);
	Signature.CreateBinary(); //Оформляю контейнер в виде бинарного вектора
	SeqTbsCertificate.Add(Signature);
	//------------------------------------------------------------------
	//----------Издатель сертификата
	DerSequence Issuer;
	//--e-mail
	DerSet Issuer_Set_Mail;
	DerSequence Issuer_Seq_Mail;
	DerCoder OIDe_mail;
	OIDe_mail.to_OBJECT_IDENTIFIER("1.2.840.113549.1.9.1");
	DerCoder str_eMail;
	str_eMail.toIA5String(Tbs.Issuer_id_at_emailAddress);
	Issuer_Seq_Mail.Add(OIDe_mail);
	Issuer_Seq_Mail.Add(str_eMail);
	Issuer_Seq_Mail.CreateBinary();
	Issuer_Set_Mail.Add(Issuer_Seq_Mail);
	Issuer_Set_Mail.CreateBinary();
	Issuer.Add(Issuer_Set_Mail);
	//--Country
	DerSet Issuer_Set_Country;
	DerSequence Issuer_Seq_Country;
	DerCoder OID_Country;
	DerCoder str_Country;
	OID_Country.to_OBJECT_IDENTIFIER("2.5.4.6");
	str_Country.toPrintableString(Tbs.Issuer_id_at_countryName);
	Issuer_Seq_Country.Add(OID_Country);
	Issuer_Seq_Country.Add(str_Country);
	Issuer_Seq_Country.CreateBinary();
	Issuer_Set_Country.Add(Issuer_Seq_Country);
	Issuer_Set_Country.CreateBinary();
	Issuer.Add(Issuer_Set_Country);
	//--Location
	/*
	DerSet Issuer_Set_Location;
	DerSequence Issuer_Seq_Location;
	DerCoder OID_Location;
	DerCoder str_Location;
	OID_Location.to_OBJECT_IDENTIFIER("2.5.4.7");
	str_Location.toPrintableString(Tbs.Issuer_id_at_localityName);
	Issuer_Seq_Location.Add(OID_Location);
	Issuer_Seq_Location.Add(str_Location);
	Issuer_Seq_Location.CreateBinary();
	Issuer_Set_Location.Add(Issuer_Seq_Location);
	Issuer_Set_Location.CreateBinary();
	Issuer.Add(Issuer_Set_Location);
	*/
	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.7", Tbs.Issuer_id_at_localityName));
	//--organizationName--
	DerSet Issuer_Set_OrganizationName;
	DerSequence Issuer_Seq_OrganizationName;
	DerCoder OID_OrganizationName;
	DerCoder str_OrganizationName;
	OID_OrganizationName.to_OBJECT_IDENTIFIER("2.5.4.10");
	str_OrganizationName.toPrintableString(Tbs.Issuer_id_at_organizationName);
	Issuer_Seq_OrganizationName.Add(OID_OrganizationName);
	Issuer_Seq_OrganizationName.Add(str_OrganizationName);
	Issuer_Seq_OrganizationName.CreateBinary();
	Issuer_Set_OrganizationName.Add(Issuer_Seq_OrganizationName);
	Issuer_Set_OrganizationName.CreateBinary();
	Issuer.Add(Issuer_Set_OrganizationName);
	//--commonName
	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.3", Tbs.Issuer_id_at_commonName));
	//---
	Issuer.CreateBinary();
	SeqTbsCertificate.Add(Issuer);

	//---------------- Validity период действия----------------------
	DerSequence Validity;
	DerCoder NotBefore;
	DerCoder NotAfter;
	QString f = Tbs.NotBefore.toUTC().toString();
	NotBefore.toUTCTime(Tbs.NotBefore.toUTC()); //Периоды действия сертификата в UTC формате
	NotAfter.toUTCTime(Tbs.NotAfter.toUTC());
	Validity.Add(NotBefore);
	Validity.Add(NotAfter);
	Validity.CreateBinary();
	SeqTbsCertificate.Add(Validity);
	//----------------subject -субъект для которого выдан сертификат. Для CA идентичен полю издатель
	DerSequence Subject;
	//--e-mail
	Subject.Add(GetIncapsulatedSetUtf8("1.2.840.113549.1.9.1", Tbs.Issuer_id_at_emailAddress));
	//--Country
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.6", Tbs.Issuer_id_at_countryName));
	//----Location
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.7", Tbs.Issuer_id_at_localityName));
	//--organizationName--
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.10", Tbs.Issuer_id_at_organizationName));
	//--commonName
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.3", Tbs.Issuer_id_at_commonName));
	Subject.CreateBinary();
	SeqTbsCertificate.Add(Subject);
	//--------------------subjectPublicKeyInfo
	//--Это поле предназначено для указания открытого ключа и идентификации криптоалгоритма, совместно с которым используется ключ 
	DerSequence subjectPublicKeyInfo;
	subjectPublicKeyInfo.Add(GetPublicKeyAlgorithm(Tbs.publicKeyParamSet, Tbs.digestParamSet, Tbs.encryptionParamSet));
	DerCoder subjectPublicKey;
	subjectPublicKey.BIT_STRING_encapsulates_EC_public_key(Tbs.Qx, Tbs.Qy);
	subjectPublicKeyInfo.Add(subjectPublicKey);

	subjectPublicKeyInfo.CreateBinary();
	SeqTbsCertificate.Add(subjectPublicKeyInfo);
	//---------------extensions  расширения сертификата 3 версии
	SeqTbsCertificate.Add(GetExtensions(Tbs));

	SeqTbsCertificate.CreateBinary(); //SetСontainerInfo();

									  //----------------------------Окончание Tbs блока------------------
	//---Формирую цифровую подпись

	DerCoder signatureValue;

	Point Qp;
	mpz_class pv;;
	signatureValue = SignRootCertificate(SeqTbsCertificate, Qp, pv);
	//----------------------------
	Certificate.Add(SeqTbsCertificate);
	//-----------signatureAlgorithm
	/*
	Алгоритм вычисления подписи содержит идентификатор криптоалгоритма,
	используемого УЦ при формировании подписи данного сертификата.
	*/
	DerSequence signatureAlgorithm;
	DerCoder OID_signatureAlgorithm;
	OID_signatureAlgorithm.to_OBJECT_IDENTIFIER("1.2.643.7.1.1.3.3"); //id-tc26-signwithdigest-gost3410-2012-256
	signatureAlgorithm.Add(OID_signatureAlgorithm);
	signatureAlgorithm.CreateBinary();
	Certificate.Add(signatureAlgorithm);

	//---------------------signatureValue содержит цифровую подпись, вычисленную по последовательности поля tbsCertificate
	//DerCoder signatureValue;
	/*
	unsigned char Sign[] = {0x01,0x25};

	unsigned long Sign_len = sizeof(Sign) / sizeof(unsigned char);
	signatureValue.to_Bit_String(Sign, Sign_len);
	*/
	Certificate.Add(signatureValue);

	Certificate.CreateBinary();


	Certificate.WriteVectorToFile(save_path);

}

void CertCreator::CreateCertificate(TBSCertificate Tbs, string save_path)
{
	DerSequence Certificate; //Корневой блок сертификата
	DerSequence SeqTbsCertificate;

	//----------------------Версия сертификата V3-----------------------
	DerContextSpecific CS_Version;

	//Cоздаю объект Integer
	DerCoder version;
	mpz_class v(2);
	version.IntegerToDer(v);

	CS_Version.Add(version); //Помещаю в контейнер
	CS_Version.CreateBinary(0x00, true); //Формирую контейнер-пишу тег и длину

	SeqTbsCertificate.Add(CS_Version);
	//------------------------------------------------------------------
	//-----------------Серийный номер сертификата-----------------------
	DerCoder  serialNumber;
	serialNumber.IntegerToDer(Tbs.serialNumber);
	SeqTbsCertificate.Add(serialNumber);
	//------------------------------------------------------------------
	//-----------------Signature Это субполе содержит идентификатор алгоритма, используемого УЦ для подписи сертификата-----------------------
	DerSequence Signature;

	DerCoder  OidAlgorithmIdentifier;
	OidAlgorithmIdentifier.to_OBJECT_IDENTIFIER(Tbs.AlgorithmIdentifier);
	Signature.Add(OidAlgorithmIdentifier);
	DerCoder x_null; //NULL
	x_null.setNull(); //Делаю Null значением
	Signature.Add(x_null);
	Signature.CreateBinary(); //Оформляю контейнер в виде бинарного вектора
	SeqTbsCertificate.Add(Signature);
	//------------------------------------------------------------------
	//----------Издатель сертификата
	DerSequence Issuer;


	//--Country
	DerSet Issuer_Set_Country;
	DerSequence Issuer_Seq_Country;
	DerCoder OID_Country;
	DerCoder str_Country;
	OID_Country.to_OBJECT_IDENTIFIER("2.5.4.6");
	str_Country.toPrintableString(Tbs.Issuer_id_at_countryName);
	Issuer_Seq_Country.Add(OID_Country);
	Issuer_Seq_Country.Add(str_Country);
	Issuer_Seq_Country.CreateBinary();
	Issuer_Set_Country.Add(Issuer_Seq_Country);
	Issuer_Set_Country.CreateBinary();
	Issuer.Add(Issuer_Set_Country);

	//--Location
	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.7", Tbs.Issuer_id_at_localityName));
	//--адрес издателя
	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.9", Tbs.Issuer_id_at_streetAddress));
	//--organizationName--
	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.10", Tbs.Issuer_id_at_organizationName));
	//--commonName
	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.3", Tbs.Issuer_id_at_commonName));

	//--e-mail
	DerSet Issuer_Set_Mail;
	DerSequence Issuer_Seq_Mail;
	DerCoder OIDe_mail;
	OIDe_mail.to_OBJECT_IDENTIFIER("1.2.840.113549.1.9.1");
	DerCoder str_eMail;
	str_eMail.toIA5String(Tbs.Issuer_id_at_emailAddress);
	Issuer_Seq_Mail.Add(OIDe_mail);
	Issuer_Seq_Mail.Add(str_eMail);
	Issuer_Seq_Mail.CreateBinary();
	Issuer_Set_Mail.Add(Issuer_Seq_Mail);
	Issuer_Set_Mail.CreateBinary();
	Issuer.Add(Issuer_Set_Mail);


	//---
	Issuer.CreateBinary();
	SeqTbsCertificate.Add(Issuer);

	//---------------- Validity период действия----------------------
	DerSequence Validity;
	DerCoder NotBefore;
	DerCoder NotAfter;
	NotBefore.toUTCTime(Tbs.NotBefore.toUTC()); //Срок действия в UTC формате
	NotAfter.toUTCTime(Tbs.NotAfter.toUTC());
	Validity.Add(NotBefore);
	Validity.Add(NotAfter);
	Validity.CreateBinary();
	SeqTbsCertificate.Add(Validity);
	//----------------subject -субъект для которого выдан сертификат. Для CA идентичен полю издатель
	DerSequence Subject;

	//--Country
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.6", Tbs.Subject_id_at_countryName));
	//----Location
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.7", Tbs.Subject_id_at_localityName));
	//--адрес издателя
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.9", Tbs.Subject_id_at_streetAddress));
	//--organizationName--
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.10", Tbs.Subject_id_at_organizationName));
	//--commonName
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.3", Tbs.Subject_id_at_commonName));

	//--e-mail
	Subject.Add(GetIncapsulatedSetUtf8("1.2.840.113549.1.9.1", Tbs.Subject_e_mailAddress));

	Subject.CreateBinary();
	SeqTbsCertificate.Add(Subject);

	//--------------------subjectPublicKeyInfo
	//--Это поле предназначено для указания открытого ключа и идентификации криптоалгоритма, совместно с которым используется ключ 
	DerSequence subjectPublicKeyInfo;
	subjectPublicKeyInfo.Add(GetPublicKeyAlgorithm(Tbs.publicKeyParamSet, Tbs.digestParamSet, Tbs.encryptionParamSet));
	DerCoder subjectPublicKey;
	//subjectPublicKey.BIT_STRING_encapsulates_EC_public_key(Tbs.Qx, Tbs.Qy);
	Point Qp;
	Qp = Tbs.GetPubKey(); //Открытый ключ
	Qp.setCurvOid(Tbs.digestParamSet); //Передаю параметры эллиптической кривой.Без них не подписать.

	subjectPublicKey.BIT_STRING_encapsulates_EC_public_key(Qp.x, Qp.y);
	subjectPublicKeyInfo.Add(subjectPublicKey);

	subjectPublicKeyInfo.CreateBinary();
	SeqTbsCertificate.Add(subjectPublicKeyInfo);
	//---------------extensions  расширения сертификата 3 версии
	SeqTbsCertificate.Add(GetExtensions(Tbs));

	SeqTbsCertificate.CreateBinary(); //SetСontainerInfo();

									  //----------------------------Окончание Tbs блока------------------
									  //---Формирую цифровую подпись
	DerCoder signatureValue;
	mpz_class prvKey; //Закрытый ключ

	signatureValue = SignRootCertificate(SeqTbsCertificate, Qp, private_key);
	//----------------------------
	Certificate.Add(SeqTbsCertificate);
	//-----------signatureAlgorithm
	/*
	Алгоритм вычисления подписи содержит идентификатор криптоалгоритма,
	используемого УЦ при формировании подписи данного сертификата.
	*/
	DerSequence signatureAlgorithm;
	DerCoder OID_signatureAlgorithm;
	OID_signatureAlgorithm.to_OBJECT_IDENTIFIER("1.2.643.7.1.1.3.3"); //id-tc26-signwithdigest-gost3410-2012-256
	signatureAlgorithm.Add(OID_signatureAlgorithm);
	DerCoder NullVal;
	NullVal.setNull(); //Тип данных NULL
	signatureAlgorithm.Add(NullVal);
	signatureAlgorithm.CreateBinary();
	Certificate.Add(signatureAlgorithm);

	//---------------------signatureValue содержит цифровую подпись, вычисленную по последовательности поля tbsCertificate
	//DerCoder signatureValue;
	/*
	unsigned char Sign[] = {0x01,0x25};

	unsigned long Sign_len = sizeof(Sign) / sizeof(unsigned char);
	signatureValue.to_Bit_String(Sign, Sign_len);
	*/
	Certificate.Add(signatureValue);
	Certificate.CreateBinary();
	//WriteCertB64
	//Certificate.WriteVectorToFile(save_path);
	Certificate.WriteCertB64(save_path);
}

void CertCreator::CreateRootCertificate(TBSCertificate Tbs, QByteArray &OutBa)
{
	DerSequence Certificate; //Корневой блок сертификата
	DerSequence SeqTbsCertificate;

	//----------------------Версия сертификата V3-----------------------
	DerContextSpecific CS_Version;

	//Cоздаю объект Integer
	DerCoder version;
	mpz_class v(2);
	version.IntegerToDer(v);

	CS_Version.Add(version); //Помещаю в контейнер
	CS_Version.CreateBinary(0x00, true); //Формирую контейнер-пишу тег и длину

	SeqTbsCertificate.Add(CS_Version);
	//------------------------------------------------------------------
	//-----------------Серийный номер сертификата-----------------------
	DerCoder  serialNumber;
	serialNumber.IntegerToDer(Tbs.serialNumber);
	SeqTbsCertificate.Add(serialNumber);
	//------------------------------------------------------------------
	//-----------------Signature Это субполе содержит идентификатор алгоритма, используемого УЦ для подписи сертификата-----------------------
	DerSequence Signature;

	DerCoder  OidAlgorithmIdentifier;
	OidAlgorithmIdentifier.to_OBJECT_IDENTIFIER(Tbs.AlgorithmIdentifier);
	Signature.Add(OidAlgorithmIdentifier);
	DerCoder x_null; //NULL
	x_null.setNull(); //Делаю Null значением
	Signature.Add(x_null);
	Signature.CreateBinary(); //Оформляю контейнер в виде бинарного вектора
	SeqTbsCertificate.Add(Signature);
	//------------------------------------------------------------------
	//----------Издатель сертификата
	DerSequence Issuer;


	//--Country
	DerSet Issuer_Set_Country;
	DerSequence Issuer_Seq_Country;
	DerCoder OID_Country;
	DerCoder str_Country;
	OID_Country.to_OBJECT_IDENTIFIER("2.5.4.6");
	str_Country.toPrintableString(Tbs.Issuer_id_at_countryName);
	Issuer_Seq_Country.Add(OID_Country);
	Issuer_Seq_Country.Add(str_Country);
	Issuer_Seq_Country.CreateBinary();
	Issuer_Set_Country.Add(Issuer_Seq_Country);
	Issuer_Set_Country.CreateBinary();
	Issuer.Add(Issuer_Set_Country);

	//--Location
	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.7", Tbs.Issuer_id_at_localityName));
	//--адрес издателя
	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.9", Tbs.Issuer_id_at_streetAddress));
	//--organizationName--
	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.10", Tbs.Issuer_id_at_organizationName));
	//--commonName
	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.3", Tbs.Issuer_id_at_commonName));

	//--e-mail
	DerSet Issuer_Set_Mail;
	DerSequence Issuer_Seq_Mail;
	DerCoder OIDe_mail;
	OIDe_mail.to_OBJECT_IDENTIFIER("1.2.840.113549.1.9.1");
	DerCoder str_eMail;
	str_eMail.toIA5String(Tbs.Issuer_id_at_emailAddress);
	Issuer_Seq_Mail.Add(OIDe_mail);
	Issuer_Seq_Mail.Add(str_eMail);
	Issuer_Seq_Mail.CreateBinary();
	Issuer_Set_Mail.Add(Issuer_Seq_Mail);
	Issuer_Set_Mail.CreateBinary();
	Issuer.Add(Issuer_Set_Mail);


	//---
	Issuer.CreateBinary();
	SeqTbsCertificate.Add(Issuer);

	//---------------- Validity период действия----------------------
	DerSequence Validity;
	DerCoder NotBefore;
	DerCoder NotAfter;
	NotBefore.toUTCTime(Tbs.NotBefore.toUTC()); //Срок действия в UTC формате
	NotAfter.toUTCTime(Tbs.NotAfter.toUTC());
	Validity.Add(NotBefore);
	Validity.Add(NotAfter);
	Validity.CreateBinary();
	SeqTbsCertificate.Add(Validity);
	//----------------subject -субъект для которого выдан сертификат. Для CA идентичен полю издатель
	DerSequence Subject;

	//--Country
	Subject.Add(GetIncapsulatedSetPntStr("2.5.4.6", Tbs.Subject_id_at_countryName));
	//----Location
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.7", Tbs.Subject_id_at_localityName));
	//--адрес издателя
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.9", Tbs.Subject_id_at_streetAddress));
	//--organizationName--
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.10", Tbs.Subject_id_at_organizationName));
	//--commonName
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.3", Tbs.Subject_id_at_commonName));

	//--e-mail
	Subject.Add(GetIncapsulatedSetIA5("1.2.840.113549.1.9.1", Tbs.Subject_e_mailAddress));

	Subject.CreateBinary();
	SeqTbsCertificate.Add(Subject);

	//--------------------subjectPublicKeyInfo
	//--Это поле предназначено для указания открытого ключа и идентификации криптоалгоритма, совместно с которым используется ключ 
	DerSequence subjectPublicKeyInfo;
	subjectPublicKeyInfo.Add(GetPublicKeyAlgorithm(Tbs.publicKeyParamSet, Tbs.digestParamSet, Tbs.encryptionParamSet));
	DerCoder subjectPublicKey;
	//subjectPublicKey.BIT_STRING_encapsulates_EC_public_key(Tbs.Qx, Tbs.Qy);
	Point Qp;
	Qp = Tbs.GetPubKey(); //Открытый ключ
	Qp.setCurvOid(Tbs.digestParamSet); //Передаю параметры эллиптической кривой.Без них не подписать.

	subjectPublicKey.BIT_STRING_encapsulates_EC_public_key(Qp.x, Qp.y);
	subjectPublicKeyInfo.Add(subjectPublicKey);

	subjectPublicKeyInfo.CreateBinary();
	SeqTbsCertificate.Add(subjectPublicKeyInfo);
	//---------------extensions  расширения сертификата 3 версии
	SeqTbsCertificate.Add(GetExtensions(Tbs));

	SeqTbsCertificate.CreateBinary(); //SetСontainerInfo();

									  //----------------------------Окончание Tbs блока------------------
									  //---Формирую цифровую подпись
	DerCoder signatureValue;
	mpz_class prvKey; //Закрытый ключ

	signatureValue = SignRootCertificate(SeqTbsCertificate, Qp, private_key);
	//----------------------------
	Certificate.Add(SeqTbsCertificate);
	//-----------signatureAlgorithm
	/*
	Алгоритм вычисления подписи содержит идентификатор криптоалгоритма,
	используемого УЦ при формировании подписи данного сертификата.
	*/
	DerSequence signatureAlgorithm;
	DerCoder OID_signatureAlgorithm;
	OID_signatureAlgorithm.to_OBJECT_IDENTIFIER("1.2.643.7.1.1.3.3"); //id-tc26-signwithdigest-gost3410-2012-256
	signatureAlgorithm.Add(OID_signatureAlgorithm);
	DerCoder NullVal;
	NullVal.setNull(); //Тип данных NULL
	signatureAlgorithm.Add(NullVal);
	signatureAlgorithm.CreateBinary();
	Certificate.Add(signatureAlgorithm);

	//---------------------signatureValue содержит цифровую подпись, вычисленную по последовательности поля tbsCertificate
	//DerCoder signatureValue;
	/*
	unsigned char Sign[] = {0x01,0x25};

	unsigned long Sign_len = sizeof(Sign) / sizeof(unsigned char);
	signatureValue.to_Bit_String(Sign, Sign_len);
	*/
	Certificate.Add(signatureValue);
	Certificate.CreateBinary();
	Certificate.WriteCertB64(OutBa); //Формирую сертификат 
}


void CertCreator::CreateCACertificate(TBSCertificate Tbs, QByteArray &OutBa)
{
	DerSequence Certificate; //Корневой блок сертификата
	DerSequence SeqTbsCertificate;

	//----------------------Версия сертификата V3-----------------------
	DerContextSpecific CS_Version;

	//Cоздаю объект Integer
	DerCoder version;
	mpz_class v(2);
	version.IntegerToDer(v);

	CS_Version.Add(version); //Помещаю в контейнер
	CS_Version.CreateBinary(0x00, true); //Формирую контейнер-пишу тег и длину

	SeqTbsCertificate.Add(CS_Version);
	//------------------------------------------------------------------
	//-----------------Серийный номер сертификата-----------------------
	DerCoder  serialNumber;
	serialNumber.IntegerToDer(Tbs.serialNumber);
	SeqTbsCertificate.Add(serialNumber);
	//------------------------------------------------------------------
	//-----------------Signature Это субполе содержит идентификатор алгоритма, используемого УЦ для подписи сертификата-----------------------
	DerSequence Signature;

	DerCoder  OidAlgorithmIdentifier;
	OidAlgorithmIdentifier.to_OBJECT_IDENTIFIER(Tbs.AlgorithmIdentifier);
	Signature.Add(OidAlgorithmIdentifier);
	DerCoder x_null; //NULL
	x_null.setNull(); //Делаю Null значением
	Signature.Add(x_null);
	Signature.CreateBinary(); //Оформляю контейнер в виде бинарного вектора
	SeqTbsCertificate.Add(Signature);
	//------------------------------------------------------------------
	//----------Издатель сертификата
	DerSequence Issuer;


	//--Country
	DerSet Issuer_Set_Country;
	DerSequence Issuer_Seq_Country;
	DerCoder OID_Country;
	DerCoder str_Country;
	OID_Country.to_OBJECT_IDENTIFIER("2.5.4.6");
	str_Country.toPrintableString(Tbs.Issuer_id_at_countryName);
	Issuer_Seq_Country.Add(OID_Country);
	Issuer_Seq_Country.Add(str_Country);
	Issuer_Seq_Country.CreateBinary();
	Issuer_Set_Country.Add(Issuer_Seq_Country);
	Issuer_Set_Country.CreateBinary();
	Issuer.Add(Issuer_Set_Country);

	//--Location
	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.7", Tbs.Issuer_id_at_localityName));
	//--адрес издателя
	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.9", Tbs.Issuer_id_at_streetAddress));
	//--organizationName--
	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.10", Tbs.Issuer_id_at_organizationName));
	//--commonName
	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.3", Tbs.Issuer_id_at_commonName));

	//--e-mail
	DerSet Issuer_Set_Mail;
	DerSequence Issuer_Seq_Mail;
	DerCoder OIDe_mail;
	OIDe_mail.to_OBJECT_IDENTIFIER("1.2.840.113549.1.9.1");
	DerCoder str_eMail;
	str_eMail.toIA5String(Tbs.Issuer_id_at_emailAddress);
	Issuer_Seq_Mail.Add(OIDe_mail);
	Issuer_Seq_Mail.Add(str_eMail);
	Issuer_Seq_Mail.CreateBinary();
	Issuer_Set_Mail.Add(Issuer_Seq_Mail);
	Issuer_Set_Mail.CreateBinary();
	Issuer.Add(Issuer_Set_Mail);


	//---
	Issuer.CreateBinary();
	SeqTbsCertificate.Add(Issuer);

	//---------------- Validity период действия----------------------
	DerSequence Validity;
	DerCoder NotBefore;
	DerCoder NotAfter;
	NotBefore.toUTCTime(Tbs.NotBefore.toUTC()); //Срок действия в UTC формате
	NotAfter.toUTCTime(Tbs.NotAfter.toUTC());
	Validity.Add(NotBefore);
	Validity.Add(NotAfter);
	Validity.CreateBinary();
	SeqTbsCertificate.Add(Validity);
	//----------------subject -субъект для которого выдан сертификат. Для CA идентичен полю издатель
	DerSequence Subject;

	//--Country
	Subject.Add(GetIncapsulatedSetPntStr("2.5.4.6", Tbs.Subject_id_at_countryName));
	//----Location
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.7", Tbs.Subject_id_at_localityName));
	//--адрес издателя
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.9", Tbs.Subject_id_at_streetAddress));
	//--organizationName--
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.10", Tbs.Subject_id_at_organizationName));
	//--commonName
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.3", Tbs.Subject_id_at_commonName));

	//--e-mail
	Subject.Add(GetIncapsulatedSetIA5("1.2.840.113549.1.9.1", Tbs.Subject_e_mailAddress));

	Subject.CreateBinary();
	SeqTbsCertificate.Add(Subject);

	//--------------------subjectPublicKeyInfo
	//--Это поле предназначено для указания открытого ключа и идентификации криптоалгоритма, совместно с которым используется ключ 
	DerSequence subjectPublicKeyInfo;
	subjectPublicKeyInfo.Add(GetPublicKeyAlgorithm(Tbs.publicKeyParamSet, Tbs.digestParamSet, Tbs.encryptionParamSet));
	DerCoder subjectPublicKey;

	Point Qp;
	Qp = Tbs.GetPubKey(); //Открытый ключ
	Qp.setCurvOid(Tbs.digestParamSet); //Передаю параметры эллиптической кривой.Без них не подписать.

	subjectPublicKey.BIT_STRING_encapsulates_EC_public_key(Qp.x, Qp.y);
	subjectPublicKeyInfo.Add(subjectPublicKey);

	subjectPublicKeyInfo.CreateBinary();
	SeqTbsCertificate.Add(subjectPublicKeyInfo);
	//---------------extensions  расширения сертификата 3 версии
	SeqTbsCertificate.Add(GetExtensions(Tbs));

	SeqTbsCertificate.CreateBinary(); //SetСontainerInfo();

									  //----------------------------Окончание Tbs блока------------------
									  //---Формирую цифровую подпись
	DerCoder signatureValue;
	mpz_class prvKey; //Закрытый ключ

	signatureValue = SignCertificate(SeqTbsCertificate,PubKey, private_key);
	//----------------------------
	Certificate.Add(SeqTbsCertificate);
	//-----------signatureAlgorithm
	/*
	Алгоритм вычисления подписи содержит идентификатор криптоалгоритма,
	используемого УЦ при формировании подписи данного сертификата.
	*/
	DerSequence signatureAlgorithm;
	DerCoder OID_signatureAlgorithm;
	OID_signatureAlgorithm.to_OBJECT_IDENTIFIER("1.2.643.7.1.1.3.3"); //id-tc26-signwithdigest-gost3410-2012-256
	signatureAlgorithm.Add(OID_signatureAlgorithm);
	DerCoder NullVal;
	NullVal.setNull(); //Тип данных NULL
	signatureAlgorithm.Add(NullVal);
	signatureAlgorithm.CreateBinary();
	Certificate.Add(signatureAlgorithm);

	//---------------------signatureValue содержит цифровую подпись, вычисленную по последовательности поля tbsCertificate
	//DerCoder signatureValue;
	/*
	unsigned char Sign[] = {0x01,0x25};

	unsigned long Sign_len = sizeof(Sign) / sizeof(unsigned char);
	signatureValue.to_Bit_String(Sign, Sign_len);
	*/
	Certificate.Add(signatureValue);
	Certificate.CreateBinary();
	Certificate.WriteCertB64(OutBa); //Формирую сертификат 
}

void CertCreator::CreateUserCertificate(TBSCertificate Tbs,QByteArray &WrBa)
{
	DerSequence Certificate; //Корневой блок сертификата
	DerSequence SeqTbsCertificate;

	//----------------------Версия сертификата V3-----------------------
	DerContextSpecific CS_Version;

	//Cоздаю объект Integer
	DerCoder version;
	mpz_class v(2);
	version.IntegerToDer(v);

	CS_Version.Add(version); //Помещаю в контейнер
	CS_Version.CreateBinary(0x00, true); //Формирую контейнер-пишу тег и длину

	SeqTbsCertificate.Add(CS_Version);
	//------------------------------------------------------------------
	//-----------------Серийный номер сертификата-----------------------
	DerCoder  serialNumber;
	serialNumber.IntegerToDer(Tbs.serialNumber);
	SeqTbsCertificate.Add(serialNumber);
	//------------------------------------------------------------------
	//-----------------Signature Это субполе содержит идентификатор алгоритма, используемого УЦ для подписи сертификата-----------------------
	DerSequence Signature;

	DerCoder  OidAlgorithmIdentifier;
	OidAlgorithmIdentifier.to_OBJECT_IDENTIFIER(Tbs.AlgorithmIdentifier);
	Signature.Add(OidAlgorithmIdentifier);
	DerCoder x_null; //NULL
	x_null.setNull(); //Делаю Null значением
	Signature.Add(x_null);
	Signature.CreateBinary(); //Оформляю контейнер в виде бинарного вектора
	SeqTbsCertificate.Add(Signature);
	//------------------------------------------------------------------
	//----------Издатель сертификата
	DerSequence Issuer;


	//--Country
	DerSet Issuer_Set_Country;
	DerSequence Issuer_Seq_Country;
	DerCoder OID_Country;
	DerCoder str_Country;
	OID_Country.to_OBJECT_IDENTIFIER("2.5.4.6");
	str_Country.toPrintableString(Tbs.Issuer_id_at_countryName);
	Issuer_Seq_Country.Add(OID_Country);
	Issuer_Seq_Country.Add(str_Country);
	Issuer_Seq_Country.CreateBinary();
	Issuer_Set_Country.Add(Issuer_Seq_Country);
	Issuer_Set_Country.CreateBinary();
	Issuer.Add(Issuer_Set_Country);

	//--Location
	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.7", Tbs.Issuer_id_at_localityName));
	//--адрес издателя
	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.9", Tbs.Issuer_id_at_streetAddress));
	//--organizationName--
	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.10", Tbs.Issuer_id_at_organizationName));
	//--commonName
	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.3", Tbs.Issuer_id_at_commonName));

	//--e-mail
	DerSet Issuer_Set_Mail;
	DerSequence Issuer_Seq_Mail;
	DerCoder OIDe_mail;
	OIDe_mail.to_OBJECT_IDENTIFIER("1.2.840.113549.1.9.1");
	DerCoder str_eMail;
	str_eMail.toIA5String(Tbs.Issuer_id_at_emailAddress);
	Issuer_Seq_Mail.Add(OIDe_mail);
	Issuer_Seq_Mail.Add(str_eMail);
	Issuer_Seq_Mail.CreateBinary();
	Issuer_Set_Mail.Add(Issuer_Seq_Mail);
	Issuer_Set_Mail.CreateBinary();
	Issuer.Add(Issuer_Set_Mail);


	//---
	Issuer.CreateBinary();
	SeqTbsCertificate.Add(Issuer);

	//---------------- Validity период действия----------------------
	DerSequence Validity;
	DerCoder NotBefore;
	DerCoder NotAfter;
	NotBefore.toUTCTime(Tbs.NotBefore.toUTC()); //Срок действия в UTC формате
	NotAfter.toUTCTime(Tbs.NotAfter.toUTC());
	Validity.Add(NotBefore);
	Validity.Add(NotAfter);
	Validity.CreateBinary();
	SeqTbsCertificate.Add(Validity);
	//----------------subject -субъект для которого выдан сертификат. Для CA идентичен полю издатель
	DerSequence Subject;

	//--Country
	Subject.Add(GetIncapsulatedSetPntStr("2.5.4.6", Tbs.Subject_id_at_countryName));
	//----Location
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.7", Tbs.Subject_id_at_localityName));
	//--адрес издателя
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.9", Tbs.Subject_id_at_streetAddress));
	//--organizationName--
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.10", Tbs.Subject_id_at_organizationName));
	//--commonName
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.3", Tbs.Subject_id_at_commonName));

	//--e-mail
	Subject.Add(GetIncapsulatedSetIA5("1.2.840.113549.1.9.1", Tbs.Subject_e_mailAddress));

	Subject.CreateBinary();
	SeqTbsCertificate.Add(Subject);

	//--------------------subjectPublicKeyInfo
	//--Это поле предназначено для указания открытого ключа и идентификации криптоалгоритма, совместно с которым используется ключ 
	DerSequence subjectPublicKeyInfo;
	subjectPublicKeyInfo.Add(GetPublicKeyAlgorithm(Tbs.publicKeyParamSet, Tbs.digestParamSet, Tbs.encryptionParamSet));
	DerCoder subjectPublicKey;

	
	subjectPublicKey.BIT_STRING_encapsulates_EC_public_key(Tbs.Qx, Tbs.Qy); //Открытый ключ

	subjectPublicKeyInfo.Add(subjectPublicKey);

	subjectPublicKeyInfo.CreateBinary();
	SeqTbsCertificate.Add(subjectPublicKeyInfo);
	//---------------extensions  расширения сертификата 3 версии
	SeqTbsCertificate.Add(GetExtensions(Tbs));

	SeqTbsCertificate.CreateBinary(); //SetСontainerInfo();

									  //----------------------------Окончание Tbs блока------------------
									  //---Формирую цифровую подпись
	DerCoder signatureValue;
	mpz_class prvKey; //Закрытый ключ

	signatureValue = SignCertificate(SeqTbsCertificate, PubKey, private_key);
	//----------------------------
	Certificate.Add(SeqTbsCertificate);
	//-----------signatureAlgorithm
	/*
	Алгоритм вычисления подписи содержит идентификатор криптоалгоритма,
	используемого УЦ при формировании подписи данного сертификата.
	*/
	DerSequence signatureAlgorithm;
	DerCoder OID_signatureAlgorithm;
	OID_signatureAlgorithm.to_OBJECT_IDENTIFIER("1.2.643.7.1.1.3.3"); //id-tc26-signwithdigest-gost3410-2012-256
	signatureAlgorithm.Add(OID_signatureAlgorithm);
	DerCoder NullVal;
	NullVal.setNull(); //Тип данных NULL
	signatureAlgorithm.Add(NullVal);
	signatureAlgorithm.CreateBinary();
	Certificate.Add(signatureAlgorithm);

	//---------------------signatureValue содержит цифровую подпись, вычисленную по последовательности поля tbsCertificate
	//DerCoder signatureValue;
	/*
	unsigned char Sign[] = {0x01,0x25};

	unsigned long Sign_len = sizeof(Sign) / sizeof(unsigned char);
	signatureValue.to_Bit_String(Sign, Sign_len);
	*/
	Certificate.Add(signatureValue);
	Certificate.CreateBinary();
	Certificate.WriteCertB64(WrBa); //Сохраняю сертификат в массив.
	//Certificate.WriteCertB64(save_path); //Формирую сертификат
}

//Возвращает блок вида
/* SET
{
SEQUENCES{
OID
PrintableString
}
}
*/
DerSet CertCreator::GetIncapsulatedSetUtf8(string OID, string Value)
{
	DerSet Set1;
	DerSequence Seq1;
	DerCoder OID1;
	DerCoder str;
	OID1.to_OBJECT_IDENTIFIER(OID);
	//str.toPrintableString(Value); -иначе русские имена не отображаются
	str.toUTF8String(Value);
	Seq1.Add(OID1);
	Seq1.Add(str);
	Seq1.CreateBinary();
	Set1.Add(Seq1);
	Set1.CreateBinary();
	return Set1;
}

DerSet CertCreator::GetIncapsulatedSetPntStr(string OID, string Value)
{
	DerSet Set1;
	DerSequence Seq1;
	DerCoder OID1;
	DerCoder str;
	OID1.to_OBJECT_IDENTIFIER(OID);
	str.toPrintableString(Value);
	Seq1.Add(OID1);
	Seq1.Add(str);
	Seq1.CreateBinary();
	Set1.Add(Seq1);
	Set1.CreateBinary();
	return Set1;
}

DerSet CertCreator::GetIncapsulatedSetIA5(string OID, string Value)
{
	DerSet Set1;
	DerSequence Seq1;
	DerCoder OID1;
	DerCoder str;
	OID1.to_OBJECT_IDENTIFIER(OID);
	str.toIA5String(Value);
	Seq1.Add(OID1);
	Seq1.Add(str);
	Seq1.CreateBinary();
	Set1.Add(Seq1);
	Set1.CreateBinary();
	return Set1;
}

/*
Возвращает блок вида
SEQUENCE
{
OBJECT IDENTIFIER
1.2.643.2.2.19
SEQUENCE
{
OBJECT IDENTIFIER
1.2.643.2.2.35.1
OBJECT IDENTIFIER
1.2.643.2.2.30.1
}
}
publicKeyParamSet – идентификатор параметров открытого ключа ;
digestParamSet – идентификатор алгоритма и параметров ;
encryptionParamSet – идентификатор алгоритма и параметров
*/
DerSequence CertCreator::GetPublicKeyAlgorithm(string publicKeyParamSet, string digestParamSet, string encryptionParamSet)
{
	DerSequence PublicKeyParameters;
	DerCoder OID_publicKeyParamSet;
	DerCoder OID_digestParamSet;
	DerCoder OID_encryptionParamSet;

	OID_publicKeyParamSet.to_OBJECT_IDENTIFIER(publicKeyParamSet);
	OID_digestParamSet.to_OBJECT_IDENTIFIER(digestParamSet);
	OID_encryptionParamSet.to_OBJECT_IDENTIFIER(encryptionParamSet);

	PublicKeyParameters.Add(OID_publicKeyParamSet);
	DerSequence Seq1;
	Seq1.Add(OID_digestParamSet);
	Seq1.Add(OID_encryptionParamSet);
	Seq1.CreateBinary();
	PublicKeyParameters.Add(Seq1);
	PublicKeyParameters.CreateBinary();

	return PublicKeyParameters;
}


//Создает и возвращает расширение сертификата
DerContextSpecific CertCreator::GetExtensions(TBSCertificate Tbs)
{
	DerContextSpecific Ex;
	DerSequence Extension;
	for (size_t i = 0;i < Tbs.extensions.size();i++)
	{
		DerSequence Field;
		DerCoder Oid;
		Oid.to_OBJECT_IDENTIFIER(Tbs.extensions.at(i).OID);
		DerCoder OctetStr;
		OctetStr.to_Octet_String(&Tbs.extensions.at(i).OCTET_STRING_array);
		Field.Add(Oid);

		//=3 блока Bool Отсутствует
		if (Tbs.extensions.at(i).Boolean != 3)
		{
			DerCoder F_bool;
			if (Tbs.extensions.at(i).Boolean == 0)
				F_bool.to_Bool(false);
			else F_bool.to_Bool(true);
			Field.Add(F_bool);
		}

		Field.Add(OctetStr);
		Field.CreateBinary();
		Extension.Add(Field);
	}
	Extension.CreateBinary();
	Ex.Add(Extension);
	Ex.CreateBinary(0x03, true);

	return Ex;
}

//Устанавливает закрытый ключ-необходимо для подписи сертификатов
void CertCreator::SetPrivateKey(mpz_class u_private_key)
{
	private_key = u_private_key;

}

void CertCreator::SetPubKey(Point Q)
{
	PubKey = Q; 
	PubKey.setCurvOid(Q.getCurvOid());
}

vector<unsigned char> CertCreator::getLastDigest()
{
	return lastDigest;
}

//Обращает порядок байт в vector. Заполняет данными src
void CertCreator::ReverseByteOrder(vector<unsigned char>& src, vector<unsigned char>& dst)
{
	for (int i = src.size() - 1;i >= 0;i--)
	{
		dst.push_back(src[i]);
	}
}

void CertCreator::GetHash256(vector<unsigned char>& x, unsigned char * hash)
{
}

void CertCreator::GetHash512(vector<unsigned char>& x, unsigned char * hash)
{
	unsigned char *uc_mess; //Сообщение
	long long mess_len; //Длина сообщения

	mess_len = (long long)x.size(); //Длина сообщения для вычисления значения ХЭШ функции.
	//Массив для хранения сообщения
	uc_mess = new unsigned char[mess_len];

	//Копирую из вектора в массив
	for (long long i = 0;i < mess_len;i++)
	{
		uc_mess[i] = x.at(i);
	}

	//Преобразовываю длину сообщения
	mess_len = mess_len * 8; //Длина сообщения в битах

	Hash3411 H;
	H.hash_512(uc_mess, mess_len, hash);//Вычисляю значение ХЭШ функции
	delete[] uc_mess;
}



//Подписывает сертификат
//Сначала идет вектор s потом вектор r
DerCoder  CertCreator::SignRootCertificate(DerSequence tbs, Point pub_key, mpz_class prv_key)
{
	//Преобразовываю порядок следования байт в TBS сертификате
	vector<unsigned char> _tbs; //Данные в правильной последовательности(старший бит идет первым)
	ReverseByteOrder(tbs.binaryvector, _tbs);//Меняю порядок

	//Размер сообщения в байтах
	unsigned long long mess_len = static_cast<unsigned long long>(_tbs.size());

	CreateSign SignGen; //Объект для создания подписи
	Groups EC2;  //Создаю кривую с заданным именем
	bool find_ec = EC2.SetParam(pub_key.getCurvOid()); //Задаю параметры кривой на основании OID
	if (!find_ec) { throw "Bad EC OID"; }; //Не известный оид
											   //Создаю точку с указанными в кривой координатами точки P
	Point P(EC2, 1); //Точка эллиптической кривой 
	Signature Sign;//Подпись
	Sign = SignGen.Sign(prv_key, reinterpret_cast<uint8_t *>(_tbs.data()), mess_len, P);

	string r_str = Sign.get_hex_r();
	string s_str = Sign.get_hex_s();

	//Ключ проверки подписи	
	Point Q1(EC2, 1); //Точка эллиптической кривой 

	//Проверяю соответствие ключевой пары в контейнере
	Point  Q2;
	Q2 = SignGen.generate_public_key(prv_key, P);
	//Значение открытого ключа из контейнера
	string x1 = pub_key.x.get_str(16);//   KeyContainer.P16().toStdString();
	string y1 = pub_key.y.get_str(16); //KeyContainer.P17().toStdString();


									 //Вычисленное значение
	string x2 = Q2.get_hex_x();
	string y2 = Q2.get_hex_y();

	if (x1 != x2) throw "Ошибка Sg4: Не удалось подписать файл";//return StrCodec->toUnicode("Ошибка Sg4: Не удалось подписать файл");
	if (y1 != y2) throw "Ошибка Sg5: Не удалось подписать файл";
	//return StrCodec->toUnicode("Ошибка Sg5: Не удалось подписать файл");

	//Задаю параметры ключу проверки подписи
	Q1.x.set_str(x1, 16);
	Q1.y.set_str(y1, 16);


	//Проверяю подпись
	VerifySign Vs;
	bool sign_status = Vs.Verify(reinterpret_cast<uint8_t *>(_tbs.data()), mess_len, P, Q1, Sign);

	if (sign_status == 0)
	{
		throw "Ошибка Sg6: Не удалось подписать файл";
	}



	mpz_class r;
	mpz_class s;
	r = Sign.get_r();
	s = Sign.get_s();


	//Преобразование больших чисел в бинарный вектор
	int s_len = (mpz_sizeinbase(s.get_mpz_t(), 2) + 7) / 8; //Длина буфера
	int r_len = (mpz_sizeinbase(r.get_mpz_t(), 2) + 7) / 8; //Длина буфера

	unsigned char *sbuff = new unsigned char[s_len];
	unsigned char *rbuff = new unsigned char[r_len];

	//Преобразование большого числа в массив
	mpz_export((void *)sbuff, NULL, 1, sizeof(unsigned char), 1, 0, s.get_mpz_t());
	mpz_export((void *)rbuff, NULL, 1, sizeof(unsigned char), 1, 0, r.get_mpz_t());

	vector<uint8_t> sign_v;

	//Собираю вектора s и r в один массив.
	//Вектор может отличаться на 1 бит
	if (s_len < 64)
	{
		int z = 64 - s_len;//Количество нулей с лева
		while (z > 0)
		{
			sign_v.push_back(0);
			z--;
		}

	}
	if (s_len > 64) throw "slen>64";

	for (int i = 0;i < s_len;i++)
	{
		sign_v.push_back(sbuff[i]);
	}


	//Вектор может отличаться на 1 бит
	if (r_len < 64)
	{
		int z = 64 - r_len;//Количество нулей с лева
		while (z > 0)
		{
			sign_v.push_back(0);
			z--;
		}

	}
	if (r_len > 64) throw "rlen>64";
	//Тест-порча подписи	rbuff[5] = 0;
	for (int i = 0;i < r_len;i++)
	{
		sign_v.push_back(rbuff[i]);
	}

	delete[] sbuff;
	delete[] rbuff;


	DerCoder signatureValue;
	signatureValue.to_Bit_String(reinterpret_cast<uint8_t *>(sign_v.data()), sign_v.size());

	return signatureValue;
}

DerCoder CertCreator::SignCertificate(DerSequence tbs, Point pub_key, mpz_class prv_key)
{
	//Преобразовываю порядок следования байт в TBS сертификате
	vector<unsigned char> _tbs; //Данные в правильной последовательности(старший бит идет первым)
	ReverseByteOrder(tbs.binaryvector, _tbs);//Меняю порядок
		
	Groups EC2;  //Создаю кривую с заданным именем
	bool find_ec = EC2.SetParam(pub_key.getCurvOid()); //Задаю параметры кривой на основании OID
	if (!find_ec) { throw "Bad EC OID"; }; //Не известный оид

	//Создаю точку с указанными в кривой координатами точки P
	Point P(EC2, 1); //Точка эллиптической кривой 
	CreateSign SignGen; //Объект для создания подписи
	Signature Sign;//Подпись
	//Размер сообщения в байтах
	unsigned long long mess_len = static_cast<unsigned long long>(_tbs.size());
	Sign = SignGen.Sign(prv_key, reinterpret_cast<uint8_t *>(_tbs.data()), mess_len, P);//Формирую подпись.
		
	//Проверяю соответствия ключевых пар-открытого,закрытого ключей.
	int good = CheckKeyPair(pub_key, prv_key,P);
	if (good==1) throw "Ошибка Sg4: Не удалось подписать файл";
	if (good==2)  throw "Ошибка Sg5: Не удалось подписать файл";

	lastDigest = SignGen.getLastDigest(); //Получаю отпечаток.


	//Проверяю подпись
	//Ключ проверки подписи	
	Point Q1(EC2, 1); //Точка эллиптической кривой 
	//Задаю параметры ключу проверки подписи
	Q1.x.set_str(pub_key.get_hex_x(), 16);
	Q1.y.set_str(pub_key.get_hex_y(), 16);

	//Проверяю подпись
	VerifySign Vs;
	bool sign_status = Vs.Verify(reinterpret_cast<uint8_t *>(_tbs.data()), mess_len, P, Q1, Sign);

	if (sign_status == 0) throw "Ошибка Sg6: Не удалось подписать файл";

	vector<uint8_t> sign_v;
	sign_v = Sign512ToVector(Sign); //Преобразовываю в вектор.
	
	DerCoder signatureValue;
	signatureValue.to_Bit_String(reinterpret_cast<uint8_t *>(sign_v.data()), sign_v.size());

	return signatureValue;
}

/*
Проверяет соответствие ключевых пар-закрытого ключа d,открытого Q.
В случае ошибки возвращает 1(не совпали x) или 2(не совпали y).Верно=0
*/
int CertCreator::CheckKeyPair(Point Q, mpz_class d, Point &P)
{

	string s =d.get_str(16);
	CreateSign SignGen; //Объект для создания подписи
	Point  Q2; //Создаю открытый ключ на основании закрытого.	
	Q2 = SignGen.generate_public_key(d, P);

	//Значение открытого ключа из контейнера
	string x1 = Q.x.get_str(16);//   KeyContainer.P16().toStdString();
	string y1 = Q.y.get_str(16); //KeyContainer.P17().toStdString();

	//Вычисленное значение
	string x2 = Q2.get_hex_x();
	string y2 = Q2.get_hex_y();

	if (x1 != x2)  return 1;
	if (y1 != y2) return 2;

	return 0;
}

vector<uint8_t> CertCreator::Sign512ToVector(Signature Sign)
{
	vector<uint8_t> sign_v; //Подпись
	mpz_class r;
	mpz_class s;
	r = Sign.get_r();
	s = Sign.get_s();


	//Преобразование больших чисел в бинарный вектор
	int s_len = (mpz_sizeinbase(s.get_mpz_t(), 2) + 7) / 8; //Длина буфера
	int r_len = (mpz_sizeinbase(r.get_mpz_t(), 2) + 7) / 8; //Длина буфера

	unsigned char *sbuff = new unsigned char[s_len];
	unsigned char *rbuff = new unsigned char[r_len];

	//Преобразование большого числа в массив
	mpz_export((void *)sbuff, NULL, 1, sizeof(unsigned char), 1, 0, s.get_mpz_t());
	mpz_export((void *)rbuff, NULL, 1, sizeof(unsigned char), 1, 0, r.get_mpz_t());

	

	//Собираю вектора s и r в один массив.
	//Вектор может отличаться на 1 бит
	if (s_len < 64)
	{
		int z = 64 - s_len;//Количество нулей с лева
		while (z > 0)
		{
			sign_v.push_back(0);
			z--;
		}

	}
	if (s_len > 64) throw "slen>64";

	for (int i = 0;i < s_len;i++)
	{
		sign_v.push_back(sbuff[i]);
	}


	//Вектор может отличаться на 1 бит
	if (r_len < 64)
	{
		int z = 64 - r_len;//Количество нулей с лева
		while (z > 0)
		{
			sign_v.push_back(0);
			z--;
		}

	}
	if (r_len > 64) throw "rlen>64";
	//Тест-порча подписи	rbuff[5] = 0;
	for (int i = 0;i < r_len;i++)
	{
		sign_v.push_back(rbuff[i]);
	}

	delete[] sbuff;
	delete[] rbuff;
	
	return sign_v;
}

mpz_class CertCreator::CRandom(mpz_class q, int KeySize)
{
	//Генерирую  псевдослучайное число на основе алгоритма Вихрь Мерсенна
	gmp_randclass Rand(gmp_randinit_mt);
	mpz_class d;
	do
	{
		//Получаю время в миллисекундах начиная с Unix эпохи(1970)		
		__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		Rand.seed(now);

		d = Rand.get_z_bits(KeySize);

	} while ((d < 0) || (d > q));

	return d;
}
