#include "binASN1SignedData.h"

binASN1SignedData::binASN1SignedData()
{
}

binASN1SignedData::~binASN1SignedData()
{
}

QByteArray binASN1SignedData::createSignedBin(rfcSignedData *sdata)
{	
	/*
	  ContentInfo ::= SEQUENCE {
		contentType ContentType,
		content [0] EXPLICIT ANY DEFINED BY contentType }

		ContentType ::= OBJECT IDENTIFIER
	   contentType показывает тип связанного содержимого.
		content представляет собой связанное с объектом содержимое. Тип содержимого
		может быть уникально определен из contentType. Типы содержимого: data,
		signed-data, enveloped-data, digested-data, encrypted-data и authenticated-data.
	*/

	DerSequence SignedData; //Корневая последовательность СMS.
	
	//Указываем тип содержимого-signed-data.
	DerCoder  ContentType;
	ContentType.to_OBJECT_IDENTIFIER("1.2.840.113549.1.7.2");
	SignedData.Add(ContentType);

	DerContextSpecific contentCS;
	DerSequence content;

	/*
	SignedData :: = SEQUENCE{
		version CMSVersion,
		digestAlgorithms DigestAlgorithmIdentifiers,
		encapContentInfo EncapsulatedContentInfo,
		certificates[0] IMPLICIT CertificateSet OPTIONAL,
		crls[1] IMPLICIT RevocationInfoChoices OPTIONAL,
		signerInfos SignerInfos }
		*/

	
		DerCoder CMSVersion;
		mpz_class v(sdata->CMSVersion);
		CMSVersion.IntegerToDer(v);	//Cоздаю объект Integer
		content.Add(CMSVersion); //Помещаю в контейнер

		//digestAlgorithms—набор идентификаторов алгоритмов расчета отпечатка сообщения.
		DerSet digestAlgorithms;
		DerSequence das;
		DerCoder  digestAlgorithmOID;
		digestAlgorithmOID.to_OBJECT_IDENTIFIER(sdata->digestAlgorithms);

		das.Add(digestAlgorithmOID);
		DerCoder null1; //NULL
		null1.setNull(); //Делаю Null значением
		das.Add(null1);

		das.CreateBinary();
		digestAlgorithms.Add(das);
		digestAlgorithms.CreateBinary();
		content.Add(digestAlgorithms);

		/*
		  EncapsulatedContentInfo :: = SEQUENCE{
			eContentType ContentType,
			eContent[0] EXPLICIT OCTET STRING OPTIONAL }

			ContentType :: = OBJECT IDENTIFIER
		*/

		DerSequence encapContentInfo;
		DerCoder ContentType1;
		ContentType1.to_OBJECT_IDENTIFIER("1.2.840.113549.1.7.1");
		encapContentInfo.Add(ContentType1);
		encapContentInfo.CreateBinary();
		content.Add(encapContentInfo);

		/*
		SignerInfo ::= SEQUENCE 
		{
		  version CMSVersion,
		  sid SignerIdentifier,
		  digestAlgorithm DigestAlgorithmIdentifier,
		  signedAttrs [0] IMPLICIT SignedAttributes OPTIONAL,
          signatureAlgorithm SignatureAlgorithmIdentifier,
          signature SignatureValue,
          unsignedAttrs [1] IMPLICIT UnsignedAttributes OPTIONAL
		}
		*/

		DerSet signerInfoSet;
		DerSequence SignerInfo; //Информация подписанта.

		DerCoder CMSVersion1;
		mpz_class v1(sdata->CMSVersion);
		CMSVersion1.IntegerToDer(v1);	//Cоздаю объект Integer
		SignerInfo.Add(CMSVersion1); //Помещаю в контейнер

		/*
		SignerIdentifier ::= CHOICE {
			issuerAndSerialNumber IssuerAndSerialNumber,
			subjectKeyIdentifier [0] SubjectKeyIdentifier }
		*/

		DerSequence signerIdentifier;
		
		//Наименование организации.
		DerSet commonNameSet;
		DerSequence commonNameSeq;

		DerCoder OIDcommonName;
		DerCoder str_commonName;
		OIDcommonName.to_OBJECT_IDENTIFIER("2.5.4.3");  
		str_commonName.toUTF8String(sdata->commonName);
		commonNameSeq.Add(OIDcommonName);
		commonNameSeq.Add(str_commonName);
		commonNameSeq.CreateBinary();
		commonNameSet.Add(commonNameSeq);
		commonNameSet.CreateBinary();

		DerSequence sgiSeq1;
		sgiSeq1.Add(commonNameSet);
		sgiSeq1.CreateBinary();



		signerIdentifier.Add(sgiSeq1);


		//Номер сертификата открытого ключа подписанта.
		DerCoder certNum;
		mpz_class num_(sdata->certNum);
		certNum.IntegerToDer(num_);
		signerIdentifier.Add(certNum); //Помещаю в контейнер

		signerIdentifier.CreateBinary();
		
		SignerInfo.Add(signerIdentifier);

		//DigestAlgorithmIdentifier-алгоритм дайджеста.
		DerSequence DigestAlgorithSeq;
		DerCoder OIDalgIden;
		DerCoder null2; //NULL
		null2.setNull(); //Делаю Null значением
		OIDalgIden.to_OBJECT_IDENTIFIER(sdata->digestAlgorithms);
		DigestAlgorithSeq.Add(OIDalgIden);
		DigestAlgorithSeq.Add(null2);
		DigestAlgorithSeq.CreateBinary();
		SignerInfo.Add(DigestAlgorithSeq);
		

		//signedAttrs [0] IMPLICIT SignedAttributes OPTIONAL,
		DerContextSpecific signedAttrs;

		DerSequence attr1;
		DerCoder OIDattr1; //contentType
		OIDattr1.to_OBJECT_IDENTIFIER("1.2.840.113549.1.9.3");
		attr1.Add(OIDattr1);

		DerSet attr1DateSet;
		DerCoder OIDattr2; //data
		OIDattr2.to_OBJECT_IDENTIFIER("1.2.840.113549.1.7.1");
		attr1DateSet.Add(OIDattr2);
		attr1DateSet.CreateBinary();
		attr1.Add(attr1DateSet);
		attr1.CreateBinary();
		
		signedAttrs.Add(attr1);
		
		//Время подписи.
		DerSequence attrSigningTime;
		DerCoder OIDattr3;//SigningTime
		OIDattr3.to_OBJECT_IDENTIFIER("1.2.840.113549.1.9.5");
		attrSigningTime.Add(OIDattr3);
		DerSet attrSigningTimeSet;
		DerCoder SigningTime;
		SigningTime.toUTCTime(sdata->signingTime);
		attrSigningTimeSet.Add(SigningTime);
		attrSigningTimeSet.CreateBinary();
		attrSigningTime.Add(attrSigningTimeSet);
		attrSigningTime.CreateBinary();

		signedAttrs.Add(attrSigningTime);

		//Дайджест сообщения.
		DerSequence attrDigestSeq;
		DerCoder OIDattr4; //Дайджест сообщения.
		OIDattr4.to_OBJECT_IDENTIFIER("1.2.840.113549.1.9.4");
		attrDigestSeq.Add(OIDattr4);
		DerSet attrDigestSet;

		DerCoder OctObj1;
	
		OctObj1.to_Octet_String(&sdata->digest);//Дайджест сообщения.  
		attrDigestSet.Add(OctObj1);
		attrDigestSet.CreateBinary();
		attrDigestSeq.Add(attrDigestSet);
		attrDigestSeq.CreateBinary();

		signedAttrs.Add(attrDigestSeq);

		//Данные сертификата подписанта.
		DerSequence signingCertSeq; // Корневой узел атрибутов данных сертификата.
		DerCoder OIDattr5; //signingCertificate
		OIDattr5.to_OBJECT_IDENTIFIER("1.2.840.113549.1.9.16.2.47");
		signingCertSeq.Add(OIDattr5);
		
	
		DerSequence signingCertSeqIn2;
				
		DerSequence signingCertSeqDigest; //Алгоритм отпечатка сертификата.
		DerCoder OID1;
		OID1.to_OBJECT_IDENTIFIER(sdata->signingCertDigestAlg);   
		signingCertSeqDigest.Add(OID1);
		signingCertSeqDigest.CreateBinary();
		signingCertSeqIn2.Add(signingCertSeqDigest);

		DerCoder octetStr1;
		
		//Отпечаток сертификата подписанта.
		octetStr1.to_Octet_String(&sdata->signingCertDigest);
		signingCertSeqIn2.Add(octetStr1);
		
		

		//-----------Данные организации------------
		DerSet cmSet1; //Название организации.
		DerSequence cmSeq1;
		DerCoder oid2;//commonName
		oid2.to_OBJECT_IDENTIFIER("2.5.4.3");
		DerCoder prStr1;
		prStr1.toUTF8String(sdata->commonName);
		cmSeq1.Add(oid2);
		cmSeq1.Add(prStr1);
		cmSeq1.CreateBinary();
		cmSet1.Add(cmSeq1);
		cmSet1.CreateBinary();
		//----------------------------------------

		DerSequence signingCertSeqB;
		signingCertSeqB.Add(cmSet1); //Добавляю данные организации.
		signingCertSeqB.CreateBinary();
	
		DerContextSpecific Cs1;
		Cs1.Add(signingCertSeqB);
		Cs1.CreateBinary(0x00, true);

		DerSequence signingCertSeqA;
		signingCertSeqA.Add(Cs1);
		signingCertSeqA.CreateBinary();

		DerSequence signingCertSeqC;
		signingCertSeqC.Add(signingCertSeqA);

		//---------Cерийный номер сертификата подписанта.
		 DerCoder certNum1;
		 mpz_class num1_(sdata->certNum);
		 certNum1.IntegerToDer(num1_);
		 signingCertSeqC.Add(certNum1);
		//----------------------------------------------
		 		
		signingCertSeqC.CreateBinary();
		signingCertSeqIn2.Add(signingCertSeqC);
		signingCertSeqIn2.CreateBinary();

		DerSequence signingCertSeqIn1;
		signingCertSeqIn1.Add(signingCertSeqIn2);
		signingCertSeqIn1.CreateBinary();

		DerSet signingCertSet; //Корневая последовательность данных сертификата.
		DerSequence signingCertSeqIn;
		signingCertSeqIn.Add(signingCertSeqIn1);
		signingCertSeqIn.CreateBinary();
		signingCertSet.Add(signingCertSeqIn);
		signingCertSet.CreateBinary();
		signingCertSeq.Add(signingCertSet);
		signingCertSeq.CreateBinary();

		signedAttrs.Add(signingCertSeq);
		
		signedAttrs.CreateBinary(0x00, true);

		SignerInfo.Add(signedAttrs);

		//----------Алгоритм ЭЦП.
		DerSequence signatureAlgorithm;
		DerCoder  oidSign;
		oidSign.to_OBJECT_IDENTIFIER(sdata->signatureAlgorithm);  
		signatureAlgorithm.Add(oidSign);

		DerCoder null3; //NULL
		null3.setNull(); //Делаю Null значением
		signatureAlgorithm.Add(null3);
		//---------------------------
		signatureAlgorithm.CreateBinary();
		SignerInfo.Add(signatureAlgorithm);

		//---------Данные ЭЦП-вектора r и s-------------------
		DerCoder signatureValue;
		signatureValue.to_Octet_String(&sdata->signVal); 
		SignerInfo.Add(signatureValue);
		//----------------------------

		SignerInfo.CreateBinary();
		signerInfoSet.Add(SignerInfo);
		signerInfoSet.CreateBinary();
		content.Add(signerInfoSet);

		content.CreateBinary();
		contentCS.Add(content);
		contentCS.CreateBinary(0x00, true); //Формирую контейнер-пишу тег и длину
		SignedData.Add(contentCS);

		SignedData.CreateBinary();
		
		
		QByteArray outBa;
		SignedData.WriteData(outBa); //В массив.
		
	return outBa;
}


//
//void CertCreator::CreateUserCertificate(TBSCertificate Tbs, QByteArray &WrBa)
//{

//	DerSequence SeqTbsCertificate;
//
//	//----------------------Версия сертификата V3-----------------------
//	DerContextSpecific CS_Version;
//
//	//Cоздаю объект Integer
//	DerCoder version;
//	mpz_class v(2);
//	version.IntegerToDer(v);
//
//	CS_Version.Add(version); //Помещаю в контейнер
//	CS_Version.CreateBinary(0x00, true); //Формирую контейнер-пишу тег и длину
//
//	SeqTbsCertificate.Add(CS_Version);
//	//------------------------------------------------------------------
//	//-----------------Серийный номер сертификата-----------------------
//	DerCoder  serialNumber;
//	serialNumber.IntegerToDer(Tbs.serialNumber);
//	SeqTbsCertificate.Add(serialNumber);
//	//------------------------------------------------------------------
//	//-----------------Signature Это субполе содержит идентификатор алгоритма, используемого УЦ для подписи сертификата-----------------------
//	DerSequence Signature;
//
//	DerCoder  OidAlgorithmIdentifier;
//	OidAlgorithmIdentifier.to_OBJECT_IDENTIFIER(Tbs.AlgorithmIdentifier);
//	Signature.Add(OidAlgorithmIdentifier);
//	DerCoder x_null; //NULL
//	x_null.setNull(); //Делаю Null значением
//	Signature.Add(x_null);
//	Signature.CreateBinary(); //Оформляю контейнер в виде бинарного вектора
//	SeqTbsCertificate.Add(Signature);
//	//------------------------------------------------------------------
//	//----------Издатель сертификата
//	DerSequence Issuer;
//
//
//	//--Country
//	DerSet Issuer_Set_Country;
//	DerSequence Issuer_Seq_Country;
//	DerCoder OID_Country;
//	DerCoder str_Country;
//	OID_Country.to_OBJECT_IDENTIFIER("2.5.4.6");
//	str_Country.toPrintableString(Tbs.Issuer_id_at_countryName);
//	Issuer_Seq_Country.Add(OID_Country);
//	Issuer_Seq_Country.Add(str_Country);
//	Issuer_Seq_Country.CreateBinary();
//	Issuer_Set_Country.Add(Issuer_Seq_Country);
//	Issuer_Set_Country.CreateBinary();
//	Issuer.Add(Issuer_Set_Country);
//
//	//--Location
//	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.7", Tbs.Issuer_id_at_localityName));
//	//--адрес издателя
//	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.9", Tbs.Issuer_id_at_streetAddress));
//	//--organizationName--
//	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.10", Tbs.Issuer_id_at_organizationName));
//	//--commonName
//	Issuer.Add(GetIncapsulatedSetUtf8("2.5.4.3", Tbs.Issuer_id_at_commonName));
//
//	//--e-mail
//	DerSet Issuer_Set_Mail;
//	DerSequence Issuer_Seq_Mail;
//	DerCoder OIDe_mail;
//	OIDe_mail.to_OBJECT_IDENTIFIER("1.2.840.113549.1.9.1");
//	DerCoder str_eMail;
//	str_eMail.toIA5String(Tbs.Issuer_id_at_emailAddress);
//	Issuer_Seq_Mail.Add(OIDe_mail);
//	Issuer_Seq_Mail.Add(str_eMail);
//	Issuer_Seq_Mail.CreateBinary();
//	Issuer_Set_Mail.Add(Issuer_Seq_Mail);
//	Issuer_Set_Mail.CreateBinary();
//	Issuer.Add(Issuer_Set_Mail);
//
//
//	//---
//	Issuer.CreateBinary();
//	SeqTbsCertificate.Add(Issuer);
//
//	//---------------- Validity период действия----------------------
//	DerSequence Validity;
//	DerCoder NotBefore;
//	DerCoder NotAfter;
//	NotBefore.toUTCTime(Tbs.NotBefore.toUTC()); //Срок действия в UTC формате
//	NotAfter.toUTCTime(Tbs.NotAfter.toUTC());
//	Validity.Add(NotBefore);
//	Validity.Add(NotAfter);
//	Validity.CreateBinary();
//	SeqTbsCertificate.Add(Validity);
//	//----------------subject -субъект для которого выдан сертификат. Для CA идентичен полю издатель
//	DerSequence Subject;
//
//	//--Country
//	Subject.Add(GetIncapsulatedSetPntStr("2.5.4.6", Tbs.Subject_id_at_countryName));
//	//----Location
//	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.7", Tbs.Subject_id_at_localityName));
//	//--адрес издателя
//	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.9", Tbs.Subject_id_at_streetAddress));
//	//--organizationName--
//	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.10", Tbs.Subject_id_at_organizationName));
//	//--commonName
//	Subject.Add(GetIncapsulatedSetUtf8("2.5.4.3", Tbs.Subject_id_at_commonName));
//
//	//--e-mail
//	Subject.Add(GetIncapsulatedSetIA5("1.2.840.113549.1.9.1", Tbs.Subject_e_mailAddress));
//
//	Subject.CreateBinary();
//	SeqTbsCertificate.Add(Subject);
//
//	//--------------------subjectPublicKeyInfo
//	//--Это поле предназначено для указания открытого ключа и идентификации криптоалгоритма, совместно с которым используется ключ 
//	DerSequence subjectPublicKeyInfo;
//	subjectPublicKeyInfo.Add(GetPublicKeyAlgorithm(Tbs.publicKeyParamSet, Tbs.digestParamSet, Tbs.encryptionParamSet));
//	DerCoder subjectPublicKey;
//
//
//	subjectPublicKey.BIT_STRING_encapsulates_EC_public_key(Tbs.Qx, Tbs.Qy); //Открытый ключ
//
//	subjectPublicKeyInfo.Add(subjectPublicKey);
//
//	subjectPublicKeyInfo.CreateBinary();
//	SeqTbsCertificate.Add(subjectPublicKeyInfo);
//	//---------------extensions  расширения сертификата 3 версии
//	SeqTbsCertificate.Add(GetExtensions(Tbs));
//
//	SeqTbsCertificate.CreateBinary(); //SetСontainerInfo();
//
//									  //----------------------------Окончание Tbs блока------------------
//									  //---Формирую цифровую подпись
//	DerCoder signatureValue;
//	mpz_class prvKey; //Закрытый ключ
//
//	signatureValue = SignCertificate(SeqTbsCertificate, PubKey, private_key);
//	//----------------------------
//	Certificate.Add(SeqTbsCertificate);
//	//-----------signatureAlgorithm
//	/*
//	Алгоритм вычисления подписи содержит идентификатор криптоалгоритма,
//	используемого УЦ при формировании подписи данного сертификата.
//	*/
//	DerSequence signatureAlgorithm;
//	DerCoder OID_signatureAlgorithm;
//	OID_signatureAlgorithm.to_OBJECT_IDENTIFIER("1.2.643.7.1.1.3.3"); //id-tc26-signwithdigest-gost3410-2012-256
//	signatureAlgorithm.Add(OID_signatureAlgorithm);
//	DerCoder NullVal;
//	NullVal.setNull(); //Тип данных NULL
//	signatureAlgorithm.Add(NullVal);
//	signatureAlgorithm.CreateBinary();
//	Certificate.Add(signatureAlgorithm);
//
//	//---------------------signatureValue содержит цифровую подпись, вычисленную по последовательности поля tbsCertificate
//	//DerCoder signatureValue;
//	/*
//	unsigned char Sign[] = {0x01,0x25};
//
//	unsigned long Sign_len = sizeof(Sign) / sizeof(unsigned char);
//	signatureValue.to_Bit_String(Sign, Sign_len);
//	*/
//	Certificate.Add(signatureValue);
//	Certificate.CreateBinary();
//	Certificate.WriteCertB64(WrBa); //Сохраняю сертификат в массив.
//									//Certificate.WriteCertB64(save_path); //Формирую сертификат
//}
