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
		if (!dir.mkpath("."))  return 0; //�� ������� ������� �������
	}

	if (!dir.exists()) return 0;
	destinationDir = DirPath;

	return true;
}

bool CipherWorker::CryptFiles(QStringList & paths_to_file, QString Password, string receiverKey, PkContainer &KeyContainer,
	string SignerIndex, string _rHashOkDH)
{
	QString Error = "";
	QString dst_Dir = "";//���� � �������� ���� ����� ����������� ����������� �����						 

	bool IsCreateDir = initDistDir(paths_to_file[0], dst_Dir);//�������������� ����� ����������,���� ����� ����������� ������������� �����.
	if (!IsCreateDir)
	{
		lastError = "�� ������� ������� ������� �������� ������������� ������.";
		return false;
	}

	int f_count = paths_to_file.count(); //���������� ������ ���������� ����������.
	
	uint8_t session_key[32]; //��������� ���� ��� �������� �����.
	
	//����� � dll LoadAsymmetricKey. ������ � �������!

	//������� ���� �������������� ����������.
	//�������������� ������ � 16 ������ ���� � �������� ������
	QByteArray HexArray = QByteArray::fromHex(receiverKey.c_str());
	//�������������� ����	
	ArraySource as_pk(reinterpret_cast<uint8_t*>(HexArray.data()), HexArray.size(), true);//pumpAll
	asymmetricKeyR.Load(as_pk); //�������� ����.

	//������� �������� ���� �������
	Point Q1;
	mpz_class user_d; //�������� ���� �������

	if (!DecryptEc_SK(Q1, user_d, Password, KeyContainer))
	{
		lastError = "������ K5 : �� ������� ������� ����.";
		return false;
	}

	signerIndex = SignerIndex; //�� ��� �� ������������.
	rHashOkDH = _rHashOkDH;

	//���� �� ���� ������
	bool errStts = false;
	for (int i = 0;i < f_count;i++)
	{
		//���������.
		for (int i = 0;i < 32;i++) session_key[i] = 0;
		
		uint8_t Reg[32]; //������� ������ ��� ������������.
		Cipher3412 Cipher; //������ ���������� ������� ������ �������� ���������� ���� 3412.
		Gost3413 BlockGost; //�������� ������������.

		BlockGost.generateIV(session_key);//�������� ��������� ����� �������� 32�����, ������� �������� ��������� ������.
		Cipher.deploymentEncryptionRoundKeys(session_key);//������������� ���������� �����.
		
		//���������� ���������� �����
		string crypted_session_key; //����������� ��������� ����.
		//������ ���������� ����, ������������� ����������.
		CryptedSessionKey(session_key, crypted_session_key);
		
		BlockGost.generateIV(Reg);//�������� ��������� ��������� ������(32�����) � ������� ��� � ������� 
		//iv ������ ���� ������ ��� ���� ������! �� ������ ���!
		 //������ ����
		errStts = CryptFile(paths_to_file[i], dst_Dir, Cipher, Reg, crypted_session_key, user_d, KeyContainer);
		if (!errStts) return false;


		//������� ������� ��������� ��������� ������.
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
	//�������� ������� -����� �������.
	sendEventThreadStatus(1);//����� �������.
	bool res = false;

	if (baseData.getProcessMode() == 1) //���������� ������.
	{
	   res = CryptFiles(baseData.getProcessFiles(), baseData.getPassword(),
	   baseData.getRAsimOpenKey(), baseData.getKeyContainer(), baseData.getSignerIndex(), baseData.getSignatureDH());
	}

	if (baseData.getProcessMode() == 2)//���������� ������.
	{
		res = DecryptFiles(baseData.getProcessFiles(), baseData.getDbPath(), baseData.getPassword(),baseData.getKeyContainer());
	}


	if (!res) //�������� ������.
	{
		string err = getLastError();
		//������� �������.
		eventsCipherWorker *ev = new eventsCipherWorker();													
		ev->setThreadStatus(3);//����� ���������� � �������.
		ev->setLastError(err);//��������� �� ������.
		QApplication::postEvent(threadEvens, ev);
		return;
	}
	sendEventThreadStatus(2);//����� ������� ��������.
}

void CipherWorker::sendEventAllProcessVal(int val)
{
	//������� ������ ������ ������� � �������� ��� �������-���������� � ������� ������ postEvent(). 
	eventsCipherWorker *ev = new eventsCipherWorker();//��������  ������� �  ����� ������ �� ������ ������,
	//�� �� �� ��������, ��� ��� ����� ��������� ��� ������� ������� ���������! 
	
	ev->setAllProcessVal(val);
	QApplication::postEvent(threadEvens, ev);
}

void CipherWorker::sendEventProcessFileInfo(string msg)
{
	if (IsDisabledEventProcess) return;

	eventsCipherWorker *ev0 = new eventsCipherWorker();//��������  ������� �  ����� ������ �� ������ ������,
													   //�� �� �� ��������, ��� ��� ����� ��������� ��� ������� ������� ���������! 
	ev0->setCurfileprocessInfo(msg);
	QApplication::postEvent(threadEvens, ev0);
}

void CipherWorker::sendEventThreadStatus(int status)
{
	if (IsDisabledEventProcess) return;
	//������� ������ ������ ������� � �������� ��� �������-���������� � ������� ������ postEvent(). 
	eventsCipherWorker *ev = new eventsCipherWorker();//��������  ������� �  ����� ������ �� ������ ������,
													  //�� �� �� ��������, ��� ��� ����� ��������� ��� ������� ������� ���������! 
	ev->setThreadStatus(status);//����� �������.
	QApplication::postEvent(threadEvens, ev);
}

void CipherWorker::sendEventCurFileProcess(int val)
{
	if (IsDisabledEventProcess) return;
	//�������� ��������� ������.
	eventsCipherWorker *ev1 = new eventsCipherWorker();
	ev1->setCurFileProcess(val);
	QApplication::postEvent(threadEvens, ev1);//������ �������.
}

void CipherWorker::sendMaxFileProcess(int val)
{
	if (IsDisabledEventProcess) return;
	//�������� ��������� ������.
	//������� ������ ������ ������� � �������� ��� �������-���������� � ������� ������ postEvent(). 
	eventsCipherWorker *ev = new eventsCipherWorker();//��������  ������� �  ����� ������ �� ������ ������,
													  //�� �� �� ��������, ��� ��� ����� ��������� ��� ������� ������� ���������! 
	ev->setMaxFileProcess(val);
	QApplication::postEvent(threadEvens, ev);
}

bool CipherWorker::CryptedSessionKey(uint8_t *session_key, string &result)
{
	//������
	//�������������� ������ ���������� ���������� � ������ 
	std::string mess_to_crypt(reinterpret_cast<char const*>(session_key), 32);

	RSAES_OAEP_SHA_Encryptor e(asymmetricKeyR); //�������� ���� ����������.
	AutoSeededRandomPool rng1; //���������� �� ���  ��������� seed
	string crypted_message = "";
	//PK_DecryptorFilter �������� ������ ��� �������, ���������� PK_Encryptor :: CreateEncryptionFilter. 
	//���� ����� ������������ ��������� � ������� VerifierFilter.
	StringSource ss1(mess_to_crypt, true, new PK_EncryptorFilter(rng1, e, new StringSink(crypted_message)) // PK_EncryptorFilter
	); // StringSource

	result = crypted_message;

	return false;
}

bool CipherWorker::DecryptEc_SK(Point & Q, mpz_class & d, QString Password, PkContainer &KeyContainer)
{
	try {

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

		string EcOid = KeyContainer.P24().toStdString();
		//�������� ������������ �������� ��� P24
		if (!CheckEcPair(Q1, d1, EcOid)) return false;

		d = d1; //������� �������� ����
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

//�������� ��������� ����������� ������ � ����
bool CipherWorker::HashedPassword(QString &Passwd, QString &salt_str, uint8_t *hash)
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
bool CipherWorker::DecryptSK(uint8_t *key, QString &CryptedKey, QString &iv, string &p_key)
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

//������ ���� �� ���������� ����
bool CipherWorker::CryptFile(QString src_path, QString dst_dir, Cipher3412 &Cipher, uint8_t *Reg, string &CryptedKey, 
	mpz_class &user_d, PkContainer &KeyContainer)
{
	QTextCodec *codec = QTextCodec::codecForName("Windows-1251");

	try
	{
		QString Error = "";
		QFileInfo finfo(src_path);
		QString file_name = finfo.fileName();
		//������ ����� � ������
		qint64 src_file_len = finfo.size(); //������ ��������� ����� � ������

		string mess_progress = "���������� ����� \"" + codec->fromUnicode(finfo.fileName()) + "\"";
		sendEventProcessFileInfo(mess_progress);

		//��� ��������� ��������� ������ ����� 16 ����!!!!!!!!! ��� ������� ����� �������
		if (src_file_len < 16)
		{
			lastError = "���� "+codec->fromUnicode(file_name)+ " ����� ������ ����� 16 ����. ���������� ����� �� ��������.";
			return false;
		}


		//������� ���� ��� ������,������ ����� ������� ������
		QFile fileIn(src_path);
		if (!fileIn.open(QIODevice::ReadOnly))
		{
			lastError = "�� ������� ������� ���� " + codec->fromUnicode(file_name) + " ��� ������.";
			return false;
		}
	

		QString dst_path = dst_dir + "/" + file_name + ".crypt";


		//������� ���� ��� ������, ���� ����� ������������ ����������� ����
		QFile fileOut(dst_path);
		//������� ������������ ����, ���� �� ����-����� ������������ �������� ����� ��������� ��������� ���
		fileOut.remove();
		
		if (!fileOut.open(QIODevice::ReadWrite))
		{
			lastError = "�� ������� ������� ���� " + codec->fromUnicode(file_name) + ".crypt ��� ������.";
			return false;
		}			


		uint8_t P[16]; //���� ��������� ����������� ����������
		uint8_t MSB[16];//��������� n �������� �������� ������ � �������� ��������
		uint8_t C_block[16]; //�������������� ����������
		uint8_t iv[32]; //��������� ������ ������������ � ��������
		uint8_t Flen[16];//���������-������ �����(��������������� �����) � ���� 16 ����
		char input_block[16]; //���� ������ ����������� �� ����� ����������� ����������

		qint64 readf_pos = 0;//������� ������� ������������ ����� �� �����
		qint64 writef_pos = 0;//������� ������������� ����� � ����
		bool GoodOpFlag = 0;//���� ���������� ���������� ��������


		//��������� ��������� ���������� �����, ���������� ��� � ����
		if (!WriteCryptFileTitle(fileOut, src_file_len, writef_pos)) return false;
		

		//������������ ������� � ����������� ����� � 0
		if (!fileIn.seek(0))
		{
			lastError = "�� ������� ������� ���� " + codec->fromUnicode(file_name) + " ��� ������.";
			return false;
		}


		//����� ������ �������� �����
		Gost3413 CBC;
		memcpy(iv, Reg, 32);//�������� �������� �������� � iv

							//���������� ��������� ������ �����
		CBC.FileLengthToArray(src_file_len, Flen);
		
		//���������� ������� �����
		CBC.getMSBfromR(Reg, MSB);//������� MSB �� ��������
		CBC.�ryptIterationCBC(&Cipher, Flen, MSB, C_block);
		//�������� ���� ����������� � ����
		GoodOpFlag = CBC.writeBlockC(writef_pos, C_block, fileOut);
		if (!GoodOpFlag)
		{			
				lastError = "������ 1 �� ������� ��������� ����.";
				return false;	
		}

		//����� �������� �����
		CBC.RegLeftShift(Reg, C_block);

		//���������� ������ � ����� ���������� ����������
		qint64 block_count = src_file_len / 16;
		//!!!!!!!!!!!!!!������������� �������� ����� ������ ����� ����� 16 ����!!!!!!!!!!!!!!!!1
		
		//�������� ��������� ������.
		 sendMaxFileProcess(block_count);//������� ������� � ������������ ��������.
				
		int progress_step = 1; //�� ������� ������ ���� �������� �������� ����
							   //���������� ��� ��������� ������ ���������-����� �������� ����������  ������� ��������� �� �������� �������� ���� 
		if (block_count > 10000) progress_step = 1000;

		for (int i = 0;i < block_count;i++)
		{
			//������������ ������� �������(����������� ����)
			if (!fileIn.seek(readf_pos))
			{
				lastError = "������ 2 �� ������� ������� ����";
				return false;
			}
			

			//�������� 16 ���� �� �����
			if (fileIn.read(input_block, 16) != 16)
			{
				lastError = "������ 3 �� ������� ������� ����";
				return false;
			}
						
			//������������� char � uint8_t
			CBC.copyFtoC(input_block, P);
			//�������� ����������
			CBC.getMSBfromR(Reg, MSB);//������� MSB �� ��������
			CBC.�ryptIterationCBC(&Cipher, P, MSB, C_block);//������ ����

			GoodOpFlag = CBC.writeBlockC(writef_pos, C_block, fileOut);//������ �����������
			if (!GoodOpFlag)
			{
				lastError = "������ 4 �� ������� ��������� ����";
				return false;
			}
		

			//����� �������� �����
			CBC.RegLeftShift(Reg, C_block);

			readf_pos += 16;
			
			if (i%progress_step == 0)
			{
				sendEventCurFileProcess(i+1); //������� ������� ��������� ��������� ������.
			}				

		}

		quint64 last_bytes = src_file_len % 16; //���������� ���������� ���� � �����
												//���� ��������� ���������� ���������
		if (last_bytes > 0)
		{
			//����� �������� ���������� ����
			//�������� x ���� �� �����
			if (fileIn.read(input_block, last_bytes) != last_bytes)
			{
				lastError = "������ 5 �� ������� ������� ����";
				return false;
			}
			

			uint8_t r_len = static_cast<uint8_t>(last_bytes);//����� ����������� ���������
															 //������� ���������� �����
			CBC.copyFtoC(input_block, P, r_len);
			//���������� ���������
			CBC.paddingMessage(P, r_len);
			//���������� ���������� �����
			//�������� ����������
			CBC.getMSBfromR(Reg, MSB);//������� MSB �� ��������
			CBC.�ryptIterationCBC(&Cipher, P, MSB, C_block);//������ ����
															//������ �����������
			GoodOpFlag = CBC.writeBlockC(writef_pos, C_block, fileOut);
			if (!GoodOpFlag)
			{
				lastError = "������ 6 �� ������� ��������� ����";
				return false;
			}

		}

		//������ ������� iv
		GoodOpFlag = CBC.writeBlockIV(writef_pos, iv, fileOut);
		if (!GoodOpFlag)
		{
			lastError = "������ 7 �� ������� ��������� ����";
			return false;
		}

		fileIn.close();

		//���������� ���������� ������������� �������
		AsInterpretator Ai;
		QByteArray AsBlock;
		Ai.Dh_Hash(AsBlock,rHashOkDH); //��� ��������� ����� ����������
		Ai.CryptedBlockKey(AsBlock, CryptedKey);//����������� ��������� ����
		writef_pos = fileOut.write(AsBlock, AsBlock.size());
		if (writef_pos != AsBlock.size())
		{
			lastError = "������ 16: �� ������� �������� ���� ";
			return false;
		}
		
		mess_progress = "������� ����� \"" + codec->fromUnicode(finfo.fileName()) + "\"";
		sendEventProcessFileInfo(mess_progress);

		
		qint64 cur_f_pos = fileOut.pos(); //������� ������� � �����

		//������ ����� 2��.   
		if (cur_f_pos >= 1900000000)
		{
			mess_progress = "���� \"" + codec->fromUnicode(finfo.fileName()) + "\" ����� 2��, �������� �� �����.";
			sendEventProcessFileInfo(mess_progress);

		}
		else
		{
			//-----------������� �����
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
			lastError = "� ������ CryptFile() �������� ���������� " + string(e.what());
			return false;	
	}

	return true;
}


//��������� ��������� ���������� �����, ���������� ��� � ����
bool CipherWorker::WriteCryptFileTitle(QFile &file, qint64 file_len, qint64 &file_pos)
{

	try {
		const uint8_t title1[7] = //������ 7 ���� ���������� ��� �� ����� ������
		{ 0xf9,0xc5, 0xa8, 0xd3, 0x47, 0xb6, 0x3a };

		//�������� ���������
		 string pgm_title = "GranitK Information security protection system ";

		 //������� �������������� �����
		//if(baseData.getRoseMode()) --������� ���� ���� �����!
			pgm_title = "I'll always have a little red rose in my heart";


		//�������� ��������� �����
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
		//���
		for (uint8_t i = 0;i < 64;i++)
		{
			file_title[pos] = 0;
			pos++;
		}

		//��������������� ����� ����� ������ � ������ �� 8 ����
		//����������� �������. ���� ������ ����� ������ 16: 16���� �����, 32 iv
		if (file_len % 16 == 0) file_len = file_len + 16 + 32;
		else
		{
			quint64 tmp1 = file_len % 16;
			file_len = file_len - tmp1;
			//��������� ����������� ������ 16 ����  ���� ��� ����� �� ������ 16
			file_len = file_len + 16 + 16 + 32;
		}
		//�������������� ����� ���������	
		for (uint8_t i = 0;i < 8;i++)
		{
			file_title[pos] = file_len & 0xFF;
			file_len = file_len >> 8;
			pos++;
		}

		//--------------������ ���������---------------
		//������������ ������� � pos
		if (!file.seek(0))
		{
			lastError = "�� ������� ������� ��������� �����.";
			return false;
		}
			

		uint8_t l = (uint8_t)file.write(reinterpret_cast<char *>(file_title), title_len);
		if (l != title_len)
		{
			lastError = "�� ������� ������� ��������� �����.";
			return false;
		}
			
		file_pos += title_len; //������� �������

	}
	catch (...)
	{
		lastError = "� ������ :WriteCryptFileTitle() �������� ����������";
		return false;
	}

	return true;
}

bool CipherWorker::SignFile(QFile & file, mpz_class & user_d, PkContainer &KeyContainer)
{
	try {

		qint64 cur_f_pos = file.pos(); //������� ������� � �����
		if (cur_f_pos == 0)
		{
			lastError = "������ Sg1: �� ������� �������� ���� ";
			return false;
		}
			

		//������������ ������� ������� ����� ����� ��� �����
		if (!file.seek(118))
		{
			lastError = "������ Sg2: �� ������� ��������� ����";
			return false;
		}
		
		
		//C������� ��� ���������� �����
		QByteArray file_content = file.read(cur_f_pos - 118); //�������� ���� ������  ������ ����� ���
		qint64 f_sz = file_content.size();
		if (f_sz != (cur_f_pos - 118))
		{
			lastError = "������ Sg3: �� ������� ��������� ����";
			return false;
		}
			
		//������ ��������� � ������
		unsigned long long mess_len = static_cast<unsigned long long>(f_sz);

		CreateSign SignGen; //������ ��� �������� �������
							//KeyContainer.P16()
		Groups EC2;
		bool findEc = EC2.SetParam(KeyContainer.P24().toStdString());
		if (!findEc)
		{
			lastError = "������ Sg3.1: �� ������� ���������� ��������� ������.";
			return false;
		}
		


		//������ ����� � ���������� � ������ ������������ ����� P
		Point P(EC2, 1); //����� ������������� ������ 
		Signature Sign;//�������
		Sign = SignGen.Sign(user_d, reinterpret_cast<uint8_t *>(file_content.data()), mess_len, P);



		//-----------------�������� �������----------------

		//���� �������� �������	
		Point Q1(EC2, 1); //����� ������������� ������ 

						  //�������� ������������ �������� ���� � ����������
		mpz_class secret_k = user_d; //�������� ���� ������������
		Point  Q2;
		Q2 = SignGen.generate_public_key(secret_k, P);
		//�������� ��������� ����� �� ����������
		string x1 = KeyContainer.P16().toStdString();
		string y1 = KeyContainer.P17().toStdString();
		//����������� ��������
		string x2 = Q2.get_hex_x();
		string y2 = Q2.get_hex_y();

		if (x1 != x2)
		{
			lastError = "������ Sg4: �� ������� ��������� ����";
			return false;
		}
			
		if (y1 != y2)
		{
			lastError = "������ Sg5: �� ������� ��������� ����";
			return false;
		}
			

		//����� ��������� ����� �������� �������
		Q1.x.set_str(x1, 16);
		Q1.y.set_str(y1, 16);

		//�������� �������
		VerifySign Vs;
		bool sign_status = Vs.Verify(reinterpret_cast<uint8_t *>(file_content.data()), mess_len, P, Q1, Sign);

		if (sign_status == 0)
		{
				lastError = "������ Sg6: �� ������� ��������� ����";
				return false;
		}

		file_content.clear();//������� ���������� ������� ������ � ������ ��� ������.

							 //----------------------------------------------
							 //���������� ������ �������
		AsInterpretator Ai;
		QByteArray AsBlock;
		Ai.Sign_r(AsBlock, Sign.get_hex_r());
		Ai.Sign_s(AsBlock, Sign.get_hex_s());
		//������ ��������� ����� ����������
		Ai.SetSignedIndex(AsBlock, signerIndex);


		//������������ ������� �������  � ����� �����
		if (!file.seek(cur_f_pos))
		{
			lastError = "������ Sg7: �� ������� ��������� ����";
			return false;
		}


		//�������� � ����
		cur_f_pos = file.write(AsBlock, AsBlock.size());
		if (cur_f_pos != AsBlock.size())
		{
			lastError = "������ Sg8: �� ������� �������� ���� ";
			return false;
		}

	}
	catch (exception& e)
	{		
			lastError = "� ������ SignFile �������� ����������";
			return false;
	}
	
	return true;
}


//��������� �������� ������������� ��������� ������
bool CipherWorker::DecryptFiles(QStringList &paths_to_file, QString DbPath, QString UserPassword,
	 PkContainer KeyContainer)
{
	try
	{
		QString Error = "";
		QString dst_Dir = "";//���� � �������� ���� ����� ����������� �������������� �����						 

		bool IsCreateDir = CreateDstDecryptDir(paths_to_file[0], dst_Dir); //������ ���������� ���� ����� ����������� �������������� �����
		if (!IsCreateDir) return false;
				
		int f_count = paths_to_file.count(); //���������� ������
		
		//���� �� ���� ������
		for (int i = 0;i < f_count;i++)
		{
			//�������� ��������� ������
			//������� ������� ��������� ��������� ������.
		  if(i!=0)sendEventAllProcessVal(i);

		
			//����������� ����
			if (!DecryptingFile(paths_to_file[i], dst_Dir, DbPath, UserPassword, KeyContainer))
				return false;
			
		}

	}
	catch (...)
	{
		lastError = "� ������ DecryptFiles �������� ����������.";
		return false;
	}

	return true;
}

//��������� ���� � �����, ������� ���������� ��� ���������� �������������� ������, ���������� ���� � ����������
bool CipherWorker::CreateDstDecryptDir(QString fpath, QString &DirPath)
{
	QFileInfo fi(fpath);
	DirPath = fi.absolutePath(); //encrypted files
	DirPath = DirPath + "/Decrypted_files";
	QDir dir(DirPath);
	if (!dir.exists())
	{
		if (!dir.mkpath("."))  return 0; //�� ������� ������� �������
	}
	else
	{
		/*
		   	�������� ������� ������ � �����. 
			��� ���������� �� ���� ��������.
			1. ���������� ��������� ��� ������� ������������ ����� �����  ���� ������ ��� �� ��������� �����,
			�������� �������� ��������� ��������. �.�. ���� ������������ ����� "��������" �� ��������� ������ ����� ���������
			�� �������� ���������.
			2. ������������ ����� �������� ����� � ����������� �������.
		*/

		QStringList files = dir.entryList();
		int readCount = 0; //���������� �������� ������.
		string FileNames = "";
		bool isFirst=true;

		/*
		  ������ ���������� ������. ��� ��� dir.entryList() ����� ���� ������ ���������� ��� �
		  "." � "..". �.�. ���� ���� ������� ����-��� ����� ����� ������ � ������.
		*/
		for (int i = 0;i < files.count();i++)
		{	
			if (files[i] == ".") continue;
			if (files[i] == "..") continue;

			if (!isFirst)FileNames = FileNames + "\r\n";

			//�������� ������ 3 �����.
			if(readCount<4)
			FileNames = FileNames + codec->fromUnicode(files[i]).toStdString();

			isFirst = false;
			readCount++;
		}


		if (readCount > 0)
		{
			lastError = "������� ���� ����� ������������ ����� " +
				codec->fromUnicode(DirPath).toStdString() + " �������� �����:\r\n" +
				FileNames + ".\r\n ������� ���������� ��� ������� ����� �������������� �����.";
			return false;
		}

	}

	if (!dir.exists()) 
	{
		lastError = "�� ������� ������� ������� �������� �������������� ������.";
		return false;
	}

	return 1;
}

bool CipherWorker::DecryptingFile(QString src_path, QString dst_dir, QString DbPath,
	QString UserPassword, PkContainer &KeyContainer)
{
	try
	{
		 sendEventCurFileProcess(0);//��������� ���������� �������� �������� ��������� �����.

		QString Error = "";
		QFileInfo src_file_info(src_path);
		QString src_file_name = src_file_info.fileName(); //��� ��������� �����
		

		//������� ���� ��� ������,������ ����� ������� ������
		QFile src_file(src_path);
		if (!src_file.open(QIODevice::ReadOnly))
		{
			lastError = "������ 8: �� ������� ������� ���� "+codec->fromUnicode(src_file_name)+ " ��� ������.";
			return false;
		}

		

		//������ ".crypt" �� ����� ����� 
		QString file_name = src_file_name.left(src_file_name.length() - 6);
		QString dst_path = dst_dir + "/" + file_name;

		//������� ���� ��� ������, ���� ����� ������������ �������������� ����
		QFile fileOut(dst_path);
		if (!fileOut.open(QIODevice::WriteOnly))
		{
			lastError = "������ 9: �� ������� ������� ���� "+codec->fromUnicode(file_name)+ " ��� ������.";
			return false;
		}
				

		//�������� ��������� ������������� �������-����� ������ ������ �� ����������� ����������
		ASreader ASr;
		if (!ASr.Read(src_file, src_file_info.size()))
		{
			lastError = "������ 9A: �� ������ ������ � ����� " + codec->fromUnicode(file_name);
			return false;
		}
			

		string mess_progress = "�������� ������� ����� \"" + codec->fromUnicode(src_file_name) + "\"";
		sendEventProcessFileInfo(mess_progress);

		qint64 flen_ = src_file_info.size();//������ �����.

		if (flen_ >= 1900000000)
		{
			string mess_progress = "����� \"" + codec->fromUnicode(src_file_name) + "\" ����� 1,9��, ������� ���.";
			sendEventProcessFileInfo(mess_progress);
		}
		else
		{
			//�������� �������.
			if (!CheckSign(ASr, src_file, DbPath, KeyContainer))
			{
				return false;
			}
		}
		


		//����������� ����� �������� ����� �������������� ������������� ����������.
		uint8_t secretKey[32];
		if(!ASdecryptSecretKey(ASr, secretKey, UserPassword, KeyContainer))
			return false;
		

		Cipher3412 Cipher; //������ ���������� ������� ������ �������� ����������
		Cipher.deploymentDecryptionRoundKeys(secretKey); //������������� ���������� �����  


		uint8_t P[16]; //���� ��������� ����������� ����������
		uint8_t MSB[16];//��������� n �������� �������� ������ � �������� ��������
		uint8_t C_block[16]; //�������������� ����������
		uint8_t tmp_block[16]; //������ ��������� ������
		uint8_t Reg[32]; // ������� �������� m = kn =  2*16
		uint8_t Flen[16];//���������-������ ����� � ���� 16 ����
		char input_block[16]; //���� ������ ����������� �� ����� ����������� �������������
		uint8_t iv[32]; //��������� ������ ��������� �� �����
		qint64 block_count;// = file_len / 16; //���������� ������ ������������ � ���������

		qint64 readf_pos = 0;//������� ������� ������������ ����� �� �����
		qint64 writef_pos = 0;//������� ������������� ����� � ����
		bool GoodOpFlag = 0;//���� ���������� ���������� ��������
		qint64 src_file_len = 0;//����� ��������������� �����
		qint64 data_len = 0;//����� ����� ������ 
		uint8_t  r_len = 0;//������� � ��������� �����

						   //����� ������ �������� �����
		Gost3413 CBC;

		//������� �������� ���������� ������� IV
		//src_file_len = src_file_info.size(); //������ �����
		//readf_pos = src_file_len - 32;//������� ��� ���������� iv

		//��������� �������� ������� iv �� ��������� �������
		GoodOpFlag = CBC.readIV(iv, src_file);
		if (!GoodOpFlag)
		{
			lastError = "������ 10: �� ������� ������� ���� "+codec->fromUnicode(src_file_name);
			return false;
		}
		

		//C������� ������ 16 ����, ����� ���������
		readf_pos = 118 + 8;//��������� �� ������ ����� ������
		GoodOpFlag = CBC.readFblock(readf_pos, input_block, src_file);
		if (!GoodOpFlag)
		{
			lastError = "������ 11: �� ������� ������� ���� "+ codec->fromUnicode(src_file_name);
			return false;
		}
	


		//�������������� ������
		CBC.copyFtoC(input_block, C_block);
		//���������� �������� ������� IV
		memcpy(Reg, iv, 32);

		//������� ������������ ����� ���������
		CBC.getMSBfromR(Reg, MSB);//������� ������� ����� MSB �� ��������
		memcpy(tmp_block, C_block, 16);//������� � ,��� ��� ��� ����������
		CBC.de�ryptIterationCBC(&Cipher, P, MSB, C_block);//�������������
														  //����� �������� �����, ������� � ������� ����������� ���������
		CBC.RegLeftShift(Reg, tmp_block);
		//�������������� ������ � ����� ���������
		CBC.ArrayToFileLength(data_len, P); //���������� data_len-����� �������������� �����
											//�������� �������� ������� ������!!!!!!

											//����� ����� �� ���������� 16, IV ������� IV
		block_count = data_len / 16; //���������� ������ � ���������

									 //�������� ��������� ������.
		sendMaxFileProcess(block_count);//������� ������� � ������������ ��������.

		int progress_step = 1; //�� ������� ������ ���� �������� �������� ����
							   //���������� ��� ��������� ������ ���������-����� �������� ����������  ������� ��������� �� �������� �������� ����
							   //��� 200�� ��� ������ ���������
		if (block_count > 10000) progress_step = 1000;
		
		 mess_progress = "��������������� ����� \"" + codec->fromUnicode(src_file_name) + "\"";
		sendEventProcessFileInfo(mess_progress);

		//���� ��������������� ������										   
		for (qint64 i = 0;i < block_count;i++)
		{
			//�������� ��������� ����
			GoodOpFlag = CBC.readFblock(readf_pos, input_block, src_file);
			if (!GoodOpFlag)
			{
				lastError = "������ 12: �� ������� ������� ���� "+codec->fromUnicode(src_file_name);
				return false;
			}
			

			CBC.copyFtoC(input_block, C_block); //�������������� ������

			CBC.getMSBfromR(Reg, MSB);//������� ������� ����� MSB �� ��������
									  //������� ���� ������, ��� ��� ���������� ���������� C_block
			memcpy(tmp_block, C_block, 16);
			CBC.de�ryptIterationCBC(&Cipher, P, MSB, C_block);//�������������

			GoodOpFlag = CBC.writeBlockC(writef_pos, P, fileOut); //��������� �������������� ���� � ����
			if (!GoodOpFlag)
			{
				lastError = "������ 13: �� ������� �������� ���� ";
				return false;
			}
				

			//����� �������� �����, ������� � ������� ����������� ���������
			CBC.RegLeftShift(Reg, tmp_block);

			if (i%progress_step == 0) sendEventCurFileProcess(i+1);//������� ������� ��������� ��������� �����. 
		}

		//������ �� ����� ��������� ������� ����� �����?
		r_len = static_cast<uint8_t>(data_len % 16);
		//����� ��������� �� ������ ������� ����� �����
		if (r_len != 0)
		{
			//�������� ��������� ����
			GoodOpFlag = CBC.readFblock(readf_pos, input_block, src_file);
			if (!GoodOpFlag)
			{
				lastError = "������ 14: �� ������� ������� ���� ";
				return false;
			}
				

			CBC.copyFtoC(input_block, C_block); //�������������� ������
			CBC.getMSBfromR(Reg, MSB);//������� ������� ����� MSB �� ��������				  
			memcpy(tmp_block, C_block, 16); //������� ���� ������, ��� ��� ���������� ���������� C_block
			CBC.de�ryptIterationCBC(&Cipher, P, MSB, C_block);//�������������

			GoodOpFlag = CBC.writeBlock(writef_pos, P, fileOut, r_len);//��������� �������������� ���� � ����
			if (!GoodOpFlag)
			{
				lastError = "������ 15: �� ������� �������� ���� ";
				return false;
			}
				
		}
		src_file.close(); //������ ����������� ����.

		fileOut.close();
	}
	catch (...)
	{
		lastError = "� ������ DecryptingFile() �������� ����������";
		return false;
	}

	return true;
}



bool CipherWorker::CheckSign(ASreader & ASr, QFile & file, QString DbPath, PkContainer &KeyContainer)
{
	try {
		QString Error = "";
		QByteArray s, r; //������� ������� ���������� �� �����
		string  k_index;
		string Qx, Qy; //���������� ��������� ����� � 16 ������ �����
		QString FIO;


		//��������� �������� ������� �� ������ 116,117
		if (!ASr.GetSignVector(r, s)) //�� ������� �������� ����� ����  ������ �� ������
		{
			lastError = "������ ��0:� ����� ����������� ����� �������. ";
			return false;
		}
		

		//��������� "�������" ��������� �����  ���������� �� �����
		if (!ASr.GetSignKeyIndex(k_index))
		{
			lastError = "������ ��1:� ����� ����������� �������� �� �������� ����� �������� �������.";
			return false;
		}
		

		//�������� ������� �����.
		if (!baseData.getRoseMode())
		{
			//��������� ���������� � ����� ������
			GranitDbI DBI(DbPath);
			if (!DBI.OpenDb())
			{
				lastError = "�� ������� ������� ���� ������.";
				return false;
			}


			//�������� ���� �������� ��.
			if (!DBI.checkDbDateEnd())
			{
				lastError = "������: ����� ���� �������� �� ����.";
				return false;
			}


			//��������� ������ ������� �������� ���� ���������� �� �� � ��� ���
			if (!DBI.getSignOpenKey(k_index, Qx, Qy, FIO))
			{
				lastError = "Error";
				return false;
			}
		}
		else
		{
			Qx=KeyContainer.P16().toStdString(); //�������� ����  ���������� X
			Qy=KeyContainer.P17().toStdString(); //�������� ����  ���������� Y
		}
					
		//-----------------��������� ������ ��� �������------------------	

		//������������ ������� ������� ����� ����� ��� �����
		if (!file.seek(118))
		{
			lastError = "������ Vs2: �� ������� ��������� ������� �����.";
			return false;
		}
	

		qint64 end_sign_data_pos; //������� ������ �������� ����� �������,� ����� ����������� ������
		end_sign_data_pos = ASr.begin_sign_block;
		end_sign_data_pos = end_sign_data_pos - 118; //���������� ������ ������� ���������� ������� �� �����

													 //�������� ��� ����������� ���������� �����
		QByteArray file_content = file.read(end_sign_data_pos);
		qint64 f_sz = file_content.size(); //������ ���������� ����� ������

		if (f_sz != end_sign_data_pos)
		{
			lastError = "������ Vs3: �� ������� ��������� ������� �����";
			return false;
		}
			
		//������ ��������� � ������
		unsigned long long mess_len = static_cast<unsigned long long>(f_sz);

		//�������������� �������� QByteArray � mpz_class
		mpz_class r_;
		mpz_class s_;

		size_t r_len = static_cast<size_t>(r.size());
		size_t s_len = static_cast<size_t>(s.size());

		mpz_import(r_.get_mpz_t(), r_len, 1, 1, 0, 0, reinterpret_cast<uint8_t *>(r.data()));
		mpz_import(s_.get_mpz_t(), s_len, 1, 1, 0, 0, reinterpret_cast<uint8_t *>(s.data()));

		Signature Sign; //������ ���������� ������� �����
		Sign.set_r(r_);
		Sign.set_s(s_);
		Sign.set_type(1); //512���

		Groups EC2;
		bool FindEc = EC2.SetParam(KeyContainer.P24().toStdString()); //����� ��������� ������ �� ��������� OID.
		if (!FindEc)
		{
			lastError = "�� ������ �������� ������.";
			return false;
		}
			

		//������ ����� � ���������� � ������ ������������ ����� P
		Point P2(EC2, 1); //����� ������������� ������ 
		Point Q1(EC2, 1); //����� ������������� ������ 

						  //������������ ���������� ��������� �����
		Q1.x.set_str(Qx, 16);
		Q1.y.set_str(Qy, 16);


		//�������� �������
		VerifySign Vs;
		bool sign_status = Vs.Verify(reinterpret_cast<uint8_t *>(file_content.data()), mess_len, P2, Q1, Sign);

		if (sign_status == 0)
		{
			lastError = "�� ������ ������� �����. ���� ��� ��������� ��� ������� ������� �����";
			return false;
		}
		


	}
	catch (exception& e)
	{
  	  lastError = "� ������ CheckSign �������� ����������" +string(e.what());
	  return false;
	}

	return true;
}

//����������� ����� �������� �����
bool CipherWorker::ASdecryptSecretKey(ASreader &Data, uint8_t *key, QString Password, PkContainer &KeyContainer)
{
	try {

		RSA::PublicKey publicKey;
		RSA::PrivateKey secret_key;

		//������������� ��������� ���� ��. ������� ��� ���������, ���� ������ �� ������ -���� �� �������� ����� ��������� 
		if (!DecryptDH_SK(publicKey, secret_key, Password,KeyContainer))
		{
			lastError = "������ AS1: �� ������ ������.";
			return false;
		}
		


		//����� ����������� �������� ����� �������� �����
		string crypted_message;
		//string reciver_hash; //������������ �������� ��������� ����� ����������
		//���� �� ���� ������ 
		bool found = 0;
		for (int i = 0;i < Data.Blocks.count();i++)
		{
			//���� ������ ���� ���������� ����������� ��������� ����
			if (Data.Blocks[i].type == 115)
			{
				//�������������� � ������
				string t1(Data.Blocks[i].data.data(), Data.Blocks[i].data.size());
				crypted_message = t1;
				found = 1;
				break;
			}
		}
		//���� �� ������
		if (found == 0)
		{
			lastError = "������ AC2: ����������� ���� ������.";
			return false;
		}
		


		//�������������	
		string decrypted_message;
		RSAES_OAEP_SHA_Decryptor decryptor(secret_key);
		AutoSeededRandomPool rng2; //���������� �� ���  ��������� seed
								   //� ������ ���� �� ������� ������������ ��������� ��������� ����������
		try
		{
			StringSource ss2(crypted_message, true, new PK_DecryptorFilter(rng2, decryptor, new StringSink(decrypted_message)
			) // PK_DecryptorFilter
			); // StringSource
		}
		catch (...)
		{
			lastError = "������ AS3: �� ������� ������������ ��������� ����. �������� ���� ��� ������������ �� ���.";
		    return false;
			
		}


		//��������� ���� ������ �������� �� 32-x ����
		if (decrypted_message.length() != 32)
		{
			lastError = "������ AS4: ���� �������������� ����������.";
			return false;
		}

		//������� ������ � ������
		for (uint8_t i = 0;i < 32;i++)
		{
			key[i] = static_cast<uint8_t>(decrypted_message[i]);
		}


	}
	catch (exception& e)
	{
		lastError = "� ������  ASdecryptSecretKey �������� ����������: "+string(e.what());
		return false;
	}

	return true;
}

//�������������� �������� ���� ����� �����-�������
bool CipherWorker::DecryptDH_SK(RSA::PublicKey &publicKey, RSA::PrivateKey &secret_key, QString Password, PkContainer &KeyContainer)
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

//��������� ���� ������ ����� �����-�������. ���� ���� �� ��������� ���������� 0
bool CipherWorker::GetDhPair(QString &open_key, string &private_key, RSA::PublicKey &publicKey, RSA::PrivateKey &secret_key)
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
		lastError = "������ K531D";
		return false;
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


	return true;
}
