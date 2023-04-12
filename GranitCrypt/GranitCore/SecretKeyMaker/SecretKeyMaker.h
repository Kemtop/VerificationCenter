#pragma once

#include <QTextCodec>

#include <QFileDialog>
#include <QDomDocument>
#include <QMetaProperty>
#include "PkRequest.h"
#include <QTimer> //��� ������ �������
#include <QKeyEvent> //��� ����������� �������� ��� �������
#include <QRandomGenerator>
#include "mpirxx.h"
#include <qdebug.h>
#include <hash3411\hash3411.h>
#include "Gost3412\Utility.h"
#include "Gost3412\Sib3412.h"
#include "Point.h"
#include "PkContainer.h"
#include <QXmlStreamWriter>
#include "rsa.h"
#include "osrng.h"
#include "CreateSign.h"
#include <Windows.h>
#include "Models\mRequest.h"
#include "SerializableTools\SerializableTools.h"
#include "VanishChest\VanishChest.h"

#include <string>

using std::string;

using namespace CryptoPP;
using CryptoPP::AutoSeededRandomPool;

#define KeyGenVersion "1.4.2.7"
//������ ���������� �����-��������� ���������� [7-���� ��������� ����������][��������� 47����][��� �������� 256���]
//[����� ���������� ����� 4�����][[iv 16����][4����� ����� ����������]���������]
#define Key_title_len 90

//�������� ������ ��� �������� ��������� �����.
class SecretKeyMaker : public QObject
{
	Q_OBJECT
private:
	static const QString  G_publicKeyAlgoritmOid;//�������� ��������� ����� �������.
	static const QString G_EcOid; //������������� ������������� ������ �� ���������  ������� ��������� ����.
	/*
	������� �������� ����  ���� � 34.10-2018 ��� ������������ �������.
	���������� 17 ������� ����.
	��������� ������ ��� ������� ����������� "������+����"
	��������� �������� K= (256���)��� ������� �� ���� � 34.11-2018.
	��������� ��������� ������ iv.
	��������� K,iv ������� �������� ����.
	*/
	bool �reateInvisibleSignKey(unsigned char *Key, QString Password, Point &Q, string &str_iv_, string &salt_, string &crypt_secret_key_);

	/*
	������� ���� ������  �������� � �������� ��� ������������ � �������� �������;
	//ECname-OID ������������� ������;Q-�������� ����; d-�������� ����
	secret_key-�������� ����
	uint8_t key_len-������ ����� � ������ 32 ��� 64
	*/
	bool CreateEcKeyPair(bool BeginSelfTest, string EcOID, uint8_t *secret_key, uint8_t key_len, Point &Q, mpz_class &d);

	//���������� 17 ������� ����.
	bool GenerateSalt(string &d);

	//�������� ������ � �����, � ������ ������ ���������� ���������
	bool HashedPassword(QString &Password, string salt, unsigned char *hash);

	//������� � ����������� �������� ���� ����� �����-�������.
	bool �reateInvisibleDHkey(QString Password, string &str_iv_, string &salt_, string &crypted_secret_DH_key_, string &DH_public_);

	//��������� ������ �������������
	bool GenerateIV(unsigned char *iv);

	//������ ��������� ���� d ������� ���������, � �������� ����� ������������ ���, � ������ ������ ���������� ���������
	//����� ����������-����� �������� ����� �� �����������
	bool CryptEcSecretKey(string &result, unsigned char *hash, unsigned char *iv, mpz_class &d);

	//������������ �������� ������ � ������
	void ByteArrayToHexString(unsigned char *array, int len, string &str);

	//������������ 16 ������ ������ � ������ unsigned char
	void HexStr_to_uchar(string str, unsigned char *array);

	//��������� �������� ���� ��� ���������� �����-�������
	bool GenerateDHpair(string & DH_public, uint8_t *private_key, uint32_t &private_key_len);

	//������������ �������� ������ � ������ ���� string �  16������ ������������� QString
	QString str_to_Hex(string & str);

	//��������� ��������� ����� � ����
	bool Key�ontainerToFile(PkContainer &KeyCont, QString PathToSave);

	//������������ ��������� ���������� �����
	bool PackContainer(QByteArray &Container, QByteArray &Packing);

	//�������� �������� ����� ������������ ����������. � ������ ������ HasErr=1;Err-�������� ��������� �� ������.
	ByteBlock GetPackKv(string & Err, bool & HasErr);

	//�������� �������� ����� ������������ ������. � ������ ������ HasErr=1;Err-�������� ��������� �� ������.
	ByteBlock GetPackMf(string & Err, bool & HasErr);

	//������� ���� �������.
	bool CreateRequestFile(QString pathToSave, mRequest &Req);

	//������ ��������� ���� ��������� �����-�������,� �������� ����� ������������ ���, � ������ ������ ���������� ���������
	bool CryptDHSecretKey(string &crypted_key, uint8_t *private_key, uint32_t &private_key_len, unsigned char *hash, unsigned char *iv);
	
	//��������� ���256 ����������
	void GetHash�ontainer(uint8_t *container, uint32_t &container_len, uint8_t *hash);

		/*
	   ��������� ������ ��������� �����������-���������� ��������� ��������� �����
	   [                    arr    54�����           ][  hash   32�����   ] 
	   [7-���� ��������� ����������][��������� 47����][��� �������� 256���][����� ���������� ����� 4�����] [��������� �����]
	   [7+47+32=86����][��������� �����] 
	*/
	void SetService_Information(uint8_t *arr, uint8_t *hash,uint32_t &conteiner_len);

	string lastError;

	//������� ����� ������ ������ �����.
	bool enableRose;

public:
	SecretKeyMaker();
	~SecretKeyMaker();

	void setRoseMode(); //�������� ���������������� ������ �����.
	

	//������� ���� ��������� ����� � ������� �� ����������.
	bool CreateKeyFile(unsigned char *Key, QString Path, QString &Password, mRequest &Req, QString pathToSaveReq);

	//������ ���� ��������� �����, ��� ������� �� ����������.
	bool CreateKeyFileNoReq(unsigned char *Key, QString Path, QString &Password);

	string getLastError();
};
