#pragma once

#include "ui_KeyGen.h"
#include <QTextCodec>
#include <QMessageBox>
#include <QFileDialog>
#include <QDomDocument>
#include <QMetaProperty>
#include "PkRequest.h"
#include <QTimer> //��� ������ �������
#include <QKeyEvent> //��� ����������� �������� ��� �������
#include <qdebug.h>
#include <QXmlStreamWriter>
#include <Windows.h>
#include "mRequest.h"
#include "SecretKeyMaker.h"

#include <QWidget>
#include <QDialog>
#include <QTextCodec>
#include <QDate>
#include <QMessageBox>



using std::string;


class KeyGen : public QMainWindow
{
	Q_OBJECT

public:
	KeyGen(QWidget *parent = Q_NULLPTR);
	~KeyGen();
private:
	Ui::KeyGenClass ui;
   
	mRequest Req; //������ ������� �� ��������� �����������.
	QTextCodec *StrCodec; //��� �������������� ���������	
	
	QTimer* KeyTimer;
	QTimer* ClockTimer;//������ ��� ������ �����. ������������ ����� ��� ������� ������
	QTimer* KeyBetweenTimer; //�������� ������ ����� �������� ������

	int TimeKeyPress; //����� ������� �������
	int TimeBetweenKeyPress;//������ ����� �������� ������
							   //����
	struct s_clock
	{
		unsigned char sec;
		unsigned char min;
	};
	s_clock Clock; //���� ��� �������� ������� ������ ���������


	int PeriodKeyPress[100];//����� ������� �������
	int pos_PeriodKeyPress; //�������  � ������� PeriodKeyPress
	
	int PeriodBetWeenKeyPush[100]; //��������� ����� ��������� ������.
	int pos_PeriodBetWeenKeyPush;

	int NowKeyCode; //��� ������� ������� ������
	int LastKeyCode; //��� ���������� ������� �������
	bool EnableKeyBetweenTimer;//���� ��������� ������� ����������� �������� ����� ��������� ������

	 //��������� ���� ��������� �������, � ������ ��������� ������� ���������� 1 � ������������� ������
	bool ClockProcess();
	//���������� ������ ��� ����������� �����
	QString GetStrClock();

	//������� ������� PeriodKeyPress[100] PeriodBetWeenKeyPush[100]
	void ClearArrays();
	

	void initBioRng();//������������� �������������� ������� ��������� �����.
	void enableAnimation();//�������� ��������.
	void rotateAnime(); //������� �������.
	int animationValue;
	QTimer animationTimer;
	
	//������������ ������� ������ � �������� � ������  Key;
	void processRawData(unsigned char *Key);
	
	//��������������� �����. ���� ����� ������ 255 ���������� ������� 8 ���. �������� �������� ����� � ���� �� 8 ���, 
	//���������� �������� �������� � ������ Excess.
	unsigned char ToUCDiget(int x,QVector<unsigned char>  &Excess);
	
	//���� ���������� ��������� �����
	bool EnableGeneratedKey;
	
	void ShowWarning(string mess);
	//���������� ��������� �� ������
	void ShowError(QString Message);

	//������� ��������� �� �������� ��������� �����.
	void setHappyMassege();	

	//������� ���� � ��������� ������. ������������ ������ ��� ������� ������������������!
	void createDebugFile(unsigned char *Key);

	//���������� �����.
	void createDebugValue(int v);

	private slots:
	void slotpushButtonW1Next();//������� �� ������ "�����" � ������ �������
	void slotpushButtonW2Back();//������� �� ������ "�����" �� ������ �������

	void slotpushButtonW3Next();//������� �� ������ "�����" �  ������� ����� ������.

	void slotpushButtonSelectKeyPath();//����� ���� � ���������� �����


	//��� �������
	void slotKeyTimerTick(); //������������ ������� ��������� ������� ������� �������
	void slotClockTimerTick(); //������������ ������� �����, � ������ ����� ��������� �������
	void slotKeyBetweenTimerTick(); //������������ ������� ������ ������� ����� �������� ������
	void slotKeyIsGenarated(); //���� ���� ��� ���� �������������
	void slotAnimationTimerTick();
	

protected:
	void keyPressEvent(QKeyEvent *); //������� ������� ������� //���� ������ ������������ ��������� ����� ������� �������� ����������
	 //������� ���������� �������  ����������� ���� ������� ������ � ������
	void keyReleaseEvent(QKeyEvent *);

	
signals:
	void KeyIsGenarated(); //������ ���� ��� ���� �������������

};

