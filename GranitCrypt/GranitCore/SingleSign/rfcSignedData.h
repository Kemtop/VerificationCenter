#pragma once

#include <QObject>
#include <string>
#include <QDateTime>

using std::string;

//Класс хранит информацию для формирования ASN1 данных для формирования 
//синтаксиса криптографических сообщений, согласно стандарта RFC 5652. В данном случае для инкапсуляции цифровой подписи.
class rfcSignedData 
{
public:
	rfcSignedData();
	~rfcSignedData();

	int CMSVersion; //Номер версии синтаксиса.
	string digestAlgorithms; //Идентификатор алгоритмов расчета отпечатка сообщения.
	string commonName;
	int certNum;//Номер сертификата подписанта.
	QDateTime signingTime; //Время подписи.
	std::vector <unsigned char> digest; //Дайджест сообщения.

	string signingCertDigestAlg; //Алгоритм отпечатка сертификата подписанта.
	std::vector <unsigned char> signingCertDigest;   //Отпечаток сертификата подписанта.
	
	string signatureAlgorithm;//Алгоритм ЭЦП.
	std::vector<unsigned char> signVal; //Электронно Цифровая Подпись.

};
