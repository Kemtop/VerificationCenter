#pragma once

#include <QObject>
#include <QFileInfo>
#include <QDir>
#include "Gost3412\Cipher3412.h"
#include "Gost3413\Gost3413.h"
#include "rsa.h"
#include "osrng.h"
#include "PkContainer.h"
#include "Point.h"
#include "CreateSign.h"
#include <QByteArray>
#include <QTextCodec>
#include "Gost3412\Utility.h"
#include "Gost3412\Sib3412.h"
#include "AsInterpretator\AsInterpretator.h"
#include "ASreader\ASreader.h"
#include "VerifySign.h"
#include "CipherGearData.h"
#include <QThread>
#include "eventsCipherWorker.h"
#include "GranitDbI\GranitDbI.h"
//#include "ASreader\ASreader.h"

using  std::string;
using namespace CryptoPP;
using CryptoPP::AutoSeededRandomPool;

//����������� ����� ����������� ����������.
class CipherWorker : public QThread
{

public:
	CipherWorker(QObject *val) :threadEvens(val)
	{
		codec = QTextCodec::codecForName("Windows-1251");
		reportInUI = false;
		enableRose = false;
	};

	//����������� ������������ � dll.
	CipherWorker()
	{
		reportInUI = false;
		enableRose = false;
		IsDisabledEventProcess = true;
	};

	~CipherWorker();

	bool CryptFiles(QStringList &paths_to_file, QString Password, string receiverKey, PkContainer &KeyContainer,
		string SignerIndex, string _rHashOkDH); //������� �����.
	
	//��������� �������� ����������� ��������� ������
	bool DecryptFiles(QStringList &paths_to_file, QString DbPath, QString UserPassword,
		PkContainer KeyContainer);

	string getLastError(); //���������� ������.
	
	void setData(CipherWorkerData data); //������������� ��������� ������. 
	void run(); //��������������� ������������� ������.
	//��������� ���� ������� �� ���������� �����.
	bool DecryptEc_SK(Point & Q, mpz_class & d, QString Password, PkContainer &KeyContainer);
	//������ 32 ������� ��������� ����.
	bool CryptedSessionKey(uint8_t *session_key, string &result);
	//������ ���� �� ���������� ����
	bool CryptFile(QString src_path, QString dst_dir, Cipher3412 &Cipher, uint8_t *Reg, string &CryptedKey,
		mpz_class &user_d, PkContainer &KeyContainer);
	RSA::PublicKey asymmetricKeyR; //�������� ���� ����������.
	//��������� ��������� ������� ��������� ������.
	bool IsDisabledEventProcess;
private:
	QObject* threadEvens; //�������, ����������� �� ������-���������� ������ �������.	
	CipherWorkerData baseData; //�������� ��������� ��� ������� ����������.
	void sendEventAllProcessVal(int val); //���������� ������� �������� ��������� ���� ������.
	void sendEventProcessFileInfo(string msg); //���������� ������� �������������� � �������� ��������� �����.
	void sendEventThreadStatus(int status);
	void sendEventCurFileProcess(int val);//���������� ������� ��������� ��������� �����.
	void sendMaxFileProcess(int val);//���������� ������� ������������� �������� ��������� ��������� �����.

	bool initDistDir(QString val, QString &destinationDir);//�������������� ����� ����������.
	bool enableRose;//������� �������������� ����� ������.

	QTextCodec *codec;
	//QString destinationDir; //���� � ���������� ���� ����� ������������ ������������ �����.(����������� ��� ��������������)
	string lastError;  //������ ��������� ������.
	string signerIndex;   //������ ��������� ����� ����������
	string rHashOkDH; //�������� ��������� ��������� ����� ��.
	bool reportInUI;//���� ��������� ������ ������ � ���������������� ���������.

	bool CheckEcPair(Point & Q, mpz_class & d, string  ecOid);
	//�������� ��������� ����������� ������ � ����
	bool HashedPassword(QString &Passwd, QString &salt_str, uint8_t *hash);
	//�������������� ��������� ���� CryptedKey ��������� key
	bool DecryptSK(uint8_t *key, QString &CryptedKey, QString &iv, string &p_key);
	
	//��������� ��������� ���������� �����, ���������� ��� � ����
	bool WriteCryptFileTitle(QFile &file, qint64 file_len, qint64 &file_pos);
	bool SignFile(QFile & file, mpz_class & user_d, PkContainer &KeyContainer); //����������� ����.

	//��������� ���� � �����, ������� ���������� ��� ���������� �������������� ������, ���������� ���� � ����������
	bool CreateDstDecryptDir(QString fpath, QString &DirPath);

	//������������� ����
	bool DecryptingFile(QString src_path, QString dst_dir, QString DbPath,
		QString UserPassword, PkContainer &KeyContainer);

	//��������� ������� �����
	bool CheckSign(ASreader &ASr, QFile &file, QString DbPath, PkContainer &KeyContainer);
	//����������� ����� �������� �����
	bool ASdecryptSecretKey(ASreader &Data, uint8_t *key, QString Password, PkContainer &KeyContainer);

	//�������������� �������� ���� ����� �����-�������
	bool DecryptDH_SK(RSA::PublicKey &publicKey, RSA::PrivateKey &secret_key,
		QString Password, PkContainer &KeyContainer);

	//��������� ���� ������ ����� �����-�������. ���� ���� �� ��������� ���������� 0
	/*
	�������� �������� � �������� ����� ����� �����-�������
	������������ �������� ��� ������������ � ����������.
	�������� �������� ����, ���� ���� ������ ���������-��������� ��������� ������������ 0
	�������� �������� ����. ���� ��� �� ������� ������������, �� ��������� ��� �� ��������-��������� ��������� ������������ 0
	������������ �������� ������� n ��������� � ��������� �����
	*/
	bool GetDhPair(QString &open_key, string &private_key, RSA::PublicKey &publicKey, RSA::PrivateKey &secret_key);
};
