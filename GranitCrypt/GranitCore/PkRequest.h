#pragma once
#include <QObject>
#include <string>
#include <QDateTime>

//КЛАСС НЕ ИСПОЛЬЗУЕТСЯ!
using  namespace  std;
//Запрос на получение ключа.
class _declspec(dllexport) PkRequest :public QObject
{
	Q_OBJECT
		//Определение свойств ,можно использовать MEMBER
		Q_PROPERTY(QString P0 READ P0 WRITE setP0) //GroupUUID
		Q_PROPERTY(QString P1 READ P1 WRITE setP1) //GroupHash
		Q_PROPERTY(QString P2 READ P2 WRITE setP2) //UserUUID
		Q_PROPERTY(QString P3 READ P3 WRITE setP3) //UserHash

		Q_PROPERTY(QString P4 READ P4 WRITE setP4) //Familia 
		Q_PROPERTY(QString P5 READ P5 WRITE setP5)//Imia
		Q_PROPERTY(QString P6 READ P6 WRITE setP6)//Otchestvo
		
		//Срок действия ключа-не исп.
		Q_PROPERTY(QDateTime P7 READ P7 WRITE setP7) //Дата начала действия ключа
		Q_PROPERTY(QDateTime P8 READ P8 WRITE setP8) //Дата окончания действия ключа

		Q_PROPERTY(QString P9 READ P9 WRITE setP9)//Ожидаемый номер макета ключа

	
		Q_PROPERTY(QString P10 READ P10 WRITE setP10) //Не используется

		//Версия ЦРК 
		Q_PROPERTY(QString P11 READ P11 WRITE setP11)

		//Версия макета ключа
		Q_PROPERTY(QString P12 READ P12 WRITE setP12)

		Q_PROPERTY(QString P13 READ P13 WRITE setP13)//Дополнительный параметр=0

		Q_PROPERTY(QString P14 READ P14 WRITE setP14) //Ожидаемый номер ключа

		//Новый открытый ключ пользователя-в случае продления ключа по ГОСТ 34.11
		Q_PROPERTY(QString P15 READ P15 WRITE setP15)

		//Подпись ЦРК выше указанных строк --не используется 
		Q_PROPERTY(QString P16 READ P16 WRITE setP16) //=0
	    //Открытый ключ пользователя для шифрования по алгоритму Диффи-Хэлмана
		Q_PROPERTY(QString P17 READ P17 WRITE setP17) //
		 //Текущий открытый ключ пользователя по ГОСТ 34.10
		Q_PROPERTY(QString P18 READ P18 WRITE setP18) //Координата x
		Q_PROPERTY(QString P19 READ P19 WRITE setP19) //Координата y
		//Алгоритмы ключа
		//PublicKeyAlgoritm-алгоритм открытого ключа
		Q_PROPERTY(QString P20 READ P20 WRITE setP20)
		//SignCertAlgoritm-алгоритм подписи сертификата
		Q_PROPERTY(QString P21 READ P21 WRITE setP21)
		//EcOid-параметры эллиптической кривой
		Q_PROPERTY(QString P22 READ P22 WRITE setP22)



private:
	QString cP0;
	QString cP1;
	QString cP2;
	QString cP3;
	QString cP4;
	QString cP5;
	QString cP6;
	QDateTime cP7;
	QDateTime cP8;
	QString cP9;
	QString cP10;
	QString cP11;
	QString cP12;
	QString cP13;
	QString cP14;
	QString cP15;
	QString cP16;
	QString cP17;
	QString cP18;
	QString cP19;
	QString cP20;
	QString cP21;
	QString cP22;
	
	

public:
	//XmlRequest();
	PkRequest(QObject *parent = 0) :
		QObject(parent)
	{
	}
	~PkRequest();
	
	void setP0(QString Prm) { cP0 = Prm; }
	QString  P0() const { return cP0; }

	void setP1(QString Prm) { cP1 = Prm; }
	QString  P1() const { return cP1; }

	void setP2(QString Prm) { cP2 = Prm; }
	QString  P2() const { return cP2; }

	void setP3(QString Prm) { cP3 = Prm; }
	QString  P3() const { return cP3; }

	void setP4(QString Prm) { cP4 = Prm; }
	QString  P4() const { return cP4; }

	void setP5(QString Prm) { cP5 = Prm; }
	QString  P5() const { return cP5; }

	void setP6(QString Prm) { cP6 = Prm; }
	QString  P6() const { return cP6; }

	void setP7(QDateTime Prm) { cP7 = Prm; }
	QDateTime  P7() const { return cP7; }

	void setP8(QDateTime Prm) { cP8 = Prm; }
	QDateTime  P8() const { return cP8; }

	void setP9(QString Prm) { cP9 = Prm; }
	QString  P9() const { return cP9; }

	void setP10(QString Prm) { cP10 = Prm; }
	QString  P10() const { return cP10; }

	void setP11(QString Prm) { cP11 = Prm; }
	QString  P11() const { return cP11; }

	void setP12(QString Prm) { cP12 = Prm; }
	QString  P12() const { return cP12; }

	void setP13(QString Prm) { cP13 = Prm; }
	QString  P13() const { return cP13; }

	void setP14(QString Prm) { cP14 = Prm; }
	QString  P14() const { return cP14; }

	void setP15(QString Prm) { cP15 = Prm; }
	QString  P15() const { return cP15; }

	void setP16(QString Prm) { cP16 = Prm; }
	QString  P16() const { return cP16; }

	void setP17(QString Prm) { cP17 = Prm; }
	QString  P17() const { return cP17; }

	void setP18(QString Prm) { cP18 = Prm; }
	QString  P18() const { return cP18; }

	void setP19(QString Prm) { cP19 = Prm; }
	QString  P19() const { return cP19; }

	void setP20(QString Prm) { cP20 = Prm; }
	QString  P20() const { return cP20; }

	void setP21(QString Prm) { cP21 = Prm; }
	QString  P21() const { return cP21; }

	void setP22(QString Prm) { cP22 = Prm; }
	QString  P22() const { return cP22; }

};

