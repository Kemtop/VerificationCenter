#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_KeyGen.h"
#include <QTextCodec>
#include <QMessageBox>
#include <QFileDialog>
#include <QDomDocument>
#include <QMetaProperty>
#include "PkRequest.h"
#include <QTimer> //Для работы таймера
#include <QKeyEvent> //Для возможности узнавать код клавиши
#include <qdebug.h>
#include <QXmlStreamWriter>
#include <Windows.h>
#include "SettingTools.h"
#include "mRequest.h"
#include "SecretKeyMaker.h"


using std::string;


class KeyGen : public QMainWindow
{
	Q_OBJECT

public:
	KeyGen(QWidget *parent = Q_NULLPTR);

private:
	Ui::KeyGenClass ui;
   
	mRequest Req; //Модель запроса на получение сертификата.
	QTextCodec *StrCodec; //Для преобразования кодировки	
	
	QTimer* KeyTimer;
	QTimer* ClockTimer;//Таймер для работы часов. Подсчитывает время для нажатия клавиш
	QTimer* KeyBetweenTimer; //Измеряет период между нажатием клавиш

	int TimeKeyPress; //Время нажатия клавиши
	int TimeBetweenKeyPress;//Период между нажатием клавиш
							   //Часы
	struct s_clock
	{
		unsigned char sec;
		unsigned char min;
	};
	s_clock Clock; //Часы для подсчета времени работы программы


	int PeriodKeyPress[100];//Время нажатия клавиши
	int pos_PeriodKeyPress; //Позиция  в массиве PeriodKeyPress
	
	int PeriodBetWeenKeyPush[100]; //Интервалы между нажатиями клавиш.
	int pos_PeriodBetWeenKeyPush;

	int NowKeyCode; //Код текущей нажатой кнопки
	int LastKeyCode; //Код предыдущей нажатой клавиши
	bool EnableKeyBetweenTimer;//Флаг включения таймера измеряющего интервал между нажатиями клавиш

	 //Реализует часы обратного отсчета, в случае истечения времени возвращает 1 и останавливает отсчет
	bool ClockProcess();
	//Возвращает строку для отображения часов
	QString GetStrClock();

	//Очищает массивы PeriodKeyPress[100] PeriodBetWeenKeyPush[100]
	void ClearArrays();
	

	void initBioRng();//Инициализация биологического датчика случайных чисел.
	void enableAnimation();//Включает анимацию.
	void rotateAnime(); //Вращает рисунок.
	int animationValue;
	QTimer animationTimer;
	
	//Обрабатывает входные данные и помещает в массив  Key;
	void processRawData(unsigned char *Key);
	
	//Преобразовывает число. Если число больше 255 возвращает младшие 8 бит. Сдвигает исходное число в лево на 8 бит, 
	//полученное значение помещает в массив Excess.
	unsigned char ToUCDiget(int x,QVector<unsigned char>  &Excess);
	
	//Флаг разрешения генерации ключа
	bool EnableGeneratedKey;
	
	void ShowWarning(string mess);
	//Отображает сообщение об ошибке
	void ShowError(QString Message);

	//Выводит сообщение об успешной генерации ключа.
	void setHappyMassege();	

	//Создает файл с секретным ключем. Используется только для анализа последовательности!
	void createDebugFile(unsigned char *Key);

	//Отладочный метод.
	void createDebugValue(int v);

	private slots:
	void slotpushButtonW1Next();//Нажатие на кнопку "Далее" в первом виджете
	void slotpushButtonW2Back();//Нажатие на кнопку "Назад" во втором виджете

	void slotpushButtonW3Next();//Нажатие на кнопку "Далее" в  виджете ввода пароля.

	void slotpushButtonSelectKeyPath();//Выбор пути к секретному файлу
	void slotpushButtonSelectReqPath();//Выбор пути к файлу запроса

	//Тик таймера
	void slotKeyTimerTick(); //Срабатывание таймера измерения времени нажатия клавиши
	void slotClockTimerTick(); //Срабатывание таймера часов, и работа часов реального времени
	void slotKeyBetweenTimerTick(); //Срабатывание таймера замера периода между нажатием клавиш
	void slotKeyIsGenarated(); //Слот того что ключ сгенерированн
	void slotAnimationTimerTick();
	void slotClose();//Нажатие на клавишу "Закрыть".

protected:
	void keyPressEvent(QKeyEvent *); //Событие нажатия клавиши //Если кнопка удерживается некоторое время событие повторно вызывается
	 //Событие отпускания клавиши  Повторяется если клавишу зажали и держут
	void keyReleaseEvent(QKeyEvent *);

	
signals:
	void KeyIsGenarated(); //Сигнал того что ключ сгенерированн

};

