#include "stdafx.h"
#include "X509Reader.h"


X509Reader::X509Reader()
{
}


X509Reader::~X509Reader()
{
}


string X509Reader::VerifiRootSign(TBSCertificate &tbs, bool & SignStatus)
{	
	string err;
	int  KeyLen;
	int HashLen;
	err = GetKeyLen(tbs, KeyLen); //Проверяю алгоритмы сертификата, и возвращаю длину ключа.
	if (!err.empty()) return err; //Возникла какая то ошибка.
								
	err = GetHasLen(tbs, HashLen);
	if (!err.empty()) return err; //Возникла какая то ошибка.

	if (KeyLen != HashLen)
		return "Алгоритм вычисления хэш функции не совпадает с алгоритмом открытого ключа.";
	
	//Параметр эллиптической кривой
	//1.2.643.7.1.2.1.2.1
	//if (!((pubkey.EсOID == "1.2.643.7.1.2.1.2.1") || (pubkey.EсOID == "1.2.643.7.1.2.1.2.2")))
	

	Signature Sign; //Объект содержащий подпись.
	Sign.set_s(tbs.GetS()); //Передаю подписи
	Sign.set_r(tbs.GetR());
	Sign.set_type(1); //512бит

	//Создаю кривую на основании идентификатора передаваемого в сертификате
	Groups EC2;
	bool ec_found;//Флаг нахождения кривой по OID
	ec_found = EC2.SetParam(tbs.digestParamSet); //На основании OID открытого ключа задаю параметры кривой
	if (!ec_found)
		return "Системе не известна эллиптическая кривая с OID=" + tbs.digestParamSet;
	
	//Создаю точку с указанными в кривой координатами точки P
	Point P2(EC2, 1); //Точка эллиптической кривой 
	//Открытый ключ-Точка эллиптической кривой 
	Point Q1(EC2,1);
	Q1.x = tbs.Qx; //Передаю координаты полученные из сертификата
	Q1.y = tbs.Qy;
	
	//Передаю открытый ключ для информирования при проверке
	tbs.Qx = Q1.x;
	tbs.Qy = Q1.y;
	
	string a1 = Q1.get_hex_x();
	string a2 = Q1.get_hex_y();
		
	vector<unsigned char> tbsBin; //Бинарное представление tbs сертификата.
	tbsBin = tbs.GetBinTbs(); //Получаю данные из сертификата.
							
	unsigned long long l = static_cast<unsigned long long>(tbsBin.size());//Количество байт в TBS сертификате

	//Передаю значение текущее подписи сертификата для отображение при проверке сертификата.
	tbs.CerR = Sign.get_hex_r();
	tbs.CerS = Sign.get_hex_s();
		
	VerifySign Vs;  //Проверяю подпись
	bool sign_status = Vs.Verify(&tbsBin[0], l, P2, Q1, Sign);
	SignStatus = sign_status; //Состояние подписи
	tbs.calcR = Vs.GetLastR(); //Передаю значение  для отображение при проверке сертификата.
	tbs.digest = Vs.GetLastDigest();
	return "";
}

string X509Reader::VerifiCertSign(TBSCertificate & tbs, TBSCertificate & PubKeyTbs, bool & SignStatus)
{
	string err;
	int  KeyLen;
	int HashLen;
	err = GetKeyLen(tbs, KeyLen); //Проверяю алгоритмы сертификата, и возвращаю длину ключа.
	if (!err.empty()) return err; //Возникла какая то ошибка.

	err = GetHasLen(tbs, HashLen);
	if (!err.empty()) return err; //Возникла какая то ошибка.

	if (KeyLen != HashLen)
		return "Алгоритм вычисления хэш функции не совпадает с алгоритмом открытого ключа.";

	//Параметр эллиптической кривой
	//1.2.643.7.1.2.1.2.1
	//if (!((pubkey.EсOID == "1.2.643.7.1.2.1.2.1") || (pubkey.EсOID == "1.2.643.7.1.2.1.2.2")))


	Signature Sign; //Объект содержащий подпись.
	Sign.set_s(tbs.GetS()); //Передаю подписи
	Sign.set_r(tbs.GetR());
	Sign.set_type(1); //512бит

					  //Создаю кривую на основании идентификатора передаваемого в сертификате
	Groups EC2;
	bool ec_found;//Флаг нахождения кривой по OID
	ec_found = EC2.SetParam(tbs.digestParamSet); //На основании OID открытого ключа задаю параметры кривой
	if (!ec_found)
		return "Системе не известна эллиптическая кривая с OID=" + tbs.digestParamSet;

	//Создаю точку с указанными в кривой координатами точки P
	Point P2(EC2, 1); //Точка эллиптической кривой 
					  
	Point Q1(EC2, 1);//Создаю  ключ проверки подписи. 
	Q1.x = PubKeyTbs.Qx; //Передаю координаты ключа проверки подписи.
	Q1.y = PubKeyTbs.Qy; //

	vector<unsigned char> tbsBin; //Бинарное представление tbs сертификата.
	tbsBin = tbs.GetBinTbs(); //Получаю данные из сертификата.

	unsigned long long l = static_cast<unsigned long long>(tbsBin.size());//Количество байт в TBS сертификате

	//Передаю значение текущее подписи сертификата для отображение при проверке сертификата.
	tbs.CerR = Sign.get_hex_r();
	tbs.CerS = Sign.get_hex_s();

	VerifySign Vs;  //Проверяю подпись
	bool sign_status = Vs.Verify(&tbsBin[0], l, P2, Q1, Sign);
	SignStatus = sign_status; //Состояние подписи
	tbs.calcR = Vs.GetLastR(); //Передаю значение  для отображение при проверке сертификата.
	tbs.digest = Vs.GetLastDigest();
	return "";

}



//Извлекает из массива содержимое сертификат в base64.
QByteArray X509Reader::TrFromBase64(QByteArray cer)
{
	QByteArray ba_cert; //Содержимое декодированного сертификата
	
	//Сертификат может быть PEM формата, проверяю это.
	int y = cer.indexOf("-----BEGIN CERTIFICATE-----"); //27символов


	if (y != -1) //Сертификат PEM формата
	{
		int end_pos = cer.indexOf("-----END CERTIFICATE-----"); //Нахожу последнюю позицию для вырезки
		if (end_pos == -1)
		{
		   //	"Сертификат представлен в PEM кодировке. Не обнаружено окончание сертификата -----END CERTIFICATE----- ";
			throw "Bad cert format.";
		}

		QByteArray ba_base64;
		//Копирую кусок с данными
		for (int i = 27;i < end_pos;i++)
		{
			ba_base64.append(cer[i]);
		}

		ba_cert = QByteArray::fromBase64(ba_base64); //Конвертирую из Base64
	}
	else //Файл в формате DER
	{
		return cer;
	}

	return ba_cert;
}

void X509Reader::WriteToFile(QByteArray arr, QString path)
{
//	QString save_path = QCoreApplication::applicationDirPath() + "/Certificates/x1.cer";
	QFile file1(path);
	file1.open(QIODevice::WriteOnly);
	file1.write(arr);
	file1.close();
}

string X509Reader::Read(QString path,TBSCertificate &dataTbs)
{
 	QFile crtfile;
	crtfile.setFileName(path);
	if (!crtfile.open(QIODevice::ReadOnly))
	{
		return "Не могу открыть файл сертификата по указанному пути.";
	}
	
	crtfile.seek(0);//Устанавливаю в нулевую позицию на всякий случай.
	QByteArray bf = crtfile.readAll(); //Считываю все что есть в файле
	crtfile.close();

	QByteArray ba_cert;
	ba_cert=TrFromBase64(bf);//Декодирую сертификат из base64.
	
	//Считываю алгоритм подписи сертификата, бинарное значение tbs,бинарное значение подписи.
	GetBinSignInfo(ba_cert, dataTbs);
	GetTbsInfo(ba_cert, dataTbs);//Считываю основные данные TBS
	dataTbs.Tr512Sign(); //Преобразовываю бинарную подпись в вектора R,S.

	return "";
}

void X509Reader::ReadBa(QByteArray ba, TBSCertificate & dataTbs)
{	
	QByteArray ba_cert;
	ba_cert = TrFromBase64(ba);//Декодирую сертификат из base64.

							   //Считываю алгоритм подписи сертификата, бинарное значение tbs,бинарное значение подписи.
	GetBinSignInfo(ba_cert, dataTbs);
	GetTbsInfo(ba_cert, dataTbs);//Считываю основные данные TBS
	dataTbs.Tr512Sign(); //Преобразовываю бинарную подпись в вектора R,S.

}

string X509Reader::CheckRootSign(QString path, TBSCertificate &dataTbs, bool &SignStatus)
{
	SignStatus = false; //Всегда считаю что подпись не верна
	
	QFile crtfile;
	crtfile.setFileName(path);
	if (!crtfile.open(QIODevice::ReadOnly))
	{
		return "Не могу открыть файл сертификата по указанному пути.";
	}
		

	crtfile.seek(0);
	QByteArray bf = crtfile.readAll(); //Все что есть в файле
	crtfile.close();

	QByteArray ba_cert;
	ba_cert = TrFromBase64(bf);//Декодирую сертификат из base64.
		
	vector<unsigned char> tbs;
	string signatureAlgorithm = ""; //Алгоритм подписи
	vector<unsigned char> signatureValue; //Подпись сертификата
	
    //Получаю данные для проверки подписи
	GetInfoForVerifi(ba_cert, tbs, signatureAlgorithm, signatureValue);

	//Извлекаю открытый ключ
	ByteQueue CerBuf; //Массив хранит данные сертификата
	CerBuf.Put(reinterpret_cast<uint8_t*>(ba_cert.data()), ba_cert.size());
	PubKeyData pubkey; //Данные публичного ключа полученные из сертификата.
	GetPublicKeyFromCert(CerBuf, pubkey, dataTbs);

	//Алгоритм подписи сертификата
	pubkey.signatureAlgorithm = signatureAlgorithm;
	//Проверка подписи сертификата
	string err = VerifiSign(pubkey, signatureValue, tbs, dataTbs, SignStatus);
	if (!err.empty()) return err;

	return "";
}


//Проверяет подпись корневого сертификата в байт массиве. 
string X509Reader::CheckRootSign(QByteArray bf, TBSCertificate &dataTbs, bool &SignStatus)
{
	SignStatus = false; //Всегда считаю что подпись не верна
		
	QByteArray ba_cert;
	ba_cert = TrFromBase64(bf);//Декодирую сертификат из base64.

	vector<unsigned char> tbs;
	string signatureAlgorithm = ""; //Алгоритм подписи
	vector<unsigned char> signatureValue; //Подпись сертификата

										  //Получаю данные для проверки подписи
	GetInfoForVerifi(ba_cert, tbs, signatureAlgorithm, signatureValue);

	//Извлекаю открытый ключ
	ByteQueue CerBuf; //Массив хранит данные сертификата
	CerBuf.Put(reinterpret_cast<uint8_t*>(ba_cert.data()), ba_cert.size());
	PubKeyData pubkey; //Данные публичного ключа полученные из сертификата.
	GetPublicKeyFromCert(CerBuf, pubkey, dataTbs);

	//Алгоритм подписи сертификата
	pubkey.signatureAlgorithm = signatureAlgorithm;
	//Проверка подписи сертификата
	string err = VerifiSign(pubkey, signatureValue, tbs, dataTbs, SignStatus);
	if (!err.empty()) return err;

	return "";
}

string X509Reader::CheckCASign(QByteArray baCA, TBSCertificate & tbsCA, TBSCertificate & TbsRoot, bool & SignStatus)
{
	SignStatus = false; //Всегда считаю что подпись не верна

	QByteArray ba_cert;
	ba_cert = TrFromBase64(baCA);//Декодирую сертификат из base64.
	
	//Считываю алгоритм подписи сертификата, бинарное значение tbs,бинарное значение подписи.
	GetBinSignInfo(ba_cert, tbsCA);
	GetTbsInfo(ba_cert, tbsCA);//Считываю основные данные TBS
	tbsCA.Tr512Sign(); //Преобразовываю бинарную подпись в вектора R,S.
	
	//Проверка подписи сертификата
	string err =VerifiCertSign(tbsCA, TbsRoot,SignStatus);
	if (!err.empty()) return err;

	return "";
}



/*
Получает информацию для проверки подписи сертификата.
tbs-последовательность tbsCertificate;
signatureAlgorithm-OID алгоритма подписи сертификата;
signatureValue-цифровая подпись сертификата.
Заполняет вектора в tbs:
vector<unsigned char> tbsBin;//Бинарное представление блока подлежащего подписи.
vector<unsigned char> signValBin; //Бинарное представление подписи сертификата.
Устанавливает алгоритм подписи сертификата.
*/
void X509Reader::GetBinSignInfo(QByteArray baCert, TBSCertificate &tbs)
{
	vector<unsigned char> tbsBin;//Бинарное представление блока подлежащего подписи.

	//Преобразовываю данные QByteArray в  ByteQueue
	ByteQueue CerArr; //Массив хранит данные сертификата
	CerArr.Put(reinterpret_cast<uint8_t*>(baCert.data()), baCert.size());
	
	/*
	Структура сертификата X.509 v3(описано в RFC 5280):

	Certificate  ::=  SEQUENCE  {
	tbsCertificate       TBSCertificate,
	signatureAlgorithm   AlgorithmIdentifier,
	signatureValue       BIT STRING  }

	TBSCertificate-SEQUENCE  последовательность,данный для подписи.

	signatureAlgorithm-последовательность,  алгоритм вычисления подписи;
	AlgorithmIdentifier  ::=  SEQUENCE  {
	algorithm               OBJECT IDENTIFIER,
	parameters              ANY DEFINED BY algorithm OPTIONAL}
	*/


	ByteQueue cer_tbs; //Содержимое Tbs сертификата.
	BERSequenceDecoder x509Cert(CerArr); //Корневая последовательность
	BERSequenceDecoder tbsCert(x509Cert);//TBS сертификат
	DERSequenceEncoder tbsEnc(cer_tbs); //Копирую TBS сертификата
	tbsCert.TransferAllTo(tbsEnc);
	tbsEnc.MessageEnd();
		
	ByteQueueToVector(cer_tbs, tbsBin); //Конвертирую TBS сертификат

	//Поиск алгоритма используемого для подписи данных сертификата
	OID sigAlgOID; //OID алгоритма подписи сертификата
	BERSequenceDecoder sigAlg(x509Cert); 
	sigAlgOID.BERDecode(sigAlg);
	sigAlg.SkipAll();
	tbs.AlgorithmIdentifier = OIDtoString(sigAlgOID);//Конвертирую в строку и сохранение в Tbs.

	//Извлекаю подпись сертификата
	SecByteBlock certSignature;
	unsigned int unused = 0;
	BERDecodeBitString(x509Cert, certSignature, unused);
	x509Cert.SkipAll();

	//Копирую цифровую подпись
	vector<unsigned char> signatureValue;
	for (int i = 0;i < certSignature.size();i++)
		signatureValue.push_back(certSignature[i]);

	//Сохраняю данные
	tbs.setTbsBin(tbsBin);
	tbs.setSignValBin(signatureValue);
}

void X509Reader::GetTbsInfo(QByteArray baCert,TBSCertificate & Tbs)
{	
	ByteQueue CerBuf; //Массив хранит данные сертификата
	CerBuf.Put(reinterpret_cast<uint8_t*>(baCert.data()), baCert.size());//Преобразовываю данные

	BERSequenceDecoder x509Cert(CerBuf);
	BERSequenceDecoder tbsCert(x509Cert);
	// ASN.1 from RFC 3280
	// TBSCertificate  ::=  SEQUENCE  {
	// version         [0]  EXPLICIT Version DEFAULT v1,

	//Используем тег контекста версии сертификата
	BERGeneralDecoder context(tbsCert, 0xa0);
	word32 ver;

	//Получаем версию сертификата-3 версия сертификата
	BERDecodeUnsigned<word32>(context, ver, INTEGER, 2, 2);
	Tbs.ver = CpIntToInt(ver); //Сохраняю версию сертификата


	 // serialNumber         CertificateSerialNumber,
	Integer serial;
	serial.BERDecode(tbsCert);
	Tbs.serialNumber = IntToMpz(serial); //Сохраняю серийный номер


										 /*
										 Считываю последовательность signature            AlgorithmIdentifier,
										 Это субполе содержит идентификатор алгоритма, используемого УЦ для подписи СЕРТ.
										 Это субполе должно содержать один тот же идентификатор алгоритма, как и в поле «signatureAlgorithm»
										 последовательности «Сертификат». Содержание субполя «(optional) Parameters» (дополнительные параметры) может
										 варьироваться (и по длине), в зависимости от указанного алгоритма.
										 */
	BERSequenceDecoder signature(tbsCert);
	OID AlgorithmIdentifierOID; //OID алгоритма подписи сертификата
	AlgorithmIdentifierOID.BERDecode(signature); //Считываю алгоритм
	Tbs.AlgorithmIdentifier = OIDtoString(AlgorithmIdentifierOID);
	signature.SkipAll();

	//Считываю последовательность содержащую данные издателя.   issuer
	BERSequenceDecoder issuerName(tbsCert);
	GetIssuerData(issuerName, Tbs); //Считывание данных издателя
	issuerName.SkipAll();

	//Считываю последовательность содержащую срок действия сертификата
	BERSequenceDecoder validity(tbsCert);
	GetValidityData(validity, Tbs); //Cохраняю срок действия сертификата


	//Считываю последовательность содержащую данные издателя.subject  Name,
	BERSequenceDecoder subjectName(tbsCert);
	GetSubjectData(subjectName, Tbs);
	subjectName.SkipAll();

	//Считываю последовательность содержащую данные публичного ключа  subjectPublicKeyInfo SubjectPublicKeyInfo,
	BERSequenceDecoder spki(tbsCert);
	
	BERSequenceDecoder SubjectPublicKeyInfo(spki);
	OID spkAlgOID; //OID алгоритма открытого ключа
	spkAlgOID.BERDecode(SubjectPublicKeyInfo); //Считываю алгоритм
	Tbs.publicKeyParamSet = OIDtoString(spkAlgOID);//Сохраняю алгоритм открытого ключа


	BERSequenceDecoder PK_Algoritms(SubjectPublicKeyInfo);
	OID PK_AlgEc;
	OID PK_AlgHash;
	PK_AlgEc.BERDecode(PK_Algoritms);  //Считываю алгоритмы
	PK_AlgHash.BERDecode(PK_Algoritms);

	Tbs.digestParamSet = OIDtoString(PK_AlgEc);//OID эллиптической кривой
	Tbs.encryptionParamSet = OIDtoString(PK_AlgHash);//OID Хэш функции.

	SecByteBlock bit_str;
	unsigned int unused = 0;
	BERDecodeBitString(spki, bit_str, unused);

	vector<uint8_t> pubKey;
	pubKey=GetPubKey512(bit_str);//Считываю закрытый ключ в vector
	Tbs.Set512PubKey(pubKey); //Конвертирую и задаю открытый ключ.

	spki.SkipAll();
	tbsCert.SkipAll();
	x509Cert.SkipAll();
}

vector<uint8_t> X509Reader::GetPubKey512(SecByteBlock pk)
{
	//В Bit String coдержится OctetString. Дабы не колдовать -извлечем значение.
	//Так как строка содержитcя больше 127 бит появляеться дополнительный байт описывающий длину

	byte os_b1 = pk[0]; //Должно быть 04 -добавить проверки
	byte os_b2 = pk[1]; //81
	byte os_b3 = pk[2]; //80
	byte os_b4 = pk[3];

	//Копирую открытый ключ
	vector<uint8_t> PubKey;
	for (int i = 3;i < pk.size();i++)
		PubKey.push_back(pk[i]);
	
	return PubKey;
}

/*
Получает информацию для проверки подписи сертификата.
tbs-последовательность tbsCertificate;
signatureAlgorithm-OID алгоритма подписи сертификата;
signatureValue-цифровая подпись сертификата.
*/
void X509Reader::GetInfoForVerifi(QByteArray ba_cert, vector<unsigned char>& tbs, string & signatureAlgorithm, vector<unsigned char>& signatureValue)
{
	//Преобразовываю данные QByteArray в  ByteQueue
	ByteQueue CerArr; //Массив хранит данные сертификата
	CerArr.Put(reinterpret_cast<uint8_t*>(ba_cert.data()), ba_cert.size());


	/*
	Структура сертификата X.509 v3(описано в RFC 5280):

	Certificate  ::=  SEQUENCE  {
	tbsCertificate       TBSCertificate,
	signatureAlgorithm   AlgorithmIdentifier,
	signatureValue       BIT STRING  }

	TBSCertificate-SEQUENCE  последовательность,данный для подписи.

	signatureAlgorithm-последовательность,  алгоритм вычисления подписи;
	AlgorithmIdentifier  ::=  SEQUENCE  {
	algorithm               OBJECT IDENTIFIER,
	parameters              ANY DEFINED BY algorithm OPTIONAL}


	*/
	ByteQueue cer_tbs;
	BERSequenceDecoder x509Cert(CerArr); //Корневая последовательность
	BERSequenceDecoder tbsCert(x509Cert);//TBS сертификат
	DERSequenceEncoder tbsEnc(cer_tbs); //Копирую TBS сертификата
	tbsCert.TransferAllTo(tbsEnc);
	tbsEnc.MessageEnd();


	//	vector<byte> v;
	ByteQueueToVector(cer_tbs, tbs); //Конвертирую TBS сертификат

//	for (int i = 4;i < v.size();i++) tbs.push_back(v[i]);


	//Поиск алгоритма используемого для подписи данных сертификата
	OID sigAlgOID; //OID алгоритма подписи сертификата
	BERSequenceDecoder sigAlg(x509Cert);
	sigAlgOID.BERDecode(sigAlg);
	sigAlg.SkipAll();
	signatureAlgorithm = OIDtoString(sigAlgOID);//Конвертирую в строку

	//Извлекаю подпись сертификата
	SecByteBlock certSignature;
	unsigned int unused = 0;
	BERDecodeBitString(x509Cert, certSignature, unused);
	x509Cert.SkipAll();

	//Копирую цифровую подпись
	for (int i = 0;i < certSignature.size();i++)
		signatureValue.push_back(certSignature[i]);

}

//Считывает данные издателя
void X509Reader::GetIssuerData(BERSequenceDecoder &issuer, TBSCertificate &Tbs)
{

	string oid_str;
	string tmp;
	while (CpIntToInt(issuer.RemainingLength()) > 0)
	{
		BERSetDecoder set(issuer);
		BERSequenceDecoder seq(set);
		OID xoid; //Получаю идентификатор объекта
		xoid.BERDecode(seq);
		oid_str = OIDtoString(xoid);

		if (oid_str == "2.5.4.6")//countryName
		{
			CryptoPP::BERDecodeTextString(seq, tmp, CryptoPP::PRINTABLE_STRING);
			Tbs.Issuer_id_at_countryName = tmp;
		}

		if (oid_str == "2.5.4.7")//localityName [2.5.4.7]
		{
			CryptoPP::BERDecodeTextString(seq, tmp, CryptoPP::UTF8_STRING);
			Tbs.Issuer_id_at_localityName = tmp;
		}

		if (oid_str == "2.5.4.10")//organizationName[2.5.4.10]
		{
			CryptoPP::BERDecodeTextString(seq, tmp, CryptoPP::UTF8_STRING);
			Tbs.Issuer_id_at_organizationName = tmp;
		}

		if (oid_str == "2.5.4.11")//organizationalUnitName [2.5.4.11] 
		{
			CryptoPP::BERDecodeTextString(seq, tmp, CryptoPP::UTF8_STRING);
			Tbs.Issuer_id_at_organizationalUnitName = tmp;
		}

		if (oid_str == "2.5.4.3")//commonName [2.5.4.3]
		{
			ByteQueue val;
			seq.CopyMessagesTo(val);
			byte t = val[0];
			if(t==0x13)
				CryptoPP::BERDecodeTextString(val, tmp, CryptoPP::PRINTABLE_STRING);
			else
			CryptoPP::BERDecodeTextString(val, tmp, CryptoPP::UTF8_STRING);

			Tbs.Issuer_id_at_commonName = tmp;
		}

		if (oid_str == "1.2.840.113549.1.9.1")//emailAddress[1.2.840.113549.1.9.1]
		{
			CryptoPP::BERDecodeTextString(seq, tmp, CryptoPP::IA5_STRING);
			Tbs.Issuer_id_at_emailAddress = tmp;
		}

		if (oid_str == "2.5.4.9")//streetAddress [2.5.4.9]
		{
			ByteQueue val;
			seq.CopyMessagesTo(val);
			byte t = val[0];

			switch (t)
			{
			case 0x13:
				CryptoPP::BERDecodeTextString(val, tmp, CryptoPP::PRINTABLE_STRING);
				break;
			case 0x16:
				CryptoPP::BERDecodeTextString(val, tmp, CryptoPP::IA5_STRING);
				break;
			case 0x0c:
				CryptoPP::BERDecodeTextString(val, tmp, CryptoPP::UTF8_STRING);
				break;

			}
			Tbs.Issuer_id_at_streetAddress = tmp;
		}
		seq.SkipAll();
	}

}

//Считывает данные владельца сертификата
void X509Reader::GetSubjectData(BERSequenceDecoder & issuer, TBSCertificate & Tbs)
{
	string oid_str;
	string tmp;
	while (CpIntToInt(issuer.RemainingLength()) > 0)
	{
		BERSetDecoder set(issuer);
		BERSequenceDecoder seq(set);
		OID xoid; //Получаю идентификатор объекта
		xoid.BERDecode(seq);
		oid_str = OIDtoString(xoid);

		if (oid_str == "2.5.4.6")//countryName
		{
			ByteQueue val;
			seq.CopyMessagesTo(val);
			byte t = val[0];
			if (t == 0x13)
				CryptoPP::BERDecodeTextString(val, tmp, CryptoPP::PRINTABLE_STRING);
			else
				CryptoPP::BERDecodeTextString(val, tmp, CryptoPP::UTF8_STRING);
			Tbs.Subject_id_at_countryName = tmp;
		}

		if (oid_str == "2.5.4.7")//localityName [2.5.4.7]
		{
			CryptoPP::BERDecodeTextString(seq, tmp, CryptoPP::UTF8_STRING);
			Tbs.Subject_id_at_localityName = tmp;
		}

		if (oid_str == "2.5.4.10")//organizationName[2.5.4.10]
		{
			CryptoPP::BERDecodeTextString(seq, tmp, CryptoPP::UTF8_STRING);
			Tbs.Subject_id_at_organizationName = tmp;
		}

		if (oid_str == "2.5.4.11")//organizationalUnitName [2.5.4.11] 
		{
			CryptoPP::BERDecodeTextString(seq, tmp, CryptoPP::UTF8_STRING);
			Tbs.Subject_id_at_organizationalUnitName = tmp;
		}

		if (oid_str == "2.5.4.3")//commonName [2.5.4.3]
		{
			ByteQueue val;
			seq.CopyMessagesTo(val);
			byte t = val[0];
			if (t == 0x13)
				CryptoPP::BERDecodeTextString(val, tmp, CryptoPP::PRINTABLE_STRING);
			else
				CryptoPP::BERDecodeTextString(val, tmp, CryptoPP::UTF8_STRING);
			Tbs.Subject_id_at_commonName = tmp;
		}

		if (oid_str == "1.2.840.113549.1.9.1")//emailAddress[1.2.840.113549.1.9.1]
		{
			ByteQueue val;
			seq.CopyMessagesTo(val);
			byte t = val[0];

			switch (t)
			{
			   case 0x13:
				  CryptoPP::BERDecodeTextString(val, tmp, CryptoPP::PRINTABLE_STRING);
			   break;
			   case 0x16:
				   CryptoPP::BERDecodeTextString(val, tmp, CryptoPP::IA5_STRING);
			   break;
			   case 0x0c:
				   CryptoPP::BERDecodeTextString(val, tmp, CryptoPP::UTF8_STRING);
			   break;

			}						
			Tbs.Subject_id_at_emailAddress = tmp;
		}

		if (oid_str == "2.5.4.9")//streetAddress [2.5.4.9]
		{
			ByteQueue val;
			seq.CopyMessagesTo(val);
			byte t = val[0];

			switch (t)
			{
			case 0x13:
				CryptoPP::BERDecodeTextString(val, tmp, CryptoPP::PRINTABLE_STRING);
				break;
			case 0x16:
				CryptoPP::BERDecodeTextString(val, tmp, CryptoPP::IA5_STRING);
				break;
			case 0x0c:
				CryptoPP::BERDecodeTextString(val, tmp, CryptoPP::UTF8_STRING);
				break;

			}
			Tbs.Subject_id_at_streetAddress = tmp;
		}

		seq.SkipAll();
	}

}

void X509Reader::GetValidityData(BERSequenceDecoder & validity, TBSCertificate & Tbs)
{
	string sDateBegin;
	CryptoPP::BERDecodeTextString(validity, sDateBegin, CryptoPP::UTC_TIME);
	string sDateEnd;
	CryptoPP::BERDecodeTextString(validity, sDateEnd, CryptoPP::UTC_TIME);

	QDateTime DateBegin;
	QDateTime DateEnd;
	//Конвертирую из строки
	QString a = "20" + QString::fromStdString(sDateBegin);
	QString b = "20" + QString::fromStdString(sDateEnd);
	a.replace("Z", "");
	b.replace("Z", "");
	DateBegin = QDateTime::fromString(a, "yyyyMMddhhmmss");
	DateEnd = QDateTime::fromString(b, "yyyyMMddhhmmss");
	Tbs.NotBefore = DateBegin;
	Tbs.NotAfter = DateEnd;

}


void X509Reader::GetPublicKeyFromCert(CryptoPP::BufferedTransformation & certin, PubKeyData &pd, TBSCertificate &Tbs)
{
	BERSequenceDecoder x509Cert(certin);
	BERSequenceDecoder tbsCert(x509Cert);
	// ASN.1 from RFC 3280
	// TBSCertificate  ::=  SEQUENCE  {
	// version         [0]  EXPLICIT Version DEFAULT v1,

	//Используем тег контекста версии сертификата
	BERGeneralDecoder context(tbsCert, 0xa0);
	word32 ver;

	//Получаем версию сертификата-3 версия сертификата
	BERDecodeUnsigned<word32>(context, ver, INTEGER, 2, 2);
	Tbs.ver = CpIntToInt(ver); //Сохраняю версию сертификата


	// serialNumber         CertificateSerialNumber,
	Integer serial;
	serial.BERDecode(tbsCert);
	Tbs.serialNumber = IntToMpz(serial); //Сохраняю серийный номер


	/*
	Считываю последовательность signature            AlgorithmIdentifier,
	Это субполе содержит идентификатор алгоритма, используемого УЦ для подписи СЕРТ.
	Это субполе должно содержать один тот же идентификатор алгоритма, как и в поле «signatureAlgorithm»
	последовательности «Сертификат». Содержание субполя «(optional) Parameters» (дополнительные параметры) может
	варьироваться (и по длине), в зависимости от указанного алгоритма.
	*/
	BERSequenceDecoder signature(tbsCert);
	OID AlgorithmIdentifierOID; //OID алгоритма подписи сертификата
	AlgorithmIdentifierOID.BERDecode(signature); //Считываю алгоритм
	Tbs.AlgorithmIdentifier = OIDtoString(AlgorithmIdentifierOID);
	signature.SkipAll();

	//Считываю последовательность содержащую данные издателя.   issuer
	BERSequenceDecoder issuerName(tbsCert);
	GetIssuerData(issuerName, Tbs); //Считывание данных издателя
	issuerName.SkipAll();

	//Считываю последовательность содержащую срок действия сертификата
	BERSequenceDecoder validity(tbsCert);
	GetValidityData(validity, Tbs); //Cохраняю срок действия сертификата


	//Считываю последовательность содержащую данные издателя.subject  Name,
	BERSequenceDecoder subjectName(tbsCert);
	GetSubjectData(subjectName, Tbs);
	subjectName.SkipAll();

	//Считываю последовательность содержащую данные публичного ключа  subjectPublicKeyInfo SubjectPublicKeyInfo,
	BERSequenceDecoder spki(tbsCert);


	BERSequenceDecoder SubjectPublicKeyInfo(spki);
	OID sigAlgOID; //OID алгоритма подписи сертификата
	sigAlgOID.BERDecode(SubjectPublicKeyInfo); //Считываю алгоритм

	pd.SetAlgoritmOID(sigAlgOID); //Сохраняю алгоритм использования ключа
	Tbs.publicKeyParamSet = OIDtoString(sigAlgOID);


	BERSequenceDecoder PK_Algoritms(SubjectPublicKeyInfo);
	OID PK_AlgEc;
	OID PK_AlgHash;
	PK_AlgEc.BERDecode(PK_Algoritms);  //Считываю алгоритмы
	PK_AlgHash.BERDecode(PK_Algoritms);
	pd.SetEсOID(PK_AlgEc);
	pd.SetHashOID(PK_AlgHash);

	Tbs.digestParamSet = OIDtoString(PK_AlgEc);
	Tbs.encryptionParamSet = OIDtoString(PK_AlgHash);

	SecByteBlock bit_str;
	unsigned int unused = 0;
	BERDecodeBitString(spki, bit_str, unused);

	pd.SetPubKey(bit_str); //Считываю закрытый ключ

	spki.SkipAll();
	tbsCert.SkipAll();
	x509Cert.SkipAll();
}




void X509Reader::ByteQueueToVector(ByteQueue &Bq, vector<unsigned char>& v)
{
	CryptoPP::lword d_size;
	d_size = Bq.TotalBytesRetrievable();
	CryptoPP::Integer d1_size;
	d1_size = d_size;
	int arr_len = static_cast<int>(d1_size.ConvertToLong());

	byte *arr = new byte[arr_len];
	Bq.Get(arr, arr_len);

	std::copy(arr, arr + arr_len, std::back_inserter(v));
	delete[]arr; arr = NULL;
}

//Конвертирует OID в String 
string X509Reader::OIDtoString(OID xOID)
{
	vector<word32> I;
	I = xOID.GetValues();
	string str = "";

	//Конвертирую
	for (int i = 0;i < I.size();i++)
	{
		if (i != 0) str = str + ".";
		str = str + std::to_string(I[i]);
	}


	return str;
}

mpz_class X509Reader::IntToMpz(Integer d)
{
	string digit = CryptoPP::IntToString(d, 16);
	mpz_class x(digit, 16);

	return x;
}

int X509Reader::CpIntToInt(Integer d)
{
	string digit = CryptoPP::IntToString(d, 10);
	int x;
	x = std::stoi(digit);
	return x;
}

/*
Проверяет подпись сертификата.
pubkey-данные открытого ключа сертификата;
signatureAlgorithm-OID алгоритма подписи сертификата;
signatureValue-цифровая подпись сертификата
*/
bool X509Reader::VerifiSign(PubKeyData pubkey, string signatureAlgorithm,
	vector<unsigned char> signatureValue, vector<unsigned char> &tbs, TBSCertificate &retdataTbs)
{
	/*
	//1.2.643.7.1.1.3.3
	//Алгоритм подписи сертификата
	if (signatureAlgorithm != "1.2.643.7.1.1.3.3")
	{
		string err;
		err = "Не верный алгоритм подписи сертификата."
			" В системе могут  использоваться только сертификаты "
			"c	алгоритмом цифровой подписи ГОСТ Р 34.10-2012 для ключей длины 512 бит"
			"(szOID_CP_GOST_R3411_12_512_R3410:1.2.643.7.1.1.3.3)."
			"В сертификате указан алгоритм с OID " + signatureAlgorithm;
		return err;
	}
	//1.2.643.7.1.1.1.2
	//Алгоритм с которым использовать открытый ключ указанный в сертификате
	if (pubkey.AlgoritmOID != "1.2.643.7.1.1.1.2")
	{
		string err;
		err = "Не верный алгоритм использования открытого ключа."
			" В системе могут  использоваться только сертификаты "
			" c алгоритмом использования открытого ключа "
			" алгоритм ГОСТ Р 34.10-2012 для ключей длины 512 бит, используемый при экспорте/импорте ключей. "
			"(szOID_CP_GOST_R3410_12_512:1.2.643.7.1.1.1.2) ."
			" В сертификате указан алгоритм " + pubkey.AlgoritmOID + " .";
		return err;
	}

	//Параметр эллиптической кривой
	//1.2.643.7.1.2.1.2.1
	if (!((pubkey.EсOID == "1.2.643.7.1.2.1.2.1") || (pubkey.EсOID == "1.2.643.7.1.2.1.2.2")))
	{
		string err;
		err = "Не верный параметр эллиптической кривой открытого ключа сертификата."
			" В системе могут  использоваться только сертификаты "
			" c параметром эллиптической кривой "
			" Параметры a, b, p, q, (x,y) цифровой подписи и алгоритма Диффи-Хеллмана на базе алгоритма ГОСТ Р 34.10-2012 512 бит по умолчанию. "
			"(szOID_tc26_gost_3410_12_512_paramSetA:1.2.643.7.1.2.1.2.1) или "
			" Параметры a, b, p, q, (x,y) цифровой подписи и алгоритма Диффи-Хеллмана на базе алгоритма ГОСТ Р 34.10-2012 512 бит, набор B"
			"(szOID_tc26_gost_3410_12_512_paramSetB:1.2.643.7.1.2.1.2.2)"
			" В сертификате указан параметр " + pubkey.EсOID + " .";
		return err;
	}

	//Параметры функции хэширования 1.2.643.7.1.1.2.3
	if (pubkey.HOid != "1.2.643.7.1.1.2.3")
	{
		string err;
		err = "Не верный алгоритм использования открытого ключа."
			" В системе могут  использоваться только сертификаты "
			" с функцией хэширования  "
			" Функция хэширования ГОСТ Р 34.11-2012, длина выхода 512 бит "
			"(szOID_CP_GOST_R3411_12_512:1.2.643.7.1.1.2.3) ."
			" В сертификате указан алгоритм " + pubkey.HOid + " .";
		return err;
	}
	*/

	//В сертификате подпись предствалена как S,R
	vector<uint8_t> v_S;
	vector<uint8_t> v_R;

	//Получаю  подпись-вектора S,R
	for (int i = 0;i < 64;i++) v_S.push_back(signatureValue[i]); //S
	for (int i = 64;i < 128;i++) v_R.push_back(signatureValue[i]); //R


	   //Преобразование векторов подписи в mpz_class
	mpz_class r_; //Вектора подписи
	mpz_class s_;

	size_t r_len = static_cast<size_t>(v_R.size());
	size_t s_len = static_cast<size_t>(v_S.size());
	mpz_import(r_.get_mpz_t(), r_len, 1, 1, 0, 0, &v_R[0]);
	mpz_import(s_.get_mpz_t(), s_len, 1, 1, 0, 0, &v_S[0]);


	//Преобразование открытого ключа
	vector<uint8_t> p_x;
	vector<uint8_t> p_y;
	//Получение координат
	for (int i = 63;i >= 0;i--)
	{
		p_x.push_back(pubkey.PubKey[i]); //x
	}

	for (int i = 127;i >= 64;i--) p_y.push_back(pubkey.PubKey[i]); //y

	mpz_class px_;//Параметры открытого ключа x,y
	mpz_class py_;

	size_t px_len = static_cast<size_t>(p_x.size());
	size_t py_len = static_cast<size_t>(p_y.size());
	mpz_import(px_.get_mpz_t(), px_len, 1, 1, 0, 0, &p_x[0]);
	mpz_import(py_.get_mpz_t(), py_len, 1, 1, 0, 0, &p_y[0]);

	Signature Sign; //Объект содержащий подпись файла

	Sign.set_r(r_);
	Sign.set_s(s_);
	Sign.set_type(1); //512бит

	//gost-3410-12-512-paramSetA
	Groups EC2(0, pubkey.EсOID);//Создаю кривую на основании идентификатора передаваемого в сертификате

	//Создаю точку с указанными в кривой координатами точки P
	Point P2(EC2, 1); //Точка эллиптической кривой 
	Point Q1(EC2, 1); //Точка эллиптической кривой 

	//Устанавливаю координаты открытого ключа
	Q1.x = px_;
	Q1.y = py_;
	//Передаю открытый ключ для информирования при проверке
	retdataTbs.Qx = px_;
	retdataTbs.Qy = py_;

	//Преобразовываю порядок следования байт в TBS сертификате
	vector<unsigned char> _tbs; //Данные в правильной последовательности(старший бит идет первым)
	ReverseByteOrder(tbs, _tbs);//Меняю порядок

	unsigned long long l = static_cast<unsigned long long>(_tbs.size());//Количество байт в TBS сертификате

	//Передаю значение текущее подписи сертификата для отображение при проверке сертификата.
	retdataTbs.CerR = Sign.get_hex_r();
	retdataTbs.CerS = Sign.get_hex_s();

	VerifySign Vs;  //Проверяю подпись
	bool sign_status = Vs.Verify(&_tbs[0], l, P2, Q1, Sign);
	retdataTbs.calcR = Vs.GetLastR(); //Передаю значение  для отображение при проверке сертификата.
	retdataTbs.digest = Vs.GetLastDigest();

	return sign_status;

}



string X509Reader::VerifiSign(PubKeyData pubkey, vector<unsigned char> signatureValue,
	vector<unsigned char> &tbs, TBSCertificate &retdataTbs, bool &SignStatus)
{
	string err;
	int  KeyLen;
	int HashLen;
	err=GetKeyLen(pubkey,KeyLen); //Проверяю алгоритмы сертификата, и возвращаю длину ключа.
	if (!err.empty()) return err; //Возникла какая то ошибка.

	err = GetHasLen(pubkey,HashLen);
	if (!err.empty()) return err; //Возникла какая то ошибка.

	if (KeyLen != HashLen)
		return "Алгоритм вычисления хэш функции не совпадает с алгоритмом открытого ключа.";
	/*
	//Параметр эллиптической кривой
	//1.2.643.7.1.2.1.2.1
	if (!((pubkey.EсOID == "1.2.643.7.1.2.1.2.1") || (pubkey.EсOID == "1.2.643.7.1.2.1.2.2")))
	*/
	
	Signature Sign; //Объект содержащий подпись.
	Sign = Get512CerSign(signatureValue); //Получаю подпись сертификата.
		
	//gost-3410-12-512-paramSetA
	//Создаю кривую на основании идентификатора передаваемого в сертификате
	Groups EC2;
	bool ec_found;//Флаг нахождения кривой по OID
	ec_found=EC2.SetParam(pubkey.EсOID); //На основании OID задаю параметры кривой
	if (!ec_found)
		return "Системе не известна эллиптическая кривая с OID="+ pubkey.EсOID;


	//Создаю точку с указанными в кривой координатами точки P
	Point P2(EC2, 1); //Точка эллиптической кривой 
	//Открытый ключ-Точка эллиптической кривой 
	Point Q1 = Get512PubKey(pubkey,EC2);
						
	//Передаю открытый ключ для информирования при проверке
	retdataTbs.Qx = Q1.x; 
	retdataTbs.Qy = Q1.y;

	//Преобразовываю порядок следования байт в TBS сертификате
	vector<unsigned char> _tbs; //Данные в правильной последовательности(старший бит идет первым)
	ReverseByteOrder(tbs, _tbs);//Меняю порядок

	unsigned long long l = static_cast<unsigned long long>(_tbs.size());//Количество байт в TBS сертификате

	;//Передаю значение текущее подписи сертификата для отображение при проверке сертификата.
	retdataTbs.CerR = Sign.get_hex_r();
	retdataTbs.CerS = Sign.get_hex_s();

	VerifySign Vs;  //Проверяю подпись
	bool sign_status = Vs.Verify(&_tbs[0], l, P2, Q1, Sign);
	SignStatus = sign_status; //Состояние подписи
	retdataTbs.calcR = Vs.GetLastR(); //Передаю значение  для отображение при проверке сертификата.
	retdataTbs.digest = Vs.GetLastDigest();

	return "";

}

string X509Reader::GetKeyLen(PubKeyData pubkey, int & KeyLen)
{
	string err;
	
	int signKeyLen = 0; //Длина  ключа подписи для алгоритма сертификата 256 512
	int PubKeyLen = 0; //Длина открытого ключа


					   //Определение длины ключа подписи
					   //1.2.643.7.1.1.3.2	Алгоритм цифровой подписи сертификата ГОСТ Р 34.10-2012 для ключей длины 256 бит
	if (pubkey.signatureAlgorithm == "1.2.643.7.1.1.3.2") signKeyLen = 256;

	//1.2.643.7.1.1.3.3 (Алгоритм цифровой подписи ГОСТ Р 34.10-2012 для ключей длины 512 бит
	if (pubkey.signatureAlgorithm == "1.2.643.7.1.1.3.3") signKeyLen = 512;

	//Неизвестный алгоритм
	if (signKeyLen == 0)
	{
		err = "Неизвестный алгоритм подписи сертификата."
			" В системе могут  использоваться только сертификаты ГОСТ Р 34.10-2012."
			"Алгоритм подписи данного сертификата: " + AlgOidName::GetInfo(pubkey.signatureAlgorithm);
		return err;
	}

	//Проверка и получение данных открытого ключа
	//Алгоритм ГОСТ Р 34.10-2012 для ключей длины 256 бит, используемый при экспорте/импорте ключей
	if (pubkey.AlgoritmOID == "1.2.643.7.1.1.1.1") PubKeyLen = 256;

	//Алгоритм ГОСТ Р 34.10 - 2012 для ключей длины 512 бит, используемый при экспорте / импорте ключей
	if (pubkey.AlgoritmOID == "1.2.643.7.1.1.1.2") PubKeyLen = 512;


	//Неизвестный алгоритм ключа
	if (PubKeyLen == 0)
	{
		err = "Не верный алгоритм  открытого ключа."
			" В системе могут  использоваться только открытый ключи удовлетворяющие ГОСТ Р 34.10-2012."
			" В сертификате указан алгоритм " + AlgOidName::GetInfo(pubkey.AlgoritmOID) + " .";
		return err;
	}


	if (signKeyLen != PubKeyLen)
	{
		err = "Сертификат содержит не совместимые алгоритмы подписи и открытого ключа.";
		return err;
	}

	KeyLen = PubKeyLen;

	return "";
}

string X509Reader::GetKeyLen(TBSCertificate & tbs, int & KeyLen)
{
	string err;

	int signKeyLen = 0; //Длина  ключа подписи для алгоритма сертификата 256 512
	int PubKeyLen = 0; //Длина открытого ключа

	//Определение длины ключа подписи
	 //1.2.643.7.1.1.3.2	Алгоритм цифровой подписи сертификата ГОСТ Р 34.10-2012 для ключей длины 256 бит
	if (tbs.AlgorithmIdentifier == "1.2.643.7.1.1.3.2") signKeyLen = 256;

	//1.2.643.7.1.1.3.3 (Алгоритм цифровой подписи ГОСТ Р 34.10-2012 для ключей длины 512 бит
	if (tbs.AlgorithmIdentifier == "1.2.643.7.1.1.3.3") signKeyLen = 512;

	//Неизвестный алгоритм
	if (signKeyLen == 0)
	{
		err = "Неизвестный алгоритм подписи сертификата."
			" В системе могут  использоваться только сертификаты ГОСТ Р 34.10-2012."
			"Алгоритм подписи данного сертификата: " + AlgOidName::GetInfo(tbs.AlgorithmIdentifier);
		return err;
	}

	//Проверка и получение данных открытого ключа
	//Алгоритм ГОСТ Р 34.10-2012 для ключей длины 256 бит, используемый при экспорте/импорте ключей
	if (tbs.publicKeyParamSet == "1.2.643.7.1.1.1.1") PubKeyLen = 256;

	//Алгоритм ГОСТ Р 34.10 - 2012 для ключей длины 512 бит, используемый при экспорте / импорте ключей
	if (tbs.publicKeyParamSet == "1.2.643.7.1.1.1.2") PubKeyLen = 512;


	//Неизвестный алгоритм ключа
	if (PubKeyLen == 0)
	{
		err = "Не верный алгоритм  открытого ключа."
			" В системе могут  использоваться только открытый ключи удовлетворяющие ГОСТ Р 34.10-2012."
			" В сертификате указан алгоритм " + AlgOidName::GetInfo(tbs.publicKeyParamSet) + " .";
		return err;
	}


	if (signKeyLen != PubKeyLen)
	{
		err = "Сертификат содержит не совместимые алгоритмы подписи и открытого ключа.";
		return err;
	}

	KeyLen = PubKeyLen;

	return "";
}

string X509Reader::GetHasLen(PubKeyData pubkey, int & HashLen)
{
	HashLen = 0;

	//1.2.643.7.1.1.2.2 Функция хэширования ГОСТ Р 34.11-2012, длина выхода 256 бит
	if (pubkey.HOid == "1.2.643.7.1.1.2.2") HashLen = 256;
	
	//1.2.643.7.1.1.2.3 Функция хэширования ГОСТ Р 34.11-2012, длина выхода 512 бит
	if (pubkey.HOid == "1.2.643.7.1.1.2.3") HashLen = 512;
	
	
	if(HashLen == 0)
	{
		string err;
		err = "Не верный алгоритм использования открытого ключа."
			" В системе могут  использоваться только сертификаты "
			" с функцией хэширования  удовлетворяющие ГОСТ Р 34.11-2012."
			" В сертификате указан алгоритм " + AlgOidName::GetInfo(pubkey.HOid)+ " .";
		return err;
	}
	
	return "";
}

string X509Reader::GetHasLen(TBSCertificate & tbs, int & HashLen)
{
	HashLen = 0;
	
	//1.2.643.7.1.1.2.2 Функция хэширования ГОСТ Р 34.11-2012, длина выхода 256 бит
	if (tbs.encryptionParamSet == "1.2.643.7.1.1.2.2") HashLen = 256;

	//1.2.643.7.1.1.2.3 Функция хэширования ГОСТ Р 34.11-2012, длина выхода 512 бит
	if (tbs.encryptionParamSet == "1.2.643.7.1.1.2.3") HashLen = 512;


	if (HashLen == 0)
	{
		string err;
		err = "Не верный алгоритм использования открытого ключа."
			" В системе могут  использоваться только сертификаты "
			" с функцией хэширования  удовлетворяющие ГОСТ Р 34.11-2012."
			" В сертификате указан алгоритм " + AlgOidName::GetInfo(tbs.digestParamSet) + " .";
		return err;
	}

	return "";
}

Signature X509Reader::Get512CerSign(vector<unsigned char>& signatureValue)
{
	//В сертификате подпись представлена как S,R
	vector<uint8_t> v_S;
	vector<uint8_t> v_R;

	//Получаю  подпись-вектора S,R
	for (int i = 0;i < 64;i++) v_S.push_back(signatureValue[i]); //S
	for (int i = 64;i < 128;i++) v_R.push_back(signatureValue[i]); //R

	 //Преобразование векторов подписи в mpz_class
	mpz_class r_; //Вектора подписи
	mpz_class s_;

	size_t r_len = static_cast<size_t>(v_R.size());
	size_t s_len = static_cast<size_t>(v_S.size());
	mpz_import(r_.get_mpz_t(), r_len, 1, 1, 0, 0, &v_R[0]);
	mpz_import(s_.get_mpz_t(), s_len, 1, 1, 0, 0, &v_S[0]);

	
	Signature Sign; //Объект содержащий подпись файла
	Sign.set_r(r_);
	Sign.set_s(s_);
	Sign.set_type(1); //512бит

	return Sign;
}

Point X509Reader::Get512PubKey(PubKeyData pubkey, Groups EC)
{
	//Преобразование открытого ключа
	vector<uint8_t> p_x;
	vector<uint8_t> p_y;
	//Получение координат
	for (int i = 63;i >= 0;i--)
	{
		p_x.push_back(pubkey.PubKey[i]); //x
	}

	for (int i = 127;i >= 64;i--) p_y.push_back(pubkey.PubKey[i]); //y

	mpz_class px_;//Параметры открытого ключа x,y
	mpz_class py_;

	size_t px_len = static_cast<size_t>(p_x.size());
	size_t py_len = static_cast<size_t>(p_y.size());
	mpz_import(px_.get_mpz_t(), px_len, 1, 1, 0, 0, &p_x[0]);
	mpz_import(py_.get_mpz_t(), py_len, 1, 1, 0, 0, &p_y[0]);

 
	Point Q1(EC, 1); //Точка эллиптической кривой 

	//Устанавливаю координаты открытого ключа
	Q1.x = px_;
	Q1.y = py_;

	return Q1;
}



void X509Reader::ReverseByteOrder(vector<unsigned char>& src, vector<unsigned char>& dst)
{
	for (int i = src.size() - 1;i >= 0;i--)
	{
		dst.push_back(src[i]);
	}
}


bool X509Reader::readSignData(rfcSignedData &sD, QByteArray in)
{
	//Преобразовываю данные QByteArray в  ByteQueue
	ByteQueue rootArr; //Корневой блок.
	rootArr.Put(reinterpret_cast<uint8_t*>(in.data()), in.size());

	BERSequenceDecoder rootSequence(rootArr); //Корневая последовательность

	OID _typeOID; //OID типа CMS.
	_typeOID.BERDecode(rootSequence);
	string typeOID =OIDtoString(_typeOID);

	if (typeOID != "1.2.840.113549.1.7.2")
	{
		lastError = "Корневой блок данных не содержит сведений о подписи. OID блока данных="+ typeOID;
		return false;
	}
			
	
	BERGeneralDecoder cs1(rootSequence, 0xA0);//CONTEXT SPECIFIC (0) 
	BERSequenceDecoder content(cs1);

	Integer v1_;
	v1_.BERDecode(content);
	int cmsV1 = static_cast<int>(v1_.ConvertToLong()); //Версия CMS.

	sD.CMSVersion = cmsV1;


	BERSetDecoder set1(content);
	BERSequenceDecoder seq1(set1);

	OID _oid1; //OID алгоритма дайджеста.
	_oid1.BERDecode(seq1);
	string strOid1 = OIDtoString(_oid1);

	sD.digestAlgorithms = strOid1;

	seq1.SkipAll();
	set1.SkipAll();
	
	BERSequenceDecoder seq2(content);

	OID _oid2; //OID типа данных.
	_oid2.BERDecode(seq2);
	string strOid2 = OIDtoString(_oid2);


	BERSetDecoder set2(content);
	BERSequenceDecoder seq3(set2);

	Integer v2_;
	v2_.BERDecode(seq3);
	int cmsV2 = static_cast<int>(v2_.ConvertToLong()); //Версия CMS.

	
	BERSequenceDecoder seq4(seq3); //Данные сертификата подписанта.
	BERSequenceDecoder seq5(seq4);
	BERSetDecoder set3(seq5);
	BERSequenceDecoder seq6(set3);

	OID _oid3; //OID название пользователя.
	_oid3.BERDecode(seq6);
	string strOid3 = OIDtoString(_oid3);
	

	ByteQueue val1_; //Значение.
	seq6.CopyMessagesTo(val1_);
	string val1 = "";

	seq6.SkipAll(); //Без этого будет исключение в программе.
	set3.SkipAll();
	seq5.SkipAll();
	
	CryptoPP::BERDecodeTextString(val1_, val1, CryptoPP::UTF8_STRING);
	QString str1 = QString::fromUtf8(val1.c_str());
	//Перекодирую.
	string val2 = str1.toLocal8Bit();
	sD.commonName = val2;

	//Номер сертификата подписанта.
	Integer certNum_;
	certNum_.BERDecode(seq4);
	int certNum = static_cast<int>(certNum_.ConvertToLong()); //Версия CMS.
	sD.certNum = certNum;

	seq4.SkipAll();

	BERSequenceDecoder seq7(seq3);
	OID _oid4; //OID название пользователя.
	_oid4.BERDecode(seq7);
	string strOid4 = OIDtoString(_oid4); //ОИД даты.

	//Пропускаю Null
	seq7.SkipAll();

	//Характеристики подписи.
	BERGeneralDecoder cs2(seq3, 0xA0);//CONTEXT SPECIFIC (0) 
	BERSequenceDecoder seq8(cs2);

	//contentType
	OID _oid5; //OID
	_oid5.BERDecode(seq8);
	string strOid5 = OIDtoString(_oid5);
	seq8.SkipAll(); //Остальное нам не нужно.


	//Время подписи.
	BERSequenceDecoder seq9(cs2);
	//signingTime
	OID _oid6; //OID
	_oid6.BERDecode(seq9);
	string strOid6 = OIDtoString(_oid6);

	BERSetDecoder set4(seq9);
	string signiningTime_;
	CryptoPP::BERDecodeTextString(set4, signiningTime_, CryptoPP::UTC_TIME);

	QDateTime signiningTime;
	//Конвертирую из строки
	QString a = "20" + QString::fromStdString(signiningTime_);
	a.replace("Z", "");
	
	signiningTime = QDateTime::fromString(a, "yyyyMMddhhmmss");
	sD.signingTime = signiningTime;

	set4.SkipAll();
	seq8.SkipAll();

	//Дайджест сообщения.
	BERSequenceDecoder seq10(cs2);
	//messageDigest
	OID _oid7; //OID
	_oid7.BERDecode(seq10);
	string strOid7 = OIDtoString(_oid7);

	//Сам дайджест.
	BERSetDecoder set5(seq10);
	string messDigest;
	CryptoPP::BERDecodeTextString(set5, messDigest, CryptoPP::OCTET_STRING);

	vector<unsigned char> tmp1(messDigest.begin(), messDigest.end());
	sD.digest=tmp1;


	set5.SkipAll();
	seq10.SkipAll();


	//Signing certificate  данные сертификата подписанта.
	BERSequenceDecoder seq11(cs2);
	//Signing certificate 
	OID _oid8; //OID
	_oid8.BERDecode(seq11);
	string strOid8 = OIDtoString(_oid8);

	BERSetDecoder set6(seq11);
	BERSequenceDecoder seq12(set6);
	BERSequenceDecoder seq13(seq12);
	BERSequenceDecoder seq14(seq13);

	BERSequenceDecoder seq15(seq14); //Алгоритм отпечатка сертификата.

	//Идентификатор отпечатка сертификата.
	OID _oid9; //OID
	_oid9.BERDecode(seq15);
	string strOid9 = OIDtoString(_oid9);
	sD.signingCertDigestAlg = strOid9;

	seq15.SkipAll();

	//Отпечаток сертификата подписанта.
	string certDigest;
	CryptoPP::BERDecodeTextString(seq14, certDigest, CryptoPP::OCTET_STRING);

	vector<unsigned char> tmp2(certDigest.begin(), certDigest.end());
	sD.signingCertDigest = tmp2;

	
	BERSequenceDecoder  seq16(seq14);
	BERSequenceDecoder  seq17(seq16);

	//Характеристики подписи.
	BERGeneralDecoder cs3(seq17, 0xA0);//CONTEXT SPECIFIC (0) 
	BERSequenceDecoder seq18(cs3);

	BERSetDecoder set7(seq18);
	BERSequenceDecoder seq19(set7);

	OID _oid10; //OID ниаименования пользователя
	_oid10.BERDecode(seq19);
	string strOid10 = OIDtoString(_oid10);
	
	//Значение.
	string certUsName;
	CryptoPP::BERDecodeTextString(seq19, certUsName, CryptoPP::UTF8_STRING);

	QString str2a = QString::fromUtf8(certUsName.c_str());
	//Перекодирую.
	string val2a = str2a.toLocal8Bit();
	seq19.SkipAll();
	set7.SkipAll();
	seq18.SkipAll();
	cs3.SkipAll();
	seq17.SkipAll();

	//Номер сертификата.
	Integer certNum2_;
	certNum2_.BERDecode(seq16);
	int certNum2 = static_cast<int>(certNum2_.ConvertToLong()); //Версия CMS.

	seq16.SkipAll();
	seq14.SkipAll();
	seq13.SkipAll();
	seq12.SkipAll();
	set6.SkipAll();
	seq11.SkipAll();
	//cs2.SkipAll();

	//Алгоритм ЭЦП.
	BERSequenceDecoder seq20(seq3);

	OID _oid11; //OID alg signature.
	_oid11.BERDecode(seq20);
	string strOid11 = OIDtoString(_oid11);
	sD.signatureAlgorithm = strOid11;

	seq20.SkipAll(); //Пропускаем null.

	//Данные ЭЦП.
	string signatureVal;
	CryptoPP::BERDecodeTextString(seq3, signatureVal, CryptoPP::OCTET_STRING);
	seq3.SkipAll();
	
	vector<unsigned char> g(signatureVal.begin(), signatureVal.end());
	sD.signVal = g;


	return true;
}

string X509Reader::getLastError()
{
	return lastError;
}
