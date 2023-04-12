#include "RequestReader.h"

//Константа защиты ключевого контейнера.
const string RequestReader::KeyChipperConstant = "17dfbfc9acfa787e242d75c7f0764bfd83e79eef08d4581a881527d92dbad1d4";

RequestReader::RequestReader()
{
}

RequestReader::~RequestReader()
{
}

bool RequestReader::Read(QString path, mRequest & Req)
{
	QByteArray fileContent;//Содержимое файла.
	QByteArray unpuckContent;//Распакованное содержимое файла.

	fileContent.clear();//Очищает содержимое массива байтов и делает его пустым.
	unpuckContent.clear();

	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
	{
		//QTextCodec *StrCodec;
		lastError = "Не удалось открыть файл макета по пути :";
		//return StrCodec->toUnicode() + FilePath;
	}

	fileContent = file.readAll();//Читаю содержимое файла
	file.close();

	//Распаковать контейнер ключа
	bool errorFlag = UnpackMaket(fileContent, unpuckContent);
	if (!errorFlag) return false;

	//Преобразовываю байт массив в объект
	

	SerializableTools Srt;
	Srt.Deserialize(&unpuckContent,&Req);
		
	return true;
}

bool RequestReader::checkRequest(mRequest & Req)
{	
	if (Req.P0().trimmed().isEmpty())
	{
		lastError = "Не верный запрос:Пустое поле Фамилия.";
		return false;
	}
	
	if (Req.P1().trimmed().isEmpty())
	{
		lastError = "Не верный запрос:Пустое поле Имя.";
		return false;
	}

	if (Req.P2().trimmed().isEmpty())
	{
		lastError = "Не верный запрос:Пустое поле Отчество.";
		return false;
	}

	if (Req.P3().trimmed().isEmpty())
	{
		lastError = "Не верный запрос:Пустое поле Подразделение.";
		return false;
	}

	if (Req.P4().trimmed().isEmpty())
	{
		lastError = "Не верный запрос:Пустое поле Наименование организации.";
		return false;
	}

	if (Req.P5().trimmed().isEmpty())
	{
		lastError = "Не верный запрос:Пустое поле ЕГРЮЛ.";
		return false;
	}

	if (Req.P6().trimmed().isEmpty())
	{
		lastError = "Не верный запрос:Пустое поле Район.";
		return false;
	}

	if (Req.P7().trimmed().isEmpty())
	{
		lastError = "Не верный запрос:Пустое поле Город.";
		return false;
	}

	if (Req.P8().trimmed().isEmpty())
	{
		lastError = "Не верный запрос:Пустое поле Адрес.";
		return false;
	}

	if (Req.P9().trimmed().isEmpty())
	{
		lastError = "Не верный запрос:Пустое поле Контактный телефон.";
		return false;
	}

	if (Req.P10().trimmed().isEmpty())
	{
		lastError = "Не верный запрос:Пустое поле Электронная почта";
		return false;
	}

	if (Req.P12().trimmed().isEmpty())
	{
		lastError = "Не верный запрос:Пустое поле Открытый ключ ДХ.";
		return false;
	}

	if (Req.P14().trimmed().isEmpty())
	{
		lastError = "Не верный запрос:Пустое поле открытый ключ, координата Х.";
		return false;
	}

	if (Req.P15().trimmed().isEmpty())
	{
		lastError = "Не верный запрос:Пустое поле открытый ключ, координата Y.";
		return false;
	}

	if (Req.P16().trimmed().isEmpty())
	{
		lastError = "Не верный запрос:Пустое поле алгоритм открытого ключа.";
		return false;
	}

	if (Req.P17().trimmed().isEmpty())
	{
		lastError = "Не верный запрос:Пустое поле параметры эллиптической кривой.";
		return false;
	}
	

	return true;
}

string RequestReader::getLastError()
{
	return  lastError;
}

//Распаковывает макет ключа из src в dst
bool RequestReader::UnpackMaket(QByteArray &src, QByteArray &dst)
{
	try
	{
		//Проверка длины файла длина должна быть 1+64+16+min16 [версия протокола 1][hash 64байта][iv 16] [сообщение мин 16]
		if ((src.length() < 97) || (src.length() > 99999))
		{
			lastError = "Неверный формат файла запроса.";
			return false;
		}

		//[версия протокола 1][hash 64байта][iv 16] [сообщение мин 16]

		//Проверка формата макета
		unsigned char format = src[0];
		if (format != 1)
		{
			lastError = "Неверный формат файла запроса.";
			return false;
		}

		//Считываю хэш 512бит
		unsigned char file_hash[64];
		int pos = 0;
		for (int i = 1;i < 65;i++)
		{
			file_hash[pos] = (unsigned char)src[i];
			pos++;
		}

		//Считываю сообщение  макета ключа
		int maket_content_len = src.size() - 65;
		unsigned char *maket_content = new unsigned char[maket_content_len];

		pos = 0;
		for (int i = 65;i < src.length();i++)
		{
			maket_content[pos] = (unsigned char)src[i];
			pos++;
		}

		//Проверяю хэш файла
		unsigned char hash512[64];
		int messlen = maket_content_len * 8;//Длина сообщения в битах
		Hash3411 H;
		H.hash_512(maket_content, (unsigned long long) messlen, hash512);

		//Сравнение хэш файла и хэш сообщения
		bool GoodHash = true;
		for (int i = 0;i < 64;i++)
		{
			if (file_hash[i] != hash512[i])
			{
				GoodHash = false;
				break;
			}
		}

		if (GoodHash == false)
		{
			delete[]maket_content; maket_content = NULL;

			lastError="Неверная контрольная сумма файла макета. Файл поврежден или был изменен третьим лицом.";
			return false;
		}

		//Копирую сообщение в ByteBlock
		//Извлекаю iv и шифрованное сообщение
		unsigned char iv[16];
		for (int i = 0;i < 16;i++) iv[i] = maket_content[i];

		int cr_message_len = maket_content_len - 16;
		unsigned char *cr_message = new unsigned char[cr_message_len];

		pos = 0;
		for (int i = 16;i < maket_content_len;i++)
		{
			cr_message[pos] = maket_content[i];
			pos++;
		}

		//Преобразовываю шифрованное сообщение в ByteBlock
		ByteBlock crypted_message(cr_message, cr_message_len);

		delete[]maket_content; maket_content = NULL;
		delete[]cr_message;cr_message = NULL;


		bool isErr = false;
		string Err = "";
		//Получаю ключ упаковки макета.
		ByteBlock cr_key = Utillity::hex_to_bytes(KeyChipperConstant);

		//Распаковываю макет.		
		Sib3412 K(cr_key);
		ByteBlock decrypt_result;
		//Преобразовываю вектор инициализации
		ByteBlock init_vector(iv, 16);

		K.CFB_Decrypt(crypted_message, decrypt_result, init_vector);

		//Преобразовываю данные в байт массив
		unsigned char *xptr = decrypt_result.byte_ptr();

		QByteArray byte_array(reinterpret_cast<char *>(xptr), decrypt_result.size());

		//Проверяю что там внутри
		QString maket_str = QString::fromUtf8(byte_array);
		QString maket_head = maket_str.mid(39, 10); //<mRequest>
		maket_head = maket_head.trimmed();

		//<?xml version="1.0" encoding="UTF-8"?><mRequest>
		if (maket_head != "<mRequest>")
		{
			lastError = "Неверный контент запроса.";
			return false;
		}

		dst = byte_array;

	}
	catch (...)
	{
		lastError = "В методе UnpackMaket() возникло исключение:";
		return false;
	}

	return true;
}