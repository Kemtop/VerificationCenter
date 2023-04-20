#include "stdafx.h"
#include "TBSCertificate.h"


TBSCertificate::TBSCertificate()
{
}


TBSCertificate::~TBSCertificate()
{
}

void TBSCertificate::setQx(string str_Qx)
{
	val_str_Qx = str_Qx;
	Qx.set_str(str_Qx, 16);
}

void TBSCertificate::setQy(string str_Qy)
{
	val_str_Qy=str_Qy;
	Qy.set_str(str_Qy, 16);
}

string TBSCertificate::getQx()
{
	return val_str_Qx;
}

string TBSCertificate::getQy()
{
	return val_str_Qy;
}


//Устанавливает расширение 2.5.29.15,  указывает цель, для которой используется сертифицированный открытый ключ.
void TBSCertificate::SetkeyUsageExtension()
{
	ExtensionField ExF1;
	ExF1.OID = "2.5.29.15"; //id-ce-keyUsage Это расширение указывает цель, для которой используется сертифицированный открытый ключ.
							//Это расширение может, по выбору издателя сертификатов, быть критичным или некритическим.
	//unsigned char val1[] = { 0x03,0x02,0x01,0x86 };
	unsigned char val1[] = { 0x03,0x02,0x01,0x06 };
	ExF1.FillArray(val1, 4);
	extensions.push_back(ExF1);
}

//Устанавливает расширение 2.5.29.19 Это расширение указывает, может ли субъект действовать как ЦС
void TBSCertificate::SetbasicConstraintsExtension()
{
	/*
	id-ce-basicConstraints
	Это расширение указывает, может ли субъект действовать как ЦС, при этом сертифицированный открытый ключ
	используется для проверки подписей сертификатов. Если это так, может быть указано ограничение длины пути сертификации.
	Это расширение может, по выбору издателя сертификатов, быть критичным или некритическим.
	*/
	ExtensionField ExF2;
	ExF2.OID = "2.5.29.19";
	unsigned char val2[] = { 0x30,0x03,0x01,0x01,0xff };
	ExF2.FillArray(val2, 5);
	//ExF2.Boolean = 1;
	extensions.push_back(ExF2);
	//--------------------------------
}

void TBSCertificate::setUsageExtensionOnlySign()
{
	ExtensionField ExF1;
	ExF1.OID = "2.5.29.15"; //id-ce-keyUsage Это расширение указывает цель, для которой используется сертифицированный открытый ключ.
							//Это расширение может, по выбору издателя сертификатов, быть критичным или некритическим.
							//unsigned char val1[] = { 0x03,0x02,0x01,0x86 };
	unsigned char val1[] = { 0x03,0x02,0x01,0x80 };
	ExF1.FillArray(val1, 4);
	extensions.push_back(ExF1);
}


/*
Это расширение идентифицирует открытый ключ. Он позволяет дифференцировать отдельные ключи,
используемые одним и тем же субъектом (например, при обновлении ключа).
Идентификатор ключа должен быть уникальным по отношению ко всем ключевым
идентификаторам субъекта, с которым он используется. Это расширение всегда не критично.
*/
void TBSCertificate::subjectKeyIdentifierExtension()
{
	ExtensionField ExF3;
	ExF3.OID = "2.5.29.14";
	//unsigned char val3[] = { 0x01,0x02,0x03 };
	unsigned char val3[] 
	={
		0x04,
		0x14,0x5D,0x92,0x01,0xE5,0xA9,0x3A,0x26,0xAD,0x92,0xC5,0xA2,0xD26,0x34,0x43,0x07,0x89,0xF0,0x34,0x00,0xBE 
	};

	ExF3.FillArray(val3, 22);
	extensions.push_back(ExF3);
}
//Версия Центра Сертификации
void TBSCertificate::verExtension()
{
	ExtensionField ExF4;
	ExF4.OID = "1.3.6.1.4.1.311.21.1";
	unsigned char val4[] = { 0x02,0x01,0x00 };
	ExF4.FillArray(val4, 3);
	extensions.push_back(ExF4);
}

void TBSCertificate::setPubKey(Point Q)
{
	pubECkey=Q;
}

Point TBSCertificate::GetPubKey()
{
	return pubECkey;
}

void TBSCertificate::setIssuerFromSubject(TBSCertificate subject)
{	
	 Issuer_id_at_emailAddress= subject.Subject_e_mailAddress;//e_mail издателя сертификата
	 Issuer_id_at_countryName = subject.Subject_id_at_countryName; //Страна издателя
	 Issuer_id_at_localityName= subject.Subject_id_at_localityName; //Местоположение издателя
	 Issuer_id_at_organizationName= subject.Subject_id_at_organizationName;//Организация издавшая сертификат
	 Issuer_id_at_organizationalUnitName = subject.Subject_id_at_organizationalUnitName; //Подразделение издателя
	 Issuer_id_at_commonName= subject.Subject_id_at_commonName; //Общепринятое имя организации издавшей сертификат.
	 Issuer_id_at_streetAddress= subject.Subject_id_at_streetAddress;//Адресс издателя
	 Issuer_id_at_emailAddress= subject.Subject_id_at_emailAddress;
}

/*
Устанавливает алгоритмы сертификата. Копирует данные из родителя.
Копируются алгоритмы сертификата,алгоритмы открытых ключей.
*/
void TBSCertificate::setAlgoritms(TBSCertificate parent)
{						
	AlgorithmIdentifier = parent.AlgorithmIdentifier;
	publicKeyParamSet = parent.publicKeyParamSet;
	digestParamSet = parent.digestParamSet ;//Параметр эллиптической кривой."1.2.643.7.1.2.1.2.1";id-tc26-gost-3410-2012-512-paramSetB 										  
	encryptionParamSet = parent.encryptionParamSet;//Тип хэш функции id-tc26-gost3411-2012-512
}

void TBSCertificate::setTbsBin(vector<unsigned char> val)
{
	tbsBin = val;
}

void TBSCertificate::setSignValBin(vector<unsigned char> val)
{
	signValBin = val;
}

//Преобразовывает подпись в mpz_class S,R.
void TBSCertificate::Tr512Sign()
{
	//В сертификате подпись представлена как S,R
	vector<uint8_t> v_S;
	vector<uint8_t> v_R;

	//Получаю  подпись-вектора S,R
	for (int i = 0;i < 64;i++) v_S.push_back(signValBin[i]); //S
	for (int i = 64;i < 128;i++) v_R.push_back(signValBin[i]); //R

	//Преобразование векторов подписи в mpz_class
	mpz_class r_; //Вектора подписи
	mpz_class s_;

	size_t r_len = static_cast<size_t>(v_R.size());
	size_t s_len = static_cast<size_t>(v_S.size());
	mpz_import(r_.get_mpz_t(), r_len, 1, 1, 0, 0, &v_R[0]);
	mpz_import(s_.get_mpz_t(), s_len, 1, 1, 0, 0, &v_S[0]);

	R = r_;
	S = s_;
}

mpz_class TBSCertificate::GetR()
{
	return R;
}

mpz_class TBSCertificate::GetS()
{
	return S;
}

void TBSCertificate::Set512PubKey(vector<uint8_t> val)
{
	//Преобразование открытого ключа
	vector<uint8_t> p_x;
	vector<uint8_t> p_y;
	//Получение координат
	for (int i = 63;i >= 0;i--)	p_x.push_back(val[i]); //x
	for (int i = 127;i >= 64;i--) p_y.push_back(val[i]); //y

	mpz_class px_;//Параметры открытого ключа x,y
	mpz_class py_;

	size_t px_len = static_cast<size_t>(p_x.size());
	size_t py_len = static_cast<size_t>(p_y.size());
	mpz_import(px_.get_mpz_t(), px_len, 1, 1, 0, 0, &p_x[0]);
	mpz_import(py_.get_mpz_t(), py_len, 1, 1, 0, 0, &p_y[0]);

	Qx = px_;
	Qy = py_;

}

vector<uint8_t> TBSCertificate::GetBinTbs()
{
	//Меняю порядок следования.
	//Данные в правильной последовательности(старший бит идет первым)
	vector<unsigned char> dst;
		for (int i = tbsBin.size() - 1;i >= 0;i--)
		{
			dst.push_back(tbsBin[i]);
		}
	return dst;
}
