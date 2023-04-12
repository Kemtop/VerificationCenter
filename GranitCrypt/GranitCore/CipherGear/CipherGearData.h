#pragma once

#include <QObject>
#include "PkContainer.h"

class CipherWorkerData : public QObject
{
	Q_OBJECT

public:
	CipherWorkerData(QObject *parent);
	CipherWorkerData();
	~CipherWorkerData();

	//Конструктор копирования.
	CipherWorkerData(const CipherWorkerData&val);
	
	void setProcessFiles(QStringList val); //Устанавливает список полных путей файлов подлежащих обработке.
	QStringList getProcessFiles() const;

	void setPassword(QString val); //Пароль к закрытому ключу.
	QString  getPassword() const; //Пароль к закрытому ключу.

	void setRAsimOpenKey(std::string val); //Асимметричный ключ получателя.
	string getRAsimOpenKey() const;

	void setKeyContainer(PkContainer val); //Контейнер ключа.
	PkContainer getKeyContainer() const;

	void setSignerIndex(string val); //Индекс получателя.
	string getSignerIndex() const;

	void setSignatureDH(string val);
	string getSignatureDH() const;

	void setProcessMode(int Val); //Режим работы.
	int getProcessMode() const;

	void setDbPath(QString val); //Устанавливает путь к Бд системы.
	QString getDbPath()const;

	void setRoseMode(bool val); //Включает упрощенный режим работы.
	bool getRoseMode()const;

	//Оператор присваивания.
	CipherWorkerData& operator=(const CipherWorkerData &val);

private:
	QStringList filesToProcess; //Список полных путей файлов подлежащих обработке.	
	QString password; //Пароль для хранения ключа.
	std::string receiverKey; //Открытый ключ получателя.
	PkContainer keyContainer; //Ключ.
	string signerIndex; //Индекс получателя.
	string signatureDH; //Сигнатуру открытого ключа получателя.
	int processmode; //Режим работы-шифрование 1, расшифровка 2.
	QString DbPath; //Путь к Бд.
	bool enableRose; //Включен упрощенный режим работы.
};
