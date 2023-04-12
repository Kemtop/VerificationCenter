#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GranitCrypt.h"
#include <QTextCodec>
#include <QMessageBox>
#include <QFileDialog>
#include <QThread>
#include <QFileinfo>
#include  <QTextBlock>
#include "Gost3412\Cipher3412.h"
#include "Gost3413\Gost3413.h"
#include "SelectKeyForm.h"
#include "RequirePswForm.h"
#include "hash3411\hash3411.h"
#include "PkContainer.h"
#include "Gost3412\Utility.h"
#include "Gost3412\Sib3412.h"
#include <QDomDocument>
#include "rsa.h"
#include "osrng.h"
#include "AsInterpretator.h"
#include "ASreader.h"
#include "CreateSign.h"
#include "Point.h"
#include "mpirxx.h"
#include "VerifySign.h"
#include "TBSCertificate.h"
#include "X509Reader.h"
#include "PathSettings.h"
#include "GranitDbI\GranitDbI.h"
#include "AboutProgramForm.h"
//����������.
#include "SerialNumTools.h"
#include "LockControl\LockControl.h"
#include "CipherWorker.h"
#include "CipherGearData.h"
#include "KeyService.h"
#include "KeyGen.h"

#define Ini_file_name "config.ini"

//������ ���������� �����-��������� ���������� [7-���� ��������� ����������][��������� 47����][��� �������� 256���][����� ���������� ����� 4�����]
#define Key_title_len 90

//������������ ��������� ��� �������� ����� ��������. ����� ��������� ����� ����� ������ �������� ������.
//#define GenKeyContainer



using  std::string;
using namespace CryptoPP; 
using CryptoPP::AutoSeededRandomPool;



class GranitCrypt : public QMainWindow
{
    Q_OBJECT

public:
    GranitCrypt(QWidget *parent = Q_NULLPTR);
	~GranitCrypt();

	//��������� ������� �� ������.
	void customEvent(QEvent* pe);
	

private:
    Ui::GranitCryptClass ui;	
	CipherWorker *chW;
	KeyGen genKey;


	/*
	  ����� ���� �� ������� ������ chW � ������� ����� ���������� ��������.
	  �� ����� �������� �� �������� �� ������ � ����� �����������(���� ������������),
	  ��� ������� ������� � ������� ������-���������� ���������.
	  �� ���� ������� ����� ������� ������ ������ � ������� �������.
	*/
	void initchW(); //�������������� ������ chW. ����� ��� ���� �������� ������.
	void deletechW(); //������� ������ ���� �� ����������. ����� ��� ���� �������� ������.

	
	int allCurProcessFiles = 0;// ����� ���������� �������������� ������.
	bool lockUi; //���� ���������� ��������� ��� ������.
	int typeProcessWorking;//����� ������ 1-����������, ���������������.
	bool hasThrObj; //���� ������������� ������� ����������. ����� ��� ���� �������� ������
	// �� �������� ������� ������ � �������.
	
	KeyService Ks; //������ ��� ������ � ��������� ������.		
	QTextCodec *StrCodec; //��� �������������� ���������

	//������ �������� ����������, ������������ ��� ����������� ����������� �������� ���������� ������, �������������� progress bar
	void showWorkComponent(int f_count);
	//�������� ������ ������ � PlainTextEdit
	void coloringTextLine(int &line_pos,QTextCharFormat &tf, QTextEdit::ExtraSelection &selection, QTextCursor &cursor);
	//����� ��������� � PlainTextEdit �� ����������, ������ progress bar 
	void writeEndMessage();
	
	//����� ��������� � PlainTextEdit �� ������.
	void paintShowError(string msg);
	//��������� ��������� ���� ������� ������
	QString FillTextEditFilesName(QStringList &paths_to_file);
	
	//���������� � PlainTextEdit ������� ��������� ������.
	void paintProcessFile(int ix);
		
	//������ ������ ���������� ���������� ��� ���������������
	QStringList ProcessingFiles; 
	
	//��������� ��������� �� ini �����. ���������� 0 ���� ini ����� �� ����������.
	bool LoadSettings(PathSettings &S);	

	//������� ������ ������ ���������� �����. ���� ������������ ���������,��� �� ������� �����-���������� false.
	bool SelectPathSK(PathSettings &S);

	//������� ��������� �����, � ������� ini ���� 
	void ClearSettings();

	//��������� ���� � ������� 1 ���� 2 ��
	void SavePaths(uint8_t param, PathSettings S);

	//���������� ���� ����� ������.���� ������������ ��������� ���������� false.
	bool ShowPasswordDialog(QString &Password);
	//������
	QString UserPassword;
	
	//�� ���� ������ �������� ������ ��������� ����� ����������, � ����� �������� ���� 
	bool GetrHashOkDH(GranitDbI &DBI);
	
	//��������� ������������� ����� �� ���������� ����.
	bool HasFile(QString path);

	//������� ���� � ���������������.
	void ShowWarning(string str);
	//������� ���� � ���������������.
	void ShowError(string str);

	//���������� Yes/No ������.
	bool ShowYNDialog(string str);
	

	//��������� ������ ������ ������������ ������ Win1251 � utf16
	QString toUtf(string str);

	//�������� ������ � ���� �� ���������� ����.
	void saveToFile(QByteArray &Ba,QString path);
	
	void CryptFiles(QStringList &paths_to_file, QString Password);

	//��������� ��������  ������ ������.
	void doOpenFiles();
		

	int badAttempPswd;//���������� �� ������ ������� ����� ������.
	/*
	  ��������� ���������� ������������� ���������� �� ������ ������� ����� ������.
	  ����=3, ������� ��������� � ��������� ������� �������� ���������.
	*/
	bool maxBadAttempPswd();

	struct Prm1
	{
		bool PwdIsChecked; //������ ��� ��������
		int ReceiverId; //�� ���������� ���������� ���������. �� � ���� ������ ������.
		string rHashOkDH; //������������ �������� ��������� ����� ����������
		string Dh_OpenKey; //�������� ���� ����������
						   //���������� ����������� ����� ������� ���� ������. ������������ ��� ������ ����� ����� ������ �����
		string SignerIndex;//������ ����� ����������
		
	};

	Prm1 Param;

	//���������� ����� � ������ � ����������.
	string HelloMessage();
	
	private slots:
	void slotMenuFileClicked(QAction *action);//���� � ���� "����"
	void slotMenuHelpClicked(QAction *action);//���� � ���� "�������"
	void slotMenuSettings(QAction *action); //���� ���������

	void slotpushButtonCrypt();//������� �� ������ �����������
	void slotpushButtonDecrypt();//������� �� ������ ������������
	void slotpushButtonSelectKey();//������� �� ������ ������� ����	

	protected:
		bool eventFilter(QObject *target, QEvent *event); //���������� ������������� �������.
	
};


