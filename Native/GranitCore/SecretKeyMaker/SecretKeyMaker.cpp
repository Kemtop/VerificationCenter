#include "SecretKeyMaker.h"
const QString SecretKeyMaker::G_publicKeyAlgoritmOid = "1.2.643.7.1.1.1.2";//�������� ��������� ����� �������.
const QString SecretKeyMaker::G_EcOid = "1.2.643.7.1.2.1.2.2"; //������������� ������������� ������ �� ���������  ������� ��������� ����.

SecretKeyMaker::SecretKeyMaker()
{
	enableRose = false; //�� �������� ����� �����������.
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


		//��������� �������� ���� ��� ������������ � �������� �������.
		Point Q;//�������� ����.
		Error = �reateInvisibleSignKey(Key, Password, Q, str_iv, salt, crypt_secret_key);
		if (!Error) return false;

		//-------------��������� ��������� � ��������� ����� ��� ��������� �����-�������-----------------------//
		string DH_iv = "";
		string DH_salt = "";
		string crypted_secret_DH_key = "";
		string DH_public = ""; //�������� ����.
		Error = �reateInvisibleDHkey(Password, DH_iv, DH_salt, crypted_secret_DH_key, DH_public);
		if (!Error) return false;
		//-------------------------------------------
		

		QDateTime tStamp = QDateTime::currentDateTime();
		//���������� ���������� �����
		PkContainer �ontainer;
		�ontainer.setP0("0.0.1");//������ ����� "0.0.1"
		�ontainer.setP1("0.0.1");//������ ���������� "0.0.1" ��

		�ontainer.setP2(Req.P4()); ////������������ �����������.
		�ontainer.setP3(Req.P5());	//�����.
		�ontainer.setP4(Req.P3()); //�������������.
		�ontainer.setP5(Req.P9()); //�������.
		�ontainer.setP6(Req.P0()); //Familia
		�ontainer.setP7(Req.P1()); //7 Imia
		�ontainer.setP8(Req.P2()); //8 Otchestvo
		�ontainer.setP9(tStamp);//��������� ���� ������ �������� �����.
		�ontainer.setP10(tStamp.addYears(1));//��������� ���� ��������� �������� �����.
		�ontainer.setP11(QString::fromLocal8Bit(crypt_secret_key.c_str())); //����������� ��������� ����
		�ontainer.setP12(QString::fromLocal8Bit(salt.c_str())); //����
		�ontainer.setP13("0"); //�������������� ��������

		�ontainer.setP14(QString::fromLocal8Bit(str_iv.c_str()));//������ ������������� IV
		�ontainer.setP15(QDateTime::currentDateTime()); //���� ��������� �����  

														//���������� ���������� ����� �����-��������

		�ontainer.setP18(QString::fromLocal8Bit(crypted_secret_DH_key.c_str(), crypted_secret_DH_key.length())); //����������� �������� ��������� ����� ����� �����-��������
		�ontainer.setP19(QString::fromLocal8Bit(DH_iv.c_str(), DH_iv.length())); //�������� ������� iv ��� ��������� ����� ����� �����-��������
		�ontainer.setP20(str_to_Hex(DH_public)); //�������� ���� ����� �����-��������
		�ontainer.setP21(QString::fromLocal8Bit(DH_salt.c_str(), DH_salt.length())); //���� ��� ������������ ��������  ��������� ����� ����� �����-��������

																					 //������� �������� ���� � ���������
		string str_Qx = Q.get_hex_x(); // Qx.get_str(16);
		string str_Qy = Q.get_hex_y();// Qy.get_str(16);

		�ontainer.setP16(QString::fromLocal8Bit(str_Qx.c_str())); //�������� ���� ������� ���������� x
		�ontainer.setP17(QString::fromLocal8Bit(str_Qy.c_str()));//�������� ���� ������� ���������� y

																 //������������ ��������� �����
		�ontainer.setP22(G_publicKeyAlgoritmOid);//PublicKeyAlgoritm-�������� ��������� ����� �������.
		�ontainer.setP23("1LgN89TY57"); //������ ����� ������� �� ��� �� ������������, �� �������� ���������� ��������������.
		�ontainer.setP24(G_EcOid);	//EcOid-��������� ������������� ������ �� �������� ������� ��� ������ ���� �������.


									//�������� ��������� ����� � ����
		Error = Key�ontainerToFile(�ontainer, Path);
		if (!Error) return false;

		//�������� ������ ��� �������.
		Req.setP11(KeyGenVersion); //������ ���������� �����.
		Req.setP12(str_to_Hex(DH_public)); //�������� ���� ����� �����-�������.

		QString x1(str_Qx.c_str());
		QString y1(str_Qy.c_str());
		//���������� ��������� ����� �������.
		Req.setP14(x1);
		Req.setP15(y1);

		Req.setP16(G_publicKeyAlgoritmOid);//PublicKeyAlgoritm-�������� ��������� ����� �������.
		Req.setP17(G_EcOid);//EcOid-��������� ������������� ������ �� ���������� ������� ��� ������ ���� �������.

		Req.setP19(Req.P19() + 1); //���������� ���������� ����� ������.

		//������ ���� ������� �� ��������� �����������.
	
		Error = CreateRequestFile(pathToSaveReq,Req);
		if (!Error) return false;

		//�������� ��� ����� ��� ���������� ������.
		QString hs = VanishChest::algHashSignKey(x1, y1);
		Req.setP18(hs);

	}
	catch (std::exception &ex)
	{
		lastError = "� ������ CreateKeyFile() �������� ����������:"+string(ex.what());
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


		//��������� �������� ���� ��� ������������ � �������� �������.
		Point Q;//�������� ����.
		Error = �reateInvisibleSignKey(Key, Password, Q, str_iv, salt, crypt_secret_key);
		if (!Error) return false;

		//-------------��������� ��������� � ��������� ����� ��� ��������� �����-�������-----------------------//
		string DH_iv = "";
		string DH_salt = "";
		string crypted_secret_DH_key = "";
		string DH_public = ""; //�������� ����.
		Error = �reateInvisibleDHkey(Password, DH_iv, DH_salt, crypted_secret_DH_key, DH_public);
		if (!Error) return false;
		//-------------------------------------------


		QDateTime tStamp = QDateTime::currentDateTime();
		//���������� ���������� �����
		PkContainer �ontainer;
		�ontainer.setP0("0.0.1");//������ ����� "0.0.1"
		�ontainer.setP1("0.0.1");//������ ���������� "0.0.1" ��

		�ontainer.setP2("Charlie"); ////������������ �����������.
		�ontainer.setP3("Sopranos");	//�����.
		�ontainer.setP4("Rings"); //�������������.
		�ontainer.setP5("series"); //�������.
		�ontainer.setP6("Patrick"); //Familia
		�ontainer.setP7("Marshall"); //7 Imia
		�ontainer.setP8("During"); //8 Otchestvo
		�ontainer.setP9(tStamp);//��������� ���� ������ �������� �����.
		�ontainer.setP10(tStamp.addYears(1));//��������� ���� ��������� �������� �����.
		�ontainer.setP11(QString::fromLocal8Bit(crypt_secret_key.c_str())); //����������� ��������� ����
		�ontainer.setP12(QString::fromLocal8Bit(salt.c_str())); //����
		�ontainer.setP13("0"); //�������������� ��������

		�ontainer.setP14(QString::fromLocal8Bit(str_iv.c_str()));//������ ������������� IV
		�ontainer.setP15(QDateTime::currentDateTime()); //���� ��������� �����  

														//���������� ���������� ����� �����-��������

		�ontainer.setP18(QString::fromLocal8Bit(crypted_secret_DH_key.c_str(), crypted_secret_DH_key.length())); //����������� �������� ��������� ����� ����� �����-��������
		�ontainer.setP19(QString::fromLocal8Bit(DH_iv.c_str(), DH_iv.length())); //�������� ������� iv ��� ��������� ����� ����� �����-��������
		�ontainer.setP20(str_to_Hex(DH_public)); //�������� ���� ����� �����-��������
		�ontainer.setP21(QString::fromLocal8Bit(DH_salt.c_str(), DH_salt.length())); //���� ��� ������������ ��������  ��������� ����� ����� �����-��������

																					 //������� �������� ���� � ���������
		string str_Qx = Q.get_hex_x(); // Qx.get_str(16);
		string str_Qy = Q.get_hex_y();// Qy.get_str(16);

		�ontainer.setP16(QString::fromLocal8Bit(str_Qx.c_str())); //�������� ���� ������� ���������� x
		�ontainer.setP17(QString::fromLocal8Bit(str_Qy.c_str()));//�������� ���� ������� ���������� y

																 //������������ ��������� �����
		�ontainer.setP22(G_publicKeyAlgoritmOid);//PublicKeyAlgoritm-�������� ��������� ����� �������.
		�ontainer.setP23("8bDkhN935SYik36i"); //������ ����� ������� �� ��� �� ������������, �� �������� ���������� ��������������.
		�ontainer.setP24(G_EcOid);	//EcOid-��������� ������������� ������ �� �������� ������� ��� ������ ���� �������.


									//�������� ��������� ����� � ����
		Error = Key�ontainerToFile(�ontainer, Path);
		if (!Error) return false;
				
	}
	catch (std::exception &ex)
	{
		lastError = "� ������ CreateKeyFile() �������� ����������:" + string(ex.what());
		return false;
	}

	return true;
}

string SecretKeyMaker::getLastError()
{
	return lastError;
}


bool SecretKeyMaker::�reateInvisibleSignKey(unsigned char *Key, QString Password, Point & Q, string & str_iv_, string & salt_, string & crypt_secret_key_)
{
	bool Error;
	string salt = "";
	mpz_class d; //�������� ����.

	string EcOID;//������������� ������������� ������.
	EcOID = G_EcOid.toStdString();
	Error = CreateEcKeyPair(true, EcOID, Key, 64, Q, d);
	if (!Error) return false;


	//��������� 17������� ����. ���� ��������� �����.
	Error = GenerateSalt(salt);
	if (!Error) return false;
	unsigned char hash256[32];

	//�������� ��� �������� ������
	Error = HashedPassword(Password, salt, hash256);
	if (!Error) return false;

	//��������� ������ �������������
	unsigned char iv[16];
	Error = GenerateIV(iv);
	if (!Error) return false;

	//������ ��������� ���� �������� �������  ���� 34.11-2012 �� ����, 
	string crypt_secret_key; //��������� ����������
	Error = CryptEcSecretKey(crypt_secret_key, hash256, iv, d);
	if (!Error) return false;

	d = 987;//������� ����.
	string str_iv = "";
	ByteArrayToHexString(iv, 16, str_iv); //����������� ���� ������ � ������

										  //������ �������������.
	str_iv_ = str_iv;
	salt_ = salt;//����.
	crypt_secret_key_ = crypt_secret_key; //����������� ��������� ����.

	return true;
}

//ECname-��� ������������ ������;Q-�������� ����	
bool SecretKeyMaker::CreateEcKeyPair(bool BeginSelfTest, string EcOID, uint8_t *secret_key, uint8_t key_len, Point &Q, mpz_class &d)
{
	try
	{
		//�������� OID ������
		Groups curEc;
		bool find_Ec = curEc.SetParam(EcOID);
		if (!find_Ec) //�� ������� ������ � ������ OID
		{
			lastError = "�� ������ OID ������������� ������.OID=" + EcOID;
			return false;
		}

		CreateSign Gen; //������ ���������� ������ ��� ��������� �����

		if (BeginSelfTest)
		{
			//���� ��������� �������� ��������� �����	
			string test_result = Gen.TestPk();
			if (test_result.length() > 0)
			{
				lastError = test_result;
				return false;
			}
		}

		//��������� �����
		Point Q1;
		mpz_class d1;
		Q1 = Gen.generate_public_key_512(secret_key, d1, EcOID);

		Q = Q1;
		d = d1;

	}
	catch (exception& e)
	{
		lastError="� ������ CreateEcKeyPair() �������� ����������:" +string(e.what());
		return false;

	}

	return true;
}


//���������� 17 ������� ����.
bool SecretKeyMaker::GenerateSalt(string &d)
{
	try
	{
		//��������� 17 ������� ����. ��� ��������� ��������� ��������� ��� ����������� ����������������
		int rnd; //�������� ���
		unsigned long urnd;
		string str_r;
		d = "";
		//�������� 40 ������� �����
		for (int i = 0;i < 10;i++)
		{
			rnd = QRandomGenerator::system()->generate();
			urnd = (unsigned long)rnd;
			str_r = std::to_string(urnd);
			d = d + str_r;
		}

		//�������������� ������ � ������� �����
		mpz_class D(d, 10);
		d = D.get_str(16); //�������������� ����� � 16������ ������

						   //���� 17 ����
		d = d.substr(0, 34);
	}
	catch (std::exception &e)
	{
		lastError= "� ������ GenerateSalt �������� ����������:" +string(e.what());
		return false;
	}

	return true;
}

//�������� ������ � �����, � ������ ������ ���������� ���������
bool SecretKeyMaker::HashedPassword(QString &Password, string salt, unsigned char *hash)
{
	try
	{
		//������ ������ ������������ ����� ������� � UTF16, ����������� � Windows-1251
		QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
		QByteArray pswd = codec->fromUnicode(Password);
		unsigned long  pswd_len = (unsigned long)pswd.length(); //����� ������ ������
																//������ ����� ������� ������
		unsigned char *tmp_pswd = new unsigned char[pswd_len];
		for (int i = 0;i < pswd_len;i++)
		{
			tmp_pswd[i] = (unsigned char)pswd[i];
		}
		//�������������� ���� � ������
		//���� � ����� ��������-�� ����� �� ������
		if (salt.length() % 2 != 0) salt = "0" + salt;

		int salt_length = salt.length();
		int sal = salt_length / 2;
		unsigned long  salt_array_len = (unsigned long)(sal); //������ ������� � ������,��� ������� ������ ��������� ����
		unsigned char *salt_array = new unsigned char[salt_array_len];

		//������������ 16 ������ ������ � ������ unsigned char
		HexStr_to_uchar(salt, salt_array);

		//�������� ������ ���������� �����������
		unsigned long  array_to_hash_len = pswd_len + salt_array_len;
		unsigned char *array_to_hash = new unsigned char[array_to_hash_len];//������ ���������� �����������
																			//������� ������ � ����
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

		//������ ��������� �������
		delete[]tmp_pswd; tmp_pswd = NULL;
		delete[]salt_array;salt_array = NULL;

		//�������
		Hash3411 H;
		unsigned long  mess_len;
		mess_len = array_to_hash_len * 8; //����� ��������� � �����
		H.hash_256(array_to_hash, (unsigned long long)mess_len, hash);

	}
	catch (...)
	{
		lastError="� ������ HashedPassword �������� ����������:";
		return false;
	}

	return true;
}

bool SecretKeyMaker::�reateInvisibleDHkey(QString Password, string & str_iv_, string & salt_, string & crypted_secret_DH_key_, string &DH_public_)
{
	string DH_public; //���� ������
	uint8_t DHprivate_key[8192];
	uint32_t DHprivate_key_len = 8192; //����������� ����� ����������������-����� GenerateDHpair �� ����� ����� ������� � ��� ���� 
									   //���� ��� ������ �����

	bool Error = GenerateDHpair(DH_public, DHprivate_key, DHprivate_key_len); //��������� �������� ����.
	if (!Error) return false;

	string salt = "";
	//��������� 17������� ����. ���� ��������� �����.
	Error = GenerateSalt(salt);
	if (!Error) return false;


	//�������� ��� �������� ������
	unsigned char hash256[32];
	Error = HashedPassword(Password, salt, hash256);
	if (!Error) return false;


	//��������� ������ �������������
	unsigned char iv[16];
	Error = GenerateIV(iv);
	if (!Error) return false;

	string crypted_secret_DH_key; //����������� ��������� ���� �����-�������

								  //������ �������� ���� ��������� �����-�������
	Error = CryptDHSecretKey(crypted_secret_DH_key, DHprivate_key, DHprivate_key_len, hash256, iv);
	if (!Error) return false;

	string DH_iv;//�������� ���������� ������� ��� ��������������� ���������� ����� ����� �-� 
	ByteArrayToHexString(iv, 16, DH_iv); //����������� ���� ������ � 16������ ������

	str_iv_ = DH_iv;
	salt_ = salt;
	crypted_secret_DH_key_ = crypted_secret_DH_key;
	DH_public_ = DH_public;

	return true;
}


//��������� ������ �������������
bool SecretKeyMaker::GenerateIV(unsigned char *iv)
{
	try
	{
		//��������� ��������� ��� ������� �����
		int rnd = QRandomGenerator::system()->generate();

		//���������  ��������������� ����� �� ������ ��������� ����� ��������
		gmp_randclass Rand(gmp_randinit_mt);
		mpz_class r;
		Rand.seed(rnd);//� �������� ���������� �������� ��������� rnd
		r = Rand.get_z_bits(128);
		//�������������� �������� ����� � ������
		mpz_export((void *)iv, NULL, 1, sizeof(unsigned char), 1, 0, r.get_mpz_t());
		/*
		//��������
		string a = r.get_str(16); //���������� ��������� �����
		mpz_class x;
		size_t len = (size_t)16;//������ �������
		//�������������� ������� � ������� �����
		mpz_import(x.get_mpz_t(), len, 1, sizeof(iv[0]), 0, 0, iv);
		string b = x.get_str(16);
		QString s1(a.c_str());
		QString s2(b.c_str());
		QMessageBox::information(this, "", s1 + "\r\n" + s2);
		*/

	}
	catch (...)
	{
		lastError="� ������ GenerateIV �������� ����������:";
		return false;
	}

	return true;
}

//������ ��������� ���� , � �������� ����� ������������ ���, � ������ ������ ���������� ���������
bool SecretKeyMaker::CryptEcSecretKey(string &result, unsigned char *hash, unsigned char *iv, mpz_class &d)
{
	try
	{
		uint8_t sk[64]; //�������� ����
		for (int i = 0;i < 64;i++) sk[i] = 0;
		//�������� �������
		unsigned int sz = mpz_sizeinbase(d.get_mpz_t(), 2);
		if ((sz / 8) > 64)
		{
			lastError = " CryptSecretKey: �� ������ ����� �����.";
			return false;
		}
			

		//�������������� �������� ����� � ������
		mpz_export((void *)sk, NULL, 1, sizeof(unsigned char), 1, 0, d.get_mpz_t());

		ByteBlock cr_key(hash, 32); //���� 
									//���������(��������� ����) ���������� ����������
		ByteBlock crypt_msg(sk, 64);
		//������ �������������
		ByteBlock iv_(iv, 16);
		//��������� ����������
		ByteBlock crypted_secret_key;
		Sib3412 CrAlgoritm(cr_key);
		//�������� ���������� � ������ �������� ����� �� �����������
		CrAlgoritm.CFB_Encrypt(crypt_msg, crypted_secret_key, iv_);
		//�������������� � 16������ ������
		result = Utillity::hex_representation(crypted_secret_key);
	}
	catch (...)
	{
		lastError="� ������ CryptSecretKey �������� ����������:";
		return false;
	}

	return true;
}

//������������ �������� ������ � ������
void SecretKeyMaker::ByteArrayToHexString(unsigned char *array, int len, string &str)
{
	//����������� ������ � ������ mpz_class
	mpz_class D;
	size_t l = (size_t)len;
	//�������������� ������� � ������� �����
	mpz_import(D.get_mpz_t(), l, 1, sizeof(unsigned char), 0, 0, array);
	str = D.get_str(16);
}


//������������ 16 ������ ������ � ������ unsigned char
void SecretKeyMaker::HexStr_to_uchar(string str, unsigned char *array)
{
	int length = str.length();
	unsigned char *p1 = array + ((unsigned char)(length / 2) - 1); //��������� ������� ������� array
	string hex;
	//���� ������� �� ������� ������ 
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

		AutoSeededRandomPool rng; //���������� �� ���  ��������� seed

		InvertibleRSAFunction params; //������� RSA � �������������� ��������� �����.
		params.GenerateRandomWithKeySize(rng, 4096); //Generate a random key or crypto parameters.

		RSA::PrivateKey privateKey(params);
		RSA::PublicKey publicKey(params);
		//sha256WithRSAEncryption
		//�������� �������� ���� � �����
		ArraySink arraySink1(private_key, private_key_len);// buffer1.size());
		privateKey.DEREncode(arraySink1);//������������ Der ���������
		private_key_len = static_cast<uint32_t>(arraySink1.TotalPutLength()); //�������� ����� ���������� ������

																			  //�������� �������� ���� � �����
		ArraySink arraySink2(public_key, public_key_len);//uffer2.size());
		publicKey.DEREncode(arraySink2);//������������ Der ���������
		public_key_len = arraySink2.TotalPutLength(); //�������� ����� ���������� ������

													  //�������������� ������ � ������
		string s2(reinterpret_cast<char const*>(public_key), public_key_len);

		DH_public = s2;

	}
	catch (...)
	{
		lastError="� ������ GenerateDHpair() �������� ����������:";
		return false;
	}

	return true;
}


QString SecretKeyMaker::str_to_Hex(string & str)
{
	QString str1, str2;
	//�������������� �� ��������� ���� � 16 ������
	str1 = QString::fromLocal8Bit(str.c_str(), str.length());
	str2 = str1.toLocal8Bit().toHex();

	return str2;
}

//��������� ��������� ����� � ����
bool SecretKeyMaker::Key�ontainerToFile(PkContainer &KeyCont, QString PathToSave)
{
	try
	{
		QString Error = "";

		QString out_str = "";
		SerializableTools::Serialize(out_str, KeyCont);//������������ �������.

															   //������� ����� ������������ � ��������� utf8
		QByteArray bytes = out_str.toUtf8();//������ � ���� ������ 
		QByteArray packing; //������������ ���������
							//���������� ���������
		if (!PackContainer(bytes, packing))
		{
			lastError = "������ W0:������ ��������.";
			return false;
		}

		//------���������� ��� 256 ������������ ����������---
		uint32_t key_data_len = static_cast<uint32_t>(packing.size());//������ ������������ ���������� �����
		uint8_t *key_data = reinterpret_cast<uint8_t*>(packing.data()); //��������� �� ������ ������
		qint64 w_result;//��������� ������ � ����-������ ������		

		uint8_t hash256[32]; //��������� ���������� ��� �������	  
		GetHash�ontainer(key_data, key_data_len, hash256);//�������� ���

														  //--------���������� ����� ��������� �����������
		uint8_t file_title[Key_title_len];//��������� �����-��������� ����������
		SetService_Information(file_title, hash256, key_data_len);

		//----------���������� ����� � ����
		QFile fileOut(PathToSave);
		if (!fileOut.open(QIODevice::WriteOnly))
		{
			lastError = "�� ������� ��������� ���� "; //PathToSave;
			return false;
		}
		

		//������������ � ������� �������
		if (!fileOut.seek(0))
		{
			lastError = "�� ������� ������� ���� ��� ������."; // PathToSave
			return false;
		}

		//-----------������ ���������
		w_result = fileOut.write(reinterpret_cast<char *>(file_title), Key_title_len);
		if (w_result != Key_title_len)
		{
			lastError = "������ W1:�� ������� ��������� ����. ";// +PathToSave;
			return false;
		}


		//----------------------------------------------- ������ ��������� �� ������ ���������
		if (!fileOut.seek(Key_title_len))
		{
			lastError = "�� ������� ������� ���� ��� ������."; //PathToSave
			return false;
		}


		//--------------������ ������������ ���������� �����--------------------
		w_result = fileOut.write(packing, key_data_len);
		if (w_result != key_data_len)
		{
			lastError = "������ W2:�� ������� ��������� ���� ";//PathToSave;
			return false;
		}

		fileOut.close();

	}
	catch (...)
	{
		lastError = "� ������ Key�ontainerToFile() �������� ����������.";
		return false;
	}

	return true;
}


//������������ ��������� ���������� �����
bool SecretKeyMaker::PackContainer(QByteArray &Container, QByteArray &Packing)
{
	//��������� ������� ������������� ��� ����������
	unsigned char iv[16];
	bool Error = GenerateIV(iv);
	if (!Error) return false;
	
	//���� ������� ��������� ���������
	bool isErr = false;
	string Err = "";

	//�������� �������� ����� ������������ ����������. � ������ ������ HasErr=1;Err-�������� ��������� �� ������.
	ByteBlock cr_key = Utillity::hex_to_bytes(GranitCore::KeyChipperConstant); //������� ���� �������� ���������� ���������� �����.
	
	//��������������  QByteArray � ByteBlock
	size_t mess_length = static_cast<size_t>(Container.size());//����� ���������
	uint8_t *d_ptr; //������ ����� ������
	d_ptr = reinterpret_cast<uint8_t*>(Container.data());

	ByteBlock message(d_ptr, mess_length); //������� ������
										   //�������������� ������ �������������
	ByteBlock init_vector(iv, 16);
	//��������� ����������
	ByteBlock crypt_result;

	Sib3412 CrAlgoritm(cr_key);
	//�������� ���������� � ������ �������� ����� �� �����������
	CrAlgoritm.CFB_Encrypt(message, crypt_result, init_vector);

	Packing.resize(20);
	//������� iv
	int pos = 0;
	for (uint8_t i = 0;i < 16;i++)
	{
		Packing[pos] = static_cast<char>(iv[i]);
		pos++;
	}

	//�������� ����� �� ������������ �����
	pos = 16;
	uint32_t tmp0, tmp1 = mess_length;
	for (uint8_t i = 0;i < 4;i++)
	{
		tmp0 = tmp1 & 0xff;
		Packing[pos] = static_cast<char>(tmp0);
		tmp1 >>= 8;
		pos++;
	}
	//����� ������������ �����
	mess_length = crypt_result.size();
	Packing.resize(mess_length + 20);
	//������� ������ 
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
		SerializableTools::Serialize(out_str, Req);//������������ �������.

														   //��������� ������� ������������� ��� ����������
		unsigned char iv[16];
		bool Error = GenerateIV(iv);
		if (!Error) return false;


		//���� ������� ��������� ���������
		bool isErr = false;
		string Err = "";
			   
		ByteBlock cr_key = Utillity::hex_to_bytes(GranitCore::KeyChipperConstant); //������� ���� �������� ������.
		
		//��������� ���������� ����������-��������� "������ �����"
		QByteArray bytes = out_str.toUtf8();//������ � ���� ������ 


											//��������������  QByteArray � ByteBlock
		size_t mess_length = bytes.length();//����� ���������
		ByteBlock message((unsigned char *)bytes.data(), mess_length);

		//�������������� ������ �������������
		ByteBlock init_vector(iv, 16);
		//��������� ����������
		ByteBlock crypt_result;

		//������ ������		
		Sib3412 CrAlgoritm(cr_key);
		//�������� ���������� � ������ �������� ����� �� �����������
		CrAlgoritm.CFB_Encrypt(message, crypt_result, init_vector);

		//������� ������ ������������� � ����������� ��������� � ���� ������
		unsigned long array_len = 16 + ((unsigned long)crypt_result.size());//iv+����������� ��������� ������
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


		//�������� ���� ��������� 512 ����
		Hash3411 H;
		unsigned long hashed_len = array_len * 8;//����� ��������� � �����
		unsigned char hash512[64];
		H.hash_512(save_array, (unsigned long long)hashed_len, hash512);

		//������� ������ ��� ������ � ����
		int save_array_to_file_len = array_len + 64 + 1;
		unsigned char *save_array_to_file = new unsigned char[save_array_to_file_len];

		//������-���� ��������� ������������ ������
		for (int i = 0;i < save_array_to_file_len;i++) save_array_to_file[i] = 0;

		//������ ���������
		save_array_to_file[0] = 1;

		//������� ��� � ������ �����
		pos = 0;
		for (int i = 1;i < 65;i++)
		{
			save_array_to_file[i] = hash512[pos];
			pos++;
		}
		//������� ���������� �����
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

		//������ ������� 
		delete[]save_array; save_array = NULL;


		//�������� � ����
		QString fileName = "Request[" + Req.P0() + Req.P1()[0] + Req.P2()[0] + "_" +
			QDateTime::currentDateTime().toString("dd-MM-yyyychhmmss") +
			"].grq"; ;

		QString filepath = pathToSave +"\\"+ fileName;

		QFile fileOut(filepath);
		if (!fileOut.open(QIODevice::WriteOnly))
		{
			delete[]save_array_to_file; save_array_to_file = NULL;
			lastError= "�� ������� ��������� ����";
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
		lastError= "� ������ CreateRequest() �������� ����������.";
		return false;
	}

	return true;
}


//������ ��������� ���� ��������� �����-�������,� �������� ����� ������������ ���, � ������ ������ ���������� ���������
bool SecretKeyMaker::CryptDHSecretKey(string &crypted_key, uint8_t *private_key, uint32_t &private_key_len, unsigned char *hash, unsigned char *iv)
{
	try
	{
		ByteBlock cr_key(hash, 32); //���� 
									//���������(��������� ����) ���������� ����������
		ByteBlock crypt_msg(private_key, private_key_len);
		//������ �������������
		ByteBlock iv_(iv, 16);
		//��������� ����������
		ByteBlock crypted_secret_key;
		Sib3412 CrAlgoritm(cr_key);
		//�������� ���������� � ������ �������� ����� �� �����������
		CrAlgoritm.CFB_Encrypt(crypt_msg, crypted_secret_key, iv_);
		//�������������� � 16������ ������
		crypted_key = Utillity::hex_representation(crypted_secret_key);

	}
	catch (...)
	{
		lastError = "� ������ CryptDHSecretKey �������� ����������.";
		return false;
	}

	return true;
}

//��������� ���256 ����������
void SecretKeyMaker::GetHash�ontainer(uint8_t *container, uint32_t &container_len, uint8_t *hash)
{
	//�������� �������� ��� �������
	Hash3411 H;
	unsigned long long  mess_len, tmp; //unsigned __int64
	tmp = static_cast<unsigned long long>(container_len);
	mess_len = tmp * 8; //����� ��������� � �����
	H.hash_256(container, mess_len, hash);
}

void SecretKeyMaker::SetService_Information(uint8_t *arr, uint8_t *hash, uint32_t &conteiner_len)
{
	const uint8_t title1[7] = //������ 7 ���� ���������� ��� �� ����� ������
	{ 0xf9,0xc5, 0xa8, 0xd3, 0x47, 0xb6, 0x3a };

	//�������� ���������
	string pgm_title;
	if(!enableRose)
	   pgm_title = "GranitK Information security protection system ";
	else
	   pgm_title = "Little Rose radials security protection system ";

	//�������� ��������� �����
	uint8_t pos = 0;
	//[7-���� ��������� ����������]
	for (uint8_t i = 0;i < 7;i++)
	{
		arr[pos] = title1[i];
		pos++;
	}
	//[��������� 47����]
	for (uint8_t i = 0;i < 47;i++)
	{
		arr[pos] = static_cast<uint8_t>(pgm_title[i]);
		pos++;
	}

	//��� 32 �����
	for (uint8_t i = 0;i < 32;i++)
	{
		arr[pos] = hash[i];
		pos++;
	}
	//������ ����� ������ 4�����
	uint32_t len = conteiner_len;
	for (uint8_t i = 0;i < 4;i++)
	{
		arr[pos] = len & 0xff;
		len >>= 8;
		pos++;
	}

}