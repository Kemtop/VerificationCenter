#include "stdafx.h"
#include "CertCreator.h"


CertCreator::CertCreator()
{
}


CertCreator::~CertCreator()
{
}

//������� ���������� CA
void CertCreator::CreateCA(TBSCertificate Tbs, string  save_path)
{
	DerSequence Certificate; //�������� ���� �����������
	DerSequence SeqTbsCertificate;

	//----------------------������ ����������� V3-----------------------
	DerContextSpecific CS_Version;

	//C����� ������ Integer
	DerCoder version;
	mpz_class v(2);
	version.IntegerToDer(v);

	CS_Version.Add(version); //������� � ���������
	CS_Version.CreateBinary(0x00, true); //�������� ���������-���� ��� � �����

	SeqTbsCertificate.Add(CS_Version);
	//------------------------------------------------------------------
	//-----------------�������� ����� �����������-----------------------
	DerCoder  serialNumber;
	serialNumber.IntegerToDer(Tbs.serialNumber);
	SeqTbsCertificate.Add(serialNumber);
	//------------------------------------------------------------------
	//-----------------Signature ��� ������� �������� ������������� ���������, ������������� �� ��� ������� �����������-----------------------
	DerSequence Signature;

	DerCoder  OidAlgorithmIdentifier;
	OidAlgorithmIdentifier.to_OBJECT_IDENTIFIER(Tbs.AlgorithmIdentifier);
	Signature.Add(OidAlgorithmIdentifier);
	Signature.CreateBinary(); //�������� ��������� � ���� ��������� �������
	SeqTbsCertificate.Add(Signature);
	//------------------------------------------------------------------
	//----------�������� �����������
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

	//---------------- Validity ������ ��������----------------------
	DerSequence Validity;
	DerCoder NotBefore;
	DerCoder NotAfter;
	QString f = Tbs.NotBefore.toUTC().toString();
	NotBefore.toUTCTime(Tbs.NotBefore.toUTC()); //������� �������� ����������� � UTC �������
	NotAfter.toUTCTime(Tbs.NotAfter.toUTC());
	Validity.Add(NotBefore);
	Validity.Add(NotAfter);
	Validity.CreateBinary();
	SeqTbsCertificate.Add(Validity);
	//----------------subject -������� ��� �������� ����� ����������. ��� CA ��������� ���� ��������
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
	//--��� ���� ������������� ��� �������� ��������� ����� � ������������� ���������������, ��������� � ������� ������������ ���� 
	DerSequence subjectPublicKeyInfo;
	subjectPublicKeyInfo.Add(GetPublicKeyAlgorithm(Tbs.publicKeyParamSet, Tbs.digestParamSet, Tbs.encryptionParamSet));
	DerCoder subjectPublicKey;
	subjectPublicKey.BIT_STRING_encapsulates_EC_public_key(Tbs.Qx, Tbs.Qy);
	subjectPublicKeyInfo.Add(subjectPublicKey);

	subjectPublicKeyInfo.CreateBinary();
	SeqTbsCertificate.Add(subjectPublicKeyInfo);
	//---------------extensions  ���������� ����������� 3 ������
	SeqTbsCertificate.Add(GetExtensions(Tbs));

	SeqTbsCertificate.CreateBinary(); //Set�ontainerInfo();

									  //----------------------------��������� Tbs �����------------------
	//---�������� �������� �������

	DerCoder signatureValue;

	Point Qp;
	mpz_class pv;;
	signatureValue = SignRootCertificate(SeqTbsCertificate, Qp, pv);
	//----------------------------
	Certificate.Add(SeqTbsCertificate);
	//-----------signatureAlgorithm
	/*
	�������� ���������� ������� �������� ������������� ���������������,
	������������� �� ��� ������������ ������� ������� �����������.
	*/
	DerSequence signatureAlgorithm;
	DerCoder OID_signatureAlgorithm;
	OID_signatureAlgorithm.to_OBJECT_IDENTIFIER("1.2.643.7.1.1.3.3"); //id-tc26-signwithdigest-gost3410-2012-256
	signatureAlgorithm.Add(OID_signatureAlgorithm);
	signatureAlgorithm.CreateBinary();
	Certificate.Add(signatureAlgorithm);

	//---------------------signatureValue �������� �������� �������, ����������� �� ������������������ ���� tbsCertificate
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
	DerSequence Certificate; //�������� ���� �����������
	DerSequence SeqTbsCertificate;

	//----------------------������ ����������� V3-----------------------
	DerContextSpecific CS_Version;

	//C����� ������ Integer
	DerCoder version;
	mpz_class v(2);
	version.IntegerToDer(v);

	CS_Version.Add(version); //������� � ���������
	CS_Version.CreateBinary(0x00, true); //�������� ���������-���� ��� � �����

	SeqTbsCertificate.Add(CS_Version);
	//------------------------------------------------------------------
	//-----------------�������� ����� �����������-----------------------
	DerCoder  serialNumber;
	serialNumber.IntegerToDer(Tbs.serialNumber);
	SeqTbsCertificate.Add(serialNumber);
	//------------------------------------------------------------------
	//-----------------Signature ��� ������� �������� ������������� ���������, ������������� �� ��� ������� �����������-----------------------
	DerSequence Signature;

	DerCoder  OidAlgorithmIdentifier;
	OidAlgorithmIdentifier.to_OBJECT_IDENTIFIER(Tbs.AlgorithmIdentifier);
	Signature.Add(OidAlgorithmIdentifier);
	DerCoder x_null; //NULL
	x_null.setNull(); //����� Null ���������
	Signature.Add(x_null);
	Signature.CreateBinary(); //�������� ��������� � ���� ��������� �������
	SeqTbsCertificate.Add(Signature);
	//------------------------------------------------------------------
	//----------�������� �����������
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
	//--����� ��������
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

	//---------------- Validity ������ ��������----------------------
	DerSequence Validity;
	DerCoder NotBefore;
	DerCoder NotAfter;
	NotBefore.toUTCTime(Tbs.NotBefore.toUTC()); //���� �������� � UTC �������
	NotAfter.toUTCTime(Tbs.NotAfter.toUTC());
	Validity.Add(NotBefore);
	Validity.Add(NotAfter);
	Validity.CreateBinary();
	SeqTbsCertificate.Add(Validity);
	//----------------subject -������� ��� �������� ����� ����������. ��� CA ��������� ���� ��������
	DerSequence Subject;

	//--Country
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.6", Tbs.Subject_id_at_countryName));
	//----Location
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.7", Tbs.Subject_id_at_localityName));
	//--����� ��������
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
	//--��� ���� ������������� ��� �������� ��������� ����� � ������������� ���������������, ��������� � ������� ������������ ���� 
	DerSequence subjectPublicKeyInfo;
	subjectPublicKeyInfo.Add(GetPublicKeyAlgorithm(Tbs.publicKeyParamSet, Tbs.digestParamSet, Tbs.encryptionParamSet));
	DerCoder subjectPublicKey;
	//subjectPublicKey.BIT_STRING_encapsulates_EC_public_key(Tbs.Qx, Tbs.Qy);
	Point Qp;
	Qp = Tbs.GetPubKey(); //�������� ����
	Qp.setCurvOid(Tbs.digestParamSet); //������� ��������� ������������� ������.��� ��� �� ���������.

	subjectPublicKey.BIT_STRING_encapsulates_EC_public_key(Qp.x, Qp.y);
	subjectPublicKeyInfo.Add(subjectPublicKey);

	subjectPublicKeyInfo.CreateBinary();
	SeqTbsCertificate.Add(subjectPublicKeyInfo);
	//---------------extensions  ���������� ����������� 3 ������
	SeqTbsCertificate.Add(GetExtensions(Tbs));

	SeqTbsCertificate.CreateBinary(); //Set�ontainerInfo();

									  //----------------------------��������� Tbs �����------------------
									  //---�������� �������� �������
	DerCoder signatureValue;
	mpz_class prvKey; //�������� ����

	signatureValue = SignRootCertificate(SeqTbsCertificate, Qp, private_key);
	//----------------------------
	Certificate.Add(SeqTbsCertificate);
	//-----------signatureAlgorithm
	/*
	�������� ���������� ������� �������� ������������� ���������������,
	������������� �� ��� ������������ ������� ������� �����������.
	*/
	DerSequence signatureAlgorithm;
	DerCoder OID_signatureAlgorithm;
	OID_signatureAlgorithm.to_OBJECT_IDENTIFIER("1.2.643.7.1.1.3.3"); //id-tc26-signwithdigest-gost3410-2012-256
	signatureAlgorithm.Add(OID_signatureAlgorithm);
	DerCoder NullVal;
	NullVal.setNull(); //��� ������ NULL
	signatureAlgorithm.Add(NullVal);
	signatureAlgorithm.CreateBinary();
	Certificate.Add(signatureAlgorithm);

	//---------------------signatureValue �������� �������� �������, ����������� �� ������������������ ���� tbsCertificate
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
	DerSequence Certificate; //�������� ���� �����������
	DerSequence SeqTbsCertificate;

	//----------------------������ ����������� V3-----------------------
	DerContextSpecific CS_Version;

	//C����� ������ Integer
	DerCoder version;
	mpz_class v(2);
	version.IntegerToDer(v);

	CS_Version.Add(version); //������� � ���������
	CS_Version.CreateBinary(0x00, true); //�������� ���������-���� ��� � �����

	SeqTbsCertificate.Add(CS_Version);
	//------------------------------------------------------------------
	//-----------------�������� ����� �����������-----------------------
	DerCoder  serialNumber;
	serialNumber.IntegerToDer(Tbs.serialNumber);
	SeqTbsCertificate.Add(serialNumber);
	//------------------------------------------------------------------
	//-----------------Signature ��� ������� �������� ������������� ���������, ������������� �� ��� ������� �����������-----------------------
	DerSequence Signature;

	DerCoder  OidAlgorithmIdentifier;
	OidAlgorithmIdentifier.to_OBJECT_IDENTIFIER(Tbs.AlgorithmIdentifier);
	Signature.Add(OidAlgorithmIdentifier);
	DerCoder x_null; //NULL
	x_null.setNull(); //����� Null ���������
	Signature.Add(x_null);
	Signature.CreateBinary(); //�������� ��������� � ���� ��������� �������
	SeqTbsCertificate.Add(Signature);
	//------------------------------------------------------------------
	//----------�������� �����������
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
	//--����� ��������
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

	//---------------- Validity ������ ��������----------------------
	DerSequence Validity;
	DerCoder NotBefore;
	DerCoder NotAfter;
	NotBefore.toUTCTime(Tbs.NotBefore.toUTC()); //���� �������� � UTC �������
	NotAfter.toUTCTime(Tbs.NotAfter.toUTC());
	Validity.Add(NotBefore);
	Validity.Add(NotAfter);
	Validity.CreateBinary();
	SeqTbsCertificate.Add(Validity);
	//----------------subject -������� ��� �������� ����� ����������. ��� CA ��������� ���� ��������
	DerSequence Subject;

	//--Country
	Subject.Add(GetIncapsulatedSetPntStr("2.5.4.6", Tbs.Subject_id_at_countryName));
	//----Location
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.7", Tbs.Subject_id_at_localityName));
	//--����� ��������
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
	//--��� ���� ������������� ��� �������� ��������� ����� � ������������� ���������������, ��������� � ������� ������������ ���� 
	DerSequence subjectPublicKeyInfo;
	subjectPublicKeyInfo.Add(GetPublicKeyAlgorithm(Tbs.publicKeyParamSet, Tbs.digestParamSet, Tbs.encryptionParamSet));
	DerCoder subjectPublicKey;
	//subjectPublicKey.BIT_STRING_encapsulates_EC_public_key(Tbs.Qx, Tbs.Qy);
	Point Qp;
	Qp = Tbs.GetPubKey(); //�������� ����
	Qp.setCurvOid(Tbs.digestParamSet); //������� ��������� ������������� ������.��� ��� �� ���������.

	subjectPublicKey.BIT_STRING_encapsulates_EC_public_key(Qp.x, Qp.y);
	subjectPublicKeyInfo.Add(subjectPublicKey);

	subjectPublicKeyInfo.CreateBinary();
	SeqTbsCertificate.Add(subjectPublicKeyInfo);
	//---------------extensions  ���������� ����������� 3 ������
	SeqTbsCertificate.Add(GetExtensions(Tbs));

	SeqTbsCertificate.CreateBinary(); //Set�ontainerInfo();

									  //----------------------------��������� Tbs �����------------------
									  //---�������� �������� �������
	DerCoder signatureValue;
	mpz_class prvKey; //�������� ����

	signatureValue = SignRootCertificate(SeqTbsCertificate, Qp, private_key);
	//----------------------------
	Certificate.Add(SeqTbsCertificate);
	//-----------signatureAlgorithm
	/*
	�������� ���������� ������� �������� ������������� ���������������,
	������������� �� ��� ������������ ������� ������� �����������.
	*/
	DerSequence signatureAlgorithm;
	DerCoder OID_signatureAlgorithm;
	OID_signatureAlgorithm.to_OBJECT_IDENTIFIER("1.2.643.7.1.1.3.3"); //id-tc26-signwithdigest-gost3410-2012-256
	signatureAlgorithm.Add(OID_signatureAlgorithm);
	DerCoder NullVal;
	NullVal.setNull(); //��� ������ NULL
	signatureAlgorithm.Add(NullVal);
	signatureAlgorithm.CreateBinary();
	Certificate.Add(signatureAlgorithm);

	//---------------------signatureValue �������� �������� �������, ����������� �� ������������������ ���� tbsCertificate
	//DerCoder signatureValue;
	/*
	unsigned char Sign[] = {0x01,0x25};

	unsigned long Sign_len = sizeof(Sign) / sizeof(unsigned char);
	signatureValue.to_Bit_String(Sign, Sign_len);
	*/
	Certificate.Add(signatureValue);
	Certificate.CreateBinary();
	Certificate.WriteCertB64(OutBa); //�������� ���������� 
}


void CertCreator::CreateCACertificate(TBSCertificate Tbs, QByteArray &OutBa)
{
	DerSequence Certificate; //�������� ���� �����������
	DerSequence SeqTbsCertificate;

	//----------------------������ ����������� V3-----------------------
	DerContextSpecific CS_Version;

	//C����� ������ Integer
	DerCoder version;
	mpz_class v(2);
	version.IntegerToDer(v);

	CS_Version.Add(version); //������� � ���������
	CS_Version.CreateBinary(0x00, true); //�������� ���������-���� ��� � �����

	SeqTbsCertificate.Add(CS_Version);
	//------------------------------------------------------------------
	//-----------------�������� ����� �����������-----------------------
	DerCoder  serialNumber;
	serialNumber.IntegerToDer(Tbs.serialNumber);
	SeqTbsCertificate.Add(serialNumber);
	//------------------------------------------------------------------
	//-----------------Signature ��� ������� �������� ������������� ���������, ������������� �� ��� ������� �����������-----------------------
	DerSequence Signature;

	DerCoder  OidAlgorithmIdentifier;
	OidAlgorithmIdentifier.to_OBJECT_IDENTIFIER(Tbs.AlgorithmIdentifier);
	Signature.Add(OidAlgorithmIdentifier);
	DerCoder x_null; //NULL
	x_null.setNull(); //����� Null ���������
	Signature.Add(x_null);
	Signature.CreateBinary(); //�������� ��������� � ���� ��������� �������
	SeqTbsCertificate.Add(Signature);
	//------------------------------------------------------------------
	//----------�������� �����������
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
	//--����� ��������
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

	//---------------- Validity ������ ��������----------------------
	DerSequence Validity;
	DerCoder NotBefore;
	DerCoder NotAfter;
	NotBefore.toUTCTime(Tbs.NotBefore.toUTC()); //���� �������� � UTC �������
	NotAfter.toUTCTime(Tbs.NotAfter.toUTC());
	Validity.Add(NotBefore);
	Validity.Add(NotAfter);
	Validity.CreateBinary();
	SeqTbsCertificate.Add(Validity);
	//----------------subject -������� ��� �������� ����� ����������. ��� CA ��������� ���� ��������
	DerSequence Subject;

	//--Country
	Subject.Add(GetIncapsulatedSetPntStr("2.5.4.6", Tbs.Subject_id_at_countryName));
	//----Location
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.7", Tbs.Subject_id_at_localityName));
	//--����� ��������
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
	//--��� ���� ������������� ��� �������� ��������� ����� � ������������� ���������������, ��������� � ������� ������������ ���� 
	DerSequence subjectPublicKeyInfo;
	subjectPublicKeyInfo.Add(GetPublicKeyAlgorithm(Tbs.publicKeyParamSet, Tbs.digestParamSet, Tbs.encryptionParamSet));
	DerCoder subjectPublicKey;

	Point Qp;
	Qp = Tbs.GetPubKey(); //�������� ����
	Qp.setCurvOid(Tbs.digestParamSet); //������� ��������� ������������� ������.��� ��� �� ���������.

	subjectPublicKey.BIT_STRING_encapsulates_EC_public_key(Qp.x, Qp.y);
	subjectPublicKeyInfo.Add(subjectPublicKey);

	subjectPublicKeyInfo.CreateBinary();
	SeqTbsCertificate.Add(subjectPublicKeyInfo);
	//---------------extensions  ���������� ����������� 3 ������
	SeqTbsCertificate.Add(GetExtensions(Tbs));

	SeqTbsCertificate.CreateBinary(); //Set�ontainerInfo();

									  //----------------------------��������� Tbs �����------------------
									  //---�������� �������� �������
	DerCoder signatureValue;
	mpz_class prvKey; //�������� ����

	signatureValue = SignCertificate(SeqTbsCertificate,PubKey, private_key);
	//----------------------------
	Certificate.Add(SeqTbsCertificate);
	//-----------signatureAlgorithm
	/*
	�������� ���������� ������� �������� ������������� ���������������,
	������������� �� ��� ������������ ������� ������� �����������.
	*/
	DerSequence signatureAlgorithm;
	DerCoder OID_signatureAlgorithm;
	OID_signatureAlgorithm.to_OBJECT_IDENTIFIER("1.2.643.7.1.1.3.3"); //id-tc26-signwithdigest-gost3410-2012-256
	signatureAlgorithm.Add(OID_signatureAlgorithm);
	DerCoder NullVal;
	NullVal.setNull(); //��� ������ NULL
	signatureAlgorithm.Add(NullVal);
	signatureAlgorithm.CreateBinary();
	Certificate.Add(signatureAlgorithm);

	//---------------------signatureValue �������� �������� �������, ����������� �� ������������������ ���� tbsCertificate
	//DerCoder signatureValue;
	/*
	unsigned char Sign[] = {0x01,0x25};

	unsigned long Sign_len = sizeof(Sign) / sizeof(unsigned char);
	signatureValue.to_Bit_String(Sign, Sign_len);
	*/
	Certificate.Add(signatureValue);
	Certificate.CreateBinary();
	Certificate.WriteCertB64(OutBa); //�������� ���������� 
}

void CertCreator::CreateUserCertificate(TBSCertificate Tbs,QByteArray &WrBa)
{
	DerSequence Certificate; //�������� ���� �����������
	DerSequence SeqTbsCertificate;

	//----------------------������ ����������� V3-----------------------
	DerContextSpecific CS_Version;

	//C����� ������ Integer
	DerCoder version;
	mpz_class v(2);
	version.IntegerToDer(v);

	CS_Version.Add(version); //������� � ���������
	CS_Version.CreateBinary(0x00, true); //�������� ���������-���� ��� � �����

	SeqTbsCertificate.Add(CS_Version);
	//------------------------------------------------------------------
	//-----------------�������� ����� �����������-----------------------
	DerCoder  serialNumber;
	serialNumber.IntegerToDer(Tbs.serialNumber);
	SeqTbsCertificate.Add(serialNumber);
	//------------------------------------------------------------------
	//-----------------Signature ��� ������� �������� ������������� ���������, ������������� �� ��� ������� �����������-----------------------
	DerSequence Signature;

	DerCoder  OidAlgorithmIdentifier;
	OidAlgorithmIdentifier.to_OBJECT_IDENTIFIER(Tbs.AlgorithmIdentifier);
	Signature.Add(OidAlgorithmIdentifier);
	DerCoder x_null; //NULL
	x_null.setNull(); //����� Null ���������
	Signature.Add(x_null);
	Signature.CreateBinary(); //�������� ��������� � ���� ��������� �������
	SeqTbsCertificate.Add(Signature);
	//------------------------------------------------------------------
	//----------�������� �����������
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
	//--����� ��������
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

	//---------------- Validity ������ ��������----------------------
	DerSequence Validity;
	DerCoder NotBefore;
	DerCoder NotAfter;
	NotBefore.toUTCTime(Tbs.NotBefore.toUTC()); //���� �������� � UTC �������
	NotAfter.toUTCTime(Tbs.NotAfter.toUTC());
	Validity.Add(NotBefore);
	Validity.Add(NotAfter);
	Validity.CreateBinary();
	SeqTbsCertificate.Add(Validity);
	//----------------subject -������� ��� �������� ����� ����������. ��� CA ��������� ���� ��������
	DerSequence Subject;

	//--Country
	Subject.Add(GetIncapsulatedSetPntStr("2.5.4.6", Tbs.Subject_id_at_countryName));
	//----Location
	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.7", Tbs.Subject_id_at_localityName));
	//--����� ��������
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
	//--��� ���� ������������� ��� �������� ��������� ����� � ������������� ���������������, ��������� � ������� ������������ ���� 
	DerSequence subjectPublicKeyInfo;
	subjectPublicKeyInfo.Add(GetPublicKeyAlgorithm(Tbs.publicKeyParamSet, Tbs.digestParamSet, Tbs.encryptionParamSet));
	DerCoder subjectPublicKey;

	
	subjectPublicKey.BIT_STRING_encapsulates_EC_public_key(Tbs.Qx, Tbs.Qy); //�������� ����

	subjectPublicKeyInfo.Add(subjectPublicKey);

	subjectPublicKeyInfo.CreateBinary();
	SeqTbsCertificate.Add(subjectPublicKeyInfo);
	//---------------extensions  ���������� ����������� 3 ������
	SeqTbsCertificate.Add(GetExtensions(Tbs));

	SeqTbsCertificate.CreateBinary(); //Set�ontainerInfo();

									  //----------------------------��������� Tbs �����------------------
									  //---�������� �������� �������
	DerCoder signatureValue;
	mpz_class prvKey; //�������� ����

	signatureValue = SignCertificate(SeqTbsCertificate, PubKey, private_key);
	//----------------------------
	Certificate.Add(SeqTbsCertificate);
	//-----------signatureAlgorithm
	/*
	�������� ���������� ������� �������� ������������� ���������������,
	������������� �� ��� ������������ ������� ������� �����������.
	*/
	DerSequence signatureAlgorithm;
	DerCoder OID_signatureAlgorithm;
	OID_signatureAlgorithm.to_OBJECT_IDENTIFIER("1.2.643.7.1.1.3.3"); //id-tc26-signwithdigest-gost3410-2012-256
	signatureAlgorithm.Add(OID_signatureAlgorithm);
	DerCoder NullVal;
	NullVal.setNull(); //��� ������ NULL
	signatureAlgorithm.Add(NullVal);
	signatureAlgorithm.CreateBinary();
	Certificate.Add(signatureAlgorithm);

	//---------------------signatureValue �������� �������� �������, ����������� �� ������������������ ���� tbsCertificate
	//DerCoder signatureValue;
	/*
	unsigned char Sign[] = {0x01,0x25};

	unsigned long Sign_len = sizeof(Sign) / sizeof(unsigned char);
	signatureValue.to_Bit_String(Sign, Sign_len);
	*/
	Certificate.Add(signatureValue);
	Certificate.CreateBinary();
	Certificate.WriteCertB64(WrBa); //�������� ���������� � ������.
	//Certificate.WriteCertB64(save_path); //�������� ����������
}

//���������� ���� ����
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
	//str.toPrintableString(Value); -����� ������� ����� �� ������������
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
���������� ���� ����
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
publicKeyParamSet � ������������� ���������� ��������� ����� ;
digestParamSet � ������������� ��������� � ���������� ;
encryptionParamSet � ������������� ��������� � ����������
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


//������� � ���������� ���������� �����������
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

		//=3 ����� Bool �����������
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

//������������� �������� ����-���������� ��� ������� ������������
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

//�������� ������� ���� � vector. ��������� ������� src
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
	unsigned char *uc_mess; //���������
	long long mess_len; //����� ���������

	mess_len = (long long)x.size(); //����� ��������� ��� ���������� �������� ��� �������.
	//������ ��� �������� ���������
	uc_mess = new unsigned char[mess_len];

	//������� �� ������� � ������
	for (long long i = 0;i < mess_len;i++)
	{
		uc_mess[i] = x.at(i);
	}

	//�������������� ����� ���������
	mess_len = mess_len * 8; //����� ��������� � �����

	Hash3411 H;
	H.hash_512(uc_mess, mess_len, hash);//�������� �������� ��� �������
	delete[] uc_mess;
}



//����������� ����������
//������� ���� ������ s ����� ������ r
DerCoder  CertCreator::SignRootCertificate(DerSequence tbs, Point pub_key, mpz_class prv_key)
{
	//�������������� ������� ���������� ���� � TBS �����������
	vector<unsigned char> _tbs; //������ � ���������� ������������������(������� ��� ���� ������)
	ReverseByteOrder(tbs.binaryvector, _tbs);//����� �������

	//������ ��������� � ������
	unsigned long long mess_len = static_cast<unsigned long long>(_tbs.size());

	CreateSign SignGen; //������ ��� �������� �������
	Groups EC2;  //������ ������ � �������� ������
	bool find_ec = EC2.SetParam(pub_key.getCurvOid()); //����� ��������� ������ �� ��������� OID
	if (!find_ec) { throw "Bad EC OID"; }; //�� ��������� ���
											   //������ ����� � ���������� � ������ ������������ ����� P
	Point P(EC2, 1); //����� ������������� ������ 
	Signature Sign;//�������
	Sign = SignGen.Sign(prv_key, reinterpret_cast<uint8_t *>(_tbs.data()), mess_len, P);

	string r_str = Sign.get_hex_r();
	string s_str = Sign.get_hex_s();

	//���� �������� �������	
	Point Q1(EC2, 1); //����� ������������� ������ 

	//�������� ������������ �������� ���� � ����������
	Point  Q2;
	Q2 = SignGen.generate_public_key(prv_key, P);
	//�������� ��������� ����� �� ����������
	string x1 = pub_key.x.get_str(16);//   KeyContainer.P16().toStdString();
	string y1 = pub_key.y.get_str(16); //KeyContainer.P17().toStdString();


									 //����������� ��������
	string x2 = Q2.get_hex_x();
	string y2 = Q2.get_hex_y();

	if (x1 != x2) throw "������ Sg4: �� ������� ��������� ����";//return StrCodec->toUnicode("������ Sg4: �� ������� ��������� ����");
	if (y1 != y2) throw "������ Sg5: �� ������� ��������� ����";
	//return StrCodec->toUnicode("������ Sg5: �� ������� ��������� ����");

	//����� ��������� ����� �������� �������
	Q1.x.set_str(x1, 16);
	Q1.y.set_str(y1, 16);


	//�������� �������
	VerifySign Vs;
	bool sign_status = Vs.Verify(reinterpret_cast<uint8_t *>(_tbs.data()), mess_len, P, Q1, Sign);

	if (sign_status == 0)
	{
		throw "������ Sg6: �� ������� ��������� ����";
	}



	mpz_class r;
	mpz_class s;
	r = Sign.get_r();
	s = Sign.get_s();


	//�������������� ������� ����� � �������� ������
	int s_len = (mpz_sizeinbase(s.get_mpz_t(), 2) + 7) / 8; //����� ������
	int r_len = (mpz_sizeinbase(r.get_mpz_t(), 2) + 7) / 8; //����� ������

	unsigned char *sbuff = new unsigned char[s_len];
	unsigned char *rbuff = new unsigned char[r_len];

	//�������������� �������� ����� � ������
	mpz_export((void *)sbuff, NULL, 1, sizeof(unsigned char), 1, 0, s.get_mpz_t());
	mpz_export((void *)rbuff, NULL, 1, sizeof(unsigned char), 1, 0, r.get_mpz_t());

	vector<uint8_t> sign_v;

	//������� ������� s � r � ���� ������.
	//������ ����� ���������� �� 1 ���
	if (s_len < 64)
	{
		int z = 64 - s_len;//���������� ����� � ����
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


	//������ ����� ���������� �� 1 ���
	if (r_len < 64)
	{
		int z = 64 - r_len;//���������� ����� � ����
		while (z > 0)
		{
			sign_v.push_back(0);
			z--;
		}

	}
	if (r_len > 64) throw "rlen>64";
	//����-����� �������	rbuff[5] = 0;
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
	//�������������� ������� ���������� ���� � TBS �����������
	vector<unsigned char> _tbs; //������ � ���������� ������������������(������� ��� ���� ������)
	ReverseByteOrder(tbs.binaryvector, _tbs);//����� �������
		
	Groups EC2;  //������ ������ � �������� ������
	bool find_ec = EC2.SetParam(pub_key.getCurvOid()); //����� ��������� ������ �� ��������� OID
	if (!find_ec) { throw "Bad EC OID"; }; //�� ��������� ���

	//������ ����� � ���������� � ������ ������������ ����� P
	Point P(EC2, 1); //����� ������������� ������ 
	CreateSign SignGen; //������ ��� �������� �������
	Signature Sign;//�������
	//������ ��������� � ������
	unsigned long long mess_len = static_cast<unsigned long long>(_tbs.size());
	Sign = SignGen.Sign(prv_key, reinterpret_cast<uint8_t *>(_tbs.data()), mess_len, P);//�������� �������.
		
	//�������� ������������ �������� ���-���������,��������� ������.
	int good = CheckKeyPair(pub_key, prv_key,P);
	if (good==1) throw "������ Sg4: �� ������� ��������� ����";
	if (good==2)  throw "������ Sg5: �� ������� ��������� ����";

	lastDigest = SignGen.getLastDigest(); //������� ���������.


	//�������� �������
	//���� �������� �������	
	Point Q1(EC2, 1); //����� ������������� ������ 
	//����� ��������� ����� �������� �������
	Q1.x.set_str(pub_key.get_hex_x(), 16);
	Q1.y.set_str(pub_key.get_hex_y(), 16);

	//�������� �������
	VerifySign Vs;
	bool sign_status = Vs.Verify(reinterpret_cast<uint8_t *>(_tbs.data()), mess_len, P, Q1, Sign);

	if (sign_status == 0) throw "������ Sg6: �� ������� ��������� ����";

	vector<uint8_t> sign_v;
	sign_v = Sign512ToVector(Sign); //�������������� � ������.
	
	DerCoder signatureValue;
	signatureValue.to_Bit_String(reinterpret_cast<uint8_t *>(sign_v.data()), sign_v.size());

	return signatureValue;
}

/*
��������� ������������ �������� ���-��������� ����� d,��������� Q.
� ������ ������ ���������� 1(�� ������� x) ��� 2(�� ������� y).�����=0
*/
int CertCreator::CheckKeyPair(Point Q, mpz_class d, Point &P)
{

	string s =d.get_str(16);
	CreateSign SignGen; //������ ��� �������� �������
	Point  Q2; //������ �������� ���� �� ��������� ���������.	
	Q2 = SignGen.generate_public_key(d, P);

	//�������� ��������� ����� �� ����������
	string x1 = Q.x.get_str(16);//   KeyContainer.P16().toStdString();
	string y1 = Q.y.get_str(16); //KeyContainer.P17().toStdString();

	//����������� ��������
	string x2 = Q2.get_hex_x();
	string y2 = Q2.get_hex_y();

	if (x1 != x2)  return 1;
	if (y1 != y2) return 2;

	return 0;
}

vector<uint8_t> CertCreator::Sign512ToVector(Signature Sign)
{
	vector<uint8_t> sign_v; //�������
	mpz_class r;
	mpz_class s;
	r = Sign.get_r();
	s = Sign.get_s();


	//�������������� ������� ����� � �������� ������
	int s_len = (mpz_sizeinbase(s.get_mpz_t(), 2) + 7) / 8; //����� ������
	int r_len = (mpz_sizeinbase(r.get_mpz_t(), 2) + 7) / 8; //����� ������

	unsigned char *sbuff = new unsigned char[s_len];
	unsigned char *rbuff = new unsigned char[r_len];

	//�������������� �������� ����� � ������
	mpz_export((void *)sbuff, NULL, 1, sizeof(unsigned char), 1, 0, s.get_mpz_t());
	mpz_export((void *)rbuff, NULL, 1, sizeof(unsigned char), 1, 0, r.get_mpz_t());

	

	//������� ������� s � r � ���� ������.
	//������ ����� ���������� �� 1 ���
	if (s_len < 64)
	{
		int z = 64 - s_len;//���������� ����� � ����
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


	//������ ����� ���������� �� 1 ���
	if (r_len < 64)
	{
		int z = 64 - r_len;//���������� ����� � ����
		while (z > 0)
		{
			sign_v.push_back(0);
			z--;
		}

	}
	if (r_len > 64) throw "rlen>64";
	//����-����� �������	rbuff[5] = 0;
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
	//���������  ��������������� ����� �� ������ ��������� ����� ��������
	gmp_randclass Rand(gmp_randinit_mt);
	mpz_class d;
	do
	{
		//������� ����� � ������������� ������� � Unix �����(1970)		
		__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		Rand.seed(now);

		d = Rand.get_z_bits(KeySize);

	} while ((d < 0) || (d > q));

	return d;
}
