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

	//������� �������� ������������� ��� �������� 
	//���������� ����������������� ���������,�������� RFC 5652.
	QByteArray createSignedBin(rfcSignedData *sdata);
};


/*
ContentInfo ::= SEQUENCE {
contentType ContentType,
content [0] EXPLICIT ANY DEFINED BY contentType }

ContentType ::= OBJECT IDENTIFIER
contentType ���������� ��� ���������� �����������.
content ������������ ����� ��������� � �������� ����������. ��� �����������
����� ���� ��������� ��������� �� contentType. ���� �����������: data,
signed-data, enveloped-data, digested-data, encrypted-data � authenticated-data.


Signed-data
��� signed-data �������� ������ ������ ���� � ����� �������� �������� ��������
��������. ���������� ��������� ����� ��������� ������������ ����� ���.


������� ����������� ���� signed-data �������� ������ � �������� ��������
����������� ���� data. ������ ��������� ���������� ����� ���� ��������
��������������� ������������ � ������� ������ ������������ (CRL ).

SignedData ::= SEQUENCE {
version CMSVersion,
digestAlgorithms DigestAlgorithmIdentifiers,
encapContentInfo EncapsulatedContentInfo,
certificates [0] IMPLICIT CertificateSet OPTIONAL,
crls [1] IMPLICIT RevocationInfoChoices OPTIONAL,
signerInfos SignerInfos }

DigestAlgorithmIdentifiers ::= SET OF DigestAlgorithmIdentifier
SignerInfos ::= SET OF SignerInfo

version ��������� ����� ������ ����������.
digestAlgorithms � ����� ��������������� ���������� ������� ���������
���������.

encapContentInfo � ����������� ����������, ��������� �� �������������� ����
� ���������� �����������.

EncapsulatedContentInfo ::= SEQUENCE {
eContentType ContentType,
eContent [0] EXPLICIT OCTET STRING OPTIONAL }

ContentType ::= OBJECT IDENTIFIER


eContentType � ������������� �������, ��������� �������� ��� �����������.
eContent � ���������� ����������, ������������ � ���� ������ �������(����� �� ����).

certificates,crls ��� ������������

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


��������� ���������� (�, �������������, �������� ����) ������������.
SignerIdentifier ::= CHOICE
{
issuerAndSerialNumber IssuerAndSerialNumber,
subjectKeyIdentifier [0] SubjectKeyIdentifier
}
����������  ������� subjectKeyIdentifier � �� ��������������
�����. '10C6A0FC00020000007D'

digestAlgorithm �������������� �������� �������� ������� ���������.



SignedAttributes ::= SET SIZE (1..MAX) OF Attribute
UnsignedAttributes ::= SET SIZE (1..MAX) OF Attribute
Attribute ::= SEQUENCE {
attrType OBJECT IDENTIFIER,
attrValues SET OF AttributeValue }
AttributeValue ::= ANY
SignatureValue ::= OCTET STRING


version � ����� ������ ����������.

SignedAttrs ������������ ����� ����������� ���������.

���� ������ ���� ������������, ��� ������ ���������
�� ������� ���� ��� ����������� ���� ��������:


*/