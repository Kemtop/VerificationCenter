#include "SecretKeyMaker.h"
const QString SecretKeyMaker::G_publicKeyAlgoritmOid = "1.2.643.7.1.1.1.2";//Алгоритм открытого ключа подписи.
const QString SecretKeyMaker::G_EcOid = "1.2.643.7.1.2.1.2.2"; //Идентификатор эллиптической кривой на основании  которой создается ключ.

SecretKeyMaker::SecretKeyMaker()
{
	enableRose = false; //Не включать режим модификации.
}

SecretKeyMaker::~SecretKeyMaker()
{
}

void SecretKeyMaker::setRoseMode()
{
	enableRose = true;
}

bool SecretKeyMaker::CreateKeyFile(unsigned char * Key, QString Path, QString & Password, mRequest &Req, QString pathToSaveReq)
{
	try
	{
		bool Error = false;
		string salt = "";
		string str_iv = "";
		string crypt_secret_key = "";


		//Генерация ключевой пары для накладывания и проверки подписи.
		Point Q;//Открытый ключ.
		Error = сreateInvisibleSignKey(Key, Password, Q, str_iv, salt, crypt_secret_key);
		if (!Error) return false;

		//-------------Генерация закрытого и открытого ключа для алгоритма Диффи-Хэлмана-----------------------//
		string DH_iv = "";
		string DH_salt = "";
		string crypted_secret_DH_key = "";
		string DH_public = ""; //Открытый ключ.
		Error = сreateInvisibleDHkey(Password, DH_iv, DH_salt, crypted_secret_DH_key, DH_public);
		if (!Error) return false;
		//-------------------------------------------
		

		QDateTime tStamp = QDateTime::currentDateTime();
		//Заполнение контейнера ключа
		PkContainer Сontainer;
		Сontainer.setP0("0.0.1");//Формат ключа "0.0.1"
		Сontainer.setP1("0.0.1");//Версия генератора "0.0.1" СК

		Сontainer.setP2(Req.P4()); ////Наименование организации.
		Сontainer.setP3(Req.P5());	//ЕГРЮЛ.
		Сontainer.setP4(Req.P3()); //Подразделение.
		Сontainer.setP5(Req.P9()); //Телефон.
		Сontainer.setP6(Req.P0()); //Familia
		Сontainer.setP7(Req.P1()); //7 Imia
		Сontainer.setP8(Req.P2()); //8 Otchestvo
		Сontainer.setP9(tStamp);//Временная дата начала действия ключа.
		Сontainer.setP10(tStamp.addYears(1));//Временная дата окончания действия ключа.
		Сontainer.setP11(QString::fromLocal8Bit(crypt_secret_key.c_str())); //шифрованный секретный ключ
		Сontainer.setP12(QString::fromLocal8Bit(salt.c_str())); //соль
		Сontainer.setP13("0"); //Дополнительный параметр

		Сontainer.setP14(QString::fromLocal8Bit(str_iv.c_str()));//Вектор инициализации IV
		Сontainer.setP15(QDateTime::currentDateTime()); //Дата генерации ключа  

														//Сохранение параметров схемы Диффи-Хеллмана

		Сontainer.setP18(QString::fromLocal8Bit(crypted_secret_DH_key.c_str(), crypted_secret_DH_key.length())); //Шифрованное значение закрытого ключа схемы Диффи-Хеллмана
		Сontainer.setP19(QString::fromLocal8Bit(DH_iv.c_str(), DH_iv.length())); //Значение вектора iv для закрытого ключа схемы Диффи-Хеллмана
		Сontainer.setP20(str_to_Hex(DH_public)); //Открытый ключ схемы Диффи-Хеллмана
		Сontainer.setP21(QString::fromLocal8Bit(DH_salt.c_str(), DH_salt.length())); //Соль для шифрованного значения  закрытого ключа схемы Диффи-Хеллмана

																					 //Передаю открытый ключ в контейнер
		string str_Qx = Q.get_hex_x(); // Qx.get_str(16);
		string str_Qy = Q.get_hex_y();// Qy.get_str(16);

		Сontainer.setP16(QString::fromLocal8Bit(str_Qx.c_str())); //Открытый ключ подписи координата x
		Сontainer.setP17(QString::fromLocal8Bit(str_Qy.c_str()));//Открытый ключ подписи координата y

																 //Устанавливаю параметры ключа
		Сontainer.setP22(G_publicKeyAlgoritmOid);//PublicKeyAlgoritm-алгоритм открытого ключа подписи.
		Сontainer.setP23("1LgN89TY57"); //Просто число которое ни где не используется, но заставит задуматься злоумышленника.
		Сontainer.setP24(G_EcOid);	//EcOid-параметры эллиптической кривой на основани которой был создан ключ подписи.


									//Сохраняю контейнер ключа в файл
		Error = KeyСontainerToFile(Сontainer, Path);
		if (!Error) return false;

		//Добавляю данные для запроса.
		Req.setP11(KeyGenVersion); //Версия генератора ключа.
		Req.setP12(str_to_Hex(DH_public)); //Открытый ключ схемы Диффи-Хэлмана.

		QString x1(str_Qx.c_str());
		QString y1(str_Qy.c_str());
		//Сохранение открытого ключа подписи.
		Req.setP14(x1);
		Req.setP15(y1);

		Req.setP16(G_publicKeyAlgoritmOid);//PublicKeyAlgoritm-алгоритм открытого ключа подписи.
		Req.setP17(G_EcOid);//EcOid-параметры эллиптической кривой на основаниии которой был создан ключ подписи.

		Req.setP19(Req.P19() + 1); //Увеличиваю порядковый номер заявки.

		//Создаю файл запроса на получение сертификата.
	
		Error = CreateRequestFile(pathToSaveReq,Req);
		if (!Error) return false;

		//Формирую хэш ключа для построения заявки.
		QString hs = VanishChest::algHashSignKey(x1, y1);
		Req.setP18(hs);

	}
	catch (std::exception &ex)
	{
		lastError = "В методе CreateKeyFile() возникло исключение:"+string(ex.what());
		return false;
	}

	return true;	
}

bool SecretKeyMaker::CreateKeyFileNoReq(unsigned char * Key, QString Path, QString & Password)
{
	try
	{
		bool Error = false;
		string salt = "";
		string str_iv = "";
		string crypt_secret_key = "";


		//Генерация ключевой пары для накладывания и проверки подписи.
		Point Q;//Открытый ключ.
		Error = сreateInvisibleSignKey(Key, Password, Q, str_iv, salt, crypt_secret_key);
		if (!Error) return false;

		//-------------Генерация закрытого и открытого ключа для алгоритма Диффи-Хэлмана-----------------------//
		string DH_iv = "";
		string DH_salt = "";
		string crypted_secret_DH_key = "";
		string DH_public = ""; //Открытый ключ.
		Error = сreateInvisibleDHkey(Password, DH_iv, DH_salt, crypted_secret_DH_key, DH_public);
		if (!Error) return false;
		//-------------------------------------------


		QDateTime tStamp = QDateTime::currentDateTime();
		//Заполнение контейнера ключа
		PkContainer Сontainer;
		Сontainer.setP0("0.0.1");//Формат ключа "0.0.1"
		Сontainer.setP1("0.0.1");//Версия генератора "0.0.1" СК

		Сontainer.setP2("Charlie"); ////Наименование организации.
		Сontainer.setP3("Sopranos");	//ЕГРЮЛ.
		Сontainer.setP4("Rings"); //Подразделение.
		Сontainer.setP5("series"); //Телефон.
		Сontainer.setP6("Patrick"); //Familia
		Сontainer.setP7("Marshall"); //7 Imia
		Сontainer.setP8("During"); //8 Otchestvo
		Сontainer.setP9(tStamp);//Временная дата начала действия ключа.
		Сontainer.setP10(tStamp.addYears(1));//Временная дата окончания действия ключа.
		Сontainer.setP11(QString::fromLocal8Bit(crypt_secret_key.c_str())); //шифрованный секретный ключ
		Сontainer.setP12(QString::fromLocal8Bit(salt.c_str())); //соль
		Сontainer.setP13("0"); //Дополнительный параметр

		Сontainer.setP14(QString::fromLocal8Bit(str_iv.c_str()));//Вектор инициализации IV
		Сontainer.setP15(QDateTime::currentDateTime()); //Дата генерации ключа  

														//Сохранение параметров схемы Диффи-Хеллмана

		Сontainer.setP18(QString::fromLocal8Bit(crypted_secret_DH_key.c_str(), crypted_secret_DH_key.length())); //Шифрованное значение закрытого ключа схемы Диффи-Хеллмана
		Сontainer.setP19(QString::fromLocal8Bit(DH_iv.c_str(), DH_iv.length())); //Значение вектора iv для закрытого ключа схемы Диффи-Хеллмана
		Сontainer.setP20(str_to_Hex(DH_public)); //Открытый ключ схемы Диффи-Хеллмана
		Сontainer.setP21(QString::fromLocal8Bit(DH_salt.c_str(), DH_salt.length())); //Соль для шифрованного значения  закрытого ключа схемы Диффи-Хеллмана

																					 //Передаю открытый ключ в контейнер
		string str_Qx = Q.get_hex_x(); // Qx.get_str(16);
		string str_Qy = Q.get_hex_y();// Qy.get_str(16);

		Сontainer.setP16(QString::fromLocal8Bit(str_Qx.c_str())); //Открытый ключ подписи координата x
		Сontainer.setP17(QString::fromLocal8Bit(str_Qy.c_str()));//Открытый ключ подписи координата y

																 //Устанавливаю параметры ключа
		Сontainer.setP22(G_publicKeyAlgoritmOid);//PublicKeyAlgoritm-алгоритм открытого ключа подписи.
		Сontainer.setP23("8bDkhN935SYik36i"); //Просто число которое ни где не используется, но заставит задуматься злоумышленника.
		Сontainer.setP24(G_EcOid);	//EcOid-параметры эллиптической кривой на основани которой был создан ключ подписи.


									//Сохраняю контейнер ключа в файл
		Error = KeyСontainerToFile(Сontainer, Path);
		if (!Error) return false;
				
	}
	catch (std::exception &ex)
	{
		lastError = "В методе CreateKeyFile() возникло исключение:" + string(ex.what());
		return false;
	}

	return true;
}

string SecretKeyMaker::getLastError()
{
	return lastError;
}


bool SecretKeyMaker::сreateInvisibleSignKey(unsigned char *Key, QString Password, Point & Q, string & str_iv_, string & salt_, string & crypt_secret_key_)
{
	bool Error;
	string salt = "";
	mpz_class d; //Закрытый ключ.

	string EcOID;//Идентификатор эллиптической кривой.
	EcOID = G_EcOid.toStdString();
	Error = CreateEcKeyPair(true, EcOID, Key, 64, Q, d);
	if (!Error) return false;


	//Генерирую 17байтную соль. Соль случайное число.
	Error = GenerateSalt(salt);
	if (!Error) return false;
	unsigned char hash256[32];

	//Вычисляю хэш соленого пароля
	Error = HashedPassword(Password, salt, hash256);
	if (!Error) return false;

	//Генерирую вектор инициализации
	unsigned char iv[16];
	Error = GenerateIV(iv);
	if (!Error) return false;

	//Шифрую секретный ключ проверки подписи  ГОСТ 34.11-2012 на хэши, 
	string crypt_secret_key; //Результат шифрования
	Error = CryptEcSecretKey(crypt_secret_key, hash256, iv, d);
	if (!Error) return false;

	d = 987;//Затираю ключ.
	string str_iv = "";
	ByteArrayToHexString(iv, 16, str_iv); //Конвертирую байт массив в строку

										  //Вектор инициализации.
	str_iv_ = str_iv;
	salt_ = salt;//Соль.
	crypt_secret_key_ = crypt_secret_key; //Шифрованный секретный ключ.

	return true;
}

//ECname-имя элиптической кривой;Q-открытый ключ	
bool SecretKeyMaker::CreateEcKeyPair(bool BeginSelfTest, string EcOID, uint8_t *secret_key, uint8_t key_len, Point &Q, mpz_class &d)
{
	try
	{
		//Проверяю OID кривой
		Groups curEc;
		bool find_Ec = curEc.SetParam(EcOID);
		if (!find_Ec) //Не найдено кривой с данным OID
		{
			lastError = "Не верный OID эллиптической кривой.OID=" + EcOID;
			return false;
		}

		CreateSign Gen; //Объект содержаций методы для генерации ключа

		if (BeginSelfTest)
		{
			//Тест алгоритма создания открытого ключа	
			string test_result = Gen.TestPk();
			if (test_result.length() > 0)
			{
				lastError = test_result;
				return false;
			}
		}

		//Генерирую ключи
		Point Q1;
		mpz_class d1;
		Q1 = Gen.generate_public_key_512(secret_key, d1, EcOID);

		Q = Q1;
		d = d1;

	}
	catch (exception& e)
	{
		lastError="В методе CreateEcKeyPair() возникло исключение:" +string(e.what());
		return false;

	}

	return true;
}


//Генерирует 17 байтную соль.
bool SecretKeyMaker::GenerateSalt(string &d)
{
	try
	{
		//Генерирую 17 байтную соль. Для генерации использую системный ГСЧ считающийся криптоустойчивым
		int rnd; //Значения ГСЧ
		unsigned long urnd;
		string str_r;
		d = "";
		//Формирую 40 байтное число
		for (int i = 0;i < 10;i++)
		{
			rnd = QRandomGenerator::system()->generate();
			urnd = (unsigned long)rnd;
			str_r = std::to_string(urnd);
			d = d + str_r;
		}

		//Преобразовываю строку в большое число
		mpz_class D(d, 10);
		d = D.get_str(16); //Преобразовываю число в 16ричную строку

						   //Беру 17 байт
		d = d.substr(0, 34);
	}
	catch (std::exception &e)
	{
		lastError= "В методе GenerateSalt возникло исключение:" +string(e.what());
		return false;
	}

	return true;
}

//Хэширует пароль с солью, в случае ошибки возвращает сообщение
bool SecretKeyMaker::HashedPassword(QString &Password, string salt, unsigned char *hash)
{
	try
	{
		//Строка пароля представлена двумя байтами в UTF16, конвертирую в Windows-1251
		QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
		QByteArray pswd = codec->fromUnicode(Password);
		unsigned long  pswd_len = (unsigned long)pswd.length(); //Длина строки пароля
																//Массив будет хранить пароль
		unsigned char *tmp_pswd = new unsigned char[pswd_len];
		for (int i = 0;i < pswd_len;i++)
		{
			tmp_pswd[i] = (unsigned char)pswd[i];
		}
		//Преобразование соли в массив
		//Если с солью проблемы-ее длина не четная
		if (salt.length() % 2 != 0) salt = "0" + salt;

		int salt_length = salt.length();
		int sal = salt_length / 2;
		unsigned long  salt_array_len = (unsigned long)(sal); //Размер массива в байтах,два символа строки описывают байт
		unsigned char *salt_array = new unsigned char[salt_array_len];

		//Конвертирует 16 ричную строку в массив unsigned char
		HexStr_to_uchar(salt, salt_array);

		//Формирую массив подлежащий хэшированию
		unsigned long  array_to_hash_len = pswd_len + salt_array_len;
		unsigned char *array_to_hash = new unsigned char[array_to_hash_len];//Строка подлежащая хэшированию
																			//Копирую пароль и соль
		int pos = 0;
		for (unsigned long i = 0;i < pswd_len;i++)
		{
			array_to_hash[pos] = tmp_pswd[i];
			pos++;
		}

		for (unsigned long i = 0;i < salt_array_len;i++)
		{
			array_to_hash[pos] = salt_array[i];
			pos++;
		}

		//Удаляю временный массивы
		delete[]tmp_pswd; tmp_pswd = NULL;
		delete[]salt_array;salt_array = NULL;

		//Хэширую
		Hash3411 H;
		unsigned long  mess_len;
		mess_len = array_to_hash_len * 8; //Длина сообщения в битах
		H.hash_256(array_to_hash, (unsigned long long)mess_len, hash);

	}
	catch (...)
	{
		lastError="В методе HashedPassword возникло исключение:";
		return false;
	}

	return true;
}

bool SecretKeyMaker::сreateInvisibleDHkey(QString Password, string & str_iv_, string & salt_, string & crypted_secret_DH_key_, string &DH_public_)
{
	string DH_public; //Пара ключей
	uint8_t DHprivate_key[8192];
	uint32_t DHprivate_key_len = 8192; //Обязательно нужно инициализировать-иначе GenerateDHpair не будет знать сколько у нее есть 
									   //байт для записи ключа

	bool Error = GenerateDHpair(DH_public, DHprivate_key, DHprivate_key_len); //Генерирую ключевую пару.
	if (!Error) return false;

	string salt = "";
	//Генерирую 17байтную соль. Соль случайное число.
	Error = GenerateSalt(salt);
	if (!Error) return false;


	//Вычисляю хэш соленого пароля
	unsigned char hash256[32];
	Error = HashedPassword(Password, salt, hash256);
	if (!Error) return false;


	//Генерирую вектор инициализации
	unsigned char iv[16];
	Error = GenerateIV(iv);
	if (!Error) return false;

	string crypted_secret_DH_key; //Шифрованный секретный ключ Диффи-Хэлмана

								  //Шифрую закрытый ключ алгоритма Диффи-Хелмана
	Error = CryptDHSecretKey(crypted_secret_DH_key, DHprivate_key, DHprivate_key_len, hash256, iv);
	if (!Error) return false;

	string DH_iv;//Значение начального вектора для расшифровывания секретного ключа схемы Д-Х 
	ByteArrayToHexString(iv, 16, DH_iv); //Конвертирую байт массив в 16ричную строку

	str_iv_ = DH_iv;
	salt_ = salt;
	crypted_secret_DH_key_ = crypted_secret_DH_key;
	DH_public_ = DH_public;

	return true;
}


//Генерирую вектор инициализации
bool SecretKeyMaker::GenerateIV(unsigned char *iv)
{
	try
	{
		//Используя системный ГСЧ получаю число
		int rnd = QRandomGenerator::system()->generate();

		//Генерирую  псевдослучайное число на основе алгоритма Вихрь Мерсенна
		gmp_randclass Rand(gmp_randinit_mt);
		mpz_class r;
		Rand.seed(rnd);//В качестве начального значения использую rnd
		r = Rand.get_z_bits(128);
		//Преобразование большого числа в массив
		mpz_export((void *)iv, NULL, 1, sizeof(unsigned char), 1, 0, r.get_mpz_t());
		/*
		//Проверка
		string a = r.get_str(16); //Полученной случайное число
		mpz_class x;
		size_t len = (size_t)16;//размер массива
		//Преобразование массива в большое число
		mpz_import(x.get_mpz_t(), len, 1, sizeof(iv[0]), 0, 0, iv);
		string b = x.get_str(16);
		QString s1(a.c_str());
		QString s2(b.c_str());
		QMessageBox::information(this, "", s1 + "\r\n" + s2);
		*/

	}
	catch (...)
	{
		lastError="В методе GenerateIV возникло исключение:";
		return false;
	}

	return true;
}

//Шифрую секретный ключ , в качестве ключа используется хэш, в случае ошибки возвращает сообщение
bool SecretKeyMaker::CryptEcSecretKey(string &result, unsigned char *hash, unsigned char *iv, mpz_class &d)
{
	try
	{
		uint8_t sk[64]; //Закрытый ключ
		for (int i = 0;i < 64;i++) sk[i] = 0;
		//Проверка размера
		unsigned int sz = mpz_sizeinbase(d.get_mpz_t(), 2);
		if ((sz / 8) > 64)
		{
			lastError = " CryptSecretKey: не верная длина ключа.";
			return false;
		}
			

		//Преобразование большого числа в массив
		mpz_export((void *)sk, NULL, 1, sizeof(unsigned char), 1, 0, d.get_mpz_t());

		ByteBlock cr_key(hash, 32); //Ключ 
									//Сообщение(секретный ключ) подлежащее шифрованию
		ByteBlock crypt_msg(sk, 64);
		//Вектор инициализации
		ByteBlock iv_(iv, 16);
		//Результат шифрования
		ByteBlock crypted_secret_key;
		Sib3412 CrAlgoritm(cr_key);
		//Выполняю шифрование в режиме обратной связи по шифротексту
		CrAlgoritm.CFB_Encrypt(crypt_msg, crypted_secret_key, iv_);
		//Преобразовываю в 16ричную строку
		result = Utillity::hex_representation(crypted_secret_key);
	}
	catch (...)
	{
		lastError="В методе CryptSecretKey возникло исключение:";
		return false;
	}

	return true;
}

//Конвертируют байтовый массив в строку
void SecretKeyMaker::ByteArrayToHexString(unsigned char *array, int len, string &str)
{
	//Конвертирую массив в объект mpz_class
	mpz_class D;
	size_t l = (size_t)len;
	//Преобразование массива в большое число
	mpz_import(D.get_mpz_t(), l, 1, sizeof(unsigned char), 0, 0, array);
	str = D.get_str(16);
}


//Конвертирует 16 ричную строку в массив unsigned char
void SecretKeyMaker::HexStr_to_uchar(string str, unsigned char *array)
{
	int length = str.length();
	unsigned char *p1 = array + ((unsigned char)(length / 2) - 1); //Последний элемент массива array
	string hex;
	//Цикл начиная со старшей ячейки 
	for (int p = length;p >= 2;p = p - 2)
	{
		hex = str.substr(p - 2, 2);
		*p1 = std::stoi(hex, nullptr, 16);
		p1--;
	}
}


bool SecretKeyMaker::GenerateDHpair(string & DH_public, uint8_t *private_key, uint32_t &private_key_len)
{
	try
	{
		uint8_t public_key[8192];
		uint32_t public_key_len = 8192;

		AutoSeededRandomPool rng; //Использует ОС для  получения seed

		InvertibleRSAFunction params; //Функция RSA с использованием закрытого ключа.
		params.GenerateRandomWithKeySize(rng, 4096); //Generate a random key or crypto parameters.

		RSA::PrivateKey privateKey(params);
		RSA::PublicKey publicKey(params);
		//sha256WithRSAEncryption
		//Сохраняю закрытый ключ в буфер
		ArraySink arraySink1(private_key, private_key_len);// buffer1.size());
		privateKey.DEREncode(arraySink1);//Используется Der кодировка
		private_key_len = static_cast<uint32_t>(arraySink1.TotalPutLength()); //Реальная длина записанных данных

																			  //Сохраняю открытый ключ в буфер
		ArraySink arraySink2(public_key, public_key_len);//uffer2.size());
		publicKey.DEREncode(arraySink2);//Используется Der кодировка
		public_key_len = arraySink2.TotalPutLength(); //Реальная длина записанных данных

													  //Преобразовываю массив в строку
		string s2(reinterpret_cast<char const*>(public_key), public_key_len);

		DH_public = s2;

	}
	catch (...)
	{
		lastError="В методе GenerateDHpair() возникло исключение:";
		return false;
	}

	return true;
}


QString SecretKeyMaker::str_to_Hex(string & str)
{
	QString str1, str2;
	//Преобразования из бинарного вида в 16 ричный
	str1 = QString::fromLocal8Bit(str.c_str(), str.length());
	str2 = str1.toLocal8Bit().toHex();

	return str2;
}

//Сохраняет контейнер ключа в файл
bool SecretKeyMaker::KeyСontainerToFile(PkContainer &KeyCont, QString PathToSave)
{
	try
	{
		QString Error = "";

		QString out_str = "";
		SerializableTools::Serialize(out_str, KeyCont);//Сериализация объекта.

															   //Русские буквы представлены в кодировке utf8
		QByteArray bytes = out_str.toUtf8();//Строку в байт массив 
		QByteArray packing; //Запакованный контейнер
							//Упаковываю контейнер
		if (!PackContainer(bytes, packing))
		{
			lastError = "Ошибка W0:Ошибка упаковки.";
			return false;
		}

		//------Вычисление хэш 256 упакованного контейнера---
		uint32_t key_data_len = static_cast<uint32_t>(packing.size());//Размер упакованного контейнера ключа
		uint8_t *key_data = reinterpret_cast<uint8_t*>(packing.data()); //Указатель на начало данных
		qint64 w_result;//Результат записи в файл-размер данных		

		uint8_t hash256[32]; //Результат вычисления хэш функции	  
		GetHashСontainer(key_data, key_data_len, hash256);//Вычисляю хэш

														  //--------Дополнение файла служебной информацией
		uint8_t file_title[Key_title_len];//Заголовок файла-служебная информация
		SetService_Information(file_title, hash256, key_data_len);

		//----------Сохранение ключа в файл
		QFile fileOut(PathToSave);
		if (!fileOut.open(QIODevice::WriteOnly))
		{
			lastError = "Не удалось сохранить файл "; //PathToSave;
			return false;
		}
		

		//Устанавливаю в нулевую позицию
		if (!fileOut.seek(0))
		{
			lastError = "Не удалось открыть файл для записи."; // PathToSave
			return false;
		}

		//-----------Запись заголовка
		w_result = fileOut.write(reinterpret_cast<char *>(file_title), Key_title_len);
		if (w_result != Key_title_len)
		{
			lastError = "Ошибка W1:Не удалось сохранить файл. ";// +PathToSave;
			return false;
		}


		//----------------------------------------------- Ставлю указатель на начало заголовка
		if (!fileOut.seek(Key_title_len))
		{
			lastError = "Не удалось открыть файл для записи."; //PathToSave
			return false;
		}


		//--------------Запись упакованного контейнера ключа--------------------
		w_result = fileOut.write(packing, key_data_len);
		if (w_result != key_data_len)
		{
			lastError = "Ошибка W2:Не удалось сохранить файл ";//PathToSave;
			return false;
		}

		fileOut.close();

	}
	catch (...)
	{
		lastError = "В методе KeyСontainerToFile() возникло исключение.";
		return false;
	}

	return true;
}


//Запаковывает контейнер секретного ключа
bool SecretKeyMaker::PackContainer(QByteArray &Container, QByteArray &Packing)
{
	//Генерация вектора инициализации для шифрования
	unsigned char iv[16];
	bool Error = GenerateIV(iv);
	if (!Error) return false;
	
	//Ключ которым шифруется сообщение
	bool isErr = false;
	string Err = "";

	//Получает значение ключа упаковывания контейнера. В случае ошибки HasErr=1;Err-содержит сообщение об ошибке.
	ByteBlock cr_key = Utillity::hex_to_bytes(GranitCore::KeyChipperConstant); //Получаю ключ упаковки контейнера секретного ключа.
	
	//Преобразовываю  QByteArray в ByteBlock
	size_t mess_length = static_cast<size_t>(Container.size());//Длина сообщения
	uint8_t *d_ptr; //Начало блока данных
	d_ptr = reinterpret_cast<uint8_t*>(Container.data());

	ByteBlock message(d_ptr, mess_length); //Копирую данные
										   //Преобразовываю вектор инициализации
	ByteBlock init_vector(iv, 16);
	//Результат шифрования
	ByteBlock crypt_result;

	Sib3412 CrAlgoritm(cr_key);
	//Выполняю шифрование в режиме обратной связи по шифротексту
	CrAlgoritm.CFB_Encrypt(message, crypt_result, init_vector);

	Packing.resize(20);
	//Копирую iv
	int pos = 0;
	for (uint8_t i = 0;i < 16;i++)
	{
		Packing[pos] = static_cast<char>(iv[i]);
		pos++;
	}

	//Сохраняю длину не шифрованного блока
	pos = 16;
	uint32_t tmp0, tmp1 = mess_length;
	for (uint8_t i = 0;i < 4;i++)
	{
		tmp0 = tmp1 & 0xff;
		Packing[pos] = static_cast<char>(tmp0);
		tmp1 >>= 8;
		pos++;
	}
	//Длина шифрованного блока
	mess_length = crypt_result.size();
	Packing.resize(mess_length + 20);
	//Копирую данные 
	for (int i = 0;i < mess_length;i++)
	{
		Packing[pos] = static_cast<char>(crypt_result[i]);
		pos++;
	}

	return true;
}

bool SecretKeyMaker::CreateRequestFile(QString pathToSave, mRequest &Req)
{
	try
	{
		QString out_str = "";
		SerializableTools::Serialize(out_str, Req);//Сериализация объекта.

														   //Генерация вектора инициализации для шифрования
		unsigned char iv[16];
		bool Error = GenerateIV(iv);
		if (!Error) return false;


		//Ключ которым шифруется сообщение
		bool isErr = false;
		string Err = "";
			   
		ByteBlock cr_key = Utillity::hex_to_bytes(GranitCore::KeyChipperConstant); //Получаю ключ упаковки макета.
		
		//Сообщение подлежащее шифрованию-контейнер "макета ключа"
		QByteArray bytes = out_str.toUtf8();//Строку в байт массив 


											//Преобразовываю  QByteArray в ByteBlock
		size_t mess_length = bytes.length();//Длина сообщения
		ByteBlock message((unsigned char *)bytes.data(), mess_length);

		//Преобразовываю вектор инициализации
		ByteBlock init_vector(iv, 16);
		//Результат шифрования
		ByteBlock crypt_result;

		//Шифрую строку		
		Sib3412 CrAlgoritm(cr_key);
		//Выполняю шифрование в режиме обратной связи по шифротексту
		CrAlgoritm.CFB_Encrypt(message, crypt_result, init_vector);

		//Собираю вектор инициализации и шифрованное сообщение в один массив
		unsigned long array_len = 16 + ((unsigned long)crypt_result.size());//iv+шифрованный контейнер макета
		unsigned char *save_array = new unsigned char[array_len];

		for (unsigned long i = 0;i < 16;i++)
		{
			save_array[i] = iv[i];
		}

		int pos = 0;
		for (unsigned long i = 16;i < array_len;i++)
		{
			save_array[i] = crypt_result[pos];
			pos++;
		}


		//Вычисляю хэшь сообщения 512 байт
		Hash3411 H;
		unsigned long hashed_len = array_len * 8;//Длина сообщения в битах
		unsigned char hash512[64];
		H.hash_512(save_array, (unsigned long long)hashed_len, hash512);

		//Собираю массив для записи в файл
		int save_array_to_file_len = array_len + 64 + 1;
		unsigned char *save_array_to_file = new unsigned char[save_array_to_file_len];

		//Очищаю-дабы проверить корректность записи
		for (int i = 0;i < save_array_to_file_len;i++) save_array_to_file[i] = 0;

		//Версия протокола
		save_array_to_file[0] = 1;

		//Копирую хэш в начало файла
		pos = 0;
		for (int i = 1;i < 65;i++)
		{
			save_array_to_file[i] = hash512[pos];
			pos++;
		}
		//Копирую содержимое файла
		pos = 0;
		for (int i = 65;i < save_array_to_file_len;i++)
		{
			save_array_to_file[i] = save_array[pos];
			pos++;
		}

		/*
		unsigned char a[10000];
		for (int i = 0;i < 10000;i++) a[i] = 0;

		for (int i = 0;i < save_array_to_file_len;i++)
		{
		a[i] = save_array_to_file[i];
		}
		*/

		//Удаляю контент 
		delete[]save_array; save_array = NULL;


		//Сохраняю в файл
		QString fileName = "Request[" + Req.P0() + Req.P1()[0] + Req.P2()[0] + "_" +
			QDateTime::currentDateTime().toString("dd-MM-yyyychhmmss") +
			"].grq"; ;

		QString filepath = pathToSave +"\\"+ fileName;

		QFile fileOut(filepath);
		if (!fileOut.open(QIODevice::WriteOnly))
		{
			delete[]save_array_to_file; save_array_to_file = NULL;
			lastError= "Не удалось сохранить файл";
			return false;
		}

		//unsigned char *d_ptr = crypt_result.byte_ptr();
		//unsigned int a_len = crypt_result.size();
		QByteArray fqa = QByteArray((char*)save_array_to_file, save_array_to_file_len);
		//int y = fqa.size();

		//QByteArray::fromRawData(const char*, int size)
		//fileOut.write(reinterpret_cast<char *>(save_array_to_file),save_array_to_file_len);
		fileOut.write(fqa);
		fileOut.close();
		delete[]save_array_to_file; save_array_to_file = NULL;


	}
	catch (...)
	{
		lastError= "В методе CreateRequest() возникло исключение.";
		return false;
	}

	return true;
}


//Шифрую секретный ключ алгоритма Диффи-Хэлмана,в качестве ключа используется хэш, в случае ошибки возвращает сообщение
bool SecretKeyMaker::CryptDHSecretKey(string &crypted_key, uint8_t *private_key, uint32_t &private_key_len, unsigned char *hash, unsigned char *iv)
{
	try
	{
		ByteBlock cr_key(hash, 32); //Ключ 
									//Сообщение(секретный ключ) подлежащее шифрованию
		ByteBlock crypt_msg(private_key, private_key_len);
		//Вектор инициализации
		ByteBlock iv_(iv, 16);
		//Результат шифрования
		ByteBlock crypted_secret_key;
		Sib3412 CrAlgoritm(cr_key);
		//Выполняю шифрование в режиме обратной связи по шифротексту
		CrAlgoritm.CFB_Encrypt(crypt_msg, crypted_secret_key, iv_);
		//Преобразовываю в 16ричную строку
		crypted_key = Utillity::hex_representation(crypted_secret_key);

	}
	catch (...)
	{
		lastError = "В методе CryptDHSecretKey возникло исключение.";
		return false;
	}

	return true;
}

//Вычисляет хэш256 контейнера
void SecretKeyMaker::GetHashСontainer(uint8_t *container, uint32_t &container_len, uint8_t *hash)
{
	//Вычисляю значение хэш функции
	Hash3411 H;
	unsigned long long  mess_len, tmp; //unsigned __int64
	tmp = static_cast<unsigned long long>(container_len);
	mess_len = tmp * 8; //Длина сообщения в битах
	H.hash_256(container, mess_len, hash);
}

void SecretKeyMaker::SetService_Information(uint8_t *arr, uint8_t *hash, uint32_t &conteiner_len)
{
	const uint8_t title1[7] = //Первые 7 байт содержащих что то вроде версии
	{ 0xf9,0xc5, 0xa8, 0xd3, 0x47, 0xb6, 0x3a };

	//Название программы
	string pgm_title;
	if(!enableRose)
	   pgm_title = "GranitK Information security protection system ";
	else
	   pgm_title = "Little Rose radials security protection system ";

	//Формирую заголовок файла
	uint8_t pos = 0;
	//[7-байт служебной информации]
	for (uint8_t i = 0;i < 7;i++)
	{
		arr[pos] = title1[i];
		pos++;
	}
	//[Заголовок 47байт]
	for (uint8_t i = 0;i < 47;i++)
	{
		arr[pos] = static_cast<uint8_t>(pgm_title[i]);
		pos++;
	}

	//Хэш 32 байта
	for (uint8_t i = 0;i < 32;i++)
	{
		arr[pos] = hash[i];
		pos++;
	}
	//Размер блока данных 4байта
	uint32_t len = conteiner_len;
	for (uint8_t i = 0;i < 4;i++)
	{
		arr[pos] = len & 0xff;
		len >>= 8;
		pos++;
	}

}