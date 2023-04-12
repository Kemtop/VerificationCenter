#pragma once

#include <QObject>
#include "SingleSign\rfcSignedData.h"
#include "ASN1\DerCoder\DerSequence.h"
#include "ASN1\DerCoder\DerContextSpecific.h"
#include "ASN1\DerCoder\DerSet.h"
#include "windows.h"


class binASN1SignedData 
{
	public:
	binASN1SignedData();
	~binASN1SignedData();

	//—оздает бинарное представлени€ Ё÷– согласно 
	//синтаксиса криптографических сообщений,стандарт RFC 5652.
	QByteArray createSignedBin(rfcSignedData *sdata);
};


/*
ContentInfo ::= SEQUENCE {
contentType ContentType,
content [0] EXPLICIT ANY DEFINED BY contentType }

ContentType ::= OBJECT IDENTIFIER
contentType показывает тип св€занного содержимого.
content представл€ет собой св€занное с объектом содержимое. “ип содержимого
может быть уникально определен из contentType. “ипы содержимого: data,
signed-data, enveloped-data, digested-data, encrypted-data и authenticated-data.


Signed-data
“ип signed-data содержит данные любого типа и может включать значени€ цифровых
подписей. —одержимое документа может подписать произвольное число лиц.


“иповым применением типа signed-data €вл€ютс€ данные с цифровой подписью
содержимого типа data. ƒругим вариантом применени€ этого типа €вл€етс€
распространение сертификатов и списков отзыва сертификатов (CRL ).

SignedData ::= SEQUENCE {
version CMSVersion,
digestAlgorithms DigestAlgorithmIdentifiers,
encapContentInfo EncapsulatedContentInfo,
certificates [0] IMPLICIT CertificateSet OPTIONAL,
crls [1] IMPLICIT RevocationInfoChoices OPTIONAL,
signerInfos SignerInfos }

DigestAlgorithmIdentifiers ::= SET OF DigestAlgorithmIdentifier
SignerInfos ::= SET OF SignerInfo

version указывает номер версии синтаксиса.
digestAlgorithms Ч набор идентификаторов алгоритмов расчета отпечатка
сообщени€.

encapContentInfo Ч подписанное содержимое, состо€щее из идентификатора типа
и собственно содержимого.

EncapsulatedContentInfo ::= SEQUENCE {
eContentType ContentType,
eContent [0] EXPLICIT OCTET STRING OPTIONAL }

ContentType ::= OBJECT IDENTIFIER


eContentType Ч идентификатор объекта, уникально задающий тип содержимого.
eContent Ч собственно содержимое, передаваемое в виде строки октетов(ћќ∆≈“ Ќ≈ быть).

certificates,crls Цне используютс€

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


”казывает сертификат (и, следовательно, открытый ключ) подписавшего.
SignerIdentifier ::= CHOICE
{
issuerAndSerialNumber IssuerAndSerialNumber,
subjectKeyIdentifier [0] SubjectKeyIdentifier
}
»спользуем  вариант subjectKeyIdentifier Ч по идентификатору
ключа. '10C6A0FC00020000007D'

digestAlgorithm идентифицирует алгоритм цифровой подписи сообщени€.



SignedAttributes ::= SET SIZE (1..MAX) OF Attribute
UnsignedAttributes ::= SET SIZE (1..MAX) OF Attribute
Attribute ::= SEQUENCE {
attrType OBJECT IDENTIFIER,
attrValues SET OF AttributeValue }
AttributeValue ::= ANY
SignatureValue ::= OCTET STRING


version Ч номер версии синтаксиса.

SignedAttrs представл€ет набор подписанных атрибутов.

≈сли данное поле присутствует, оно должно содержать
по крайней мере два приведенных ниже атрибута:


*/