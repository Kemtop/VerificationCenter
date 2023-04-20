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
	err = GetKeyLen(tbs, KeyLen); //�������� ��������� �����������, � ��������� ����� �����.
	if (!err.empty()) return err; //�������� ����� �� ������.
								
	err = GetHasLen(tbs, HashLen);
	if (!err.empty()) return err; //�������� ����� �� ������.

	if (KeyLen != HashLen)
		return "�������� ���������� ��� ������� �� ��������� � ���������� ��������� �����.";
	
	//�������� ������������� ������
	//1.2.643.7.1.2.1.2.1
	//if (!((pubkey.E�OID == "1.2.643.7.1.2.1.2.1") || (pubkey.E�OID == "1.2.643.7.1.2.1.2.2")))
	

	Signature Sign; //������ ���������� �������.
	Sign.set_s(tbs.GetS()); //������� �������
	Sign.set_r(tbs.GetR());
	Sign.set_type(1); //512���

	//������ ������ �� ��������� �������������� ������������� � �����������
	Groups EC2;
	bool ec_found;//���� ���������� ������ �� OID
	ec_found = EC2.SetParam(tbs.digestParamSet); //�� ��������� OID ��������� ����� ����� ��������� ������
	if (!ec_found)
		return "������� �� �������� ������������� ������ � OID=" + tbs.digestParamSet;
	
	//������ ����� � ���������� � ������ ������������ ����� P
	Point P2(EC2, 1); //����� ������������� ������ 
	//�������� ����-����� ������������� ������ 
	Point Q1(EC2,1);
	Q1.x = tbs.Qx; //������� ���������� ���������� �� �����������
	Q1.y = tbs.Qy;
	
	//������� �������� ���� ��� �������������� ��� ��������
	tbs.Qx = Q1.x;
	tbs.Qy = Q1.y;
	
	string a1 = Q1.get_hex_x();
	string a2 = Q1.get_hex_y();
		
	vector<unsigned char> tbsBin; //�������� ������������� tbs �����������.
	tbsBin = tbs.GetBinTbs(); //������� ������ �� �����������.
							
	unsigned long long l = static_cast<unsigned long long>(tbsBin.size());//���������� ���� � TBS �����������

	//������� �������� ������� ������� ����������� ��� ����������� ��� �������� �����������.
	tbs.CerR = Sign.get_hex_r();
	tbs.CerS = Sign.get_hex_s();
		
	VerifySign Vs;  //�������� �������
	bool sign_status = Vs.Verify(&tbsBin[0], l, P2, Q1, Sign);
	SignStatus = sign_status; //��������� �������
	tbs.calcR = Vs.GetLastR(); //������� ��������  ��� ����������� ��� �������� �����������.
	tbs.digest = Vs.GetLastDigest();
	return "";
}

string X509Reader::VerifiCertSign(TBSCertificate & tbs, TBSCertificate & PubKeyTbs, bool & SignStatus)
{
	string err;
	int  KeyLen;
	int HashLen;
	err = GetKeyLen(tbs, KeyLen); //�������� ��������� �����������, � ��������� ����� �����.
	if (!err.empty()) return err; //�������� ����� �� ������.

	err = GetHasLen(tbs, HashLen);
	if (!err.empty()) return err; //�������� ����� �� ������.

	if (KeyLen != HashLen)
		return "�������� ���������� ��� ������� �� ��������� � ���������� ��������� �����.";

	//�������� ������������� ������
	//1.2.643.7.1.2.1.2.1
	//if (!((pubkey.E�OID == "1.2.643.7.1.2.1.2.1") || (pubkey.E�OID == "1.2.643.7.1.2.1.2.2")))


	Signature Sign; //������ ���������� �������.
	Sign.set_s(tbs.GetS()); //������� �������
	Sign.set_r(tbs.GetR());
	Sign.set_type(1); //512���

					  //������ ������ �� ��������� �������������� ������������� � �����������
	Groups EC2;
	bool ec_found;//���� ���������� ������ �� OID
	ec_found = EC2.SetParam(tbs.digestParamSet); //�� ��������� OID ��������� ����� ����� ��������� ������
	if (!ec_found)
		return "������� �� �������� ������������� ������ � OID=" + tbs.digestParamSet;

	//������ ����� � ���������� � ������ ������������ ����� P
	Point P2(EC2, 1); //����� ������������� ������ 
					  
	Point Q1(EC2, 1);//������  ���� �������� �������. 
	Q1.x = PubKeyTbs.Qx; //������� ���������� ����� �������� �������.
	Q1.y = PubKeyTbs.Qy; //

	vector<unsigned char> tbsBin; //�������� ������������� tbs �����������.
	tbsBin = tbs.GetBinTbs(); //������� ������ �� �����������.

	unsigned long long l = static_cast<unsigned long long>(tbsBin.size());//���������� ���� � TBS �����������

	//������� �������� ������� ������� ����������� ��� ����������� ��� �������� �����������.
	tbs.CerR = Sign.get_hex_r();
	tbs.CerS = Sign.get_hex_s();

	VerifySign Vs;  //�������� �������
	bool sign_status = Vs.Verify(&tbsBin[0], l, P2, Q1, Sign);
	SignStatus = sign_status; //��������� �������
	tbs.calcR = Vs.GetLastR(); //������� ��������  ��� ����������� ��� �������� �����������.
	tbs.digest = Vs.GetLastDigest();
	return "";

}



//��������� �� ������� ���������� ���������� � base64.
QByteArray X509Reader::TrFromBase64(QByteArray cer)
{
	QByteArray ba_cert; //���������� ��������������� �����������
	
	//���������� ����� ���� PEM �������, �������� ���.
	int y = cer.indexOf("-----BEGIN CERTIFICATE-----"); //27��������


	if (y != -1) //���������� PEM �������
	{
		int end_pos = cer.indexOf("-----END CERTIFICATE-----"); //������ ��������� ������� ��� �������
		if (end_pos == -1)
		{
		   //	"���������� ����������� � PEM ���������. �� ���������� ��������� ����������� -----END CERTIFICATE----- ";
			throw "Bad cert format.";
		}

		QByteArray ba_base64;
		//������� ����� � �������
		for (int i = 27;i < end_pos;i++)
		{
			ba_base64.append(cer[i]);
		}

		ba_cert = QByteArray::fromBase64(ba_base64); //����������� �� Base64
	}
	else //���� � ������� DER
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
		return "�� ���� ������� ���� ����������� �� ���������� ����.";
	}
	
	crtfile.seek(0);//������������ � ������� ������� �� ������ ������.
	QByteArray bf = crtfile.readAll(); //�������� ��� ��� ���� � �����
	crtfile.close();

	QByteArray ba_cert;
	ba_cert=TrFromBase64(bf);//��������� ���������� �� base64.
	
	//�������� �������� ������� �����������, �������� �������� tbs,�������� �������� �������.
	GetBinSignInfo(ba_cert, dataTbs);
	GetTbsInfo(ba_cert, dataTbs);//�������� �������� ������ TBS
	dataTbs.Tr512Sign(); //�������������� �������� ������� � ������� R,S.

	return "";
}

void X509Reader::ReadBa(QByteArray ba, TBSCertificate & dataTbs)
{	
	QByteArray ba_cert;
	ba_cert = TrFromBase64(ba);//��������� ���������� �� base64.

							   //�������� �������� ������� �����������, �������� �������� tbs,�������� �������� �������.
	GetBinSignInfo(ba_cert, dataTbs);
	GetTbsInfo(ba_cert, dataTbs);//�������� �������� ������ TBS
	dataTbs.Tr512Sign(); //�������������� �������� ������� � ������� R,S.

}

string X509Reader::CheckRootSign(QString path, TBSCertificate &dataTbs, bool &SignStatus)
{
	SignStatus = false; //������ ������ ��� ������� �� �����
	
	QFile crtfile;
	crtfile.setFileName(path);
	if (!crtfile.open(QIODevice::ReadOnly))
	{
		return "�� ���� ������� ���� ����������� �� ���������� ����.";
	}
		

	crtfile.seek(0);
	QByteArray bf = crtfile.readAll(); //��� ��� ���� � �����
	crtfile.close();

	QByteArray ba_cert;
	ba_cert = TrFromBase64(bf);//��������� ���������� �� base64.
		
	vector<unsigned char> tbs;
	string signatureAlgorithm = ""; //�������� �������
	vector<unsigned char> signatureValue; //������� �����������
	
    //������� ������ ��� �������� �������
	GetInfoForVerifi(ba_cert, tbs, signatureAlgorithm, signatureValue);

	//�������� �������� ����
	ByteQueue CerBuf; //������ ������ ������ �����������
	CerBuf.Put(reinterpret_cast<uint8_t*>(ba_cert.data()), ba_cert.size());
	PubKeyData pubkey; //������ ���������� ����� ���������� �� �����������.
	GetPublicKeyFromCert(CerBuf, pubkey, dataTbs);

	//�������� ������� �����������
	pubkey.signatureAlgorithm = signatureAlgorithm;
	//�������� ������� �����������
	string err = VerifiSign(pubkey, signatureValue, tbs, dataTbs, SignStatus);
	if (!err.empty()) return err;

	return "";
}


//��������� ������� ��������� ����������� � ���� �������. 
string X509Reader::CheckRootSign(QByteArray bf, TBSCertificate &dataTbs, bool &SignStatus)
{
	SignStatus = false; //������ ������ ��� ������� �� �����
		
	QByteArray ba_cert;
	ba_cert = TrFromBase64(bf);//��������� ���������� �� base64.

	vector<unsigned char> tbs;
	string signatureAlgorithm = ""; //�������� �������
	vector<unsigned char> signatureValue; //������� �����������

										  //������� ������ ��� �������� �������
	GetInfoForVerifi(ba_cert, tbs, signatureAlgorithm, signatureValue);

	//�������� �������� ����
	ByteQueue CerBuf; //������ ������ ������ �����������
	CerBuf.Put(reinterpret_cast<uint8_t*>(ba_cert.data()), ba_cert.size());
	PubKeyData pubkey; //������ ���������� ����� ���������� �� �����������.
	GetPublicKeyFromCert(CerBuf, pubkey, dataTbs);

	//�������� ������� �����������
	pubkey.signatureAlgorithm = signatureAlgorithm;
	//�������� ������� �����������
	string err = VerifiSign(pubkey, signatureValue, tbs, dataTbs, SignStatus);
	if (!err.empty()) return err;

	return "";
}

string X509Reader::CheckCASign(QByteArray baCA, TBSCertificate & tbsCA, TBSCertificate & TbsRoot, bool & SignStatus)
{
	SignStatus = false; //������ ������ ��� ������� �� �����

	QByteArray ba_cert;
	ba_cert = TrFromBase64(baCA);//��������� ���������� �� base64.
	
	//�������� �������� ������� �����������, �������� �������� tbs,�������� �������� �������.
	GetBinSignInfo(ba_cert, tbsCA);
	GetTbsInfo(ba_cert, tbsCA);//�������� �������� ������ TBS
	tbsCA.Tr512Sign(); //�������������� �������� ������� � ������� R,S.
	
	//�������� ������� �����������
	string err =VerifiCertSign(tbsCA, TbsRoot,SignStatus);
	if (!err.empty()) return err;

	return "";
}



/*
�������� ���������� ��� �������� ������� �����������.
tbs-������������������ tbsCertificate;
signatureAlgorithm-OID ��������� ������� �����������;
signatureValue-�������� ������� �����������.
��������� ������� � tbs:
vector<unsigned char> tbsBin;//�������� ������������� ����� ����������� �������.
vector<unsigned char> signValBin; //�������� ������������� ������� �����������.
������������� �������� ������� �����������.
*/
void X509Reader::GetBinSignInfo(QByteArray baCert, TBSCertificate &tbs)
{
	vector<unsigned char> tbsBin;//�������� ������������� ����� ����������� �������.

	//�������������� ������ QByteArray �  ByteQueue
	ByteQueue CerArr; //������ ������ ������ �����������
	CerArr.Put(reinterpret_cast<uint8_t*>(baCert.data()), baCert.size());
	
	/*
	��������� ����������� X.509 v3(������� � RFC 5280):

	Certificate  ::=  SEQUENCE  {
	tbsCertificate       TBSCertificate,
	signatureAlgorithm   AlgorithmIdentifier,
	signatureValue       BIT STRING  }

	TBSCertificate-SEQUENCE  ������������������,������ ��� �������.

	signatureAlgorithm-������������������,  �������� ���������� �������;
	AlgorithmIdentifier  ::=  SEQUENCE  {
	algorithm               OBJECT IDENTIFIER,
	parameters              ANY DEFINED BY algorithm OPTIONAL}
	*/


	ByteQueue cer_tbs; //���������� Tbs �����������.
	BERSequenceDecoder x509Cert(CerArr); //�������� ������������������
	BERSequenceDecoder tbsCert(x509Cert);//TBS ����������
	DERSequenceEncoder tbsEnc(cer_tbs); //������� TBS �����������
	tbsCert.TransferAllTo(tbsEnc);
	tbsEnc.MessageEnd();
		
	ByteQueueToVector(cer_tbs, tbsBin); //����������� TBS ����������

	//����� ��������� ������������� ��� ������� ������ �����������
	OID sigAlgOID; //OID ��������� ������� �����������
	BERSequenceDecoder sigAlg(x509Cert); 
	sigAlgOID.BERDecode(sigAlg);
	sigAlg.SkipAll();
	tbs.AlgorithmIdentifier = OIDtoString(sigAlgOID);//����������� � ������ � ���������� � Tbs.

	//�������� ������� �����������
	SecByteBlock certSignature;
	unsigned int unused = 0;
	BERDecodeBitString(x509Cert, certSignature, unused);
	x509Cert.SkipAll();

	//������� �������� �������
	vector<unsigned char> signatureValue;
	for (int i = 0;i < certSignature.size();i++)
		signatureValue.push_back(certSignature[i]);

	//�������� ������
	tbs.setTbsBin(tbsBin);
	tbs.setSignValBin(signatureValue);
}

void X509Reader::GetTbsInfo(QByteArray baCert,TBSCertificate & Tbs)
{	
	ByteQueue CerBuf; //������ ������ ������ �����������
	CerBuf.Put(reinterpret_cast<uint8_t*>(baCert.data()), baCert.size());//�������������� ������

	BERSequenceDecoder x509Cert(CerBuf);
	BERSequenceDecoder tbsCert(x509Cert);
	// ASN.1 from RFC 3280
	// TBSCertificate  ::=  SEQUENCE  {
	// version         [0]  EXPLICIT Version DEFAULT v1,

	//���������� ��� ��������� ������ �����������
	BERGeneralDecoder context(tbsCert, 0xa0);
	word32 ver;

	//�������� ������ �����������-3 ������ �����������
	BERDecodeUnsigned<word32>(context, ver, INTEGER, 2, 2);
	Tbs.ver = CpIntToInt(ver); //�������� ������ �����������


	 // serialNumber         CertificateSerialNumber,
	Integer serial;
	serial.BERDecode(tbsCert);
	Tbs.serialNumber = IntToMpz(serial); //�������� �������� �����


										 /*
										 �������� ������������������ signature            AlgorithmIdentifier,
										 ��� ������� �������� ������������� ���������, ������������� �� ��� ������� ����.
										 ��� ������� ������ ��������� ���� ��� �� ������������� ���������, ��� � � ���� �signatureAlgorithm�
										 ������������������ �����������. ���������� ������� �(optional) Parameters� (�������������� ���������) �����
										 ������������� (� �� �����), � ����������� �� ���������� ���������.
										 */
	BERSequenceDecoder signature(tbsCert);
	OID AlgorithmIdentifierOID; //OID ��������� ������� �����������
	AlgorithmIdentifierOID.BERDecode(signature); //�������� ��������
	Tbs.AlgorithmIdentifier = OIDtoString(AlgorithmIdentifierOID);
	signature.SkipAll();

	//�������� ������������������ ���������� ������ ��������.   issuer
	BERSequenceDecoder issuerName(tbsCert);
	GetIssuerData(issuerName, Tbs); //���������� ������ ��������
	issuerName.SkipAll();

	//�������� ������������������ ���������� ���� �������� �����������
	BERSequenceDecoder validity(tbsCert);
	GetValidityData(validity, Tbs); //C������� ���� �������� �����������


	//�������� ������������������ ���������� ������ ��������.subject  Name,
	BERSequenceDecoder subjectName(tbsCert);
	GetSubjectData(subjectName, Tbs);
	subjectName.SkipAll();

	//�������� ������������������ ���������� ������ ���������� �����  subjectPublicKeyInfo SubjectPublicKeyInfo,
	BERSequenceDecoder spki(tbsCert);
	
	BERSequenceDecoder SubjectPublicKeyInfo(spki);
	OID spkAlgOID; //OID ��������� ��������� �����
	spkAlgOID.BERDecode(SubjectPublicKeyInfo); //�������� ��������
	Tbs.publicKeyParamSet = OIDtoString(spkAlgOID);//�������� �������� ��������� �����


	BERSequenceDecoder PK_Algoritms(SubjectPublicKeyInfo);
	OID PK_AlgEc;
	OID PK_AlgHash;
	PK_AlgEc.BERDecode(PK_Algoritms);  //�������� ���������
	PK_AlgHash.BERDecode(PK_Algoritms);

	Tbs.digestParamSet = OIDtoString(PK_AlgEc);//OID ������������� ������
	Tbs.encryptionParamSet = OIDtoString(PK_AlgHash);//OID ��� �������.

	SecByteBlock bit_str;
	unsigned int unused = 0;
	BERDecodeBitString(spki, bit_str, unused);

	vector<uint8_t> pubKey;
	pubKey=GetPubKey512(bit_str);//�������� �������� ���� � vector
	Tbs.Set512PubKey(pubKey); //����������� � ����� �������� ����.

	spki.SkipAll();
	tbsCert.SkipAll();
	x509Cert.SkipAll();
}

vector<uint8_t> X509Reader::GetPubKey512(SecByteBlock pk)
{
	//� Bit String co�������� OctetString. ���� �� ��������� -�������� ��������.
	//��� ��� ������ ��������c� ������ 127 ��� ����������� �������������� ���� ����������� �����

	byte os_b1 = pk[0]; //������ ���� 04 -�������� ��������
	byte os_b2 = pk[1]; //81
	byte os_b3 = pk[2]; //80
	byte os_b4 = pk[3];

	//������� �������� ����
	vector<uint8_t> PubKey;
	for (int i = 3;i < pk.size();i++)
		PubKey.push_back(pk[i]);
	
	return PubKey;
}

/*
�������� ���������� ��� �������� ������� �����������.
tbs-������������������ tbsCertificate;
signatureAlgorithm-OID ��������� ������� �����������;
signatureValue-�������� ������� �����������.
*/
void X509Reader::GetInfoForVerifi(QByteArray ba_cert, vector<unsigned char>& tbs, string & signatureAlgorithm, vector<unsigned char>& signatureValue)
{
	//�������������� ������ QByteArray �  ByteQueue
	ByteQueue CerArr; //������ ������ ������ �����������
	CerArr.Put(reinterpret_cast<uint8_t*>(ba_cert.data()), ba_cert.size());


	/*
	��������� ����������� X.509 v3(������� � RFC 5280):

	Certificate  ::=  SEQUENCE  {
	tbsCertificate       TBSCertificate,
	signatureAlgorithm   AlgorithmIdentifier,
	signatureValue       BIT STRING  }

	TBSCertificate-SEQUENCE  ������������������,������ ��� �������.

	signatureAlgorithm-������������������,  �������� ���������� �������;
	AlgorithmIdentifier  ::=  SEQUENCE  {
	algorithm               OBJECT IDENTIFIER,
	parameters              ANY DEFINED BY algorithm OPTIONAL}


	*/
	ByteQueue cer_tbs;
	BERSequenceDecoder x509Cert(CerArr); //�������� ������������������
	BERSequenceDecoder tbsCert(x509Cert);//TBS ����������
	DERSequenceEncoder tbsEnc(cer_tbs); //������� TBS �����������
	tbsCert.TransferAllTo(tbsEnc);
	tbsEnc.MessageEnd();


	//	vector<byte> v;
	ByteQueueToVector(cer_tbs, tbs); //����������� TBS ����������

//	for (int i = 4;i < v.size();i++) tbs.push_back(v[i]);


	//����� ��������� ������������� ��� ������� ������ �����������
	OID sigAlgOID; //OID ��������� ������� �����������
	BERSequenceDecoder sigAlg(x509Cert);
	sigAlgOID.BERDecode(sigAlg);
	sigAlg.SkipAll();
	signatureAlgorithm = OIDtoString(sigAlgOID);//����������� � ������

	//�������� ������� �����������
	SecByteBlock certSignature;
	unsigned int unused = 0;
	BERDecodeBitString(x509Cert, certSignature, unused);
	x509Cert.SkipAll();

	//������� �������� �������
	for (int i = 0;i < certSignature.size();i++)
		signatureValue.push_back(certSignature[i]);

}

//��������� ������ ��������
void X509Reader::GetIssuerData(BERSequenceDecoder &issuer, TBSCertificate &Tbs)
{

	string oid_str;
	string tmp;
	while (CpIntToInt(issuer.RemainingLength()) > 0)
	{
		BERSetDecoder set(issuer);
		BERSequenceDecoder seq(set);
		OID xoid; //������� ������������� �������
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

//��������� ������ ��������� �����������
void X509Reader::GetSubjectData(BERSequenceDecoder & issuer, TBSCertificate & Tbs)
{
	string oid_str;
	string tmp;
	while (CpIntToInt(issuer.RemainingLength()) > 0)
	{
		BERSetDecoder set(issuer);
		BERSequenceDecoder seq(set);
		OID xoid; //������� ������������� �������
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
	//����������� �� ������
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

	//���������� ��� ��������� ������ �����������
	BERGeneralDecoder context(tbsCert, 0xa0);
	word32 ver;

	//�������� ������ �����������-3 ������ �����������
	BERDecodeUnsigned<word32>(context, ver, INTEGER, 2, 2);
	Tbs.ver = CpIntToInt(ver); //�������� ������ �����������


	// serialNumber         CertificateSerialNumber,
	Integer serial;
	serial.BERDecode(tbsCert);
	Tbs.serialNumber = IntToMpz(serial); //�������� �������� �����


	/*
	�������� ������������������ signature            AlgorithmIdentifier,
	��� ������� �������� ������������� ���������, ������������� �� ��� ������� ����.
	��� ������� ������ ��������� ���� ��� �� ������������� ���������, ��� � � ���� �signatureAlgorithm�
	������������������ �����������. ���������� ������� �(optional) Parameters� (�������������� ���������) �����
	������������� (� �� �����), � ����������� �� ���������� ���������.
	*/
	BERSequenceDecoder signature(tbsCert);
	OID AlgorithmIdentifierOID; //OID ��������� ������� �����������
	AlgorithmIdentifierOID.BERDecode(signature); //�������� ��������
	Tbs.AlgorithmIdentifier = OIDtoString(AlgorithmIdentifierOID);
	signature.SkipAll();

	//�������� ������������������ ���������� ������ ��������.   issuer
	BERSequenceDecoder issuerName(tbsCert);
	GetIssuerData(issuerName, Tbs); //���������� ������ ��������
	issuerName.SkipAll();

	//�������� ������������������ ���������� ���� �������� �����������
	BERSequenceDecoder validity(tbsCert);
	GetValidityData(validity, Tbs); //C������� ���� �������� �����������


	//�������� ������������������ ���������� ������ ��������.subject  Name,
	BERSequenceDecoder subjectName(tbsCert);
	GetSubjectData(subjectName, Tbs);
	subjectName.SkipAll();

	//�������� ������������������ ���������� ������ ���������� �����  subjectPublicKeyInfo SubjectPublicKeyInfo,
	BERSequenceDecoder spki(tbsCert);


	BERSequenceDecoder SubjectPublicKeyInfo(spki);
	OID sigAlgOID; //OID ��������� ������� �����������
	sigAlgOID.BERDecode(SubjectPublicKeyInfo); //�������� ��������

	pd.SetAlgoritmOID(sigAlgOID); //�������� �������� ������������� �����
	Tbs.publicKeyParamSet = OIDtoString(sigAlgOID);


	BERSequenceDecoder PK_Algoritms(SubjectPublicKeyInfo);
	OID PK_AlgEc;
	OID PK_AlgHash;
	PK_AlgEc.BERDecode(PK_Algoritms);  //�������� ���������
	PK_AlgHash.BERDecode(PK_Algoritms);
	pd.SetE�OID(PK_AlgEc);
	pd.SetHashOID(PK_AlgHash);

	Tbs.digestParamSet = OIDtoString(PK_AlgEc);
	Tbs.encryptionParamSet = OIDtoString(PK_AlgHash);

	SecByteBlock bit_str;
	unsigned int unused = 0;
	BERDecodeBitString(spki, bit_str, unused);

	pd.SetPubKey(bit_str); //�������� �������� ����

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

//������������ OID � String 
string X509Reader::OIDtoString(OID xOID)
{
	vector<word32> I;
	I = xOID.GetValues();
	string str = "";

	//�����������
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
��������� ������� �����������.
pubkey-������ ��������� ����� �����������;
signatureAlgorithm-OID ��������� ������� �����������;
signatureValue-�������� ������� �����������
*/
bool X509Reader::VerifiSign(PubKeyData pubkey, string signatureAlgorithm,
	vector<unsigned char> signatureValue, vector<unsigned char> &tbs, TBSCertificate &retdataTbs)
{
	/*
	//1.2.643.7.1.1.3.3
	//�������� ������� �����������
	if (signatureAlgorithm != "1.2.643.7.1.1.3.3")
	{
		string err;
		err = "�� ������ �������� ������� �����������."
			" � ������� �����  �������������� ������ ����������� "
			"c	���������� �������� ������� ���� � 34.10-2012 ��� ������ ����� 512 ���"
			"(szOID_CP_GOST_R3411_12_512_R3410:1.2.643.7.1.1.3.3)."
			"� ����������� ������ �������� � OID " + signatureAlgorithm;
		return err;
	}
	//1.2.643.7.1.1.1.2
	//�������� � ������� ������������ �������� ���� ��������� � �����������
	if (pubkey.AlgoritmOID != "1.2.643.7.1.1.1.2")
	{
		string err;
		err = "�� ������ �������� ������������� ��������� �����."
			" � ������� �����  �������������� ������ ����������� "
			" c ���������� ������������� ��������� ����� "
			" �������� ���� � 34.10-2012 ��� ������ ����� 512 ���, ������������ ��� ��������/������� ������. "
			"(szOID_CP_GOST_R3410_12_512:1.2.643.7.1.1.1.2) ."
			" � ����������� ������ �������� " + pubkey.AlgoritmOID + " .";
		return err;
	}

	//�������� ������������� ������
	//1.2.643.7.1.2.1.2.1
	if (!((pubkey.E�OID == "1.2.643.7.1.2.1.2.1") || (pubkey.E�OID == "1.2.643.7.1.2.1.2.2")))
	{
		string err;
		err = "�� ������ �������� ������������� ������ ��������� ����� �����������."
			" � ������� �����  �������������� ������ ����������� "
			" c ���������� ������������� ������ "
			" ��������� a, b, p, q, (x,y) �������� ������� � ��������� �����-�������� �� ���� ��������� ���� � 34.10-2012 512 ��� �� ���������. "
			"(szOID_tc26_gost_3410_12_512_paramSetA:1.2.643.7.1.2.1.2.1) ��� "
			" ��������� a, b, p, q, (x,y) �������� ������� � ��������� �����-�������� �� ���� ��������� ���� � 34.10-2012 512 ���, ����� B"
			"(szOID_tc26_gost_3410_12_512_paramSetB:1.2.643.7.1.2.1.2.2)"
			" � ����������� ������ �������� " + pubkey.E�OID + " .";
		return err;
	}

	//��������� ������� ����������� 1.2.643.7.1.1.2.3
	if (pubkey.HOid != "1.2.643.7.1.1.2.3")
	{
		string err;
		err = "�� ������ �������� ������������� ��������� �����."
			" � ������� �����  �������������� ������ ����������� "
			" � �������� �����������  "
			" ������� ����������� ���� � 34.11-2012, ����� ������ 512 ��� "
			"(szOID_CP_GOST_R3411_12_512:1.2.643.7.1.1.2.3) ."
			" � ����������� ������ �������� " + pubkey.HOid + " .";
		return err;
	}
	*/

	//� ����������� ������� ������������ ��� S,R
	vector<uint8_t> v_S;
	vector<uint8_t> v_R;

	//�������  �������-������� S,R
	for (int i = 0;i < 64;i++) v_S.push_back(signatureValue[i]); //S
	for (int i = 64;i < 128;i++) v_R.push_back(signatureValue[i]); //R


	   //�������������� �������� ������� � mpz_class
	mpz_class r_; //������� �������
	mpz_class s_;

	size_t r_len = static_cast<size_t>(v_R.size());
	size_t s_len = static_cast<size_t>(v_S.size());
	mpz_import(r_.get_mpz_t(), r_len, 1, 1, 0, 0, &v_R[0]);
	mpz_import(s_.get_mpz_t(), s_len, 1, 1, 0, 0, &v_S[0]);


	//�������������� ��������� �����
	vector<uint8_t> p_x;
	vector<uint8_t> p_y;
	//��������� ���������
	for (int i = 63;i >= 0;i--)
	{
		p_x.push_back(pubkey.PubKey[i]); //x
	}

	for (int i = 127;i >= 64;i--) p_y.push_back(pubkey.PubKey[i]); //y

	mpz_class px_;//��������� ��������� ����� x,y
	mpz_class py_;

	size_t px_len = static_cast<size_t>(p_x.size());
	size_t py_len = static_cast<size_t>(p_y.size());
	mpz_import(px_.get_mpz_t(), px_len, 1, 1, 0, 0, &p_x[0]);
	mpz_import(py_.get_mpz_t(), py_len, 1, 1, 0, 0, &p_y[0]);

	Signature Sign; //������ ���������� ������� �����

	Sign.set_r(r_);
	Sign.set_s(s_);
	Sign.set_type(1); //512���

	//gost-3410-12-512-paramSetA
	Groups EC2(0, pubkey.E�OID);//������ ������ �� ��������� �������������� ������������� � �����������

	//������ ����� � ���������� � ������ ������������ ����� P
	Point P2(EC2, 1); //����� ������������� ������ 
	Point Q1(EC2, 1); //����� ������������� ������ 

	//������������ ���������� ��������� �����
	Q1.x = px_;
	Q1.y = py_;
	//������� �������� ���� ��� �������������� ��� ��������
	retdataTbs.Qx = px_;
	retdataTbs.Qy = py_;

	//�������������� ������� ���������� ���� � TBS �����������
	vector<unsigned char> _tbs; //������ � ���������� ������������������(������� ��� ���� ������)
	ReverseByteOrder(tbs, _tbs);//����� �������

	unsigned long long l = static_cast<unsigned long long>(_tbs.size());//���������� ���� � TBS �����������

	//������� �������� ������� ������� ����������� ��� ����������� ��� �������� �����������.
	retdataTbs.CerR = Sign.get_hex_r();
	retdataTbs.CerS = Sign.get_hex_s();

	VerifySign Vs;  //�������� �������
	bool sign_status = Vs.Verify(&_tbs[0], l, P2, Q1, Sign);
	retdataTbs.calcR = Vs.GetLastR(); //������� ��������  ��� ����������� ��� �������� �����������.
	retdataTbs.digest = Vs.GetLastDigest();

	return sign_status;

}



string X509Reader::VerifiSign(PubKeyData pubkey, vector<unsigned char> signatureValue,
	vector<unsigned char> &tbs, TBSCertificate &retdataTbs, bool &SignStatus)
{
	string err;
	int  KeyLen;
	int HashLen;
	err=GetKeyLen(pubkey,KeyLen); //�������� ��������� �����������, � ��������� ����� �����.
	if (!err.empty()) return err; //�������� ����� �� ������.

	err = GetHasLen(pubkey,HashLen);
	if (!err.empty()) return err; //�������� ����� �� ������.

	if (KeyLen != HashLen)
		return "�������� ���������� ��� ������� �� ��������� � ���������� ��������� �����.";
	/*
	//�������� ������������� ������
	//1.2.643.7.1.2.1.2.1
	if (!((pubkey.E�OID == "1.2.643.7.1.2.1.2.1") || (pubkey.E�OID == "1.2.643.7.1.2.1.2.2")))
	*/
	
	Signature Sign; //������ ���������� �������.
	Sign = Get512CerSign(signatureValue); //������� ������� �����������.
		
	//gost-3410-12-512-paramSetA
	//������ ������ �� ��������� �������������� ������������� � �����������
	Groups EC2;
	bool ec_found;//���� ���������� ������ �� OID
	ec_found=EC2.SetParam(pubkey.E�OID); //�� ��������� OID ����� ��������� ������
	if (!ec_found)
		return "������� �� �������� ������������� ������ � OID="+ pubkey.E�OID;


	//������ ����� � ���������� � ������ ������������ ����� P
	Point P2(EC2, 1); //����� ������������� ������ 
	//�������� ����-����� ������������� ������ 
	Point Q1 = Get512PubKey(pubkey,EC2);
						
	//������� �������� ���� ��� �������������� ��� ��������
	retdataTbs.Qx = Q1.x; 
	retdataTbs.Qy = Q1.y;

	//�������������� ������� ���������� ���� � TBS �����������
	vector<unsigned char> _tbs; //������ � ���������� ������������������(������� ��� ���� ������)
	ReverseByteOrder(tbs, _tbs);//����� �������

	unsigned long long l = static_cast<unsigned long long>(_tbs.size());//���������� ���� � TBS �����������

	;//������� �������� ������� ������� ����������� ��� ����������� ��� �������� �����������.
	retdataTbs.CerR = Sign.get_hex_r();
	retdataTbs.CerS = Sign.get_hex_s();

	VerifySign Vs;  //�������� �������
	bool sign_status = Vs.Verify(&_tbs[0], l, P2, Q1, Sign);
	SignStatus = sign_status; //��������� �������
	retdataTbs.calcR = Vs.GetLastR(); //������� ��������  ��� ����������� ��� �������� �����������.
	retdataTbs.digest = Vs.GetLastDigest();

	return "";

}

string X509Reader::GetKeyLen(PubKeyData pubkey, int & KeyLen)
{
	string err;
	
	int signKeyLen = 0; //�����  ����� ������� ��� ��������� ����������� 256 512
	int PubKeyLen = 0; //����� ��������� �����


					   //����������� ����� ����� �������
					   //1.2.643.7.1.1.3.2	�������� �������� ������� ����������� ���� � 34.10-2012 ��� ������ ����� 256 ���
	if (pubkey.signatureAlgorithm == "1.2.643.7.1.1.3.2") signKeyLen = 256;

	//1.2.643.7.1.1.3.3 (�������� �������� ������� ���� � 34.10-2012 ��� ������ ����� 512 ���
	if (pubkey.signatureAlgorithm == "1.2.643.7.1.1.3.3") signKeyLen = 512;

	//����������� ��������
	if (signKeyLen == 0)
	{
		err = "����������� �������� ������� �����������."
			" � ������� �����  �������������� ������ ����������� ���� � 34.10-2012."
			"�������� ������� ������� �����������: " + AlgOidName::GetInfo(pubkey.signatureAlgorithm);
		return err;
	}

	//�������� � ��������� ������ ��������� �����
	//�������� ���� � 34.10-2012 ��� ������ ����� 256 ���, ������������ ��� ��������/������� ������
	if (pubkey.AlgoritmOID == "1.2.643.7.1.1.1.1") PubKeyLen = 256;

	//�������� ���� � 34.10 - 2012 ��� ������ ����� 512 ���, ������������ ��� �������� / ������� ������
	if (pubkey.AlgoritmOID == "1.2.643.7.1.1.1.2") PubKeyLen = 512;


	//����������� �������� �����
	if (PubKeyLen == 0)
	{
		err = "�� ������ ��������  ��������� �����."
			" � ������� �����  �������������� ������ �������� ����� ��������������� ���� � 34.10-2012."
			" � ����������� ������ �������� " + AlgOidName::GetInfo(pubkey.AlgoritmOID) + " .";
		return err;
	}


	if (signKeyLen != PubKeyLen)
	{
		err = "���������� �������� �� ����������� ��������� ������� � ��������� �����.";
		return err;
	}

	KeyLen = PubKeyLen;

	return "";
}

string X509Reader::GetKeyLen(TBSCertificate & tbs, int & KeyLen)
{
	string err;

	int signKeyLen = 0; //�����  ����� ������� ��� ��������� ����������� 256 512
	int PubKeyLen = 0; //����� ��������� �����

	//����������� ����� ����� �������
	 //1.2.643.7.1.1.3.2	�������� �������� ������� ����������� ���� � 34.10-2012 ��� ������ ����� 256 ���
	if (tbs.AlgorithmIdentifier == "1.2.643.7.1.1.3.2") signKeyLen = 256;

	//1.2.643.7.1.1.3.3 (�������� �������� ������� ���� � 34.10-2012 ��� ������ ����� 512 ���
	if (tbs.AlgorithmIdentifier == "1.2.643.7.1.1.3.3") signKeyLen = 512;

	//����������� ��������
	if (signKeyLen == 0)
	{
		err = "����������� �������� ������� �����������."
			" � ������� �����  �������������� ������ ����������� ���� � 34.10-2012."
			"�������� ������� ������� �����������: " + AlgOidName::GetInfo(tbs.AlgorithmIdentifier);
		return err;
	}

	//�������� � ��������� ������ ��������� �����
	//�������� ���� � 34.10-2012 ��� ������ ����� 256 ���, ������������ ��� ��������/������� ������
	if (tbs.publicKeyParamSet == "1.2.643.7.1.1.1.1") PubKeyLen = 256;

	//�������� ���� � 34.10 - 2012 ��� ������ ����� 512 ���, ������������ ��� �������� / ������� ������
	if (tbs.publicKeyParamSet == "1.2.643.7.1.1.1.2") PubKeyLen = 512;


	//����������� �������� �����
	if (PubKeyLen == 0)
	{
		err = "�� ������ ��������  ��������� �����."
			" � ������� �����  �������������� ������ �������� ����� ��������������� ���� � 34.10-2012."
			" � ����������� ������ �������� " + AlgOidName::GetInfo(tbs.publicKeyParamSet) + " .";
		return err;
	}


	if (signKeyLen != PubKeyLen)
	{
		err = "���������� �������� �� ����������� ��������� ������� � ��������� �����.";
		return err;
	}

	KeyLen = PubKeyLen;

	return "";
}

string X509Reader::GetHasLen(PubKeyData pubkey, int & HashLen)
{
	HashLen = 0;

	//1.2.643.7.1.1.2.2 ������� ����������� ���� � 34.11-2012, ����� ������ 256 ���
	if (pubkey.HOid == "1.2.643.7.1.1.2.2") HashLen = 256;
	
	//1.2.643.7.1.1.2.3 ������� ����������� ���� � 34.11-2012, ����� ������ 512 ���
	if (pubkey.HOid == "1.2.643.7.1.1.2.3") HashLen = 512;
	
	
	if(HashLen == 0)
	{
		string err;
		err = "�� ������ �������� ������������� ��������� �����."
			" � ������� �����  �������������� ������ ����������� "
			" � �������� �����������  ��������������� ���� � 34.11-2012."
			" � ����������� ������ �������� " + AlgOidName::GetInfo(pubkey.HOid)+ " .";
		return err;
	}
	
	return "";
}

string X509Reader::GetHasLen(TBSCertificate & tbs, int & HashLen)
{
	HashLen = 0;
	
	//1.2.643.7.1.1.2.2 ������� ����������� ���� � 34.11-2012, ����� ������ 256 ���
	if (tbs.encryptionParamSet == "1.2.643.7.1.1.2.2") HashLen = 256;

	//1.2.643.7.1.1.2.3 ������� ����������� ���� � 34.11-2012, ����� ������ 512 ���
	if (tbs.encryptionParamSet == "1.2.643.7.1.1.2.3") HashLen = 512;


	if (HashLen == 0)
	{
		string err;
		err = "�� ������ �������� ������������� ��������� �����."
			" � ������� �����  �������������� ������ ����������� "
			" � �������� �����������  ��������������� ���� � 34.11-2012."
			" � ����������� ������ �������� " + AlgOidName::GetInfo(tbs.digestParamSet) + " .";
		return err;
	}

	return "";
}

Signature X509Reader::Get512CerSign(vector<unsigned char>& signatureValue)
{
	//� ����������� ������� ������������ ��� S,R
	vector<uint8_t> v_S;
	vector<uint8_t> v_R;

	//�������  �������-������� S,R
	for (int i = 0;i < 64;i++) v_S.push_back(signatureValue[i]); //S
	for (int i = 64;i < 128;i++) v_R.push_back(signatureValue[i]); //R

	 //�������������� �������� ������� � mpz_class
	mpz_class r_; //������� �������
	mpz_class s_;

	size_t r_len = static_cast<size_t>(v_R.size());
	size_t s_len = static_cast<size_t>(v_S.size());
	mpz_import(r_.get_mpz_t(), r_len, 1, 1, 0, 0, &v_R[0]);
	mpz_import(s_.get_mpz_t(), s_len, 1, 1, 0, 0, &v_S[0]);

	
	Signature Sign; //������ ���������� ������� �����
	Sign.set_r(r_);
	Sign.set_s(s_);
	Sign.set_type(1); //512���

	return Sign;
}

Point X509Reader::Get512PubKey(PubKeyData pubkey, Groups EC)
{
	//�������������� ��������� �����
	vector<uint8_t> p_x;
	vector<uint8_t> p_y;
	//��������� ���������
	for (int i = 63;i >= 0;i--)
	{
		p_x.push_back(pubkey.PubKey[i]); //x
	}

	for (int i = 127;i >= 64;i--) p_y.push_back(pubkey.PubKey[i]); //y

	mpz_class px_;//��������� ��������� ����� x,y
	mpz_class py_;

	size_t px_len = static_cast<size_t>(p_x.size());
	size_t py_len = static_cast<size_t>(p_y.size());
	mpz_import(px_.get_mpz_t(), px_len, 1, 1, 0, 0, &p_x[0]);
	mpz_import(py_.get_mpz_t(), py_len, 1, 1, 0, 0, &p_y[0]);

 
	Point Q1(EC, 1); //����� ������������� ������ 

	//������������ ���������� ��������� �����
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
	//�������������� ������ QByteArray �  ByteQueue
	ByteQueue rootArr; //�������� ����.
	rootArr.Put(reinterpret_cast<uint8_t*>(in.data()), in.size());

	BERSequenceDecoder rootSequence(rootArr); //�������� ������������������

	OID _typeOID; //OID ���� CMS.
	_typeOID.BERDecode(rootSequence);
	string typeOID =OIDtoString(_typeOID);

	if (typeOID != "1.2.840.113549.1.7.2")
	{
		lastError = "�������� ���� ������ �� �������� �������� � �������. OID ����� ������="+ typeOID;
		return false;
	}
			
	
	BERGeneralDecoder cs1(rootSequence, 0xA0);//CONTEXT SPECIFIC (0) 
	BERSequenceDecoder content(cs1);

	Integer v1_;
	v1_.BERDecode(content);
	int cmsV1 = static_cast<int>(v1_.ConvertToLong()); //������ CMS.

	sD.CMSVersion = cmsV1;


	BERSetDecoder set1(content);
	BERSequenceDecoder seq1(set1);

	OID _oid1; //OID ��������� ���������.
	_oid1.BERDecode(seq1);
	string strOid1 = OIDtoString(_oid1);

	sD.digestAlgorithms = strOid1;

	seq1.SkipAll();
	set1.SkipAll();
	
	BERSequenceDecoder seq2(content);

	OID _oid2; //OID ���� ������.
	_oid2.BERDecode(seq2);
	string strOid2 = OIDtoString(_oid2);


	BERSetDecoder set2(content);
	BERSequenceDecoder seq3(set2);

	Integer v2_;
	v2_.BERDecode(seq3);
	int cmsV2 = static_cast<int>(v2_.ConvertToLong()); //������ CMS.

	
	BERSequenceDecoder seq4(seq3); //������ ����������� ����������.
	BERSequenceDecoder seq5(seq4);
	BERSetDecoder set3(seq5);
	BERSequenceDecoder seq6(set3);

	OID _oid3; //OID �������� ������������.
	_oid3.BERDecode(seq6);
	string strOid3 = OIDtoString(_oid3);
	

	ByteQueue val1_; //��������.
	seq6.CopyMessagesTo(val1_);
	string val1 = "";

	seq6.SkipAll(); //��� ����� ����� ���������� � ���������.
	set3.SkipAll();
	seq5.SkipAll();
	
	CryptoPP::BERDecodeTextString(val1_, val1, CryptoPP::UTF8_STRING);
	QString str1 = QString::fromUtf8(val1.c_str());
	//�����������.
	string val2 = str1.toLocal8Bit();
	sD.commonName = val2;

	//����� ����������� ����������.
	Integer certNum_;
	certNum_.BERDecode(seq4);
	int certNum = static_cast<int>(certNum_.ConvertToLong()); //������ CMS.
	sD.certNum = certNum;

	seq4.SkipAll();

	BERSequenceDecoder seq7(seq3);
	OID _oid4; //OID �������� ������������.
	_oid4.BERDecode(seq7);
	string strOid4 = OIDtoString(_oid4); //��� ����.

	//��������� Null
	seq7.SkipAll();

	//�������������� �������.
	BERGeneralDecoder cs2(seq3, 0xA0);//CONTEXT SPECIFIC (0) 
	BERSequenceDecoder seq8(cs2);

	//contentType
	OID _oid5; //OID
	_oid5.BERDecode(seq8);
	string strOid5 = OIDtoString(_oid5);
	seq8.SkipAll(); //��������� ��� �� �����.


	//����� �������.
	BERSequenceDecoder seq9(cs2);
	//signingTime
	OID _oid6; //OID
	_oid6.BERDecode(seq9);
	string strOid6 = OIDtoString(_oid6);

	BERSetDecoder set4(seq9);
	string signiningTime_;
	CryptoPP::BERDecodeTextString(set4, signiningTime_, CryptoPP::UTC_TIME);

	QDateTime signiningTime;
	//����������� �� ������
	QString a = "20" + QString::fromStdString(signiningTime_);
	a.replace("Z", "");
	
	signiningTime = QDateTime::fromString(a, "yyyyMMddhhmmss");
	sD.signingTime = signiningTime;

	set4.SkipAll();
	seq8.SkipAll();

	//�������� ���������.
	BERSequenceDecoder seq10(cs2);
	//messageDigest
	OID _oid7; //OID
	_oid7.BERDecode(seq10);
	string strOid7 = OIDtoString(_oid7);

	//��� ��������.
	BERSetDecoder set5(seq10);
	string messDigest;
	CryptoPP::BERDecodeTextString(set5, messDigest, CryptoPP::OCTET_STRING);

	vector<unsigned char> tmp1(messDigest.begin(), messDigest.end());
	sD.digest=tmp1;


	set5.SkipAll();
	seq10.SkipAll();


	//Signing certificate  ������ ����������� ����������.
	BERSequenceDecoder seq11(cs2);
	//Signing certificate 
	OID _oid8; //OID
	_oid8.BERDecode(seq11);
	string strOid8 = OIDtoString(_oid8);

	BERSetDecoder set6(seq11);
	BERSequenceDecoder seq12(set6);
	BERSequenceDecoder seq13(seq12);
	BERSequenceDecoder seq14(seq13);

	BERSequenceDecoder seq15(seq14); //�������� ��������� �����������.

	//������������� ��������� �����������.
	OID _oid9; //OID
	_oid9.BERDecode(seq15);
	string strOid9 = OIDtoString(_oid9);
	sD.signingCertDigestAlg = strOid9;

	seq15.SkipAll();

	//��������� ����������� ����������.
	string certDigest;
	CryptoPP::BERDecodeTextString(seq14, certDigest, CryptoPP::OCTET_STRING);

	vector<unsigned char> tmp2(certDigest.begin(), certDigest.end());
	sD.signingCertDigest = tmp2;

	
	BERSequenceDecoder  seq16(seq14);
	BERSequenceDecoder  seq17(seq16);

	//�������������� �������.
	BERGeneralDecoder cs3(seq17, 0xA0);//CONTEXT SPECIFIC (0) 
	BERSequenceDecoder seq18(cs3);

	BERSetDecoder set7(seq18);
	BERSequenceDecoder seq19(set7);

	OID _oid10; //OID ������������� ������������
	_oid10.BERDecode(seq19);
	string strOid10 = OIDtoString(_oid10);
	
	//��������.
	string certUsName;
	CryptoPP::BERDecodeTextString(seq19, certUsName, CryptoPP::UTF8_STRING);

	QString str2a = QString::fromUtf8(certUsName.c_str());
	//�����������.
	string val2a = str2a.toLocal8Bit();
	seq19.SkipAll();
	set7.SkipAll();
	seq18.SkipAll();
	cs3.SkipAll();
	seq17.SkipAll();

	//����� �����������.
	Integer certNum2_;
	certNum2_.BERDecode(seq16);
	int certNum2 = static_cast<int>(certNum2_.ConvertToLong()); //������ CMS.

	seq16.SkipAll();
	seq14.SkipAll();
	seq13.SkipAll();
	seq12.SkipAll();
	set6.SkipAll();
	seq11.SkipAll();
	//cs2.SkipAll();

	//�������� ���.
	BERSequenceDecoder seq20(seq3);

	OID _oid11; //OID alg signature.
	_oid11.BERDecode(seq20);
	string strOid11 = OIDtoString(_oid11);
	sD.signatureAlgorithm = strOid11;

	seq20.SkipAll(); //���������� null.

	//������ ���.
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
