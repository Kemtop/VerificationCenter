#pragma once
//Таблица типов данных
class Asn1Tags
{
public:
	Asn1Tags();
	~Asn1Tags();

	enum
	{
		Boolean = 0x01,
	Integer = 0x02,
	BitString = 0x03,
	OctetString = 0x04,
	Null = 0x05,
	ObjectIdentifier = 0x06,
	External = 0x08,
	Enumerated = 0x0a,
	Sequence = 0x10, //SEQUENCE 30
	SequenceOf = 0x10, // for completeness
	Set = 0x11,
	SetOf = 0x11, // for completeness

	NumericString = 0x12,
	PrintableString = 0x13,
	T61String = 0x14,
	VideotexString = 0x15,
	IA5String = 0x16,
	UtcTime = 0x17,
	GeneralizedTime = 0x18,
	GraphicString = 0x19,
	VisibleString = 0x1a,
	GeneralString = 0x1b,
	UniversalString = 0x1c,
	BmpString = 0x1e,
	Utf8String = 0x0c,

	Constructed = 0x20,
	Application = 0x40,
	Tagged = 0x80

	};


	


	/*
	static const unsigned char Boolean;
	static const unsigned char Integer; 
	static const unsigned char BitString;
	static const unsigned char OctetString; 
	static const unsigned char Null; 
	static const unsigned char ObjectIdentifier; 
	static const unsigned char External;
	static const unsigned char Enumerated;
	static const unsigned char Sequence;
	static const unsigned char SequenceOf; // for completeness
	static const unsigned char Set;
	static const unsigned char SetOf; // for completeness

	static const unsigned char NumericString;
	static const unsigned char PrintableString; 
	static const unsigned char T61String;
	static const unsigned char VideotexString; 
	static const unsigned char IA5String; 
	static const unsigned char UtcTime;
	static const unsigned char GeneralizedTime; 
	static const unsigned char GraphicString; 
	static const unsigned char VisibleString; 
	static const unsigned char GeneralString; 
	static const unsigned char UniversalString; 
	static const unsigned char BmpString; 
	static const unsigned char Utf8String; 

	static const unsigned char Constructed; 
	static const unsigned char Application; 
	static const unsigned char Tagged;
	*/
};

