#include "stdafx.h"
#include "KeyService.h"


KeyService::KeyService()
{
	KeyContainerIsLoad = false;
	cachedSk = false;
	enableRose = false;
}


KeyService::~KeyService()
{
}

bool KeyService::LoadKey(QString PathToSecretKey)
{
	//Проверка существования файла
	QFileInfo f_info(PathToSecretKey);
	string alarm_mess = "";
	if (!f_info.exists())
	{
		QTextCodec *StrCodec;
		StrCodec = QTextCodec::codecForName("Windows-1251"); //Установка кодировки

		lastError = "ОШИБКА Л0:Не удалось найти файл ключа по указанному пути: " + 
			StrCodec->fromUnicode(PathToSecretKey) +
			". Убедитесь что флеш накопитель вставлен в компьютер.";
		return false;
	}
		

	//Проверка секретного ключа и получение длины
	//Возможно требуется запретить вызов в случае когда пользователь в первые выбирает ключ, функция будет вызвана два раза
	uint32_t conteiner_length = 0; //Размер контейнера ключа
	if (!CheckSK(PathToSecretKey, conteiner_length)) return false;

	//Считываю контейнер секретного ключа
	QFile file(PathToSecretKey);
	if (!file.open(QIODevice::ReadOnly))
	{
		lastError = "ОШИБКА Л1:Не удалось считать файл секретного ключа.";
		return false;
	}


	QByteArray Content; //Массив в который помещаются считанные данные

						//Устанавливаю в позицию начала блока данных контейнера
	if (!file.seek(Key_title_len))
	{
		lastError = "ОШИБКА Л2:Не  удалось считать файл секретного ключа.";
		file.close();
		return false;
	}

	//Чтение контейнера ключа
	Content = file.read(conteiner_length);
	if (Content.size() == 0)
	{
		lastError = "ОШИБКА Л3:Не удалось считать файл секретного ключа.";
		file.close();
		return false;
	}

	//Длина меньше блока iv(16) и блока описания длины данных(4)
	if (Content.size() < 20)
	{
		lastError = "ОШИБКА Л4:Не верный  файл секретного ключа.";
		file.close();
		return false;
	}

	file.close();

	//Распаковывает контейнер секретного ключа и проверяю его заголовок, в случает ошибки =0
	QByteArray Result;
	int ret = UnpackSKContainer(Content, Result);
	if (ret != 1) //Возникли ошибки.
	{
		switch (ret)
		{
		case 2:
			lastError = "ОШИБКА Л5.1:Не удалось считать библиотеку.";
			return false;
		case 3:
			lastError = "ОШИБКА Л5.2:Не удалось загрузить функцию из библиотеки.";
			return false;
		case 4:
			lastError = "ОШИБКА Л5.3:Не верная функция  библиотеки.";
			return false;
		case 5:
			lastError = "ОШИБКА Л5:Не верный  файл секретного ключа.";
			return false;
		default:
			break;
		}

	}

	//Преобразовываю контейнер в объект
	KeyContainerIsLoad = 0;
	if (!DeserializeKeyContainer(&Result, &KeyContainer))
	{
		lastError = "ОШИБКА Л6:Не верный  файл секретного ключа.";
		return false;
	}
	KeyContainerIsLoad = 1; //Контейнер был успешно считан

	return true;
}


/*
Проверяет секретный ключ, в случае ошибки возвращает 0
Проверка поверхностная. Проверяется соответствие заголовка и контрольной суммы контейнера ключа
Возвращает размер контейнера ключа container_length
*/
bool KeyService::CheckSK(QString PathToSK, uint32_t &container_length)
{
	QFileInfo file_info(PathToSK);
	qint64 f_size = file_info.size();
	/*
	Размер файла не удалось получить или размер файла меньше заголовка
	*/

	if (f_size == 0)
	{
		lastError = "ОШИБКА Р0:Не удалось открыть файл секретного ключа.";
		return false;
	}

	if (f_size < Key_title_len)
	{
		lastError = "ОШИБКА Р1:Не корректный размер файла секретного ключа.";
		return false;
	}

	//Считать размер контейнера секретного ключа, и проверить его размер
	QFile file(PathToSK);
	if (!file.open(QIODevice::ReadOnly))
	{
		lastError = "ОШИБКА Р2:Не удалось считать файл секретного ключа.";
		return false;
	}

	qint64 r_len;//Размер считанного блока данных
	uint8_t title[Key_title_len]; //Заголовок файла
	r_len = file.read(reinterpret_cast<char*>(title), Key_title_len);
	if (r_len != Key_title_len)
	{
		lastError = "ОШИБКА Р3:Не удалось считать файл секретного ключа.";
		file.close();
		return false;
	}

	//-------------Проверка правильности заголовка----------
	const uint8_t title1[7] = //Первые 7 байт содержащих что то вроде версии
	{ 0xf9,0xc5, 0xa8, 0xd3, 0x47, 0xb6, 0x3a };

	//Название программы
	string pgm_title;
	if (!enableRose)
		pgm_title = "GranitK Information security protection system ";
	else
		pgm_title = "Little Rose radials security protection system ";

	uint8_t pos = 0;
	//[7-байт служебной информации]
	for (uint8_t i = 0;i < 7;i++)
	{
		if (title1[i] != title[pos])
		{
			lastError = "ОШИБКА Р4:Файл секретного ключа, не является файлом секретного ключа СЗИ \"ГранитК\".";
			file.close();
			return false;
		}

		pos++;
	}
	//[Заголовок 47байт]
	for (uint8_t i = 0;i < 47;i++)
	{

		if (title[pos] != static_cast<uint8_t>(pgm_title[i]))
		{
			lastError = "ОШИБКА Р5:Файл секретного ключа, не является файлом секретного ключа СЗИ \"ГранитК\".";
			file.close();
			return false;
		}
		pos++;
	}

	//---------Проверка длины блока контейнера
	uint32_t conteiner_len = 0, tmp1 = 0; //Длина контейнера ключа
	pos = Key_title_len - 4;
	for (uint8_t i = 0;i < 4;i++)
	{
		tmp1 = title[pos];
		conteiner_len |= tmp1 << i * 8;
		pos++;
	}
	//Длина ключа не может быть менее чем заголовок+контейнер
	qint64 key_len = Key_title_len + static_cast<qint64>(conteiner_len);
	if (f_size < key_len)
	{
		lastError = "ОШИБКА Р6:Не верный размер файла секретного ключа.";
		file.close();
		return false;
	}

	//-------------Вычисление хэш контейнера	
	//Считываю контейнер
	//Устанавливаю в позицию начала блока данных контейнера
	if (!file.seek(Key_title_len))
	{
		lastError = "ОШИБКА Р7:Не  удалось считать файл секретного ключа.";
		file.close();
		return false;
	}

	//-------Данные контейнера
	uint8_t *data_block = new uint8_t[conteiner_len];
	//Чтение
	r_len = file.read(reinterpret_cast<char*>(data_block), static_cast<quint64>(conteiner_len));
	if (r_len != static_cast<quint64>(conteiner_len))
	{
		lastError = "ОШИБКА Р8:Не  удалось считать файл секретного ключа.";
		file.close();
		delete[]data_block;data_block = NULL;
		return false;
	}


	uint8_t hash256[32];
	Hash3411 H;
	unsigned long long  mess_len, tmp; //unsigned __int64
	tmp = static_cast<unsigned long long>(conteiner_len);
	mess_len = tmp * 8; //Длина сообщения в битах
	H.hash_256(data_block, mess_len, hash256);

	delete[]data_block;data_block = NULL;

	//Проверка Хэш
	pos = 54; //Начало хэш
	for (uint8_t i = 0;i < 32;i++)
	{
		if (title[pos] != hash256[i])
		{
			lastError = "ОШИБКА Р9:Не верная контрольная сумма файла. Файл поврежден или был изменен третьим лицом.";
			file.close();
			return false;
		}
		pos++;
	}
	//Успех
	file.close();
	container_length = conteiner_len;

	return true;
}



//Получает значение ключа упаковывания контейнера. В случае ошибки HasErr=1;Err-содержит сообщение об ошибке.
ByteBlock  KeyService::GetPackKv(string & Err, bool & HasErr)
{
	HMODULE hLib;
	hLib = LoadLibraryA("libCn3q.dll");
	if (hLib == 0)
	{
		Err = "Ошибка РК1:Не удалось загрузить библиотеку.";
		HasErr = 1;
		return NULL;
	}

	typedef int(*AreApisANSI)(char *Linear, int *Len, char *Value); //Тип данных для упаковки контейнера ключа.


	AreApisANSI areApis = (AreApisANSI)GetProcAddress(hLib, "AreApisANSI"); //Получаю адрес функции в библиотеке.
	if (areApis == 0)  //Не удалось загрузить функцию.
	{
		Err = "Ошибка РК2:Не удалось загрузить функцию из библиотеки.";
		HasErr = 1;
		return NULL;
	}


	char LinearA[128];
	char ValueA[300] = "t9034l9ff0--s78";
	int aLen = 537;
	int retA = areApis(LinearA, &aLen, ValueA); //Вызываю функцию.
	if (retA != 16537) //Странная функция в dll.
	{
		Err = "Ошибка РК3:Ошибка функции библиотеки.";
		HasErr = 1;
		return NULL;
	}

	string lA(LinearA);
	ByteBlock cr_key = Utillity::hex_to_bytes(lA); //Crypt_Key_For_Container
	bool fret = FreeLibrary(hLib);
	if (fret == 0)
	{
		Err = "Ошибка РК4:Ошибка трансляции библиотеки.";
		HasErr = 1;
		return NULL;
	}

	return cr_key;
}



//Расшифровывает контейнер секретного ключа и проверяю его заголовок, в случает ошибки =0
int KeyService::UnpackSKContainer(QByteArray &C, QByteArray &Result)
{

	uint8_t *data_ptr; //Указатель на начало данных в контейнере С
	char *c_ptr;
	c_ptr = C.data();
	data_ptr = reinterpret_cast<uint8_t *>(c_ptr);

	ByteBlock init_vector(data_ptr, 16); //Вектор инициализации

	uint32_t data_len = C.size() - 20; //Размер сообщения
	data_ptr = data_ptr + 20; //Устанавливаю указатель на начало блока данных
	ByteBlock crypted_message(data_ptr, data_len); //Шифрованное сообщение		
												   //Ключ которым шифруется сообщение

	HMODULE hLib;
	hLib = LoadLibraryA("libCn3q.dll");
	if (hLib == 0) return 2; //Не удалось загрузить библиотеку.
	AreApisANSI areApis = (AreApisANSI)GetProcAddress(hLib, "AreApisANSI");
	if (areApis == 0) return 3; //Не удалось загрузить функцию.

	char LinearA[128];
	char ValueA[300] = "t9034l9ff0--s78";
	int aLen = 537;
	int retA = areApis(LinearA, &aLen, ValueA);
	if (retA != 16537) return 4; //Странная функция в dll.

	string lA(LinearA);
	FreeLibrary(hLib);

	ByteBlock cr_key = Utillity::hex_to_bytes(lA); //Crypt_Key_For_Container

	Sib3412 K(cr_key);
	ByteBlock decrypt_result;
	//Расшифровываю
	K.CFB_Decrypt(crypted_message, decrypt_result, init_vector);
	//Преобразовываю данные в байт массив
	unsigned char *xptr = decrypt_result.byte_ptr();
	QByteArray byte_array(reinterpret_cast<char *>(xptr), decrypt_result.size());

	//Проверяю что там внутри
	QString maket_str = QString::fromUtf8(byte_array);
	QString maket_head = maket_str.mid(39, 13); 
	maket_head = maket_head.trimmed();

	if (maket_head != "<PkContainer>")	return 5;

	Result.resize(byte_array.size());
	for (int i = 0;i < byte_array.size();i++)
	{
		Result[i] = byte_array[i];
	}

	return 1;
}




/*
int KeyService::UnpackSKContainer(QByteArray &C, QByteArray &Result)
{

	uint8_t *data_ptr; //Указатель на начало данных в контейнере С
	char *c_ptr;
	c_ptr = C.data();
	data_ptr = reinterpret_cast<uint8_t *>(c_ptr);

	ByteBlock init_vector(data_ptr, 16); //Вектор инициализации

	uint32_t data_len = C.size() - 20; //Размер сообщения
	data_ptr = data_ptr + 20; //Устанавливаю указатель на начало блока данных
	ByteBlock crypted_message(data_ptr, data_len); //Шифрованное сообщение
	string k = FakeCryptKey::GetKey(100);
												   //Ключ которым шифруется сообщение
	//ByteBlock cr_key = Utillity::hex_to_bytes(Crypt_Key_For_Container);
	ByteBlock cr_key = Utillity::hex_to_bytes(k.c_str());

	Kuznyechik2 K(cr_key);
	ByteBlock decrypt_result;
	//Расшифровываю
	K.CFB_Decrypt(crypted_message, decrypt_result, init_vector);
	//Преобразовываю данные в байт массив
	unsigned char *xptr = decrypt_result.byte_ptr();
	QByteArray byte_array(reinterpret_cast<char *>(xptr), decrypt_result.size());

	//Проверяю что там внутри
	QString maket_str = QString::fromUtf8(byte_array);
	QString maket_head = maket_str.left(8); //<XmlKey>

	if (maket_head != "<XmlKey>")	return false;

	Result.resize(byte_array.size());
	for (int i = 0;i < byte_array.size();i++)
	{
		Result[i] = byte_array[i];
	}

	return true;
}
*/

//Десериализует контейнер
bool KeyService::DeserializeKeyContainer(QByteArray *Maket, QObject *object)
{
	QDomDocument doc;

	QString         errorString;
	int             errorLine;
	int             errorColumn;

	if (doc.setContent(*Maket, false, &errorString, &errorLine, &errorColumn))
	{
		QDomElement root = doc.documentElement();
		QString value;//Значение узла
					  //Цикл по свойствам класса	
		for (int i = 0;i < object->metaObject()->propertyCount();i++)
		{
			QMetaProperty prop = object->metaObject()->property(i);
			QString propName = prop.name();
			if (propName == "objectName") continue;

			//Листаю узлы в файле
			for (int i = 0; i < root.childNodes().count(); i++)
			{
				QDomElement node = root.childNodes().at(i).toElement();
				//Нашел узел с нужными свойствами
				if (node.nodeName() == propName)
				{
					value = node.toElement().text();
					object->setProperty(propName.toUtf8().data(), QVariant(value));

				}
			}
		}

	}
	else
	{
		return false;
	}
	return true;
}

//Проверяет пароль
bool KeyService::CheckPassword(QString psw)
{
	RSA::PublicKey publicKey;
	RSA::PrivateKey secret_key;

	//Расшифровываю секретный ключ ДХ. Пытаюсь его загрузить, если пароль не верный -ключ не возможно будет загрузить 
	if (!DecryptDH_SK(psw, publicKey, secret_key))
	{
		lastError = "Не верный пароль.";
		return false;
	}

	//Расшифровываю и проверяю ключевую пару подписи
	Point  Q;
	mpz_class d;
	if (!DecryptEc_SK(psw, Q, d))
	{
		lastError = "Не верный пароль.";
		return false;
	}


	//Сохраняю секретный ключ.
	Skd.SetSk(d);
	//Param.IsLoadSk = true; //Пароль был проверен,секретный ключ был считан.
	  //Затираю
	//Затираю ключ размером 512байт.
	for (int i = 0;i < 512;i++)
	{
		d >>= 1;
	}
	d = 33554432;

	//Затираю пароль
	QString cl;
	for (int i = 0;i < psw.length();i++)
	{
		psw[i] = 't';
	}

	return true;
}

//Возвращает открытый ключ
Point KeyService::GetPk()
{
	return	pubECkey;
}

//Возвращает закрытый ключ
mpz_class KeyService::GetPrvEcKey()
{
	return prvEcKey;
}

string KeyService::GetCertAlgorithmIdentifier()
{
	string val;
	val = KeyContainer.P23().toStdString();	//SignCertAlgoritm-алгоритм подписи сертификата
	return val;
}

string KeyService::GetPublicKeyAlgoritm()
{
	string val;
	val = KeyContainer.P22().toStdString();//PublicKeyAlgoritm-алгоритм открытого ключа
	return val;
}

string KeyService::GetEcParam()
{
	string val;
	val = KeyContainer.P24().toStdString();	//EcOid-параметры эллиптической кривой
	return val;
}

string KeyService::getLastError()
{
	return lastError;
}

bool KeyService::getKeyContainerStatus()
{
	return KeyContainerIsLoad;
}

PkContainer KeyService::getContainer() const
{
	return KeyContainer;
}


//Расшифровывает закрытый ключ схемы Диффи-Хэлмана
bool KeyService::DecryptDH_SK(QString Password, RSA::PublicKey &publicKey, RSA::PrivateKey &secret_key)
{
	uint8_t hash256[32];
	QString hex_salt; //Соль в 16 ричном формате
	hex_salt = KeyContainer.P21(); //Значение соли для алгоритма ДХ
	HashedPassword(Password, hex_salt, hash256);
	//Используя хэш как ключ для расшифровывания сообщения. Расшифровываю СК схемы ДХ
	string DH_pk; //закрытый ключ
	DecryptSK(hash256, KeyContainer.P18(), KeyContainer.P19(), DH_pk);
	/*
	Тестирование ключевых пар содержащихся в контейнере.
	Грузится открытый ключ, если ключ нельзя загрузить-выводится вообщение возвращается 0
	Грузится закрытый ключ. Если его не удалось расшифровать, то загрузить его не возможно-выводится вообщение возвращается 0
	Сравнивается значение модулей n открытого и закрытого ключа
	*/
	if (!GetDhPair(KeyContainer.P20(), DH_pk, publicKey, secret_key)) return 0;

	return true;
}

//Получает результат хэширования пароля и соли
bool KeyService::HashedPassword(QString &Passwd, QString &salt_str, uint8_t *hash)
{
	try
	{
		//Строка пароля представлена двумя байтами в UTF16, конвертирую в Windows-1251
		QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
		QByteArray pswd = codec->fromUnicode(Passwd);
		//Преобразовываю соль в виде 16 ричной строки в байт массив
		QByteArray salt = QByteArray::fromHex(salt_str.toUtf8());


		int pswd_len = pswd.size();//Размер пароля
		int salt_len = salt.size();//Размер соли

		QByteArray data_to_hash;//Массив байт подлежащий хэшированию
		data_to_hash.resize(pswd_len + salt_len);

		//Копирую данные
		int pos = 0;
		//Копирую пароль
		for (int i = 0;i < pswd_len;i++)
		{
			data_to_hash[pos] = pswd[i];
			pos++;
		}

		//Копирую соль
		for (int i = 0;i < salt_len;i++)
		{
			data_to_hash[pos] = salt[i];
			pos++;
		}


		//Хэширую
		Hash3411 H;
		unsigned long long  mess_len, tmp;
		tmp = static_cast<unsigned long long>(data_to_hash.size());
		mess_len = tmp * 8; //Длина сообщения в битах
		H.hash_256(reinterpret_cast<uint8_t*>(data_to_hash.data()), mess_len, hash);

	}
	catch (...)
	{
		return 0;
	}

	return 1;
}


//Расшифровывает секретный ключ CryptedKey используя key
bool KeyService::DecryptSK(uint8_t *key, QString &CryptedKey, QString &iv, string &p_key)
{

	//Преобразовываю начальный вектор ввиде 16 ричной строки в байт массив
	QByteArray bIV = QByteArray::fromHex(iv.toUtf8());
	//Преобразовываю строку в 16ричном формате, зашифрованного закрытого ключа в байт массив
	QByteArray crypted_array = QByteArray::fromHex(CryptedKey.toUtf8());


	//Преобразовываю вектор инициализации
	ByteBlock init_vector(reinterpret_cast<uint8_t*>(bIV.data()), 16);
	//Преобразовываю шифрованное сообщение
	ByteBlock crypted_message(reinterpret_cast<uint8_t*>(crypted_array.data()), crypted_array.size());
	//Результат расшифровывания
	ByteBlock decrypt_result;
	//Ключ
	ByteBlock cr_key(key, 32);
	Sib3412 K(cr_key);
	K.CFB_Decrypt(crypted_message, decrypt_result, init_vector);

	//Преобразовываю в 16ричную строку
	p_key = Utillity::hex_representation(decrypt_result);

	return 1;
}

//Проверяет пару ключей схемы Диффи-Хелмана. Если пара не совпадает возвращает 0
bool KeyService::GetDhPair(QString &open_key, string &private_key, RSA::PublicKey &publicKey, RSA::PrivateKey &secret_key)
{
	//Загрузка открытого ключа
	QByteArray arr1 = QByteArray::fromHex(open_key.toUtf8());
	

	//String Source не работает!!!!!
	ArraySource as1(reinterpret_cast<uint8_t*>(arr1.data()), arr1.size(), true /*pumpAll*/);

	//Если открытый ключ поврежден или не верный формат будет исключение
	try
	{
		publicKey.Load(as1);
	}
	catch (...)
	{
		lastError = "ОШИБКА K531";
		return false;
	}



	//Загрузка закрытого ключа
	QByteArray arr2 = QByteArray::fromHex(private_key.c_str());
	ArraySource as2(reinterpret_cast<uint8_t*>(arr2.data()), arr2.size(), true /*pumpAll*/);
	try
	{
		secret_key.Load(as2);
	}
	catch (...)
	{
		return 0;
	}

	//Сравнения значения n открытого ключа и закрытого ключа
	CryptoPP::Integer n1;//Закрытый ключ
	CryptoPP::Integer n2;//Открытый ключ

	n1 = secret_key.GetModulus();
	n2 = publicKey.GetModulus();

	if (n1.Compare(n2) != 0)
	{
		lastError = "ОШИБКА K532";
		return false;
	}


	return 1;
}

bool KeyService::DecryptEc_SK(QString Password, Point & Q, mpz_class & d)
{
	try
	{
		uint8_t hash256[32];
		QString hex_salt; //Соль в 16 ричном формате
		hex_salt = KeyContainer.P12(); //Значение соли для алгоритма подписи
		HashedPassword(Password, hex_salt, hash256);
		//Используя хэш как ключ для расшифровывания сообщения. Расшифровываю СК подписи
		string Ec_pk; //закрытый ключ
		DecryptSK(hash256, KeyContainer.P11(), KeyContainer.P14(), Ec_pk);

		Point Q1;
		//Закрытый ключ пользователя. Если строка содержит символы отличные от 16 ричной системы счисления возникнет исключение
		mpz_class d1(Ec_pk, 16);
		string  str_x, str_y;
		str_x = KeyContainer.P16().toStdString();
		str_y = KeyContainer.P17().toStdString();

		Q1.x.set_str(str_x, 16);
		Q1.y.set_str(str_y, 16);

		//Проверка соответствия ключевых пар
		string EcOid = KeyContainer.P24().toStdString();
		if (!CheckEcPair(Q1, d1, EcOid)) return false;

		//Посмотреть нужны ли эти строки!
		pubECkey = Q1;//Сохраняю открытый ключ для эллиптической криптографии
		pubECkey.setCurvOid(EcOid); //Задаю параметры кривой.
		prvEcKey = d1;//Сохраняю закрытый ключ для эллиптической криптографии


		d = d1; //Передаю закрытый ключ
	}
	catch (...)
	{
		return false;
	}
	return true;
}


bool KeyService::CheckEcPair(Point & Q, mpz_class & d, string ec_oid)
{
	Groups EC2;
	bool find_ec = EC2.SetParam(ec_oid);//Устанавливаю параметры криво на основании ее OID
	if (!find_ec)
	{
		lastError= "Bad Ec OID.";
		return false;
	}
		

	//Создаю точку с указанными в кривой координатами точки P
	Point P2(EC2, 1); //Точка эллиптической кривой 

	CreateSign Sg;
	Point Q1;
	Q1 = Sg.generate_public_key(d, P2); //Генерирую открытый ключ

										//Проверка координат
	if (Q.x != Q1.x) return false;
	if (Q.y != Q1.y) return false;

	return  true;
}

bool KeyService::CheckKeyDateValidity()
{
	//	Q_PROPERTY(QDateTime P9 READ P9 WRITE setP9) //Дата начала действия ключа
	//Q_PROPERTY(QDateTime P10 READ P10 WRITE setP10) //Дата окончания действия ключа

	QDateTime now = QDateTime::currentDateTime();

	if (now < KeyContainer.P9())
	{
		lastError = "Ошибка K 715: Дата начала действия ключа еще не наступила. Ключ начнет действовать " +
			KeyContainer.P9().toString("dd.MM.yyyy hh:mm:ss").toStdString() + ".";
		return false;
	}

	if (now > KeyContainer.P10())
	{
		lastError = "Ошибка K 716: Срок  действия ключа истек " +
			KeyContainer.P10().toString("dd.MM.yyyy hh:mm:ss").toStdString() + ".";
		return false;
	}

	return true;
}

//Проверяет соответствие закрытого ключа и сертификата в БД для данного ключа.
bool KeyService::CheckKeyCert(GranitDbI &Dbi)
{
	
	//Проверяю срок действия БД.
	if (!Dbi.checkDbDateEnd())
	{
		lastError = "Ошибка: Истек срок действия БД СКЗИ.";
		return false;
    }
	
	//Читаю сертификат ключа из БД системы.
	bool Estts = Dbi.GetUserCert(KeyContainer.P16(), KeyContainer.P17(), KeyContainer.P6(), KeyContainer.P7(), KeyContainer.P8());
	if (!Estts) 
	{
		lastError = Dbi.getLastError();
		return false;
	}

	
	QByteArray uBa = Dbi.getCert(); //Получаю считанный сертификат.

	//Считываю  сертификат пользователя;
	TBSCertificate  tbsUser;
	X509Reader R;
	R.ReadBa(uBa, tbsUser);

	//Проверяю сроки действия сертификата.
	Estts = CheckValidityCert(tbsUser);
	if (!Estts) return false;
	 

	string err;

	//Проверка соответствия открытого ключа и ключа представленного в сертификате.
	//	//Сравнивает открытый ключ сертификата и ключ в контейнере.
	if (!CompareKeys(tbsUser))
	{
		lastError = "Не совпадение ключа пользователя и сертификата в БД системы.";
		return false;
	}
		

	//Получаю сертификат CA.
	QByteArray baCAcert;
	bool IsError = Dbi.getCACert(baCAcert);
	if (!IsError)
	{
		lastError = "Ошибка.:Код РБ146.";
		return false;
	}
	

	//Читаю сертификат CA.
	TBSCertificate  tbsCA;
	R.ReadBa(baCAcert, tbsCA);

	//Проверяю сроки действия сертификата.
	Estts = CheckValidityCert(tbsCA);
	if (!Estts) return false;

		
	//Проверка подписи пользовательского сертификата.
	bool signStatus;
	err = R.VerifiCertSign(tbsUser, tbsCA, signStatus); //Проверка подписи
	if (!err.empty()) 
	{
		lastError = err;
		return false;
	}
		

	if (signStatus == false)
	{
		lastError = "Неверная подпись пользовательского сертификата.";
		return false;
	}

	//Проверка подписи сертификата CA
	//Чтение сертификата Root
	QByteArray baRootCert;
	IsError = Dbi.getRootCert(baRootCert);
	if (!IsError) 
	{
		lastError = "Ошибка.:Код РБ147.";
		return false;
	}


	//Читаю сертификат Root.
	TBSCertificate  tbsRoot;
	R.ReadBa(baRootCert, tbsRoot);

	//Проверяю сроки действия сертификата.
	Estts = CheckValidityCert(tbsRoot);
	if (!Estts) return false;
	

	//Проверка подписи сертификата CA
	err = R.VerifiCertSign(tbsCA, tbsRoot, signStatus); //Проверка подписи
	if (!err.empty())
	{
		lastError = err;
		return false;
	}

	if (signStatus == false)
	{
		lastError = "Неверная подпись  сертификата удостоверяющего центра(CA).";
		return false;
	}

	//Содержимое пользовательского сертификата.
	tbsUserCertificate = tbsUser;
	
	return true;
}



bool KeyService::GetSkInfo(string & Familia, string & Imia, string & Otchestvo, string & DateBegin, string & DateEnd, string & DaysLeft)
{
	if (KeyContainerIsLoad == false) return false; //Контейнер ключа не был загружен.

											   /*
											   Q_PROPERTY(QString P6 READ P6 WRITE setP6) //Familia
											   Q_PROPERTY(QString P7 READ P7 WRITE setP7) //Imia
											   Q_PROPERTY(QString P8 READ P8 WRITE setP8) //Otchestvo
											   */
	Familia = KeyContainer.P6().toLocal8Bit();
	Imia = KeyContainer.P7().toLocal8Bit();
	Otchestvo = KeyContainer.P8().toLocal8Bit();

	//Q_PROPERTY(QDateTime P10 READ P10 WRITE setP10) //Дата окончания действия ключа
	//Q_PROPERTY(QDateTime P9 READ P9 WRITE setP9) //Дата начала действия ключа
	DateBegin = KeyContainer.P9().toString("dd.MM.yyyy hh:mm:ss").toStdString();
	DateEnd = KeyContainer.P10().toString("dd.MM.yyyy hh:mm:ss").toStdString();

	//Количество оставшихся дней до окончания срока действия ключа.
	QDateTime curentDt = QDateTime::currentDateTime(); //Текущая дата.
	QDateTime DtEnd = KeyContainer.P10(); //Дата окончания действия ключа.

	int daysLeft = curentDt.daysTo(DtEnd); //Количество оставшихся дней.
	DaysLeft = std::to_string(daysLeft);

	return true;
}


void KeyService::enableCachedSk()
{
	cachedSk = true;
}

bool KeyService::isCacheSk()
{
	return Skd.hasKey();
}

void KeyService::getSignerIndex(string & Value)
{
	string val = "";
	QString Qx, Qy;

	Qx = KeyContainer.P16();//открытый ключ подписи координата X
	Qy = KeyContainer.P17();//открытый ключ подписи координата Y
	GranitDbI::getSignerIndex(Qx,Qy,val);

	Value = val;
}

mpz_class KeyService::getSignPk()
{
	return Skd.GetSk();
}

void KeyService::getSignOpenKey(string &Qx, string &Qy)
{
	string x1 = KeyContainer.P16().toStdString();
	string y1 = KeyContainer.P17().toStdString();
	Qx = x1;
	Qy = y1;
}

void KeyService::Clear()
{
	if (!Skd.hasKey()) return; //Ключ не был загружен. Не чего очищать.

	Skd.Clear(); //Очистка секретного ключа.
}

TBSCertificate KeyService::getUserCert()
{
	return tbsUserCertificate;
}

void KeyService::setRoseMode()
{
	enableRose = true;
}

string KeyService::getOpenDHkey()
{
	string  str_dh;
	str_dh = KeyContainer.P20().toStdString();

	return str_dh;
}

bool KeyService::getEcParam(string & Value)
{
	if (!KeyContainerIsLoad) return false;

	string param = "";
	param = KeyContainer.P24().toStdString();
	Value = param;
	
	return true;
}

//Проверяет срок действия сертификата. В случае проблем выдает сообщение. Возвращает false. Успех =true.
bool KeyService::CheckValidityCert(TBSCertificate cert)
{
	//Проверка организации издателя на пустоту.
	string iOrg = cert.Issuer_id_at_organizationName;
	if (iOrg.empty())
	{
		lastError = "Ошибка С 713A: Пустое поле organizationName издателя сертификата пользователя.";
		return false;
	}


	QDateTime NotBefore;
	QDateTime NotAfter;

	NotBefore = cert.NotBefore;
	NotAfter = cert.NotAfter;

	QDateTime now = QDateTime::currentDateTime();
	if (NotBefore > now) //Сертификат не начал еще действовать.
	{
		lastError = "Ошибка С 713: Сертификат выданный O=" +
			QTextCodec::codecForName("CP1251")->fromUnicode(cert.Subject_id_at_organizationName.c_str()).toStdString() +
			" ;CN=" + QTextCodec::codecForName("CP1251")->fromUnicode(cert.Subject_id_at_commonName.c_str()).toStdString() +
			" еще не начал действовать. Сертификат начнет действовать " + NotBefore.toString("dd.MM.yyyy hh:mm:ss").toStdString()
			+ ".";

		return false;
	}

	if (NotAfter < now) //Сертификат закончился.
	{
		lastError = "Ошибка С 714: Срок действия сертификата выданный O=" +
			QTextCodec::codecForName("CP1251")->fromUnicode(cert.Subject_id_at_organizationName.c_str()).toStdString() +
			" ;CN=" + QTextCodec::codecForName("CP1251")->fromUnicode(cert.Subject_id_at_commonName.c_str()).toStdString() +
			"закончился. Срок окончания действия сертификата истек " + NotAfter.toString("dd.MM.yyyy hh:mm:ss").toStdString()
			+ ".";

		return false;
	}

	return true;
}

//Сравнивает открытый ключ сертификата и ключ в контейнере.
bool KeyService::CompareKeys(TBSCertificate &tbsUser)
{
	string  str_x, str_y;
	str_x = KeyContainer.P16().toStdString();
	str_y = KeyContainer.P17().toStdString();
	mpz_class Qx(str_x, 16); //Открытый ключ из контейнера.
	mpz_class Qy(str_y, 16);

	mpz_class CertQx, CertQy; //Значение открытого ключа из сертификата
	CertQx = tbsUser.Qx;
	CertQy = tbsUser.Qy;

	//Сравниваю значения ключей.
	if (Qx != CertQx)	return false;
	if (Qy != CertQy)	return false;

	return true;
}