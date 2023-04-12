#pragma once
#include <QObject>

class  mRequest : public QObject
{
	Q_OBJECT
		//Определение свойств ,можно использовать MEMBER
		Q_PROPERTY(QString P0 READ P0 WRITE setP0) //Фамилия
		Q_PROPERTY(QString P1 READ P1 WRITE setP1) //Имя 
		Q_PROPERTY(QString P2 READ P2 WRITE setP2) //Отчество
		Q_PROPERTY(QString P3 READ P3 WRITE setP3) //Подразделение

		Q_PROPERTY(QString P4 READ P4 WRITE setP4) //Наименование организации
		Q_PROPERTY(QString P5 READ P5 WRITE setP5)//Егрюл

		Q_PROPERTY(QString P6 READ P6 WRITE setP6)//Район
		Q_PROPERTY(QString P7 READ P7 WRITE setP7) //Город
		Q_PROPERTY(QString P8 READ P8 WRITE setP8) //Адрес
		Q_PROPERTY(QString P9 READ P9 WRITE setP9) //Контактный телефон
		Q_PROPERTY(QString P10 READ P10 WRITE setP10) //Электронная почта

		Q_PROPERTY(QString P11 READ P11 WRITE setP11) //Версия генератора ключа.
		Q_PROPERTY(QString P12 READ P12 WRITE setP12)//Открытый ключ пользователя для шифрования по алгоритму Диффи-Хэлмана
		Q_PROPERTY(QString P13 READ P13 WRITE setP13)//Зарезервировано для будущего использования, и для запутывания злоумышленника. 
	
	
		//Текущий открытый ключ пользователя по ГОСТ 34.10.
		Q_PROPERTY(QString P14 READ P14 WRITE setP14)//Координата x 
		Q_PROPERTY(QString P15 READ P15 WRITE setP15)//Координата y
         //Алгоритмы ключа
		Q_PROPERTY(QString P16 READ P16 WRITE setP16)//PublicKeyAlgoritm-алгоритм открытого ключа
		Q_PROPERTY(QString P17 READ P17 WRITE setP17)//EcOid-параметры эллиптической кривой
		//Хэш ключа подписи. Заполняется только после формирования запроса!!! Используется только для формирования WORD документа.
		//Так мы усложняем злоумышленнику подделку данных.
		Q_PROPERTY(QString P18 READ P18 WRITE setP18)
		Q_PROPERTY(int P19 READ P19 WRITE setP19) //Порядковый номер заявки со стороны клиента.
		
		
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
	int cP19;

public:
		
	mRequest(QObject *parent = 0): QObject(parent)
	{
		cP19=0;
	}
	~mRequest();


	//Конструктор копирования.
	mRequest(const mRequest &val)
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
		//setP20(val.P20());
		//setP21(val.P21());
		//setP22(val.P22());
		//setP23(val.P23());
		//setP24(val.P24());
	}

	//Оператор присваивания.
	mRequest& operator=(const mRequest &val)
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
		//setP20(val.P20());
		//setP21(val.P21());
		//setP22(val.P22());
		//setP23(val.P23());
		//setP24(val.P24());

		return *this;
	}


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

	void setP7(QString Prm) { cP7 = Prm; }
	QString  P7() const { return cP7; }

	void setP8(QString Prm) { cP8 = Prm; }
	QString  P8() const { return cP8; }

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

	void setP14(QString Prm) { cP14= Prm; }
	QString  P14() const { return cP14; }

	void setP15(QString Prm) { cP15 = Prm; }
	QString  P15() const { return cP15; }

	void setP16(QString Prm) { cP16 = Prm; }
	QString  P16() const { return cP16; }

	void setP17(QString Prm) { cP17 = Prm; }
	QString  P17() const { return cP17; }

	void setP18(QString Prm) { cP18 = Prm; }
	QString  P18() const { return cP18; }

	void setP19(int Prm) { cP19 = Prm; }
	int  P19() const { return cP19; }
};
