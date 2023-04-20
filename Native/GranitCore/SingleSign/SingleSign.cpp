#include "SingleSign.h"




SingleSign::SingleSign()
{
	lastError="";
}

SingleSign::~SingleSign()
{
}

bool SingleSign::GrKSignData(uint8_t * Message, int Mlen,string & SignStr, uint32_t & Slen)
{		
	//Формирую подпись.
	CreateSign SignGen; //Объект для создания подписи

	Groups EC2;
	string Ecparam = "";
	if (!ks->getEcParam(Ecparam))
	{
		lastError = "Ошибка Sg3.1A: Не был загружен ключ. Сначала загрузите ключ.";
		return false;
	}
	
	bool findEc = EC2.SetParam(Ecparam); //Установка параметров кривой.
	if (!findEc)
	{
		lastError = "Ошибка Sg3.1: Не удалось определить параметры функции.";
		return false;
	}
		
	
	//За кешировали ключ во время проверки пароля? Не был проверен пароль, а следовательно не получен закрытый ключ.
	if (!ks->isCacheSk())
	{
		lastError = "Ошибка Sg03.1: Не был проверен пароль ключа.";
		return false;
	}

	//Создаю точку с указанными в кривой координатами точки P
	Point P(EC2, 1); //Точка эллиптической кривой 
	Signature Sign;//Подпись

	//Подписываю сообщение.
	mpz_class  user_d = ks->getSignPk();
	//Размер сообщения в байтах
	unsigned long long mess_len = static_cast<unsigned long long>(Mlen);
	Sign = SignGen.Sign(user_d, Message, mess_len, P);

	
	//-----------------Проверка подписи. На стадии внедрения-обязательно!----------------

	//Ключ проверки подписи	
	Point Q1(EC2, 1); //Точка эллиптической кривой 

	//Проверяю соответствие ключевой пары в контейнере
	mpz_class secret_k = user_d; //Закрытый ключ пользователя
	Point  Q2;
	Q2 = SignGen.generate_public_key(secret_k, P);

	//Значение открытого ключа из контейнера
	string x1 ="";
	string y1 = "";
	ks->getSignOpenKey(x1,y1);


	//Вычисленное значение
	string x2 = Q2.get_hex_x();
	string y2 = Q2.get_hex_y();

	if (x1 != x2)
	{
		lastError = "Ошибка Sg4: Не удалось подписать файл.";
		return false;
	}

	if (y1 != y2) 
	{
		lastError = "Ошибка Sg5: Не удалось подписать файл.";
		return false;
	}
	

	//Задаю параметры ключу проверки подписи
	Q1.x.set_str(x1, 16);
	Q1.y.set_str(y1, 16);

	//Проверяю подпись
	VerifySign Vs;
	bool sign_status = Vs.Verify(Message, mess_len, P, Q1, Sign);

	if (sign_status == 0)
	{
    	lastError = "Ошибка Sg6: Не удалось подписать файл.";
		return false;	
	}


	//Формирования подписи согласно ASN 1.

	//Получаю содержимое пользовательского сертификата.
	TBSCertificate  tbsUserCertificate = ks->getUserCert();

	//Данные CMS.
	rfcSignedData sd; 
	sd.CMSVersion = 1; //Номер версии синтаксиса CMS.
	sd.digestAlgorithms = "1.2.643.7.1.1.2.3"; //Идентификатор алгоритма расчета отпечатка сообщения.
	sd.commonName = tbsUserCertificate.Subject_id_at_commonName; //Организация.
	sd.certNum = mpz_get_ui(tbsUserCertificate.serialNumber.get_mpz_t()); //Номер сертификата подписанта.
	sd.signingCertDigestAlg = "1.2.643.7.1.1.2.3"; //Алгоритм отпечатка сертификата подписанта.

	 //Значение отпечатка сертификата подписанта 64бит.
	string str_certDiges = tbsUserCertificate.digest;
	QByteArray baDgst = QByteArray::fromHex(str_certDiges.c_str());

	std::vector<unsigned char> vbaDgst(
		baDgst.begin(), baDgst.end());										  
	
	sd.signingCertDigest = vbaDgst;
					
	sd.signingTime = QDateTime::currentDateTime(); //Время накладывания подписи.

	//Считываю дайджест сообщения.
	sd.digest = SignGen.getLastDigest();
									 
	
	sd.signatureAlgorithm = "1.2.643.7.1.1.1.2"; //Алгоритм эцп.
	
	//Сама ЭЦП.
    string sxR=Sign.get_hex_r();
	string sxS = Sign.get_hex_s();


	//Каждая строка должна содержать 128символов. Дополним нулями с лева.

	string nr = "";
	int cntV = sxR.length();
	if (cntV<128)
	{
		while (cntV != 128)
		{
			nr = nr + "0";
			cntV++;
		}

		sxR = nr+sxR;
	}

	 nr = "";
	 cntV = sxS.length();
	 if (cntV<128)
	 {
		 while (cntV != 128)
		 {
			 nr = nr + "0";
			 cntV++;
		 }

		 sxS = nr + sxS;
	 }


	 if (sxR.length() != 128)
	 {
		 lastError = "Ошибка Sg7R: Не удалось подписать файл. Вектор r имеет не верную длину:"
			 +std::to_string(sxR.length())+" символов. Должно быть 128.";
		 return false;
	 }


	 if (sxS.length() != 128)
	 {
		 lastError = "Ошибка Sg7S: Не удалось подписать файл. Вектор s имеет не верную длину:"
			 + std::to_string(sxS.length()) + " символов. Должно быть 128.";
		 return false;
	 }

	 //Формирую итоговую строку.
	 string signData = sxR + sxS;	 
	 QByteArray das = QByteArray::fromHex(signData.c_str());
		
	 for (int z = 0;z < das.size();z++)
	 {
		 sd.signVal.push_back(das[z]);
	 }
	
	 
	//Формирую ASN1 представление подписи.
	binASN1SignedData CS;
	QByteArray outBa = CS.createSignedBin(&sd);

	QByteArray ba_as_hex_string = outBa.toHex(); //16 ричное представление.

	
	/*
	//Для тестов. Сохраняет подпись в файл.
	QFile file(QString::fromStdString("D:\\10\\test1.pem"));
	file.open(QIODevice::WriteOnly);
	file.write(outBa);
	file.close();
	*/
					
	string Sgn = ba_as_hex_string.toStdString();

	Slen = Sgn.length();
	SignStr = Sgn;

	
	return true;
}

bool SingleSign::GrKCheckSign(uint8_t * Message, int Mlen, string SignValue, string & FIOp)
{	
	//Чтение и распаковка бинарных данных.
	QByteArray inData = QByteArray::fromHex(SignValue.c_str());
	
	X509Reader X509R;
	rfcSignedData sD;
	if (!X509R.readSignData(sD, inData))
	{
		lastError = X509R.getLastError();
		return false;
	}
		

	//16 ричное представление отпечатка сертификата подписанта.
	QByteArray  hexCertDigest(reinterpret_cast<const char*>(sD.signingCertDigest.data()), sD.signingCertDigest.size());

	std::string strCertDigest = hexCertDigest.toHex().toStdString(); //Отпечаток сертификата подписанта.
	string сertnum = std::to_string(sD.certNum); //Номер сертификата подписанта.


	QByteArray certData; //Сертификат подписанта.
	bool errFlag = dBi->findUserCert(strCertDigest, сertnum, certData);
	
	//Не найден сертификата подписанта.
	if (certData.size() == 0)
	{
		lastError = "Ошибка СhSg001: В БД системы не найден сертификата подписанта. Номер сертификата:"+ сertnum+
			" Отпечаток сертификата:"+ strCertDigest;
		return false;
	}

	X509Reader sR;

	//Читаю байт массив и преобразовываю его в TBSCertificate.
	TBSCertificate dataTbs;
	sR.ReadBa(certData,dataTbs);

	//Создаю кривую на основании идентификатора передаваемого в сертификате
	Groups EC2;
	bool ec_found;//Флаг нахождения кривой по OID
	ec_found = EC2.SetParam(dataTbs.digestParamSet); //На основании OID открытого ключа задаю параметры кривой
	if (!ec_found)
	{
		lastError= "Системе не известна эллиптическая кривая с OID=" + dataTbs.digestParamSet;
		return false;
	}
			

	//Создаю точку с указанными в кривой координатами точки P
	Point P2(EC2, 1); //Точка эллиптической кривой 

	Point Q1(EC2, 1);//Создаю  ключ проверки подписи. 
	Q1.x = dataTbs.Qx; //Передаю координаты ключа проверки подписи.
	Q1.y = dataTbs.Qy; //
			

	Signature Sign; //Объект содержащий подпись файла

	//Парсинг бинарных данных.
	if(!Sign.parceBin(&sD.signVal))
	{
		lastError = Sign.getLastError();
		return false;
	}


	//Проверяю подпись
	VerifySign Vs;
	unsigned long long mess_len = static_cast<unsigned long long>(Mlen); //Длина сообщения.
	bool sign_status = Vs.Verify(Message, mess_len, P2, Q1, Sign);

	if (sign_status == 0)
	{
		lastError= "Не верная подпись.";
		return false;
	}
	else
	{
		//Передаю ФИО подписанта.
		FIOp = sD.commonName;
	}


	return true;
}

void SingleSign::ClearRAM()
{	
	ks->Clear();
}

string SingleSign::getLastError()
{
	return lastError;
}

void SingleSign::setKeyService(KeyService & KS)
{
	ks = &KS;
}

void SingleSign::setDb(GranitDbI & Db)
{
	dBi = &Db;
}
