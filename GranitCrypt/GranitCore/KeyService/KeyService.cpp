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
	//�������� ������������� �����
	QFileInfo f_info(PathToSecretKey);
	string alarm_mess = "";
	if (!f_info.exists())
	{
		QTextCodec *StrCodec;
		StrCodec = QTextCodec::codecForName("Windows-1251"); //��������� ���������

		lastError = "������ �0:�� ������� ����� ���� ����� �� ���������� ����: " + 
			StrCodec->fromUnicode(PathToSecretKey) +
			". ��������� ��� ���� ���������� �������� � ���������.";
		return false;
	}
		

	//�������� ���������� ����� � ��������� �����
	//�������� ��������� ��������� ����� � ������ ����� ������������ � ������ �������� ����, ������� ����� ������� ��� ����
	uint32_t conteiner_length = 0; //������ ���������� �����
	if (!CheckSK(PathToSecretKey, conteiner_length)) return false;

	//�������� ��������� ���������� �����
	QFile file(PathToSecretKey);
	if (!file.open(QIODevice::ReadOnly))
	{
		lastError = "������ �1:�� ������� ������� ���� ���������� �����.";
		return false;
	}


	QByteArray Content; //������ � ������� ���������� ��������� ������

						//������������ � ������� ������ ����� ������ ����������
	if (!file.seek(Key_title_len))
	{
		lastError = "������ �2:��  ������� ������� ���� ���������� �����.";
		file.close();
		return false;
	}

	//������ ���������� �����
	Content = file.read(conteiner_length);
	if (Content.size() == 0)
	{
		lastError = "������ �3:�� ������� ������� ���� ���������� �����.";
		file.close();
		return false;
	}

	//����� ������ ����� iv(16) � ����� �������� ����� ������(4)
	if (Content.size() < 20)
	{
		lastError = "������ �4:�� ������  ���� ���������� �����.";
		file.close();
		return false;
	}

	file.close();

	//������������� ��������� ���������� ����� � �������� ��� ���������, � ������� ������ =0
	QByteArray Result;
	int ret = UnpackSKContainer(Content, Result);
	if (ret != 1) //�������� ������.
	{
		switch (ret)
		{
		case 2:
			lastError = "������ �5.1:�� ������� ������� ����������.";
			return false;
		case 3:
			lastError = "������ �5.2:�� ������� ��������� ������� �� ����������.";
			return false;
		case 4:
			lastError = "������ �5.3:�� ������ �������  ����������.";
			return false;
		case 5:
			lastError = "������ �5:�� ������  ���� ���������� �����.";
			return false;
		default:
			break;
		}

	}

	//�������������� ��������� � ������
	KeyContainerIsLoad = 0;
	if (!DeserializeKeyContainer(&Result, &KeyContainer))
	{
		lastError = "������ �6:�� ������  ���� ���������� �����.";
		return false;
	}
	KeyContainerIsLoad = 1; //��������� ��� ������� ������

	return true;
}


/*
��������� ��������� ����, � ������ ������ ���������� 0
�������� �������������. ����������� ������������ ��������� � ����������� ����� ���������� �����
���������� ������ ���������� ����� container_length
*/
bool KeyService::CheckSK(QString PathToSK, uint32_t &container_length)
{
	QFileInfo file_info(PathToSK);
	qint64 f_size = file_info.size();
	/*
	������ ����� �� ������� �������� ��� ������ ����� ������ ���������
	*/

	if (f_size == 0)
	{
		lastError = "������ �0:�� ������� ������� ���� ���������� �����.";
		return false;
	}

	if (f_size < Key_title_len)
	{
		lastError = "������ �1:�� ���������� ������ ����� ���������� �����.";
		return false;
	}

	//������� ������ ���������� ���������� �����, � ��������� ��� ������
	QFile file(PathToSK);
	if (!file.open(QIODevice::ReadOnly))
	{
		lastError = "������ �2:�� ������� ������� ���� ���������� �����.";
		return false;
	}

	qint64 r_len;//������ ���������� ����� ������
	uint8_t title[Key_title_len]; //��������� �����
	r_len = file.read(reinterpret_cast<char*>(title), Key_title_len);
	if (r_len != Key_title_len)
	{
		lastError = "������ �3:�� ������� ������� ���� ���������� �����.";
		file.close();
		return false;
	}

	//-------------�������� ������������ ���������----------
	const uint8_t title1[7] = //������ 7 ���� ���������� ��� �� ����� ������
	{ 0xf9,0xc5, 0xa8, 0xd3, 0x47, 0xb6, 0x3a };

	//�������� ���������
	string pgm_title;
	if (!enableRose)
		pgm_title = "GranitK Information security protection system ";
	else
		pgm_title = "Little Rose radials security protection system ";

	uint8_t pos = 0;
	//[7-���� ��������� ����������]
	for (uint8_t i = 0;i < 7;i++)
	{
		if (title1[i] != title[pos])
		{
			lastError = "������ �4:���� ���������� �����, �� �������� ������ ���������� ����� ��� \"�������\".";
			file.close();
			return false;
		}

		pos++;
	}
	//[��������� 47����]
	for (uint8_t i = 0;i < 47;i++)
	{

		if (title[pos] != static_cast<uint8_t>(pgm_title[i]))
		{
			lastError = "������ �5:���� ���������� �����, �� �������� ������ ���������� ����� ��� \"�������\".";
			file.close();
			return false;
		}
		pos++;
	}

	//---------�������� ����� ����� ����������
	uint32_t conteiner_len = 0, tmp1 = 0; //����� ���������� �����
	pos = Key_title_len - 4;
	for (uint8_t i = 0;i < 4;i++)
	{
		tmp1 = title[pos];
		conteiner_len |= tmp1 << i * 8;
		pos++;
	}
	//����� ����� �� ����� ���� ����� ��� ���������+���������
	qint64 key_len = Key_title_len + static_cast<qint64>(conteiner_len);
	if (f_size < key_len)
	{
		lastError = "������ �6:�� ������ ������ ����� ���������� �����.";
		file.close();
		return false;
	}

	//-------------���������� ��� ����������	
	//�������� ���������
	//������������ � ������� ������ ����� ������ ����������
	if (!file.seek(Key_title_len))
	{
		lastError = "������ �7:��  ������� ������� ���� ���������� �����.";
		file.close();
		return false;
	}

	//-------������ ����������
	uint8_t *data_block = new uint8_t[conteiner_len];
	//������
	r_len = file.read(reinterpret_cast<char*>(data_block), static_cast<quint64>(conteiner_len));
	if (r_len != static_cast<quint64>(conteiner_len))
	{
		lastError = "������ �8:��  ������� ������� ���� ���������� �����.";
		file.close();
		delete[]data_block;data_block = NULL;
		return false;
	}


	uint8_t hash256[32];
	Hash3411 H;
	unsigned long long  mess_len, tmp; //unsigned __int64
	tmp = static_cast<unsigned long long>(conteiner_len);
	mess_len = tmp * 8; //����� ��������� � �����
	H.hash_256(data_block, mess_len, hash256);

	delete[]data_block;data_block = NULL;

	//�������� ���
	pos = 54; //������ ���
	for (uint8_t i = 0;i < 32;i++)
	{
		if (title[pos] != hash256[i])
		{
			lastError = "������ �9:�� ������ ����������� ����� �����. ���� ��������� ��� ��� ������� ������� �����.";
			file.close();
			return false;
		}
		pos++;
	}
	//�����
	file.close();
	container_length = conteiner_len;

	return true;
}



//�������� �������� ����� ������������ ����������. � ������ ������ HasErr=1;Err-�������� ��������� �� ������.
ByteBlock  KeyService::GetPackKv(string & Err, bool & HasErr)
{
	HMODULE hLib;
	hLib = LoadLibraryA("libCn3q.dll");
	if (hLib == 0)
	{
		Err = "������ ��1:�� ������� ��������� ����������.";
		HasErr = 1;
		return NULL;
	}

	typedef int(*AreApisANSI)(char *Linear, int *Len, char *Value); //��� ������ ��� �������� ���������� �����.


	AreApisANSI areApis = (AreApisANSI)GetProcAddress(hLib, "AreApisANSI"); //������� ����� ������� � ����������.
	if (areApis == 0)  //�� ������� ��������� �������.
	{
		Err = "������ ��2:�� ������� ��������� ������� �� ����������.";
		HasErr = 1;
		return NULL;
	}


	char LinearA[128];
	char ValueA[300] = "t9034l9ff0--s78";
	int aLen = 537;
	int retA = areApis(LinearA, &aLen, ValueA); //������� �������.
	if (retA != 16537) //�������� ������� � dll.
	{
		Err = "������ ��3:������ ������� ����������.";
		HasErr = 1;
		return NULL;
	}

	string lA(LinearA);
	ByteBlock cr_key = Utillity::hex_to_bytes(lA); //Crypt_Key_For_Container
	bool fret = FreeLibrary(hLib);
	if (fret == 0)
	{
		Err = "������ ��4:������ ���������� ����������.";
		HasErr = 1;
		return NULL;
	}

	return cr_key;
}



//�������������� ��������� ���������� ����� � �������� ��� ���������, � ������� ������ =0
int KeyService::UnpackSKContainer(QByteArray &C, QByteArray &Result)
{

	uint8_t *data_ptr; //��������� �� ������ ������ � ���������� �
	char *c_ptr;
	c_ptr = C.data();
	data_ptr = reinterpret_cast<uint8_t *>(c_ptr);

	ByteBlock init_vector(data_ptr, 16); //������ �������������

	uint32_t data_len = C.size() - 20; //������ ���������
	data_ptr = data_ptr + 20; //������������ ��������� �� ������ ����� ������
	ByteBlock crypted_message(data_ptr, data_len); //����������� ���������		
												   //���� ������� ��������� ���������

	HMODULE hLib;
	hLib = LoadLibraryA("libCn3q.dll");
	if (hLib == 0) return 2; //�� ������� ��������� ����������.
	AreApisANSI areApis = (AreApisANSI)GetProcAddress(hLib, "AreApisANSI");
	if (areApis == 0) return 3; //�� ������� ��������� �������.

	char LinearA[128];
	char ValueA[300] = "t9034l9ff0--s78";
	int aLen = 537;
	int retA = areApis(LinearA, &aLen, ValueA);
	if (retA != 16537) return 4; //�������� ������� � dll.

	string lA(LinearA);
	FreeLibrary(hLib);

	ByteBlock cr_key = Utillity::hex_to_bytes(lA); //Crypt_Key_For_Container

	Sib3412 K(cr_key);
	ByteBlock decrypt_result;
	//�������������
	K.CFB_Decrypt(crypted_message, decrypt_result, init_vector);
	//�������������� ������ � ���� ������
	unsigned char *xptr = decrypt_result.byte_ptr();
	QByteArray byte_array(reinterpret_cast<char *>(xptr), decrypt_result.size());

	//�������� ��� ��� ������
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

	uint8_t *data_ptr; //��������� �� ������ ������ � ���������� �
	char *c_ptr;
	c_ptr = C.data();
	data_ptr = reinterpret_cast<uint8_t *>(c_ptr);

	ByteBlock init_vector(data_ptr, 16); //������ �������������

	uint32_t data_len = C.size() - 20; //������ ���������
	data_ptr = data_ptr + 20; //������������ ��������� �� ������ ����� ������
	ByteBlock crypted_message(data_ptr, data_len); //����������� ���������
	string k = FakeCryptKey::GetKey(100);
												   //���� ������� ��������� ���������
	//ByteBlock cr_key = Utillity::hex_to_bytes(Crypt_Key_For_Container);
	ByteBlock cr_key = Utillity::hex_to_bytes(k.c_str());

	Kuznyechik2 K(cr_key);
	ByteBlock decrypt_result;
	//�������������
	K.CFB_Decrypt(crypted_message, decrypt_result, init_vector);
	//�������������� ������ � ���� ������
	unsigned char *xptr = decrypt_result.byte_ptr();
	QByteArray byte_array(reinterpret_cast<char *>(xptr), decrypt_result.size());

	//�������� ��� ��� ������
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

//������������� ���������
bool KeyService::DeserializeKeyContainer(QByteArray *Maket, QObject *object)
{
	QDomDocument doc;

	QString         errorString;
	int             errorLine;
	int             errorColumn;

	if (doc.setContent(*Maket, false, &errorString, &errorLine, &errorColumn))
	{
		QDomElement root = doc.documentElement();
		QString value;//�������� ����
					  //���� �� ��������� ������	
		for (int i = 0;i < object->metaObject()->propertyCount();i++)
		{
			QMetaProperty prop = object->metaObject()->property(i);
			QString propName = prop.name();
			if (propName == "objectName") continue;

			//������ ���� � �����
			for (int i = 0; i < root.childNodes().count(); i++)
			{
				QDomElement node = root.childNodes().at(i).toElement();
				//����� ���� � ������� ����������
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

//��������� ������
bool KeyService::CheckPassword(QString psw)
{
	RSA::PublicKey publicKey;
	RSA::PrivateKey secret_key;

	//������������� ��������� ���� ��. ������� ��� ���������, ���� ������ �� ������ -���� �� �������� ����� ��������� 
	if (!DecryptDH_SK(psw, publicKey, secret_key))
	{
		lastError = "�� ������ ������.";
		return false;
	}

	//������������� � �������� �������� ���� �������
	Point  Q;
	mpz_class d;
	if (!DecryptEc_SK(psw, Q, d))
	{
		lastError = "�� ������ ������.";
		return false;
	}


	//�������� ��������� ����.
	Skd.SetSk(d);
	//Param.IsLoadSk = true; //������ ��� ��������,��������� ���� ��� ������.
	  //�������
	//������� ���� �������� 512����.
	for (int i = 0;i < 512;i++)
	{
		d >>= 1;
	}
	d = 33554432;

	//������� ������
	QString cl;
	for (int i = 0;i < psw.length();i++)
	{
		psw[i] = 't';
	}

	return true;
}

//���������� �������� ����
Point KeyService::GetPk()
{
	return	pubECkey;
}

//���������� �������� ����
mpz_class KeyService::GetPrvEcKey()
{
	return prvEcKey;
}

string KeyService::GetCertAlgorithmIdentifier()
{
	string val;
	val = KeyContainer.P23().toStdString();	//SignCertAlgoritm-�������� ������� �����������
	return val;
}

string KeyService::GetPublicKeyAlgoritm()
{
	string val;
	val = KeyContainer.P22().toStdString();//PublicKeyAlgoritm-�������� ��������� �����
	return val;
}

string KeyService::GetEcParam()
{
	string val;
	val = KeyContainer.P24().toStdString();	//EcOid-��������� ������������� ������
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


//�������������� �������� ���� ����� �����-�������
bool KeyService::DecryptDH_SK(QString Password, RSA::PublicKey &publicKey, RSA::PrivateKey &secret_key)
{
	uint8_t hash256[32];
	QString hex_salt; //���� � 16 ������ �������
	hex_salt = KeyContainer.P21(); //�������� ���� ��� ��������� ��
	HashedPassword(Password, hex_salt, hash256);
	//��������� ��� ��� ���� ��� ��������������� ���������. ������������� �� ����� ��
	string DH_pk; //�������� ����
	DecryptSK(hash256, KeyContainer.P18(), KeyContainer.P19(), DH_pk);
	/*
	������������ �������� ��� ������������ � ����������.
	�������� �������� ����, ���� ���� ������ ���������-��������� ��������� ������������ 0
	�������� �������� ����. ���� ��� �� ������� ������������, �� ��������� ��� �� ��������-��������� ��������� ������������ 0
	������������ �������� ������� n ��������� � ��������� �����
	*/
	if (!GetDhPair(KeyContainer.P20(), DH_pk, publicKey, secret_key)) return 0;

	return true;
}

//�������� ��������� ����������� ������ � ����
bool KeyService::HashedPassword(QString &Passwd, QString &salt_str, uint8_t *hash)
{
	try
	{
		//������ ������ ������������ ����� ������� � UTF16, ����������� � Windows-1251
		QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
		QByteArray pswd = codec->fromUnicode(Passwd);
		//�������������� ���� � ���� 16 ������ ������ � ���� ������
		QByteArray salt = QByteArray::fromHex(salt_str.toUtf8());


		int pswd_len = pswd.size();//������ ������
		int salt_len = salt.size();//������ ����

		QByteArray data_to_hash;//������ ���� ���������� �����������
		data_to_hash.resize(pswd_len + salt_len);

		//������� ������
		int pos = 0;
		//������� ������
		for (int i = 0;i < pswd_len;i++)
		{
			data_to_hash[pos] = pswd[i];
			pos++;
		}

		//������� ����
		for (int i = 0;i < salt_len;i++)
		{
			data_to_hash[pos] = salt[i];
			pos++;
		}


		//�������
		Hash3411 H;
		unsigned long long  mess_len, tmp;
		tmp = static_cast<unsigned long long>(data_to_hash.size());
		mess_len = tmp * 8; //����� ��������� � �����
		H.hash_256(reinterpret_cast<uint8_t*>(data_to_hash.data()), mess_len, hash);

	}
	catch (...)
	{
		return 0;
	}

	return 1;
}


//�������������� ��������� ���� CryptedKey ��������� key
bool KeyService::DecryptSK(uint8_t *key, QString &CryptedKey, QString &iv, string &p_key)
{

	//�������������� ��������� ������ ����� 16 ������ ������ � ���� ������
	QByteArray bIV = QByteArray::fromHex(iv.toUtf8());
	//�������������� ������ � 16������ �������, �������������� ��������� ����� � ���� ������
	QByteArray crypted_array = QByteArray::fromHex(CryptedKey.toUtf8());


	//�������������� ������ �������������
	ByteBlock init_vector(reinterpret_cast<uint8_t*>(bIV.data()), 16);
	//�������������� ����������� ���������
	ByteBlock crypted_message(reinterpret_cast<uint8_t*>(crypted_array.data()), crypted_array.size());
	//��������� ���������������
	ByteBlock decrypt_result;
	//����
	ByteBlock cr_key(key, 32);
	Sib3412 K(cr_key);
	K.CFB_Decrypt(crypted_message, decrypt_result, init_vector);

	//�������������� � 16������ ������
	p_key = Utillity::hex_representation(decrypt_result);

	return 1;
}

//��������� ���� ������ ����� �����-�������. ���� ���� �� ��������� ���������� 0
bool KeyService::GetDhPair(QString &open_key, string &private_key, RSA::PublicKey &publicKey, RSA::PrivateKey &secret_key)
{
	//�������� ��������� �����
	QByteArray arr1 = QByteArray::fromHex(open_key.toUtf8());
	

	//String Source �� ��������!!!!!
	ArraySource as1(reinterpret_cast<uint8_t*>(arr1.data()), arr1.size(), true /*pumpAll*/);

	//���� �������� ���� ��������� ��� �� ������ ������ ����� ����������
	try
	{
		publicKey.Load(as1);
	}
	catch (...)
	{
		lastError = "������ K531";
		return false;
	}



	//�������� ��������� �����
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

	//��������� �������� n ��������� ����� � ��������� �����
	CryptoPP::Integer n1;//�������� ����
	CryptoPP::Integer n2;//�������� ����

	n1 = secret_key.GetModulus();
	n2 = publicKey.GetModulus();

	if (n1.Compare(n2) != 0)
	{
		lastError = "������ K532";
		return false;
	}


	return 1;
}

bool KeyService::DecryptEc_SK(QString Password, Point & Q, mpz_class & d)
{
	try
	{
		uint8_t hash256[32];
		QString hex_salt; //���� � 16 ������ �������
		hex_salt = KeyContainer.P12(); //�������� ���� ��� ��������� �������
		HashedPassword(Password, hex_salt, hash256);
		//��������� ��� ��� ���� ��� ��������������� ���������. ������������� �� �������
		string Ec_pk; //�������� ����
		DecryptSK(hash256, KeyContainer.P11(), KeyContainer.P14(), Ec_pk);

		Point Q1;
		//�������� ���� ������������. ���� ������ �������� ������� �������� �� 16 ������ ������� ��������� ��������� ����������
		mpz_class d1(Ec_pk, 16);
		string  str_x, str_y;
		str_x = KeyContainer.P16().toStdString();
		str_y = KeyContainer.P17().toStdString();

		Q1.x.set_str(str_x, 16);
		Q1.y.set_str(str_y, 16);

		//�������� ������������ �������� ���
		string EcOid = KeyContainer.P24().toStdString();
		if (!CheckEcPair(Q1, d1, EcOid)) return false;

		//���������� ����� �� ��� ������!
		pubECkey = Q1;//�������� �������� ���� ��� ������������� ������������
		pubECkey.setCurvOid(EcOid); //����� ��������� ������.
		prvEcKey = d1;//�������� �������� ���� ��� ������������� ������������


		d = d1; //������� �������� ����
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
	bool find_ec = EC2.SetParam(ec_oid);//������������ ��������� ����� �� ��������� �� OID
	if (!find_ec)
	{
		lastError= "Bad Ec OID.";
		return false;
	}
		

	//������ ����� � ���������� � ������ ������������ ����� P
	Point P2(EC2, 1); //����� ������������� ������ 

	CreateSign Sg;
	Point Q1;
	Q1 = Sg.generate_public_key(d, P2); //��������� �������� ����

										//�������� ���������
	if (Q.x != Q1.x) return false;
	if (Q.y != Q1.y) return false;

	return  true;
}

bool KeyService::CheckKeyDateValidity()
{
	//	Q_PROPERTY(QDateTime P9 READ P9 WRITE setP9) //���� ������ �������� �����
	//Q_PROPERTY(QDateTime P10 READ P10 WRITE setP10) //���� ��������� �������� �����

	QDateTime now = QDateTime::currentDateTime();

	if (now < KeyContainer.P9())
	{
		lastError = "������ K 715: ���� ������ �������� ����� ��� �� ���������. ���� ������ ����������� " +
			KeyContainer.P9().toString("dd.MM.yyyy hh:mm:ss").toStdString() + ".";
		return false;
	}

	if (now > KeyContainer.P10())
	{
		lastError = "������ K 716: ����  �������� ����� ����� " +
			KeyContainer.P10().toString("dd.MM.yyyy hh:mm:ss").toStdString() + ".";
		return false;
	}

	return true;
}

//��������� ������������ ��������� ����� � ����������� � �� ��� ������� �����.
bool KeyService::CheckKeyCert(GranitDbI &Dbi)
{
	
	//�������� ���� �������� ��.
	if (!Dbi.checkDbDateEnd())
	{
		lastError = "������: ����� ���� �������� �� ����.";
		return false;
    }
	
	//����� ���������� ����� �� �� �������.
	bool Estts = Dbi.GetUserCert(KeyContainer.P16(), KeyContainer.P17(), KeyContainer.P6(), KeyContainer.P7(), KeyContainer.P8());
	if (!Estts) 
	{
		lastError = Dbi.getLastError();
		return false;
	}

	
	QByteArray uBa = Dbi.getCert(); //������� ��������� ����������.

	//��������  ���������� ������������;
	TBSCertificate  tbsUser;
	X509Reader R;
	R.ReadBa(uBa, tbsUser);

	//�������� ����� �������� �����������.
	Estts = CheckValidityCert(tbsUser);
	if (!Estts) return false;
	 

	string err;

	//�������� ������������ ��������� ����� � ����� ��������������� � �����������.
	//	//���������� �������� ���� ����������� � ���� � ����������.
	if (!CompareKeys(tbsUser))
	{
		lastError = "�� ���������� ����� ������������ � ����������� � �� �������.";
		return false;
	}
		

	//������� ���������� CA.
	QByteArray baCAcert;
	bool IsError = Dbi.getCACert(baCAcert);
	if (!IsError)
	{
		lastError = "������.:��� ��146.";
		return false;
	}
	

	//����� ���������� CA.
	TBSCertificate  tbsCA;
	R.ReadBa(baCAcert, tbsCA);

	//�������� ����� �������� �����������.
	Estts = CheckValidityCert(tbsCA);
	if (!Estts) return false;

		
	//�������� ������� ����������������� �����������.
	bool signStatus;
	err = R.VerifiCertSign(tbsUser, tbsCA, signStatus); //�������� �������
	if (!err.empty()) 
	{
		lastError = err;
		return false;
	}
		

	if (signStatus == false)
	{
		lastError = "�������� ������� ����������������� �����������.";
		return false;
	}

	//�������� ������� ����������� CA
	//������ ����������� Root
	QByteArray baRootCert;
	IsError = Dbi.getRootCert(baRootCert);
	if (!IsError) 
	{
		lastError = "������.:��� ��147.";
		return false;
	}


	//����� ���������� Root.
	TBSCertificate  tbsRoot;
	R.ReadBa(baRootCert, tbsRoot);

	//�������� ����� �������� �����������.
	Estts = CheckValidityCert(tbsRoot);
	if (!Estts) return false;
	

	//�������� ������� ����������� CA
	err = R.VerifiCertSign(tbsCA, tbsRoot, signStatus); //�������� �������
	if (!err.empty())
	{
		lastError = err;
		return false;
	}

	if (signStatus == false)
	{
		lastError = "�������� �������  ����������� ��������������� ������(CA).";
		return false;
	}

	//���������� ����������������� �����������.
	tbsUserCertificate = tbsUser;
	
	return true;
}



bool KeyService::GetSkInfo(string & Familia, string & Imia, string & Otchestvo, string & DateBegin, string & DateEnd, string & DaysLeft)
{
	if (KeyContainerIsLoad == false) return false; //��������� ����� �� ��� ��������.

											   /*
											   Q_PROPERTY(QString P6 READ P6 WRITE setP6) //Familia
											   Q_PROPERTY(QString P7 READ P7 WRITE setP7) //Imia
											   Q_PROPERTY(QString P8 READ P8 WRITE setP8) //Otchestvo
											   */
	Familia = KeyContainer.P6().toLocal8Bit();
	Imia = KeyContainer.P7().toLocal8Bit();
	Otchestvo = KeyContainer.P8().toLocal8Bit();

	//Q_PROPERTY(QDateTime P10 READ P10 WRITE setP10) //���� ��������� �������� �����
	//Q_PROPERTY(QDateTime P9 READ P9 WRITE setP9) //���� ������ �������� �����
	DateBegin = KeyContainer.P9().toString("dd.MM.yyyy hh:mm:ss").toStdString();
	DateEnd = KeyContainer.P10().toString("dd.MM.yyyy hh:mm:ss").toStdString();

	//���������� ���������� ���� �� ��������� ����� �������� �����.
	QDateTime curentDt = QDateTime::currentDateTime(); //������� ����.
	QDateTime DtEnd = KeyContainer.P10(); //���� ��������� �������� �����.

	int daysLeft = curentDt.daysTo(DtEnd); //���������� ���������� ����.
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

	Qx = KeyContainer.P16();//�������� ���� ������� ���������� X
	Qy = KeyContainer.P17();//�������� ���� ������� ���������� Y
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
	if (!Skd.hasKey()) return; //���� �� ��� ��������. �� ���� �������.

	Skd.Clear(); //������� ���������� �����.
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

//��������� ���� �������� �����������. � ������ ������� ������ ���������. ���������� false. ����� =true.
bool KeyService::CheckValidityCert(TBSCertificate cert)
{
	//�������� ����������� �������� �� �������.
	string iOrg = cert.Issuer_id_at_organizationName;
	if (iOrg.empty())
	{
		lastError = "������ � 713A: ������ ���� organizationName �������� ����������� ������������.";
		return false;
	}


	QDateTime NotBefore;
	QDateTime NotAfter;

	NotBefore = cert.NotBefore;
	NotAfter = cert.NotAfter;

	QDateTime now = QDateTime::currentDateTime();
	if (NotBefore > now) //���������� �� ����� ��� �����������.
	{
		lastError = "������ � 713: ���������� �������� O=" +
			QTextCodec::codecForName("CP1251")->fromUnicode(cert.Subject_id_at_organizationName.c_str()).toStdString() +
			" ;CN=" + QTextCodec::codecForName("CP1251")->fromUnicode(cert.Subject_id_at_commonName.c_str()).toStdString() +
			" ��� �� ����� �����������. ���������� ������ ����������� " + NotBefore.toString("dd.MM.yyyy hh:mm:ss").toStdString()
			+ ".";

		return false;
	}

	if (NotAfter < now) //���������� ����������.
	{
		lastError = "������ � 714: ���� �������� ����������� �������� O=" +
			QTextCodec::codecForName("CP1251")->fromUnicode(cert.Subject_id_at_organizationName.c_str()).toStdString() +
			" ;CN=" + QTextCodec::codecForName("CP1251")->fromUnicode(cert.Subject_id_at_commonName.c_str()).toStdString() +
			"����������. ���� ��������� �������� ����������� ����� " + NotAfter.toString("dd.MM.yyyy hh:mm:ss").toStdString()
			+ ".";

		return false;
	}

	return true;
}

//���������� �������� ���� ����������� � ���� � ����������.
bool KeyService::CompareKeys(TBSCertificate &tbsUser)
{
	string  str_x, str_y;
	str_x = KeyContainer.P16().toStdString();
	str_y = KeyContainer.P17().toStdString();
	mpz_class Qx(str_x, 16); //�������� ���� �� ����������.
	mpz_class Qy(str_y, 16);

	mpz_class CertQx, CertQy; //�������� ��������� ����� �� �����������
	CertQx = tbsUser.Qx;
	CertQy = tbsUser.Qy;

	//��������� �������� ������.
	if (Qx != CertQx)	return false;
	if (Qy != CertQy)	return false;

	return true;
}