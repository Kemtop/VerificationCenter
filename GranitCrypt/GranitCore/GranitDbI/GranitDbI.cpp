#include "GranitDbI.h"



GranitDbI::GranitDbI(QString PathDb)
{
	PathToDb = PathDb;
	UserId = -100;
}

GranitDbI::GranitDbI()
{
	UserId = -100;
}


GranitDbI::~GranitDbI()
{
}

void GranitDbI::setDbPath(QString PathDb)
{
	PathToDb = PathDb;
}

//Открывает соединение с ББ по указанному пути.
bool GranitDbI::OpenDb()
{
	//БД уже открыта
	if (Db.isOpen()) return true;

	Db = QSqlDatabase::addDatabase("QSQLITE", "CertDb");
	Db.setDatabaseName(PathToDb);

	if (Db.open() == false)
	{
		ShowError("Ошибка БС 10: Не удалось открыть базу данных системы.");
		return false;
	}

	return true;
}

bool GranitDbI::OpenDb(string PathPlugins, string PathDb, string & Error)
{
	//БД уже открыта
	if (Db.isOpen()) return true;


	QString _PathDb = QString::fromLocal8Bit(PathDb.c_str());

	//Указываю путь к плагинам, среди которых содержится драйвер QSQLITE.
	QStringList liblist;
	liblist.push_back(QString::fromLocal8Bit(PathPlugins.c_str()));
	QCoreApplication::setLibraryPaths(liblist);
	
	//ShowDriversList(); //Вывести список драйверов

	Db = QSqlDatabase::addDatabase("QSQLITE", "QSQLITE");
	Db.setDatabaseName(_PathDb);
	if (Db.open() == false)
	{
		QString m = Db.lastError().databaseText();
		Error = m.toStdString();
		return false;
	}

	return true;
}

void GranitDbI::CloseDb()
{
	Db.close();
}


void GranitDbI::ShowDriversList()
{
	list<QString> DrList = QSqlDatabase::drivers().toStdList();

	//Сборка строки сообщения
	string Mess = "";
	auto  current = DrList.begin();
	auto last = DrList.end();

	while (current != last)
	{
		QString str = *current;
		Mess += str.toStdString() + "\r\n";

		current = std::next(current, 1);
	}

	//MessageBoxA(GetActiveWindow(), Mess.c_str(), "SqlDriverList", MB_ICONINFORMATION);
	
}


//Получает сертификат пользователя на основании данных открытого ключа подписи.
bool GranitDbI::GetUserCert(QString Qx, QString Qy, QString F, QString I, QString O)
{
	string Kx = Qx.toStdString();
	string Ky = Qy.toStdString();
	string Qindex = VanishChest::QkeyMD5(Kx, Ky); //Индекс на основании открытого ключа.
	QString QIndex = QString::fromStdString(Qindex);

	QString FIO = F + " " + I + " " + O;

	//Ищу пользователя на основание индекса открытого ключа.
	QString sql = "select * from TableT2 where B=:ix";
	if (!OpenDb())
	{
		lastError = "Ошибка БС 700: Не удалось открыть список сертификатов.";
		return false;
    }

	QSqlQuery query(Db);

	if(!query.prepare(sql))
	{
		lastError = query.lastError().databaseText().toStdString();
		return false;
	}

	query.bindValue(":ix", QIndex);
	
	bool bad = query.exec();
	if (!bad)
	{
		lastError = query.lastError().databaseText().toStdString();
		return false;
	}
		

	//Листаю результат если он есть.
	bool found = false; //Флаг нахождения результата.
	int UserId_ = -10; //Ид пользователя
	int LineStts = -10; //Статус строки.

	//Необходимо так как существует крайне малая вероятность что в очень большой БД индексы могут совпасть.
	while (query.next())
	{
		QString enFIO_ = query.value("A").toString(); //Получаю значение поля ФИО.
		QString FIO_ = VanishChest::UnModifyExUserFIO(enFIO_); //Преобразовываю в нормальный вид.

		if (FIO == FIO_) //Строка найдена.
		{
		    UserId_ = query.value("id").toInt(); //Ид пользователя
			int LineStts= query.value("E").toInt(); //Статус строки.
			found = true;
			break;
		}
	}
	
	//Нет записей
	if (found==false)
	{
		lastError="Ошибка БС 701: Отсутствует сертификат для данного ключа.";
		return false;
	}

    //Запись удалена.
	if (LineStts==0)
	{
		lastError = "Ошибка БС 702: Владелец ключа удален в системе или аннулирован сертификат.";
		return false;
	}

	//Поиск не аннулированного сертификата.
	sql = "select * from TableT3 where J=1 and A="+QString::number(UserId_);
	if (!query.prepare(sql))
	{
		lastError = query.lastError().databaseText().toStdString();
		return false;
	}

	 bad = query.exec();
	if (!bad)
	{
		lastError = query.lastError().databaseText().toStdString();
		return false;
	}

	found = false; //Думаю что нет действующего сертификата.
	QString G;
	QByteArray CertA; //Найденный сертификат.
	QString CrcCert;//Значение контрольной суммы сертификата.
	int CertSize;//Размер сертификата.
	QString DtBegin; //Дата начала действия сертификата
	QString DtEnd; //Дата окончания действия сертификата


	while (query.next())
	{
		//Сравниваю индексы.
		G= query.value("G").toString(); //Индекс на основании открытого ключа пользователя.
		if (G != QIndex) continue; //Индекс не совпал.
		
		//Сертификат найден получаю данные.
		CertA = query.value("B").toByteArray();

		 CrcCert=query.value("C").toString();//Значение контрольной суммы сертификата.
		 CertSize=query.value("D").toInt();//Размер сертификата.
		 DtBegin=query.value("E").toString(); //Дата начала действия сертификата
		 DtEnd= query.value("F").toString(); //Дата окончания действия сертификата
		
		found = true;
		break;
	}

	//Сертификат не обнаружен.
	if (found == false)
	{
		lastError="Ошибка БС 703: Отсутствует сертификат для данного ключа. Возможно он аннулирован или не получен.";
		return false;
	}

	//Проверка контрольной суммы сертификата.
	if(!CheckCertCrc(CertA, CertSize, CrcCert))
	{
		lastError = "Ошибка БС 704: Сертификат открытого ключа поврежден или был изменен третьим лицом.";
		return false;
	}


	//Проверка периода действия сертификата.
	if(!CheckCertUsePeriod(DtBegin, DtEnd)) return false;

	//Передаю данные
	UserId = UserId_;
	UserCert = CertA;
	UserQIndex = Qindex;

	return true;
}

bool GranitDbI::getSignerIndex(QString Qx, QString Qy, string &Qindex)
{
	string Kx = Qx.toStdString();
	string Ky = Qy.toStdString();
	Qindex = VanishChest::QkeyMD5(Kx, Ky); //Индекс на основании открытого ключа.
	
	return true;
}


//Получает сертификат пользователя на основании данных открытого ключа подписи, с минимумом проверок.
//Используется для сохранения сертификата пользователя из БД.
bool GranitDbI::GetUserCert(QString Qx, QString Qy, QString F, QString I, QString O, QByteArray &uCert_)
{
	string Kx = Qx.toStdString();
	string Ky = Qy.toStdString();
	string Qindex = VanishChest::QkeyMD5(Kx, Ky); //Индекс на основании открытого ключа.
	QString QIndex = QString::fromStdString(Qindex);

	QString FIO = F + " " + I + " " + O;

	//Ищу пользователя на основание индекса открытого ключа.
	QString sql = "select * from TableT2 where B=:ix";
	if (!OpenDb())
	{
		ShowError("Ошибка БС 700: Не удалось открыть список сертификатов.");
		return false;
	}

	QSqlQuery query(Db);

	if (!query.prepare(sql))
	{
		QMessageBox::critical(0, "Error", query.lastError().databaseText());
		return false;
	}

	query.bindValue(":ix", QIndex);

	bool bad = query.exec();
	if (!bad)
	{
		QMessageBox::critical(0, "Error", query.lastError().databaseText());
		return false;
	}


	//Листаю результат если он есть.
	bool found = false; //Флаг нахождения результата.
	int UserId_ = -10; //Ид пользователя
	int LineStts = -10; //Статус строки.

						//Необходимо так как существует крайне малая вероятность что в очень большой БД индексы могут совпасть.
	while (query.next())
	{
		QString enFIO_ = query.value("A").toString(); //Получаю значение поля ФИО.
		QString FIO_ = VanishChest::UnModifyExUserFIO(enFIO_); //Преобразовываю в нормальный вид.

		if (FIO == FIO_) //Строка найдена.
		{
			UserId_ = query.value("id").toInt(); //Ид пользователя
			int LineStts = query.value("E").toInt(); //Статус строки.
			found = true;
			break;
		}
	}

	//Нет записей
	if (found == false)
	{
		ShowError("Ошибка БС 701: Отсутствует сертификат для данного ключа.");
		return false;
	}


	//Поиск сертификата, даже если он аннулирован.
	sql = "select * from TableT3 where  A=" + QString::number(UserId_);
	if (!query.prepare(sql))
	{
		QMessageBox::critical(0, "Error", query.lastError().databaseText());
		return false;
	}

	bad = query.exec();
	if (!bad)
	{
		QMessageBox::critical(0, "Error", query.lastError().databaseText());
		return false;
	}

	found = false; //Думаю что нет действующего сертификата.
	QString G;
	QByteArray CertA; //Найденный сертификат.
	QString CrcCert;//Значение контрольной суммы сертификата.
	int CertSize;//Размер сертификата.
	QString DtBegin; //Дата начала действия сертификата
	QString DtEnd; //Дата окончания действия сертификата


	while (query.next())
	{
		//Сравниваю индексы.
		G = query.value("G").toString(); //Индекс на основании открытого ключа пользователя.
		if (G != QIndex) continue; //Индекс не совпал.

								   //Сертификат найден получаю данные.
		CertA = query.value("B").toByteArray();

		CrcCert = query.value("C").toString();//Значение контрольной суммы сертификата.
		CertSize = query.value("D").toInt();//Размер сертификата.
		DtBegin = query.value("E").toString(); //Дата начала действия сертификата
		DtEnd = query.value("F").toString(); //Дата окончания действия сертификата

		found = true;
		break;
	}

	//Сертификат не обнаружен.
	if (found == false)
	{
		ShowError("Ошибка БС 703: Отсутствует сертификат для данного ключа.");
		return false;
	}

	//Проверка контрольной суммы сертификата.
	if (!CheckCertCrc(CertA, CertSize, CrcCert))
	{
		ShowError("Ошибка БС 704: Сертификат открытого ключа поврежден или был изменен третьим лицом.");
		return false;
	}

	//Передаю данные
	uCert_ = CertA;

	return true;
}

bool GranitDbI::GetUserCert(PkContainer & KeyContainer, QByteArray &uCert)
{
	bool Estts = GetUserCert(KeyContainer.P16(), KeyContainer.P17(), KeyContainer.P6(),
		KeyContainer.P7(), KeyContainer.P8(), uCert);
	if (!Estts) return false;

}


QByteArray GranitDbI::getCert()
{
	return UserCert;
}

//Получает сертификат CA. В случае ошибки выводит сообщение. Возвращает false.
bool GranitDbI::getCACert(QByteArray &CertCA)
{
	//Выбираю действующий сертификат CA.
	QString sql = "select * from TableT5 where A=1 AND H=1";

	QSqlQuery query(Db);

	if (!query.prepare(sql))
	{
		QMessageBox::critical(0, "Error", query.lastError().databaseText());
		return false;
	}

	bool bad = query.exec();
	if (!bad)
	{
		QMessageBox::critical(0, "Error", query.lastError().databaseText());
		return false;
	}


	QByteArray CAcert_;
	QString certHash;
	int certLen;

	int count = 0;//Количество строк.
	while (query.next())
	{
		CAcert_=query.value("B").toByteArray();
		certHash = query.value("C").toString();
		certLen = query.value("D").toInt();
		count++;
	}


	if (count == 0)
	{
		string str = "Ошибка БС 707A:Не обнаружено действующего сертификата СА.";
		ShowError(str);
		return false;
	}

	//Найдено более 1го сертификата CA.
	if (count > 1)
	{
		string str = "Ошибка БС 707: Обнаружено более 1 действующего сертификата СА("+std::to_string(count)+").";
		ShowError(str);
		return false;
	}



	//Проверяю длину сертификата.
	if (certLen != CAcert_.size())
	{
		ShowError("Ошибка БС 708: Сертификат СА поврежден или был изменен третьим лицом.");
		return false;
	}



	//Проверяю контрольную сумму сертификата.
	QString curCertHash = VanishChest::CrcSystemCertificate(CAcert_);
	if (curCertHash != certHash)
	{
		ShowError("Ошибка БС 709: Сертификат СА поврежден или был изменен третьим лицом.");
		return false;
	}

	
	CertCA = CAcert_;

	return true;
}
//Получает сертификат CA. В случае ошибки выводит сообщение. Возвращает false если ошибка.
bool GranitDbI::getRootCert(QByteArray &CertRoot)
{
	//Выбираю действующий сертификат CA.
	QString sql = "select * from TableT5 where A=0 AND H=1";

	QSqlQuery query(Db);

	if (!query.prepare(sql))
	{
		QMessageBox::critical(0, "Error", query.lastError().databaseText());
		return false;
	}

	bool bad = query.exec();
	if (!bad)
	{
		QMessageBox::critical(0, "Error", query.lastError().databaseText());
		return false;
	}


	QByteArray Rootcert_;
	QString certHash;
	int certLen;

	int count = 0;//Количество строк.
	while (query.next())
	{
		Rootcert_ = query.value("B").toByteArray();
		certHash = query.value("C").toString();
		certLen = query.value("D").toInt();
		count++;
	}

	//Найдено более 1го сертификата Root.
	if (count > 1)
	{
		string str = "Ошибка БС 710: Обнаружено более 1 действующего сертификата Root(" + std::to_string(count) + ").";
		ShowError(str);
		return false;
	}

	if (count == 0)
	{
		string str = "Ошибка БС 710A: Не обнаружено  действующего сертификата Root.";
		ShowError(str);
		return false;
	}


	//Проверяю длину сертификата.
	if (certLen != Rootcert_.size())
	{
		ShowError("Ошибка БС 711: Сертификат Root поврежден или был изменен третьим лицом.");
		return false;
	}



	//Проверяю контрольную сумму сертификата.
	QString curCertHash = VanishChest::CrcSystemCertificate(Rootcert_);
	if (curCertHash != certHash)
	{
		ShowError("Ошибка БС 712: Сертификат Root поврежден или был изменен третьим лицом.");
		return false;
	}

	CertRoot = Rootcert_;

	return true;
}


//Получаю список открытых ключей пользователей. Используется для выбора пользователя которому отправлять сообщение.
//В случае ошибок выводи сообщение и возвращает false.
bool GranitDbI::getOpenKeyList(QStandardItemModel *model)
{
	QString sql = "select t2.id,t2.A,t3.E,t3.F from TableT2 t2 join TableT3 t3 on t3.A=t2.id "
	" where t2.E=1"; 

	QSqlQuery query(Db);
	
	if (!query.prepare(sql))
	{
		QMessageBox::critical(0, "Error", query.lastError().databaseText());
		return false;
	}
	

	bool bad = query.exec();
	if (!bad)
	{
		QMessageBox::critical(0, "Error", query.lastError().databaseText());
		return false;
	}


	QString colNames[4] = {"id","A","E","F"};
	QString tmp;

	//Заполняю модель
	int row = 0;
	while (query.next())
	{
		for (int col = 0;col < 4;col++) //Цикл по столбцам.
		{
			tmp = query.value(colNames[col]).toString();
			if (col == 1) //FIO преобразовываю
			{
				tmp = VanishChest::UnModifyExUserFIO(tmp);
			}
			QStandardItem *item = new 	QStandardItem(tmp);
			model->setItem(row, col, item);
		}
		row++;
	}
		

	QTextCodec *StrCodec = QTextCodec::codecForName("Windows-1251");
	QString str1 = StrCodec->toUnicode("Ф.И.О.");
	model->setHeaderData(1, Qt::Horizontal, str1.toStdString().c_str());

	str1 = StrCodec->toUnicode("С");
	model->setHeaderData(2, Qt::Horizontal, str1.toStdString().c_str());

	str1 = StrCodec->toUnicode("По");
	model->setHeaderData(3, Qt::Horizontal, str1.toStdString().c_str());	

	return true;
}

//Получает открытый ключ схемы Д-Х для данного пользователя. В случае ошибок выводи сообщение и возвращает false.
bool GranitDbI::getDHOpenKey(string & rHashOkDH, string & Dh_OpenKey, int UserId)
{
	QString sql = "select C,D,E from TableT4 where E=1 AND A=?";
		
	QSqlQuery query(Db);

	if (!query.prepare(sql))
	{
		QMessageBox::critical(0, "Error", query.lastError().databaseText());
		return false;
	}

	query.addBindValue(UserId);

	bool bad = query.exec();
	if (!bad)
	{
		QMessageBox::critical(0, "Error", query.lastError().databaseText());
		return false;
	}

	//Считываю значение хэш открытого ключа ДХ
	int cnt = 0;

	QString Dh_OpenKey_;
	QString hCrcDH_;

	int Status = 0;
	while (query.next())
	{
		hCrcDH_= query.value("C").toString();
		Dh_OpenKey_= query.value("D").toString();

		Status = query.value("E").toInt(); //Состояние строки.
		cnt++;
	}

	//Нет строк.
	if (cnt == 0)
	{
		ShowError("Ошибка Д 718: Отсутствуют данные.");
		return false;
	}
	
	//Найденно более 1-й строки.
	if (cnt>1)
	{
		string h = "Ошибка Д 719: Множественное отображение.n=" + std::to_string(cnt)+".";
		ShowError(h);
		return false;
	}

	//Строка удалена
	if (Status == 0)
	{
		ShowError("Ошибка Д 720: Отсутствуют данные.");
		return false;
	}

	//Пустая строка
	if (Dh_OpenKey_.isEmpty())
	{
		ShowError("Ошибка Д 721: Отсутствуют данные.");
		return false;
	}

	//Пустая строка
	if (hCrcDH_.isEmpty())
	{
		ShowError("Ошибка Д 722: Отсутствуют данные.");
		return false;
	}


	QString Crc; //Контрольная сумма для проверки открытого ключа.
	VanishChest::CrcDHKey(Crc, Dh_OpenKey_); //Формирую контрольную сумму.
	
	//Проверяю контрольную сумму.
	if (Crc != hCrcDH_)
	{
		ShowError("Ошибка Д 723: Поврежден асимметричный блок.");
		return false;
	}


	QString DHkey; //Открытый ключ схемы ДХ в нормальном виде.
	VanishChest::UnModifyDHKey(Dh_OpenKey_, DHkey);

	rHashOkDH = hCrcDH_.toStdString();
	Dh_OpenKey = DHkey.toStdString();

	return true;
}

string GranitDbI::getQIndex()
{
	return UserQIndex;
}

//Используя индекс пользователя, получает данные открытого ключа(ключа проверки подписи).
bool GranitDbI::getSignOpenKey(string & Qindex, string & Qx, string & Qy, QString & FIO)
{
	QString _Qindex(Qindex.c_str()); //Индекс открытого ключа подписанта
	
	//Разложить в случае много поточной работы. Или закешировать в памяти.
	QString sql = "select t2.A xFIO,t6.B,t6.C from TableT2 t2 inner join TableT6 t6 on t6.A=t2.id "\
	" where t2.B=:ix";

	QSqlQuery query(Db);

	if (!query.prepare(sql))
	{
		QMessageBox::critical(0, "Error", query.lastError().databaseText());
		return false;
	}

	query.bindValue(":ix",_Qindex);


	bool bad = query.exec();
	if (!bad)
	{
		QMessageBox::critical(0, "Error", query.lastError().databaseText());
		return false;
	}

	QString _FIO, _Qx, _Qy;
	int cnt = 0; //Количество записей.
	while (query.next())
	{
		_FIO= query.value(0).toString(); //xFIO,t6.B,t6.C
		_Qx=query.value(1).toString();
		_Qy= query.value(2).toString();
		cnt++;
	}

	//Нет записей.
	if (cnt == 0)
	{
		ShowError("Ошибка 724: Не найдены данные ключа проверки подписи.");
		return false;
	}

	//Записей более 1.
	if (cnt>1)
	{
		string h = "Ошибка 725: Множественные ссылки на данные ключа проверки подписи.n="+std::to_string(cnt)+".";
		ShowError(h);
		return false;
	}

	QString FIO_ = VanishChest::UnModifyExUserFIO(_FIO); //Преобразовываю в нормальный вид.
	
	//Считываю значение FIO,C,D
	FIO = FIO_;

	Qx = _Qx.toStdString(); 
	Qy = _Qy.toStdString();

	return true;
}

bool GranitDbI::findUserCert(string digest, string num, QByteArray &cert)
{
	//Разложить в случае много поточной работы. Или закешировать в памяти.
	QString sql = "select t3.* from TableT3 t3"\
		" where t3.K=:k and t3.L=:l and J=1;"; //J=1 Действующий сетрификат.

	QSqlQuery query(Db);

	if (!query.prepare(sql))
	{
		lastError = query.lastError().databaseText().toStdString();
		return false;
	}

	query.bindValue(":k", QString::fromStdString(num));
	query.bindValue(":l", QString::fromStdString(digest));


	bool bad = query.exec();
	if (!bad)
	{
		lastError = query.lastError().databaseText().toStdString();
		return false;
	}

	
	int cnt = 0; //Количество записей.
	QByteArray res;

	while (query.next())
	{
	    res=query.value("B").toByteArray();
		cnt++;
	}

	cert = res;

	//Нет записей.
	if (cnt == 0) return true;
	

	//Записей более 1.
	if (cnt>1)
	{
		lastError= "Ошибка 725AB: Множественные ссылки на данные ключа проверки подписи.n=" + std::to_string(cnt) + ".";
		return false;
	}
	
	return true;
}

bool GranitDbI::getDbDateEnd(QString &dtEnd)
{
	if (!OpenDb())
	{
		lastError="Ошибка БС 700: Не удалось открыть список сертификатов.";
		return false;
	}

	QString sql = "select A,B from TableT7";
	QSqlQuery query(Db);

	if (!query.prepare(sql))
	{
		lastError = query.lastError().databaseText().toStdString();
		return false;
	}


	bool bad = query.exec();
	if (!bad)
	{
		lastError = query.lastError().databaseText().toStdString();
		return false;
	}

	query.next();
	QString mdtEnd= query.value("A").toString(); //Модифицированная дата.
	QString crc = query.value("B").toString();//Контрольная сумма.

	//Проверка контрольной суммы.
	QString calcCrc = VanishChest::getCrcEndDbDate(mdtEnd);
	if (crc != calcCrc)
	{
		lastError = "Error", "Error 457Y: Bad CRC.";
		return false;
	}

	//Привожу дату в нормальный вид.
	bool errFlag = false;
	QString _dtEnd=VanishChest::UnModifyEndDbDate(mdtEnd, errFlag);
	if (errFlag) //Ошибка.
	{
		lastError = "Error 458Y: Bad value.";
		return false;
	}

	dtEnd = _dtEnd;

	return true;
}

bool GranitDbI::checkDbDateEnd()
{
	//Получаю дату окончания действия БД.
	QString dtEnd = "";
	if (!getDbDateEnd(dtEnd)) return false;
	
	//Проверяю дату окончания действия БД.
	QDate DbEndDt = QDate::fromString(dtEnd, "dd.MM.yyyy");

	//Проверка даты на валидность, вдруг пользователь внес какую то ахинею в БД.
	if (!DbEndDt.isValid())
	{
		return false;
	}

	QDate nowDt = QDateTime::currentDateTime().date();
	if (nowDt > DbEndDt) //Срок действия БД истек.
	{
		return false;
	}
	
	return true;
}

string GranitDbI::getLastError()
{
	return lastError;
}




//Получает позицию открытого ключа в ListSignOpenKey  по указанному индексу. Если позиция не найдена возвращает -1.
bool GranitDbI::getCachSignOK(string & index, string & Qx, string & Qy, QString & FIO)
{
	QList<modelOpenSignKey>::iterator i;
	for (i = ListSignOpenKey.begin(); i != ListSignOpenKey.end(); ++i)
	{
		modelOpenSignKey x = *i;
		if (x.Index == index)
		{
			Qx = x.Qx;
			Qy = x.Qy;
			FIO = x.FIO;
			return true;
		}
		//string f=*i.
			//if(*i.)
	}
		//cout << *i << endl;

	//	QList<modelOpenSignKey> ListSignOpenKey;

	return false;
}

//Проверяет контрольную сумму сертификата полученного из бд на основание crc и его длины. В случае ошибок выводит сообщение
//возвращает false.
bool GranitDbI::CheckCertCrc(QByteArray & Cert, int Len, QString & crc)
{
	if (Cert.size() != Len) //Не совпадение размеров сертификатов.
		return false;

	QString crc_ = VanishChest::CrcUserCertificate(Cert); //Проблемы с контрольной суммой сертификата.
	if(crc!=crc_) return false;

	return true;
}

//Проверяет строковые значения периода действия сертификата. Выдает сообщения. В случае проблем возвращает false.
bool GranitDbI::CheckCertUsePeriod(QString DtBegin, QString DtEnd)
{
	QDateTime dtb = QDateTime::fromString(DtBegin, "dd.MM.yyyy hh:mm:ss");
	QDateTime dte = QDateTime::fromString(DtEnd, "dd.MM.yyyy hh:mm:ss");

	QDateTime now = QDateTime::currentDateTime();
	
	if (now < dtb)
	{
		lastError = "Ошибка БС 705: Сертификат не начал действовать. Дата начала действия сертификата: "+DtBegin.toStdString();
		return false;
	}


	if (now > dte)
	{
		lastError = "Ошибка БС 706: Сертификат просрочен. Дата окончания действия сертификата: " + DtEnd.toStdString();
		return false;
	}


	return true;
}

QString GranitDbI::GetSignOkFromDb(string & index, string & Qx, string & Qy, QString & FIO)
{
	/*
	try
	{
		QString Qindex(index.c_str()); //Индекс открытого ключа подписанта

		QString sql = "select FIO,C,D from OpenKeys where B = :ix";
		QSqlQuery query(Db);

		if (!query.prepare(sql))
		{
			QMessageBox::critical(0, "Error", query.lastError().databaseText());
			return false;
		}

		query.bindValue(":ix", Qindex);


		bool bad = query.exec();
		if (!bad)
		{
			QMessageBox::critical(0, "Error", query.lastError().databaseText());
			return false;
		}

		//Нет записей
		if (query.record().count() == 0)
		{
			return StrCodec->toUnicode("В базе данных не обнаружен ключ проверки подписи подписанта");
		}


		//Считываю значение FIO,C,D
		query.next();
		FIO = query.value(0).toString();
		QString tmp;
		tmp = query.value(1).toString();//C-SignKeyX.
		Qx = tmp.toStdString();
		tmp = query.value(2).toString();//D-SignKeyX.
		Qy = tmp.toStdString();

	}
	catch (exception& e)
	{
		return StrCodec->toUnicode("В методе GetSignOkFromDb возникло исключение") + e.what();
	}
	*/
	return "";
}

void GranitDbI::ShowError(string message)
{
	QTextCodec *StrCodec; //Для преобразования кодировки
	StrCodec = QTextCodec::codecForName("Windows-1251"); //Установка кодировки
	QString err = StrCodec->toUnicode(message.c_str());
	QString title= StrCodec->toUnicode("Ошибка.");
	QMessageBox::critical(0, title, err);
}
