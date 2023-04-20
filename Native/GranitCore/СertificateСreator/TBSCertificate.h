#pragma once
#include <string>
#include "mpirxx.h"
#include <vector>
#include <QDateTime>
#include "ExtensionField.h"
#include "Point.h"

using std::string;

class Q_DECL_EXPORT TBSCertificate
{
public:
	TBSCertificate();
	~TBSCertificate();

	int ver; //Версия сертификата
	mpz_class serialNumber;
	string AlgorithmIdentifier; //Идентификатор алгоритма, используемого УЦ для подписи сертификата
	//string Issuer_e_mailAddress;//e_mail издателя сертификата
	string Issuer_id_at_countryName; //Страна издателя
	string Issuer_id_at_localityName; //Местоположение издателя
	string Issuer_id_at_organizationName;//Организация издавшая сертификат
	string Issuer_id_at_organizationalUnitName; //Подразделение издателя
	string Issuer_id_at_commonName; //Общепринятое имя организации издавшей сертификат.
	string Issuer_id_at_streetAddress;//Адресс издателя
	string Issuer_id_at_emailAddress;


	QDateTime  NotBefore; //Период действия сертификата
	QDateTime NotAfter;
	//Данные субъекта который связан с открытым ключем.
	string Subject_e_mailAddress;//e_mail издателя сертификата
	string Subject_id_at_countryName; //Страна издателя
	string Subject_id_at_localityName; //Местоположение издателя
	string Subject_id_at_organizationName;//Организация издавшая сертификат
	string Subject_id_at_organizationalUnitName;
	string Subject_id_at_commonName; //Общепринятое имя организации издавшей сертификат.
	string Subject_id_at_streetAddress;//Адрес владельца сертификата
	string Subject_id_at_emailAddress;
	 // subjectPublicKeyInfo
	string publicKeyParamSet;
	string digestParamSet;
	string encryptionParamSet;
	//--subjectPublicKey
	mpz_class Qx;
	mpz_class Qy;

	//---Extensions --расширение 
	vector <ExtensionField>  extensions;
	string CerR=""; //Подпись сертификата(содержащаяся в файле сертификата),вектор R.
	string CerS=""; //Подпись сертификата(содержащаяся в файле сертификата),вектор S.
	string calcR=""; //Значение вектора R в результате вычисления подписи сертификата.
	string calcS=""; //Значение вектора S в результате вычисления подписи сертификата.
	string  digest; //Дайджест сообщения.

	//Порядок циклической подгруппы группы точек эллиптической кривой -удалить
	mpz_class q;
	//Задает координтау x ключа проверки подписи(открытого ключа)
	void setQx(string str_Qx);	
	//Задает координтау x ключа проверки подписи(открытого ключа)
	void setQy(string str_Qy);
	//Возвращает строковое значение ключа проверки подписи(открытого ключа) установленного методами  setQx 
	string getQx();
	//Возвращает строковое значение ключа проверки подписи(открытого ключа) установленного методами  setQy
	string getQy();

	//Устанавливает расширение 2.5.29.15,  указывает цель, для которой используется сертифицированный открытый ключ.
	void SetkeyUsageExtension();
	//Устанавливает расширение 2.5.29.19 Это расширение указывает, может ли субъект действовать как ЦС
	void SetbasicConstraintsExtension();
	//Устанавливает расширение только для подписи
	void setUsageExtensionOnlySign();
   
	/*
	Это расширение идентифицирует открытый ключ. Он позволяет дифференцировать отдельные ключи, 
	используемые одним и тем же субъектом (например, при обновлении ключа).
     Идентификатор ключа должен быть уникальным по отношению ко всем ключевым
	 идентификаторам субъекта, с которым он используется. Это расширение всегда не критично.
	*/
	void subjectKeyIdentifierExtension();
	//Версия Центра Сертификации
	void verExtension();
	//Устанавливает открытый ключ
	void setPubKey(Point Q);
	//Считывает открытый ключ
	Point GetPubKey();
	//Устанавливает данные издателя на основании данных subject. 
	void setIssuerFromSubject(TBSCertificate subject);
	/*
	   Устанавливает алгоритмы сертификата. Копирует данные из родителя.
	   Копируются алгоритмы сертификата,алгоритмы открытых ключей. 
	*/
	void setAlgoritms(TBSCertificate parent);
		
	void setTbsBin(vector<unsigned char> val);
	void setSignValBin(vector<unsigned char> val);

	void Tr512Sign(); //Преобразовывает подпись в mpz_class S,R.
	//Возвращает вектор R подписи.
	mpz_class GetR();
	//Возвращает вектор S подписи.
	mpz_class GetS();
	//Устанавливает значение открытого ключа-вектора Qx,Qy.
	void Set512PubKey(vector<uint8_t> val);
	//Возвращает бинарное представление tbs.
	vector<uint8_t> GetBinTbs();


private:
	//Строковое представление открытого ключа. Заполняется методами setQx setQy 
	string val_str_Qx;
	string val_str_Qy;

	//Открытый ключ владельца сертификата.
	Point pubECkey;

	vector<unsigned char> tbsBin;//Бинарное представление блока подлежащего подписи считанное из сертификата.
	vector<unsigned char> signValBin; //Бинарное представление подписи сертификата считанное из сертификата.

	mpz_class S; //Векторы подписи сертификата.
	mpz_class R;
};

