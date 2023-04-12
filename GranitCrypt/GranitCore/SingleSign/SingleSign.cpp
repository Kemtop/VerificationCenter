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
	//�������� �������.
	CreateSign SignGen; //������ ��� �������� �������

	Groups EC2;
	string Ecparam = "";
	if (!ks->getEcParam(Ecparam))
	{
		lastError = "������ Sg3.1A: �� ��� �������� ����. ������� ��������� ����.";
		return false;
	}
	
	bool findEc = EC2.SetParam(Ecparam); //��������� ���������� ������.
	if (!findEc)
	{
		lastError = "������ Sg3.1: �� ������� ���������� ��������� �������.";
		return false;
	}
		
	
	//�� ���������� ���� �� ����� �������� ������? �� ��� �������� ������, � ������������� �� ������� �������� ����.
	if (!ks->isCacheSk())
	{
		lastError = "������ Sg03.1: �� ��� �������� ������ �����.";
		return false;
	}

	//������ ����� � ���������� � ������ ������������ ����� P
	Point P(EC2, 1); //����� ������������� ������ 
	Signature Sign;//�������

	//���������� ���������.
	mpz_class  user_d = ks->getSignPk();
	//������ ��������� � ������
	unsigned long long mess_len = static_cast<unsigned long long>(Mlen);
	Sign = SignGen.Sign(user_d, Message, mess_len, P);

	
	//-----------------�������� �������. �� ������ ���������-�����������!----------------

	//���� �������� �������	
	Point Q1(EC2, 1); //����� ������������� ������ 

	//�������� ������������ �������� ���� � ����������
	mpz_class secret_k = user_d; //�������� ���� ������������
	Point  Q2;
	Q2 = SignGen.generate_public_key(secret_k, P);

	//�������� ��������� ����� �� ����������
	string x1 ="";
	string y1 = "";
	ks->getSignOpenKey(x1,y1);


	//����������� ��������
	string x2 = Q2.get_hex_x();
	string y2 = Q2.get_hex_y();

	if (x1 != x2)
	{
		lastError = "������ Sg4: �� ������� ��������� ����.";
		return false;
	}

	if (y1 != y2) 
	{
		lastError = "������ Sg5: �� ������� ��������� ����.";
		return false;
	}
	

	//����� ��������� ����� �������� �������
	Q1.x.set_str(x1, 16);
	Q1.y.set_str(y1, 16);

	//�������� �������
	VerifySign Vs;
	bool sign_status = Vs.Verify(Message, mess_len, P, Q1, Sign);

	if (sign_status == 0)
	{
    	lastError = "������ Sg6: �� ������� ��������� ����.";
		return false;	
	}


	//������������ ������� �������� ASN 1.

	//������� ���������� ����������������� �����������.
	TBSCertificate  tbsUserCertificate = ks->getUserCert();

	//������ CMS.
	rfcSignedData sd; 
	sd.CMSVersion = 1; //����� ������ ���������� CMS.
	sd.digestAlgorithms = "1.2.643.7.1.1.2.3"; //������������� ��������� ������� ��������� ���������.
	sd.commonName = tbsUserCertificate.Subject_id_at_commonName; //�����������.
	sd.certNum = mpz_get_ui(tbsUserCertificate.serialNumber.get_mpz_t()); //����� ����������� ����������.
	sd.signingCertDigestAlg = "1.2.643.7.1.1.2.3"; //�������� ��������� ����������� ����������.

	 //�������� ��������� ����������� ���������� 64���.
	string str_certDiges = tbsUserCertificate.digest;
	QByteArray baDgst = QByteArray::fromHex(str_certDiges.c_str());

	std::vector<unsigned char> vbaDgst(
		baDgst.begin(), baDgst.end());										  
	
	sd.signingCertDigest = vbaDgst;
					
	sd.signingTime = QDateTime::currentDateTime(); //����� ������������ �������.

	//�������� �������� ���������.
	sd.digest = SignGen.getLastDigest();
									 
	
	sd.signatureAlgorithm = "1.2.643.7.1.1.1.2"; //�������� ���.
	
	//���� ���.
    string sxR=Sign.get_hex_r();
	string sxS = Sign.get_hex_s();


	//������ ������ ������ ��������� 128��������. �������� ������ � ����.

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
		 lastError = "������ Sg7R: �� ������� ��������� ����. ������ r ����� �� ������ �����:"
			 +std::to_string(sxR.length())+" ��������. ������ ���� 128.";
		 return false;
	 }


	 if (sxS.length() != 128)
	 {
		 lastError = "������ Sg7S: �� ������� ��������� ����. ������ s ����� �� ������ �����:"
			 + std::to_string(sxS.length()) + " ��������. ������ ���� 128.";
		 return false;
	 }

	 //�������� �������� ������.
	 string signData = sxR + sxS;	 
	 QByteArray das = QByteArray::fromHex(signData.c_str());
		
	 for (int z = 0;z < das.size();z++)
	 {
		 sd.signVal.push_back(das[z]);
	 }
	
	 
	//�������� ASN1 ������������� �������.
	binASN1SignedData CS;
	QByteArray outBa = CS.createSignedBin(&sd);

	QByteArray ba_as_hex_string = outBa.toHex(); //16 ������ �������������.

	
	/*
	//��� ������. ��������� ������� � ����.
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
	//������ � ���������� �������� ������.
	QByteArray inData = QByteArray::fromHex(SignValue.c_str());
	
	X509Reader X509R;
	rfcSignedData sD;
	if (!X509R.readSignData(sD, inData))
	{
		lastError = X509R.getLastError();
		return false;
	}
		

	//16 ������ ������������� ��������� ����������� ����������.
	QByteArray  hexCertDigest(reinterpret_cast<const char*>(sD.signingCertDigest.data()), sD.signingCertDigest.size());

	std::string strCertDigest = hexCertDigest.toHex().toStdString(); //��������� ����������� ����������.
	string �ertnum = std::to_string(sD.certNum); //����� ����������� ����������.


	QByteArray certData; //���������� ����������.
	bool errFlag = dBi->findUserCert(strCertDigest, �ertnum, certData);
	
	//�� ������ ����������� ����������.
	if (certData.size() == 0)
	{
		lastError = "������ �hSg001: � �� ������� �� ������ ����������� ����������. ����� �����������:"+ �ertnum+
			" ��������� �����������:"+ strCertDigest;
		return false;
	}

	X509Reader sR;

	//����� ���� ������ � �������������� ��� � TBSCertificate.
	TBSCertificate dataTbs;
	sR.ReadBa(certData,dataTbs);

	//������ ������ �� ��������� �������������� ������������� � �����������
	Groups EC2;
	bool ec_found;//���� ���������� ������ �� OID
	ec_found = EC2.SetParam(dataTbs.digestParamSet); //�� ��������� OID ��������� ����� ����� ��������� ������
	if (!ec_found)
	{
		lastError= "������� �� �������� ������������� ������ � OID=" + dataTbs.digestParamSet;
		return false;
	}
			

	//������ ����� � ���������� � ������ ������������ ����� P
	Point P2(EC2, 1); //����� ������������� ������ 

	Point Q1(EC2, 1);//������  ���� �������� �������. 
	Q1.x = dataTbs.Qx; //������� ���������� ����� �������� �������.
	Q1.y = dataTbs.Qy; //
			

	Signature Sign; //������ ���������� ������� �����

	//������� �������� ������.
	if(!Sign.parceBin(&sD.signVal))
	{
		lastError = Sign.getLastError();
		return false;
	}


	//�������� �������
	VerifySign Vs;
	unsigned long long mess_len = static_cast<unsigned long long>(Mlen); //����� ���������.
	bool sign_status = Vs.Verify(Message, mess_len, P2, Q1, Sign);

	if (sign_status == 0)
	{
		lastError= "�� ������ �������.";
		return false;
	}
	else
	{
		//������� ��� ����������.
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
