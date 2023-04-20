#pragma once
#include <QObject>
#include <QDateTime>
using namespace std; //
//Контейнер ключа
class _declspec(dllexport) PkContainer :public QObject
{
	Q_OBJECT
		//Определение свойств ,можно использовать MEMBER

		Q_PROPERTY(QString P0 READ P0 WRITE setP0) //Версия  ключа
		Q_PROPERTY(QString P1 READ P1 WRITE setP1) //Версия программы генератора ключа

		Q_PROPERTY(QString P2 READ P2 WRITE setP2) //Наименование организации.
		Q_PROPERTY(QString P3 READ P3 WRITE setP3) //ЕГРЮЛ.
		Q_PROPERTY(QString P4 READ P4 WRITE setP4) //Подразделение.
		Q_PROPERTY(QString P5 READ P5 WRITE setP5) //Телефон.
		Q_PROPERTY(QString P6 READ P6 WRITE setP6) //Familia
		Q_PROPERTY(QString P7 READ P7 WRITE setP7) //Imia
		Q_PROPERTY(QString P8 READ P8 WRITE setP8) //Otchestvo

		Q_PROPERTY(QDateTime P9 READ P9 WRITE setP9) //Зарезервировано(не используется).
		Q_PROPERTY(QDateTime P10 READ P10 WRITE setP10) //Зарезервировано(не используется).

		Q_PROPERTY(QString P11 READ P11 WRITE setP11)//шифрованный секретный ключ
		Q_PROPERTY(QString P12 READ P12 WRITE setP12)//соль
		Q_PROPERTY(QString P13 READ P13 WRITE setP13)//Дополнительный параметр
		Q_PROPERTY(QString P14 READ P14 WRITE setP14)//Вектор инициализации IV для ключа ГОСТ 34.11>

		Q_PROPERTY(QDateTime   P15 READ P15 WRITE setP15) //Дата генерации ключа  // Дополнительные параметры
		Q_PROPERTY(QString  P16 READ P16 WRITE setP16) //открытый ключ  координата X
		Q_PROPERTY(QString  P17 READ P17 WRITE setP17) //открытый ключ  координата Y
		Q_PROPERTY(QString  P18 READ P18 WRITE setP18) //Шифрованное значение закрытого ключа схемы Диффи-Хеллмана
		Q_PROPERTY(QString  P19 READ P19 WRITE setP19) //Значение вектора iv для закрытого ключа схемы Диффи-Хеллмана
		Q_PROPERTY(QString  P20 READ P20 WRITE setP20) //Открытый ключ схемы Диффи-Хеллмана
		Q_PROPERTY(QString  P21 READ P21 WRITE setP21) //Значение соли для закрытого ключа схемы Диффи-Хеллмана

		//Алгоритмы ключа
		//PublicKeyAlgoritm-алгоритм открытого ключа
		Q_PROPERTY(QString P22 READ P22 WRITE setP22)
	    Q_PROPERTY(QString P23 READ P23 WRITE setP23) //Зарезервировано.
		//EcOid-параметры эллиптической кривой(для открытого ключа подписи по ГОСТ)
		Q_PROPERTY(QString P24 READ P24 WRITE setP24)

private:
	QString cP0;
	QString cP1;
	QString cP2;
	QString cP3;
	QString cP4;
	QString cP5;
	QString cP6;
	QString cP7;
	QString cP8;
	QDateTime cP9;
	QDateTime cP10;
	QString cP11;
	QString cP12;
	QString cP13;
	QString cP14;
	QDateTime   cP15;
	QString  cP16;
	QString  cP17;
	QString cP18;
	QString cP19;
	QString cP20;
	QString cP21;
	QString cP22;
	QString cP23;
	QString cP24;

public:

	PkContainer(QObject *parent = 0) :
		QObject(parent) 
	{
		;
	}
	~PkContainer();

	//Конструктор копирования.
	PkContainer(const PkContainer &val)
	{
		setP0(val.P0());
		setP1(val.P1());
		setP2(val.P2());
		setP3(val.P3());
		setP4(val.P4());
		setP5(val.P5());
		setP6(val.P6());
		setP7(val.P7());
		setP8(val.P8());
		setP9(val.P9());
		setP10(val.P10());
		setP11(val.P11());
		setP12(val.P12());
		setP13(val.P13());
		setP14(val.P14());
		setP15(val.P15());
		setP16(val.P16());
		setP17(val.P17());
		setP18(val.P18());
		setP19(val.P19());
		setP20(val.P20());
		setP21(val.P21());
		setP22(val.P22());
		setP23(val.P23());
		setP24(val.P24());
	}

	//Оператор присваивания.
	PkContainer& operator=(const PkContainer &val)
	{
		//проверка на самоприсваивание
		if (this == &val) return *this;

		setP0(val.P0());
		setP1(val.P1());
		setP2(val.P2());
		setP3(val.P3());
		setP4(val.P4());
		setP5(val.P5());
		setP6(val.P6());
		setP7(val.P7());
		setP8(val.P8());
		setP9(val.P9());
		setP10(val.P10());
		setP11(val.P11());
		setP12(val.P12());
		setP13(val.P13());
		setP14(val.P14());
		setP15(val.P15());
		setP16(val.P16());
		setP17(val.P17());
		setP18(val.P18());
		setP19(val.P19());
		setP20(val.P20());
		setP21(val.P21());
		setP22(val.P22());
		setP23(val.P23());
		setP24(val.P24());
		
		return *this;
	}

	void setP0(QString P) { cP0 = P; };
	QString P0() const { return cP0; }

	void setP1(QString P) { cP1 = P; };
	QString P1() const { return cP1; }

	void setP2(QString P) { cP2 = P; };
	QString P2() const { return cP2; }

	void setP3(QString P) { cP3 = P; };
	QString P3() const { return cP3; }

	void setP4(QString P) { cP4 = P; };
	QString P4() const { return cP4; }

	void setP5(QString P) { cP5 = P; };
	QString P5() const { return cP5; }

	void setP6(QString P) { cP6 = P; };
	QString P6() const { return cP6; }

	void setP7(QString P) { cP7 = P; };
	QString P7() const { return cP7; }
	
	void setP8(QString P) { cP8 = P; };
	QString P8() const { return cP8; }
	
	void setP9(QDateTime P) { cP9 = P; };
	QDateTime P9() const { return cP9; }

	void setP10(QDateTime P) { cP10 = P; };
	QDateTime P10() const { return cP10; }
	//---
	void setP11(QString P) { cP11 = P; };
	QString P11() const { return cP11; }

	void setP12(QString P) { cP12 = P; };
	QString P12() const { return cP12; }

	void setP13(QString P) { cP13 = P; };
	QString P13() const { return cP13; }

	void setP14(QString P) { cP14 = P; };
	QString P14() const { return cP14; }

	void setP15(QDateTime  P) { cP15 = P; };
	QDateTime P15() const { return cP15; }

	void setP16(QString P) { cP16 = P; };
	QString P16() const { return cP16; }

	void setP17(QString P) { cP17 = P; };
	QString P17() const { return cP17; }

	void setP18(QString P) { cP18 = P; };
	QString P18() const { return cP18; }

	void setP19(QString P) { cP19 = P; };
	QString P19() const { return cP19; }

	void setP20(QString P) { cP20 = P; };
	QString P20() const { return cP20; }

	void setP21(QString P) { cP21 = P; };
	QString P21() const { return cP21; }

	void setP22(QString P) { cP22 = P; };
	QString P22() const { return cP22; }

	void setP23(QString P) { cP23 = P; };
	QString P23() const { return cP23; }
	
	void setP24(QString P) { cP24 = P; };
	QString P24() const { return cP24; }
};

