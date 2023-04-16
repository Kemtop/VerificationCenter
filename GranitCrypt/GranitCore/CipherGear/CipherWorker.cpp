#include "CipherWorker.h"


CipherWorker::~CipherWorker()
{
}


bool CipherWorker::initDistDir(QString val, QString &destinationDir)
{
	QFileInfo fi(val);
	QString DirPath = fi.absolutePath(); //encrypted files
	DirPath = DirPath + "/Encrypted_files";
	QDir dir(DirPath);
	if (!dir.exists())
	{
		if (!dir.mkpath("."))  return 0; //Не удалось создать каталог
	}

	if (!dir.exists()) return 0;
	destinationDir = DirPath;

	return true;
}

bool CipherWorker::CryptFiles(QStringList & paths_to_file, QString Password, string receiverKey, PkContainer &KeyContainer,
	string SignerIndex, string _rHashOkDH)
{
	QString Error = "";
	QString dst_Dir = "";//Путь к каталогу куда будут сохраняться шифрованные файлы						 

	bool IsCreateDir = initDistDir(paths_to_file[0], dst_Dir);//Инициализирует папку назначения,куда будут сохраняться зашифрованные файлы.
	if (!IsCreateDir)
	{
		lastError = "Не удалось создать каталог хранения зашифрованных файлов.";
		return false;
	}

	int f_count = paths_to_file.count(); //Количество файлов подлежащих шифрованию.
	
	uint8_t session_key[32]; //Сеансовый ключ для блочного шифра.
	
	//вынес в dll LoadAsymmetricKey. Убрать в будущем!

	//Кэширую ключ асимметричного шифрования.
	//Преобразовываю строку в 16 ричном виде в бинарный формат
	QByteArray HexArray = QByteArray::fromHex(receiverKey.c_str());
	//Преобразовываю ключ	
	ArraySource as_pk(reinterpret_cast<uint8_t*>(HexArray.data()), HexArray.size(), true);//pumpAll
	asymmetricKeyR.Load(as_pk); //Загружаю ключ.

	//Получаю закрытый ключ подписи
	Point Q1;
	mpz_class user_d; //закрытый ключ подписи

	if (!DecryptEc_SK(Q1, user_d, Password, KeyContainer))
	{
		lastError = "Ошибка K5 : Не удалось считать ключ.";
		return false;
	}

	signerIndex = SignerIndex; //Ни где не используется.
	rHashOkDH = _rHashOkDH;

	//Цикл по всем файлам
	bool errStts = false;
	for (int i = 0;i < f_count;i++)
	{
		//Обнуление.
		for (int i = 0;i < 32;i++) session_key[i] = 0;
		
		uint8_t Reg[32]; //Регистр сдвига для гаммирования.
		Cipher3412 Cipher; //Объект содержащий базовые методы блочного шифрования ГОСТ 3412.
		Gost3413 BlockGost; //Алгоритм гаммирования.

		BlockGost.generateIV(session_key);//Формирую случайное число размером 32байта, которое является сеансовым ключом.
		Cipher.deploymentEncryptionRoundKeys(session_key);//Развертывание секретного ключа.
		
		//Шифрование секретного ключа
		string crypted_session_key; //Шифрованный сеансовый ключ.
		//Шифрую сессионный ключ, асимметричным алгоритмом.
		CryptedSessionKey(session_key, crypted_session_key);
		
		BlockGost.generateIV(Reg);//Формирую случайный начальный вектор(32байта) и помещаю его в регистр 
		//iv должен быть разный для всех файлов! Не менять код!
		 //Шифрую файл
		errStts = CryptFile(paths_to_file[i], dst_Dir, Cipher, Reg, crypted_session_key, user_d, KeyContainer);
		if (!errStts) return false;


		//Посылаю событие прогресса обработки файлов.
		sendEventAllProcessVal(i + 1);				
	}
	
	return true;
}

string CipherWorker::getLastError()
{
	return lastError;
}

void CipherWorker::setData(CipherWorkerData data)
{
	baseData = data;
}

void CipherWorker::run()
{
	//Посылаем событие -поток запущен.
	sendEventThreadStatus(1);//Поток запущен.
	bool res = false;

	if (baseData.getProcessMode() == 1) //Шифрование файлов.
	{
	   res = CryptFiles(baseData.getProcessFiles(), baseData.getPassword(),
	   baseData.getRAsimOpenKey(), baseData.getKeyContainer(), baseData.getSignerIndex(), baseData.getSignatureDH());
	}

	if (baseData.getProcessMode() == 2)//Шифрование файлов.
	{
		res = DecryptFiles(baseData.getProcessFiles(), baseData.getDbPath(), baseData.getPassword(),baseData.getKeyContainer());
	}


	if (!res) //Возникла ошибка.
	{
		string err = getLastError();
		//Посылаю событие.
		eventsCipherWorker *ev = new eventsCipherWorker();													
		ev->setThreadStatus(3);//Поток выполнился с ошибкой.
		ev->setLastError(err);//Сообщение об ошибке.
		QApplication::postEvent(threadEvens, ev);
		return;
	}
	sendEventThreadStatus(2);//Поток успешно завершен.
}

void CipherWorker::sendEventAllProcessVal(int val)
{
	//Создаем объект класса события и высылаем его объекту-получателю с помощью метода postEvent(). 
	eventsCipherWorker *ev = new eventsCipherWorker();//Создание  события в  очень похоже на утечку памяти,
	//но ею не является, так как после обработки все объекты событий удаляются! 
	
	ev->setAllProcessVal(val);
	QApplication::postEvent(threadEvens, ev);
}

void CipherWorker::sendEventProcessFileInfo(string msg)
{
	if (IsDisabledEventProcess) return;

	eventsCipherWorker *ev0 = new eventsCipherWorker();//Создание  события в  очень похоже на утечку памяти,
													   //но ею не является, так как после обработки все объекты событий удаляются! 
	ev0->setCurfileprocessInfo(msg);
	QApplication::postEvent(threadEvens, ev0);
}

void CipherWorker::sendEventThreadStatus(int status)
{
	if (IsDisabledEventProcess) return;
	//Создаем объект класса события и высылаем его объекту-получателю с помощью метода postEvent(). 
	eventsCipherWorker *ev = new eventsCipherWorker();//Создание  события в  очень похоже на утечку памяти,
													  //но ею не является, так как после обработки все объекты событий удаляются! 
	ev->setThreadStatus(status);//Поток запущен.
	QApplication::postEvent(threadEvens, ev);
}

void CipherWorker::sendEventCurFileProcess(int val)
{
	if (IsDisabledEventProcess) return;
	//Прогресс обработки файлов.
	eventsCipherWorker *ev1 = new eventsCipherWorker();
	ev1->setCurFileProcess(val);
	QApplication::postEvent(threadEvens, ev1);//Послал событие.
}

void CipherWorker::sendMaxFileProcess(int val)
{
	if (IsDisabledEventProcess) return;
	//Прогресс обработки файлов.
	//Создаем объект класса события и высылаем его объекту-получателю с помощью метода postEvent(). 
	eventsCipherWorker *ev = new eventsCipherWorker();//Создание  события в  очень похоже на утечку памяти,
													  //но ею не является, так как после обработки все объекты событий удаляются! 
	ev->setMaxFileProcess(val);
	QApplication::postEvent(threadEvens, ev);
}

bool CipherWorker::CryptedSessionKey(uint8_t *session_key, string &result)
{
	//Шифрую
	//Преобразовываю массив подлежащий шифрования в строку 
	std::string mess_to_crypt(reinterpret_cast<char const*>(session_key), 32);

	RSAES_OAEP_SHA_Encryptor e(asymmetricKeyR); //Открытый ключ получателя.
	AutoSeededRandomPool rng1; //Использует ОС для  получения seed
	string crypted_message = "";
	//PK_DecryptorFilter является прокси для фильтра, созданного PK_Encryptor :: CreateEncryptionFilter. 
	//Этот класс обеспечивает симметрию с помощью VerifierFilter.
	StringSource ss1(mess_to_crypt, true, new PK_EncryptorFilter(rng1, e, new StringSink(crypted_message)) // PK_EncryptorFilter
	); // StringSource

	result = crypted_message;

	return false;
}

bool CipherWorker::DecryptEc_SK(Point & Q, mpz_class & d, QString Password, PkContainer &KeyContainer)
{
	try {

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

		string EcOid = KeyContainer.P24().toStdString();
		//Проверка соответствия ключевых пар P24
		if (!CheckEcPair(Q1, d1, EcOid)) return false;

		d = d1; //Передаю закрытый ключ
	}
	catch (...)
	{
		return false;
	}
	return true;
}

bool CipherWorker::CheckEcPair(Point & Q, mpz_class & d, string ecOid)
{
	Groups EC2;  //(ec_name);
	bool findEc = EC2.SetParam(ecOid);
	if (!findEc)
	{
		lastError = "Bad EC param.";
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

//Получает результат хэширования пароля и соли
bool CipherWorker::HashedPassword(QString &Passwd, QString &salt_str, uint8_t *hash)
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
bool CipherWorker::DecryptSK(uint8_t *key, QString &CryptedKey, QString &iv, string &p_key)
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

//Шифрую файл по указанному пути
bool CipherWorker::CryptFile(QString src_path, QString dst_dir, Cipher3412 &Cipher, uint8_t *Reg, string &CryptedKey, 
	mpz_class &user_d, PkContainer &KeyContainer)
{
	QTextCodec *codec = QTextCodec::codecForName("Windows-1251");

	try
	{
		QString Error = "";
		QFileInfo finfo(src_path);
		QString file_name = finfo.fileName();
		//Размер файла в байтах
		qint64 src_file_len = finfo.size(); //Размер исходного файла в байтах

		string mess_progress = "Шифрование файла \"" + codec->fromUnicode(finfo.fileName()) + "\"";
		sendEventProcessFileInfo(mess_progress);

		//НЕТ механизма обработки файлов менее 16 байт!!!!!!!!! При желании можно сделать
		if (src_file_len < 16)
		{
			lastError = "Файл "+codec->fromUnicode(file_name)+ " имеет размер менее 16 байт. Шифрование файла не возможно.";
			return false;
		}


		//Открыть файл для чтения,откуда будут браться данные
		QFile fileIn(src_path);
		if (!fileIn.open(QIODevice::ReadOnly))
		{
			lastError = "Не удалось открыть файл " + codec->fromUnicode(file_name) + " для чтения.";
			return false;
		}
	

		QString dst_path = dst_dir + "/" + file_name + ".crypt";


		//Открыть файл для записи, куда будет записываться шифрованный файл
		QFile fileOut(dst_path);
		//Удалить существующий файл, если он есть-вдруг пользователь повторно нажал шифровать несколько раз
		fileOut.remove();
		
		if (!fileOut.open(QIODevice::ReadWrite))
		{
			lastError = "Не удалось открыть файл " + codec->fromUnicode(file_name) + ".crypt для записи.";
			return false;
		}			


		uint8_t P[16]; //Блок сообщения подлежащего шифрованию
		uint8_t MSB[16];//значением n разрядов регистра сдвига с большими номерами
		uint8_t C_block[16]; //результирующий шифротекст
		uint8_t iv[32]; //Начальный вектор содержащийся в регистре
		uint8_t Flen[16];//Заголовок-размер файла(информационного блока) в виде 16 байт
		char input_block[16]; //Блок данных считываемых из файла подлежащего шифрованию

		qint64 readf_pos = 0;//Текущая позиция считываемого блока из файла
		qint64 writef_pos = 0;//Текущая записываемого блока в файл
		bool GoodOpFlag = 0;//Флаг успешности выполнения операции


		//Формирует заголовок шифруемого файла, записывает его в файл
		if (!WriteCryptFileTitle(fileOut, src_file_len, writef_pos)) return false;
		

		//Устанавливаю позицию в считываемом файле в 0
		if (!fileIn.seek(0))
		{
			lastError = "Не удалось открыть файл " + codec->fromUnicode(file_name) + " для чтения.";
			return false;
		}


		//Режим работы блочного шифра
		Gost3413 CBC;
		memcpy(iv, Reg, 32);//Сохраняю значение регистра в iv

							//Заполнение заголовка длиной файла
		CBC.FileLengthToArray(src_file_len, Flen);
		
		//Шифрование размера файла
		CBC.getMSBfromR(Reg, MSB);//Получаю MSB из регистра
		CBC.сryptIterationCBC(&Cipher, Flen, MSB, C_block);
		//Сохраняю блок шифротекста в файл
		GoodOpFlag = CBC.writeBlockC(writef_pos, C_block, fileOut);
		if (!GoodOpFlag)
		{			
				lastError = "Ошибка 1 Не удалось сохранить файл.";
				return false;	
		}

		//Сдвиг регистра влево
		CBC.RegLeftShift(Reg, C_block);

		//Количество блоков в файле подлежащих шифрованию
		qint64 block_count = src_file_len / 16;
		//!!!!!!!!!!!!!!Предусмотреть ситуацию когда размер файла менее 16 байт!!!!!!!!!!!!!!!!1
		
		//Прогресс обработки файлов.
		 sendMaxFileProcess(block_count);//Посылаю событие о максимальном значении.
				
		int progress_step = 1; //На сколько блоков бить движение прогресс бара
							   //Необходимо для ускорения работы алгоритма-иначе огромное количество  времени тратиться на движения прогресс бара 
		if (block_count > 10000) progress_step = 1000;

		for (int i = 0;i < block_count;i++)
		{
			//Устанавливаю текущую позицию(считываемый файл)
			if (!fileIn.seek(readf_pos))
			{
				lastError = "Ошибка 2 Не удалось считать файл";
				return false;
			}
			

			//Считываю 16 байт из файла
			if (fileIn.read(input_block, 16) != 16)
			{
				lastError = "Ошибка 3 Не удалось считать файл";
				return false;
			}
						
			//Преобразоываю char в uint8_t
			CBC.copyFtoC(input_block, P);
			//Итерация шифрования
			CBC.getMSBfromR(Reg, MSB);//Получаю MSB из регистра
			CBC.сryptIterationCBC(&Cipher, P, MSB, C_block);//шифрую блок

			GoodOpFlag = CBC.writeBlockC(writef_pos, C_block, fileOut);//Запись шифротекста
			if (!GoodOpFlag)
			{
				lastError = "Ошибка 4 Не удалось сохранить файл";
				return false;
			}
		

			//Сдвиг регистра влево
			CBC.RegLeftShift(Reg, C_block);

			readf_pos += 16;
			
			if (i%progress_step == 0)
			{
				sendEventCurFileProcess(i+1); //Посылаю событие прогресса обработки файлов.
			}				

		}

		quint64 last_bytes = src_file_len % 16; //Количество оставшихся байт в файле
												//Если требуется дополнение сообщения
		if (last_bytes > 0)
		{
			//Читаю значение оставшихся байт
			//Считываю x байт из файла
			if (fileIn.read(input_block, last_bytes) != last_bytes)
			{
				lastError = "Ошибка 5 Не удалось считать файл";
				return false;
			}
			

			uint8_t r_len = static_cast<uint8_t>(last_bytes);//Длина оставшегося сообщения
															 //Копирую оставшиеся байты
			CBC.copyFtoC(input_block, P, r_len);
			//Дополнение сообщения
			CBC.paddingMessage(P, r_len);
			//Шифрование последнего блока
			//Итерация шифрования
			CBC.getMSBfromR(Reg, MSB);//Получаю MSB из регистра
			CBC.сryptIterationCBC(&Cipher, P, MSB, C_block);//шифрую блок
															//Запись шифротектса
			GoodOpFlag = CBC.writeBlockC(writef_pos, C_block, fileOut);
			if (!GoodOpFlag)
			{
				lastError = "Ошибка 6 Не удалось сохранить файл";
				return false;
			}

		}

		//Запись вектора iv
		GoodOpFlag = CBC.writeBlockIV(writef_pos, iv, fileOut);
		if (!GoodOpFlag)
		{
			lastError = "Ошибка 7 Не удалось сохранить файл";
			return false;
		}

		fileIn.close();

		//Добавление параметров ассиметричной системы
		AsInterpretator Ai;
		QByteArray AsBlock;
		Ai.Dh_Hash(AsBlock,rHashOkDH); //Хэш открытого ключа получателя
		Ai.CryptedBlockKey(AsBlock, CryptedKey);//Шифрованный сеансовый ключ
		writef_pos = fileOut.write(AsBlock, AsBlock.size());
		if (writef_pos != AsBlock.size())
		{
			lastError = "Ошибка 16: Не удалось записать файл ";
			return false;
		}
		
		mess_progress = "Подпись файла \"" + codec->fromUnicode(finfo.fileName()) + "\"";
		sendEventProcessFileInfo(mess_progress);

		
		qint64 cur_f_pos = fileOut.pos(); //Текущая позиция в файле

		//Размер более 2Гб.   
		if (cur_f_pos >= 1900000000)
		{
			mess_progress = "Файл \"" + codec->fromUnicode(finfo.fileName()) + "\" более 2Гб, подписан не будет.";
			sendEventProcessFileInfo(mess_progress);

		}
		else
		{
			//-----------Подпись файла
			if (!SignFile(fileOut, user_d, KeyContainer))
			{
				lastError = "";
				return false;
			}
		}	
		
		fileOut.close();

	}
	catch (exception& e)
	{
			lastError = "В методе CryptFile() возникло исключение " + string(e.what());
			return false;	
	}

	return true;
}


//Формирует заголовок шифруемого файла, записывает его в файл
bool CipherWorker::WriteCryptFileTitle(QFile &file, qint64 file_len, qint64 &file_pos)
{

	try {
		const uint8_t title1[7] = //Первые 7 байт содержащих что то вроде версии
		{ 0xf9,0xc5, 0xa8, 0xd3, 0x47, 0xb6, 0x3a };

		//Название программы
		 string pgm_title = "GranitK Information security protection system ";

		 //Включен альтернативный режим
		//if(baseData.getRoseMode()) --Удалить весь этот мусор!
			pgm_title = "I'll always have a little red rose in my heart";


		//Формирую заголовок файла
		const  uint8_t title_len = 126;
		uint8_t file_title[title_len];
		uint8_t pos = 0;
		for (uint8_t i = 0;i < 7;i++)
		{
			file_title[pos] = title1[i];
			pos++;
		}
		for (uint8_t i = 0;i < 47;i++)
		{
			file_title[pos] = static_cast<uint8_t>(pgm_title[i]);
			pos++;
		}
		//Хэш
		for (uint8_t i = 0;i < 64;i++)
		{
			file_title[pos] = 0;
			pos++;
		}

		//Преобразовывает длину блока данных в массив из 8 байт
		//Определение размера. Если размер файла кратен 16: 16байт длина, 32 iv
		if (file_len % 16 == 0) file_len = file_len + 16 + 32;
		else
		{
			quint64 tmp1 = file_len % 16;
			file_len = file_len - tmp1;
			//Сообщение дополняется блоком 16 байт  если его длина не кратна 16
			file_len = file_len + 16 + 16 + 32;
		}
		//Преобразование длины сообщения	
		for (uint8_t i = 0;i < 8;i++)
		{
			file_title[pos] = file_len & 0xFF;
			file_len = file_len >> 8;
			pos++;
		}

		//--------------Запись заголовка---------------
		//Устанавливаю позицию в pos
		if (!file.seek(0))
		{
			lastError = "Не удалось создать заголовок файла.";
			return false;
		}
			

		uint8_t l = (uint8_t)file.write(reinterpret_cast<char *>(file_title), title_len);
		if (l != title_len)
		{
			lastError = "Не удалось создать заголовок файла.";
			return false;
		}
			
		file_pos += title_len; //Сдвигаю позицию

	}
	catch (...)
	{
		lastError = "В методе :WriteCryptFileTitle() возникло исключение";
		return false;
	}

	return true;
}

bool CipherWorker::SignFile(QFile & file, mpz_class & user_d, PkContainer &KeyContainer)
{
	try {

		qint64 cur_f_pos = file.pos(); //Текущая позиция в файле
		if (cur_f_pos == 0)
		{
			lastError = "Ошибка Sg1: Не удалось записать файл ";
			return false;
		}
			

		//Устанавливаю текущую позицию после блока хэш файла
		if (!file.seek(118))
		{
			lastError = "Ошибка Sg2: Не удалось подписать файл";
			return false;
		}
		
		
		//Cчитываю все содержимое файла
		QByteArray file_content = file.read(cur_f_pos - 118); //Считываю блок данных  идущий после хэш
		qint64 f_sz = file_content.size();
		if (f_sz != (cur_f_pos - 118))
		{
			lastError = "Ошибка Sg3: Не удалось подписать файл";
			return false;
		}
			
		//Размер сообщения в байтах
		unsigned long long mess_len = static_cast<unsigned long long>(f_sz);

		CreateSign SignGen; //Объект для создания подписи
							//KeyContainer.P16()
		Groups EC2;
		bool findEc = EC2.SetParam(KeyContainer.P24().toStdString());
		if (!findEc)
		{
			lastError = "Ошибка Sg3.1: Не удалось определить параметры кривой.";
			return false;
		}
		


		//Создаю точку с указанными в кривой координатами точки P
		Point P(EC2, 1); //Точка эллиптической кривой 
		Signature Sign;//Подпись
		Sign = SignGen.Sign(user_d, reinterpret_cast<uint8_t *>(file_content.data()), mess_len, P);



		//-----------------Проверка подписи----------------

		//Ключ проверки подписи	
		Point Q1(EC2, 1); //Точка эллиптической кривой 

						  //Проверяю соответствие ключевой пары в контейнере
		mpz_class secret_k = user_d; //Закрытый ключ пользователя
		Point  Q2;
		Q2 = SignGen.generate_public_key(secret_k, P);
		//Значение открытого ключа из контейнера
		string x1 = KeyContainer.P16().toStdString();
		string y1 = KeyContainer.P17().toStdString();
		//Вычисленное значение
		string x2 = Q2.get_hex_x();
		string y2 = Q2.get_hex_y();

		if (x1 != x2)
		{
			lastError = "Ошибка Sg4: Не удалось подписать файл";
			return false;
		}
			
		if (y1 != y2)
		{
			lastError = "Ошибка Sg5: Не удалось подписать файл";
			return false;
		}
			

		//Задаю параметры ключу проверки подписи
		Q1.x.set_str(x1, 16);
		Q1.y.set_str(y1, 16);

		//Проверяю подпись
		VerifySign Vs;
		bool sign_status = Vs.Verify(reinterpret_cast<uint8_t *>(file_content.data()), mess_len, P, Q1, Sign);

		if (sign_status == 0)
		{
				lastError = "Ошибка Sg6: Не удалось подписать файл";
				return false;
		}

		file_content.clear();//Очищает содержимое массива байтов и делает его пустым.

							 //----------------------------------------------
							 //Добавление данных подписи
		AsInterpretator Ai;
		QByteArray AsBlock;
		Ai.Sign_r(AsBlock, Sign.get_hex_r());
		Ai.Sign_s(AsBlock, Sign.get_hex_s());
		//Индекс открытого ключа подписанта
		Ai.SetSignedIndex(AsBlock, signerIndex);


		//Устанавливаю текущую позицию  в конец файла
		if (!file.seek(cur_f_pos))
		{
			lastError = "Ошибка Sg7: Не удалось подписать файл";
			return false;
		}


		//Сохраняю в файл
		cur_f_pos = file.write(AsBlock, AsBlock.size());
		if (cur_f_pos != AsBlock.size())
		{
			lastError = "Ошибка Sg8: Не удалось записать файл ";
			return false;
		}

	}
	catch (exception& e)
	{		
			lastError = "В методе SignFile возникло исключение";
			return false;
	}
	
	return true;
}


//Выполняет операцию расшифрования выбранных файлов
bool CipherWorker::DecryptFiles(QStringList &paths_to_file, QString DbPath, QString UserPassword,
	 PkContainer KeyContainer)
{
	try
	{
		QString Error = "";
		QString dst_Dir = "";//Путь к каталогу куда будут сохраняться расшифрованные файлы						 

		bool IsCreateDir = CreateDstDecryptDir(paths_to_file[0], dst_Dir); //Создаю директорию куда будут сохраняться расшифрованные файлы
		if (!IsCreateDir) return false;
				
		int f_count = paths_to_file.count(); //Количество файлов
		
		//Цикл по всем файлам
		for (int i = 0;i < f_count;i++)
		{
			//Прогресс обработки файлов
			//Посылаю событие прогресса обработки файлов.
		  if(i!=0)sendEventAllProcessVal(i);

		
			//Расшифровую файл
			if (!DecryptingFile(paths_to_file[i], dst_Dir, DbPath, UserPassword, KeyContainer))
				return false;
			
		}

	}
	catch (...)
	{
		lastError = "В методе DecryptFiles возникло исключение.";
		return false;
	}

	return true;
}

//Используя путь к файлу, создает директорию для сохранения расшифрованных файлов, возвращает путь к директории
bool CipherWorker::CreateDstDecryptDir(QString fpath, QString &DirPath)
{
	QFileInfo fi(fpath);
	DirPath = fi.absolutePath(); //encrypted files
	DirPath = DirPath + "/Decrypted_files";
	QDir dir(DirPath);
	if (!dir.exists())
	{
		if (!dir.mkpath("."))  return 0; //Не удалось создать каталог
	}
	else
	{
		/*
		   	Проверка наличия файлов в папке. 
			Это необходимо по двум причинам.
			1. Касперский антивирус при попытки перезаписать точно такой  файл думает что он программа вирус,
			пытается откатить изменения программ. Т.е. если пользователь будет "играться" то антивирус примет такое поведения
			за вирусную программу.
			2. Пользователь может затереть файлы с одинаковыми именами.
		*/

		QStringList files = dir.entryList();
		int readCount = 0; //Количество реальных файлов.
		string FileNames = "";
		bool isFirst=true;

		/*
		  Считаю количество файлов. Так как dir.entryList() кроме имен файлов возвращает еще и
		  "." и "..". Т.е. даже если каталог пуст-все равно будут записи в списке.
		*/
		for (int i = 0;i < files.count();i++)
		{	
			if (files[i] == ".") continue;
			if (files[i] == "..") continue;

			if (!isFirst)FileNames = FileNames + "\r\n";

			//Показать первые 3 файла.
			if(readCount<4)
			FileNames = FileNames + codec->fromUnicode(files[i]).toStdString();

			isFirst = false;
			readCount++;
		}


		if (readCount > 0)
		{
			lastError = "Каталог куда будут расшифрованы файлы " +
				codec->fromUnicode(DirPath).toStdString() + " содержит файлы:\r\n" +
				FileNames + ".\r\n Сначала перенесите или удалите ранее расшифрованные файлы.";
			return false;
		}

	}

	if (!dir.exists()) 
	{
		lastError = "Не удалось создать каталог хранения расшифрованных файлов.";
		return false;
	}

	return 1;
}

bool CipherWorker::DecryptingFile(QString src_path, QString dst_dir, QString DbPath,
	QString UserPassword, PkContainer &KeyContainer)
{
	try
	{
		 sendEventCurFileProcess(0);//Сбрасываю предыдущее значение процесса обработки файла.

		QString Error = "";
		QFileInfo src_file_info(src_path);
		QString src_file_name = src_file_info.fileName(); //Имя исходного файла
		

		//Открыть файл для чтения,откуда будут браться данные
		QFile src_file(src_path);
		if (!src_file.open(QIODevice::ReadOnly))
		{
			lastError = "Ошибка 8: Не удалось открыть файл "+codec->fromUnicode(src_file_name)+ " для чтения.";
			return false;
		}

		

		//Удаляю ".crypt" из имени файла 
		QString file_name = src_file_name.left(src_file_name.length() - 6);
		QString dst_path = dst_dir + "/" + file_name;

		//Открыть файл для записи, куда будет записываться расшифрованный файл
		QFile fileOut(dst_path);
		if (!fileOut.open(QIODevice::WriteOnly))
		{
			lastError = "Ошибка 9: Не удалось открыть файл "+codec->fromUnicode(file_name)+ " для записи.";
			return false;
		}
				

		//Считываю параметры асимметричной системы-блоки данных идущие за шифрованным сообщением
		ASreader ASr;
		if (!ASr.Read(src_file, src_file_info.size()))
		{
			lastError = "Ошибка 9A: Не верный данные в файле " + codec->fromUnicode(file_name);
			return false;
		}
			

		string mess_progress = "Проверка подписи файла \"" + codec->fromUnicode(src_file_name) + "\"";
		sendEventProcessFileInfo(mess_progress);

		qint64 flen_ = src_file_info.size();//Размер файла.

		if (flen_ >= 1900000000)
		{
			string mess_progress = "Файла \"" + codec->fromUnicode(src_file_name) + "\" более 1,9Гб, подписи нет.";
			sendEventProcessFileInfo(mess_progress);
		}
		else
		{
			//Проверка подписи.
			if (!CheckSign(ASr, src_file, DbPath, KeyContainer))
			{
				return false;
			}
		}
		


		//Расшифровка ключа блочного шифра зашифрованного асимметричным алгоритмом.
		uint8_t secretKey[32];
		if(!ASdecryptSecretKey(ASr, secretKey, UserPassword, KeyContainer))
			return false;
		

		Cipher3412 Cipher; //Объект содержащий базовые методы блочного шифрования
		Cipher.deploymentDecryptionRoundKeys(secretKey); //Развертывание секретного ключа  


		uint8_t P[16]; //Блок сообщения подлежащего шифрованию
		uint8_t MSB[16];//значением n разрядов регистра сдвига с большими номерами
		uint8_t C_block[16]; //результирующий шифротекст
		uint8_t tmp_block[16]; //хранит временные данные
		uint8_t Reg[32]; // Регистр размером m = kn =  2*16
		uint8_t Flen[16];//Заголовок-размер файла в виде 16 байт
		char input_block[16]; //Блок данных считываемых из файла подлежащего расшифрованию
		uint8_t iv[32]; //Начальный вектор считанный из файла
		qint64 block_count;// = file_len / 16; //Количество блоков содержащихся в сообщении

		qint64 readf_pos = 0;//Текущая позиция считываемого блока из файла
		qint64 writef_pos = 0;//Текущая записываемого блока в файл
		bool GoodOpFlag = 0;//Флаг успешности выполнения операции
		qint64 src_file_len = 0;//Длина расшифровуемого файла
		qint64 data_len = 0;//Длина блока данных 
		uint8_t  r_len = 0;//Остаток в последнем блоке

						   //Режим работы блочного шифра
		Gost3413 CBC;

		//Считать значение начального вектора IV
		//src_file_len = src_file_info.size(); //Размер файла
		//readf_pos = src_file_len - 32;//Позиция где начинается iv

		//Считывает значение вектора iv по указанной позиции
		GoodOpFlag = CBC.readIV(iv, src_file);
		if (!GoodOpFlag)
		{
			lastError = "Ошибка 10: Не удалось считать файл "+codec->fromUnicode(src_file_name);
			return false;
		}
		

		//Cчитываю первые 16 байт, длина сообщения
		readf_pos = 118 + 8;//Установка на начало блока данных
		GoodOpFlag = CBC.readFblock(readf_pos, input_block, src_file);
		if (!GoodOpFlag)
		{
			lastError = "Ошибка 11: Не удалось считать файл "+ codec->fromUnicode(src_file_name);
			return false;
		}
	


		//Преобразовываю данные
		CBC.copyFtoC(input_block, C_block);
		//Заполнение регистра данными IV
		memcpy(Reg, iv, 32);

		//Пытаюсь расшифровать длину сообщения
		CBC.getMSBfromR(Reg, MSB);//Получаю старшие байты MSB из регистра
		memcpy(tmp_block, C_block, 16);//Копирую С ,так как оно измениться
		CBC.deсryptIterationCBC(&Cipher, P, MSB, C_block);//Расшифровываю
														  //Сдвиг регистра влево, помещая в регистр шифрованное сообщение
		CBC.RegLeftShift(Reg, tmp_block);
		//Преобразовываю массив в длину сообщения
		CBC.ArrayToFileLength(data_len, P); //Фактически data_len-длина зашифрованного файла
											//Добавить проверку размера данных!!!!!!

											//Длину файла мы обработали 16, IV считали IV
		block_count = data_len / 16; //Количество блоков в сообщении

									 //Прогресс обработки файлов.
		sendMaxFileProcess(block_count);//Посылаю событие о максимальном значении.

		int progress_step = 1; //На сколько блоков бить движение прогресс бара
							   //Необходимо для ускорения работы алгоритма-иначе огромное количество  времени тратиться на движения прогресс бара
							   //При 200мб это сильно ощущается
		if (block_count > 10000) progress_step = 1000;
		
		 mess_progress = "Расшифровывание файла \"" + codec->fromUnicode(src_file_name) + "\"";
		sendEventProcessFileInfo(mess_progress);

		//Цикл расшифровывания блоков										   
		for (qint64 i = 0;i < block_count;i++)
		{
			//Считываю очередной блок
			GoodOpFlag = CBC.readFblock(readf_pos, input_block, src_file);
			if (!GoodOpFlag)
			{
				lastError = "Ошибка 12: Не удалось считать файл "+codec->fromUnicode(src_file_name);
				return false;
			}
			

			CBC.copyFtoC(input_block, C_block); //Преобразовываю данные

			CBC.getMSBfromR(Reg, MSB);//Получаю старшие байты MSB из регистра
									  //Копирую блок данных, так как измениться содержимое C_block
			memcpy(tmp_block, C_block, 16);
			CBC.deсryptIterationCBC(&Cipher, P, MSB, C_block);//Расшифровываю

			GoodOpFlag = CBC.writeBlockC(writef_pos, P, fileOut); //Записываю расшифрованный блок в файл
			if (!GoodOpFlag)
			{
				lastError = "Ошибка 13: Не удалось записать файл ";
				return false;
			}
				

			//Сдвиг регистра влево, помещая в регистр шифрованное сообщение
			CBC.RegLeftShift(Reg, tmp_block);

			if (i%progress_step == 0) sendEventCurFileProcess(i+1);//Посылаю событие прогресса обработки файла. 
		}

		//Кратна ли длина сообщения размеру блока шифра?
		r_len = static_cast<uint8_t>(data_len % 16);
		//Длина сообщения не кратна размеру блока шифра
		if (r_len != 0)
		{
			//Считываю очередной блок
			GoodOpFlag = CBC.readFblock(readf_pos, input_block, src_file);
			if (!GoodOpFlag)
			{
				lastError = "Ошибка 14: Не удалось считать файл ";
				return false;
			}
				

			CBC.copyFtoC(input_block, C_block); //Преобразовываю данные
			CBC.getMSBfromR(Reg, MSB);//Получаю старшие байты MSB из регистра				  
			memcpy(tmp_block, C_block, 16); //Копирую блок данных, так как измениться содержимое C_block
			CBC.deсryptIterationCBC(&Cipher, P, MSB, C_block);//Расшифровываю

			GoodOpFlag = CBC.writeBlock(writef_pos, P, fileOut, r_len);//Записываю расшифрованный блок в файл
			if (!GoodOpFlag)
			{
				lastError = "Ошибка 15: Не удалось записать файл ";
				return false;
			}
				
		}
		src_file.close(); //Закрыл шифрованный файл.

		fileOut.close();
	}
	catch (...)
	{
		lastError = "В методе DecryptingFile() возникло исключение";
		return false;
	}

	return true;
}



bool CipherWorker::CheckSign(ASreader & ASr, QFile & file, QString DbPath, PkContainer &KeyContainer)
{
	try {
		QString Error = "";
		QByteArray s, r; //Вектора подписи полученные из файла
		string  k_index;
		string Qx, Qy; //Координаты открытого ключа в 16 ричной форме
		QString FIO;


		//Получение векторов подписи из блоков 116,117
		if (!ASr.GetSignVector(r, s)) //Не удалось получить какой либо  вектор из блоков
		{
			lastError = "Ошибка Пр0:В файле отсутствуют блоки подписи. ";
			return false;
		}
		

		//Получения "индекса" открытого ключа  подписанта из файла
		if (!ASr.GetSignKeyIndex(k_index))
		{
			lastError = "Ошибка Пр1:В файле отсутствуют сведения об открытом ключе проверки подписи.";
			return false;
		}
		

		//Отключен простой режим.
		if (!baseData.getRoseMode())
		{
			//Открывает соединение с базой данных
			GranitDbI DBI(DbPath);
			if (!DBI.OpenDb())
			{
				lastError = "Не удалось открыть базу данных.";
				return false;
			}


			//Проверяю срок действия БД.
			if (!DBI.checkDbDateEnd())
			{
				lastError = "Ошибка: Истек срок действия БД СКЗИ.";
				return false;
			}


			//Используя индекс получаю открытый ключ подписанта из БД и его ФИО
			if (!DBI.getSignOpenKey(k_index, Qx, Qy, FIO))
			{
				lastError = "Error";
				return false;
			}
		}
		else
		{
			Qx=KeyContainer.P16().toStdString(); //открытый ключ  координата X
			Qy=KeyContainer.P17().toStdString(); //открытый ключ  координата Y
		}
					
		//-----------------Получение данных для подписи------------------	

		//Устанавливаю текущую позицию после блока хэш файла
		if (!file.seek(118))
		{
			lastError = "Ошибка Vs2: Не удалось проверить подпись файла.";
			return false;
		}
	

		qint64 end_sign_data_pos; //Позиция начала векторов блока подписи,и конца подписанных данных
		end_sign_data_pos = ASr.begin_sign_block;
		end_sign_data_pos = end_sign_data_pos - 118; //Количество данных которые необходимо считать из файла

													 //Считываю все подписанное содержимое файла
		QByteArray file_content = file.read(end_sign_data_pos);
		qint64 f_sz = file_content.size(); //Размер считанного блока данных

		if (f_sz != end_sign_data_pos)
		{
			lastError = "Ошибка Vs3: Не удалось проверить подпись файла";
			return false;
		}
			
		//Размер сообщения в байтах
		unsigned long long mess_len = static_cast<unsigned long long>(f_sz);

		//Преобразование векторов QByteArray в mpz_class
		mpz_class r_;
		mpz_class s_;

		size_t r_len = static_cast<size_t>(r.size());
		size_t s_len = static_cast<size_t>(s.size());

		mpz_import(r_.get_mpz_t(), r_len, 1, 1, 0, 0, reinterpret_cast<uint8_t *>(r.data()));
		mpz_import(s_.get_mpz_t(), s_len, 1, 1, 0, 0, reinterpret_cast<uint8_t *>(s.data()));

		Signature Sign; //Объект содержащий подпись файла
		Sign.set_r(r_);
		Sign.set_s(s_);
		Sign.set_type(1); //512бит

		Groups EC2;
		bool FindEc = EC2.SetParam(KeyContainer.P24().toStdString()); //Задаю параметры кривой на основание OID.
		if (!FindEc)
		{
			lastError = "Не верный параметр кривой.";
			return false;
		}
			

		//Создаю точку с указанными в кривой координатами точки P
		Point P2(EC2, 1); //Точка эллиптической кривой 
		Point Q1(EC2, 1); //Точка эллиптической кривой 

						  //Устанавливаю координаты открытого ключа
		Q1.x.set_str(Qx, 16);
		Q1.y.set_str(Qy, 16);


		//Проверяю подпись
		VerifySign Vs;
		bool sign_status = Vs.Verify(reinterpret_cast<uint8_t *>(file_content.data()), mess_len, P2, Q1, Sign);

		if (sign_status == 0)
		{
			lastError = "Не верная подпись файла. Файл был поврежден или изменен третьим лицом";
			return false;
		}
		


	}
	catch (exception& e)
	{
  	  lastError = "В методе CheckSign возникло исключение" +string(e.what());
	  return false;
	}

	return true;
}

//Расшифровка ключа блочного шифра
bool CipherWorker::ASdecryptSecretKey(ASreader &Data, uint8_t *key, QString Password, PkContainer &KeyContainer)
{
	try {

		RSA::PublicKey publicKey;
		RSA::PrivateKey secret_key;

		//Расшифровываю секретный ключ ДХ. Пытаюсь его загрузить, если пароль не верный -ключ не возможно будет загрузить 
		if (!DecryptDH_SK(publicKey, secret_key, Password,KeyContainer))
		{
			lastError = "Ошибка AS1: Не верный пароль.";
			return false;
		}
		


		//Читаю шифрованное значение ключа блочного шифра
		string crypted_message;
		//string reciver_hash; //Хэшированное значение открытого ключа получателя
		//Цикл по всем блокам 
		bool found = 0;
		for (int i = 0;i < Data.Blocks.count();i++)
		{
			//Если найден блок содержащий шифрованный сеансовый ключ
			if (Data.Blocks[i].type == 115)
			{
				//Преобразовываю в строку
				string t1(Data.Blocks[i].data.data(), Data.Blocks[i].data.size());
				crypted_message = t1;
				found = 1;
				break;
			}
		}
		//Блок не найден
		if (found == 0)
		{
			lastError = "Ошибка AC2: Отсутствует блок данных.";
			return false;
		}
		


		//Расшифровываю	
		string decrypted_message;
		RSAES_OAEP_SHA_Decryptor decryptor(secret_key);
		AutoSeededRandomPool rng2; //Использует ОС для  получения seed
								   //В случае если не удалось расшифровать сообщение возникает исключение
		try
		{
			StringSource ss2(crypted_message, true, new PK_DecryptorFilter(rng2, decryptor, new StringSink(decrypted_message)
			) // PK_DecryptorFilter
			); // StringSource
		}
		catch (...)
		{
			lastError = "Ошибка AS3: Не удалось расшифровать сеансовый ключ. Возможно файл был предназначен не вам.";
		    return false;
			
		}


		//Сеансовый ключ должен состоять из 32-x байт
		if (decrypted_message.length() != 32)
		{
			lastError = "Ошибка AS4: Сбой асимметричного шифрования.";
			return false;
		}

		//Копирую данные в массив
		for (uint8_t i = 0;i < 32;i++)
		{
			key[i] = static_cast<uint8_t>(decrypted_message[i]);
		}


	}
	catch (exception& e)
	{
		lastError = "В методе  ASdecryptSecretKey возникло исключение: "+string(e.what());
		return false;
	}

	return true;
}

//Расшифровывает закрытый ключ схемы Диффи-Хэлмана
bool CipherWorker::DecryptDH_SK(RSA::PublicKey &publicKey, RSA::PrivateKey &secret_key, QString Password, PkContainer &KeyContainer)
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

//Проверяет пару ключей схемы Диффи-Хелмана. Если пара не совпадает возвращает 0
bool CipherWorker::GetDhPair(QString &open_key, string &private_key, RSA::PublicKey &publicKey, RSA::PrivateKey &secret_key)
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
		lastError = "ОШИБКА K531D";
		return false;
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


	return true;
}
