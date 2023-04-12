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
//Библиотека.
#include "SerialNumTools.h"
#include "LockControl\LockControl.h"
#include "CipherWorker.h"
#include "CipherGearData.h"
#include "KeyService.h"
#include "KeyGen.h"

#define Ini_file_name "config.ini"

//Размер Заголовока файла-служебная информация [7-байт служебной информации][Заголовок 47байт][Хэш контента 256бит][Длина контейнера ключа 4байта]
#define Key_title_len 90

//Генерировать контейнер для хранения ключа продукта. Нужно выполнять когда будет готова релизная версия.
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

	//Обработка событий от потока.
	void customEvent(QEvent* pe);
	

private:
    Ui::GranitCryptClass ui;	
	CipherWorker *chW;
	KeyGen genKey;


	/*
	  Можно было бы удалять объект chW в событие после выполнения операции.
	  Но такой алгоритм не работает на машине с одним процессором(ядро однопоточное),
	  при попытки удалить в событии объект-высыпается программа.
	  По этой причине будем удалять объект только в крайних случаях.
	*/
	void initchW(); //Инициализирует объект chW. Нужно для одно поточной машины.
	void deletechW(); //Удаляет объект если он существует. Нужно для одно поточной машины.

	
	int allCurProcessFiles = 0;// Общее количество обрабатываемых файлов.
	bool lockUi; //Флаг блокировки контролов при ошибке.
	int typeProcessWorking;//Режим работы 1-шифрование, расшифровывание.
	bool hasThrObj; //Флаг существования объекта шифрования. Нужно для одно поточной машины
	// не возможно удалить объект в событии.
	
	KeyService Ks; //Объект для работы с секретным ключом.		
	QTextCodec *StrCodec; //Для преобразования кодировки

	//Делает видимыми компоненты, используемые для визуального отображения процесса шифрования файлов, инициализирует progress bar
	void showWorkComponent(int f_count);
	//Выделяет цветом строки в PlainTextEdit
	void coloringTextLine(int &line_pos,QTextCharFormat &tf, QTextEdit::ExtraSelection &selection, QTextCursor &cursor);
	//Пишет сообщение в PlainTextEdit об успешности, меняет progress bar 
	void writeEndMessage();
	
	//Пишет сообщение в PlainTextEdit об ошибке.
	void paintShowError(string msg);
	//Заполняет текстовое поле именами файлов
	QString FillTextEditFilesName(QStringList &paths_to_file);
	
	//Отображает в PlainTextEdit процесс обработки файлов.
	void paintProcessFile(int ix);
		
	//Список файлов подлежащих шифрования или расшифровыванию
	QStringList ProcessingFiles; 
	
	//Загружает настройки из ini файла. Возвращает 0 если ini файла не существует.
	bool LoadSettings(PathSettings &S);	

	//Выводит диалог выбора секретного ключа. Если пользователь отказался,или не найдено ключа-возвращает false.
	bool SelectPathSK(PathSettings &S);

	//Очищает настройки путей, и удаляет ini файл 
	void ClearSettings();

	//Сохраняет пути в реестре 1 ключ 2 БД
	void SavePaths(uint8_t param, PathSettings S);

	//Отображает окно ввода пароля.Если пользователь отказался возвращает false.
	bool ShowPasswordDialog(QString &Password);
	//Пароль
	QString UserPassword;
	
	//Из базы ключей получает индекс открытого ключа получателя, а также открытый ключ 
	bool GetrHashOkDH(GranitDbI &DBI);
	
	//Проверяет существование файла по указанному пути.
	bool HasFile(QString path);

	//Выводит окно с предупреждением.
	void ShowWarning(string str);
	//Выводит окно с предупреждением.
	void ShowError(string str);

	//Отображает Yes/No диалог.
	bool ShowYNDialog(string str);
	

	//Используя данные кодека конвертирует строку Win1251 в utf16
	QString toUtf(string str);

	//Сохраняю массив в файл по указанному пути.
	void saveToFile(QByteArray &Ba,QString path);
	
	void CryptFiles(QStringList &paths_to_file, QString Password);

	//Выполняет действия  выбора файлов.
	void doOpenFiles();
		

	int badAttempPswd;//Количество не верных попыток ввода пароля.
	/*
	  Проверяет достижение максимального количества не верных попыток ввода пароля.
	  Если=3, выводит сообщение и запускает процесс закрытия программы.
	*/
	bool maxBadAttempPswd();

	struct Prm1
	{
		bool PwdIsChecked; //Пароль был проверен
		int ReceiverId; //Ид получателя секретного сообщения. Ид в базе данных ключей.
		string rHashOkDH; //Хэшированное значение открытого ключа получателя
		string Dh_OpenKey; //Открытый ключ получателя
						   //Переменная описывающая какая клавиша была нажата. Используется для вызова слота после выбора путей
		string SignerIndex;//Индекс ключа подписанта
		
	};

	Prm1 Param;

	//Возвращает текст о работе с программой.
	string HelloMessage();
	
	private slots:
	void slotMenuFileClicked(QAction *action);//Клик в меню "Файл"
	void slotMenuHelpClicked(QAction *action);//Клик в меню "Справка"
	void slotMenuSettings(QAction *action); //Меню настройки

	void slotpushButtonCrypt();//Нажатие на кнопку зашифровать
	void slotpushButtonDecrypt();//Нажатие на кнопку расшифровать
	void slotpushButtonSelectKey();//Нажатие на кнопку выбрать ключ	

	protected:
		bool eventFilter(QObject *target, QEvent *event); //Обработчик перехваченных событий.
	
};


