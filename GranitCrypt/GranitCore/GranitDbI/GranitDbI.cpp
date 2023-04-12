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

//��������� ���������� � �� �� ���������� ����.
bool GranitDbI::OpenDb()
{
	//�� ��� �������
	if (Db.isOpen()) return true;

	Db = QSqlDatabase::addDatabase("QSQLITE", "CertDb");
	Db.setDatabaseName(PathToDb);

	if (Db.open() == false)
	{
		ShowError("������ �� 10: �� ������� ������� ���� ������ �������.");
		return false;
	}

	return true;
}

bool GranitDbI::OpenDb(string PathPlugins, string PathDb, string & Error)
{
	//�� ��� �������
	if (Db.isOpen()) return true;


	QString _PathDb = QString::fromLocal8Bit(PathDb.c_str());

	//�������� ���� � ��������, ����� ������� ���������� ������� QSQLITE.
	QStringList liblist;
	liblist.push_back(QString::fromLocal8Bit(PathPlugins.c_str()));
	QCoreApplication::setLibraryPaths(liblist);
	
	//ShowDriversList(); //������� ������ ���������

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

	//������ ������ ���������
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


//�������� ���������� ������������ �� ��������� ������ ��������� ����� �������.
bool GranitDbI::GetUserCert(QString Qx, QString Qy, QString F, QString I, QString O)
{
	string Kx = Qx.toStdString();
	string Ky = Qy.toStdString();
	string Qindex = VanishChest::QkeyMD5(Kx, Ky); //������ �� ��������� ��������� �����.
	QString QIndex = QString::fromStdString(Qindex);

	QString FIO = F + " " + I + " " + O;

	//��� ������������ �� ��������� ������� ��������� �����.
	QString sql = "select * from TableT2 where B=:ix";
	if (!OpenDb())
	{
		lastError = "������ �� 700: �� ������� ������� ������ ������������.";
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
		

	//������ ��������� ���� �� ����.
	bool found = false; //���� ���������� ����������.
	int UserId_ = -10; //�� ������������
	int LineStts = -10; //������ ������.

	//���������� ��� ��� ���������� ������ ����� ����������� ��� � ����� ������� �� ������� ����� ��������.
	while (query.next())
	{
		QString enFIO_ = query.value("A").toString(); //������� �������� ���� ���.
		QString FIO_ = VanishChest::UnModifyExUserFIO(enFIO_); //�������������� � ���������� ���.

		if (FIO == FIO_) //������ �������.
		{
		    UserId_ = query.value("id").toInt(); //�� ������������
			int LineStts= query.value("E").toInt(); //������ ������.
			found = true;
			break;
		}
	}
	
	//��� �������
	if (found==false)
	{
		lastError="������ �� 701: ����������� ���������� ��� ������� �����.";
		return false;
	}

    //������ �������.
	if (LineStts==0)
	{
		lastError = "������ �� 702: �������� ����� ������ � ������� ��� ����������� ����������.";
		return false;
	}

	//����� �� ��������������� �����������.
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

	found = false; //����� ��� ��� ������������ �����������.
	QString G;
	QByteArray CertA; //��������� ����������.
	QString CrcCert;//�������� ����������� ����� �����������.
	int CertSize;//������ �����������.
	QString DtBegin; //���� ������ �������� �����������
	QString DtEnd; //���� ��������� �������� �����������


	while (query.next())
	{
		//��������� �������.
		G= query.value("G").toString(); //������ �� ��������� ��������� ����� ������������.
		if (G != QIndex) continue; //������ �� ������.
		
		//���������� ������ ������� ������.
		CertA = query.value("B").toByteArray();

		 CrcCert=query.value("C").toString();//�������� ����������� ����� �����������.
		 CertSize=query.value("D").toInt();//������ �����������.
		 DtBegin=query.value("E").toString(); //���� ������ �������� �����������
		 DtEnd= query.value("F").toString(); //���� ��������� �������� �����������
		
		found = true;
		break;
	}

	//���������� �� ���������.
	if (found == false)
	{
		lastError="������ �� 703: ����������� ���������� ��� ������� �����. �������� �� ����������� ��� �� �������.";
		return false;
	}

	//�������� ����������� ����� �����������.
	if(!CheckCertCrc(CertA, CertSize, CrcCert))
	{
		lastError = "������ �� 704: ���������� ��������� ����� ��������� ��� ��� ������� ������� �����.";
		return false;
	}


	//�������� ������� �������� �����������.
	if(!CheckCertUsePeriod(DtBegin, DtEnd)) return false;

	//������� ������
	UserId = UserId_;
	UserCert = CertA;
	UserQIndex = Qindex;

	return true;
}

bool GranitDbI::getSignerIndex(QString Qx, QString Qy, string &Qindex)
{
	string Kx = Qx.toStdString();
	string Ky = Qy.toStdString();
	Qindex = VanishChest::QkeyMD5(Kx, Ky); //������ �� ��������� ��������� �����.
	
	return true;
}


//�������� ���������� ������������ �� ��������� ������ ��������� ����� �������, � ��������� ��������.
//������������ ��� ���������� ����������� ������������ �� ��.
bool GranitDbI::GetUserCert(QString Qx, QString Qy, QString F, QString I, QString O, QByteArray &uCert_)
{
	string Kx = Qx.toStdString();
	string Ky = Qy.toStdString();
	string Qindex = VanishChest::QkeyMD5(Kx, Ky); //������ �� ��������� ��������� �����.
	QString QIndex = QString::fromStdString(Qindex);

	QString FIO = F + " " + I + " " + O;

	//��� ������������ �� ��������� ������� ��������� �����.
	QString sql = "select * from TableT2 where B=:ix";
	if (!OpenDb())
	{
		ShowError("������ �� 700: �� ������� ������� ������ ������������.");
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


	//������ ��������� ���� �� ����.
	bool found = false; //���� ���������� ����������.
	int UserId_ = -10; //�� ������������
	int LineStts = -10; //������ ������.

						//���������� ��� ��� ���������� ������ ����� ����������� ��� � ����� ������� �� ������� ����� ��������.
	while (query.next())
	{
		QString enFIO_ = query.value("A").toString(); //������� �������� ���� ���.
		QString FIO_ = VanishChest::UnModifyExUserFIO(enFIO_); //�������������� � ���������� ���.

		if (FIO == FIO_) //������ �������.
		{
			UserId_ = query.value("id").toInt(); //�� ������������
			int LineStts = query.value("E").toInt(); //������ ������.
			found = true;
			break;
		}
	}

	//��� �������
	if (found == false)
	{
		ShowError("������ �� 701: ����������� ���������� ��� ������� �����.");
		return false;
	}


	//����� �����������, ���� ���� �� �����������.
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

	found = false; //����� ��� ��� ������������ �����������.
	QString G;
	QByteArray CertA; //��������� ����������.
	QString CrcCert;//�������� ����������� ����� �����������.
	int CertSize;//������ �����������.
	QString DtBegin; //���� ������ �������� �����������
	QString DtEnd; //���� ��������� �������� �����������


	while (query.next())
	{
		//��������� �������.
		G = query.value("G").toString(); //������ �� ��������� ��������� ����� ������������.
		if (G != QIndex) continue; //������ �� ������.

								   //���������� ������ ������� ������.
		CertA = query.value("B").toByteArray();

		CrcCert = query.value("C").toString();//�������� ����������� ����� �����������.
		CertSize = query.value("D").toInt();//������ �����������.
		DtBegin = query.value("E").toString(); //���� ������ �������� �����������
		DtEnd = query.value("F").toString(); //���� ��������� �������� �����������

		found = true;
		break;
	}

	//���������� �� ���������.
	if (found == false)
	{
		ShowError("������ �� 703: ����������� ���������� ��� ������� �����.");
		return false;
	}

	//�������� ����������� ����� �����������.
	if (!CheckCertCrc(CertA, CertSize, CrcCert))
	{
		ShowError("������ �� 704: ���������� ��������� ����� ��������� ��� ��� ������� ������� �����.");
		return false;
	}

	//������� ������
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

//�������� ���������� CA. � ������ ������ ������� ���������. ���������� false.
bool GranitDbI::getCACert(QByteArray &CertCA)
{
	//������� ����������� ���������� CA.
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

	int count = 0;//���������� �����.
	while (query.next())
	{
		CAcert_=query.value("B").toByteArray();
		certHash = query.value("C").toString();
		certLen = query.value("D").toInt();
		count++;
	}


	if (count == 0)
	{
		string str = "������ �� 707A:�� ���������� ������������ ����������� ��.";
		ShowError(str);
		return false;
	}

	//������� ����� 1�� ����������� CA.
	if (count > 1)
	{
		string str = "������ �� 707: ���������� ����� 1 ������������ ����������� ��("+std::to_string(count)+").";
		ShowError(str);
		return false;
	}



	//�������� ����� �����������.
	if (certLen != CAcert_.size())
	{
		ShowError("������ �� 708: ���������� �� ��������� ��� ��� ������� ������� �����.");
		return false;
	}



	//�������� ����������� ����� �����������.
	QString curCertHash = VanishChest::CrcSystemCertificate(CAcert_);
	if (curCertHash != certHash)
	{
		ShowError("������ �� 709: ���������� �� ��������� ��� ��� ������� ������� �����.");
		return false;
	}

	
	CertCA = CAcert_;

	return true;
}
//�������� ���������� CA. � ������ ������ ������� ���������. ���������� false ���� ������.
bool GranitDbI::getRootCert(QByteArray &CertRoot)
{
	//������� ����������� ���������� CA.
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

	int count = 0;//���������� �����.
	while (query.next())
	{
		Rootcert_ = query.value("B").toByteArray();
		certHash = query.value("C").toString();
		certLen = query.value("D").toInt();
		count++;
	}

	//������� ����� 1�� ����������� Root.
	if (count > 1)
	{
		string str = "������ �� 710: ���������� ����� 1 ������������ ����������� Root(" + std::to_string(count) + ").";
		ShowError(str);
		return false;
	}

	if (count == 0)
	{
		string str = "������ �� 710A: �� ����������  ������������ ����������� Root.";
		ShowError(str);
		return false;
	}


	//�������� ����� �����������.
	if (certLen != Rootcert_.size())
	{
		ShowError("������ �� 711: ���������� Root ��������� ��� ��� ������� ������� �����.");
		return false;
	}



	//�������� ����������� ����� �����������.
	QString curCertHash = VanishChest::CrcSystemCertificate(Rootcert_);
	if (curCertHash != certHash)
	{
		ShowError("������ �� 712: ���������� Root ��������� ��� ��� ������� ������� �����.");
		return false;
	}

	CertRoot = Rootcert_;

	return true;
}


//������� ������ �������� ������ �������������. ������������ ��� ������ ������������ �������� ���������� ���������.
//� ������ ������ ������ ��������� � ���������� false.
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

	//�������� ������
	int row = 0;
	while (query.next())
	{
		for (int col = 0;col < 4;col++) //���� �� ��������.
		{
			tmp = query.value(colNames[col]).toString();
			if (col == 1) //FIO ��������������
			{
				tmp = VanishChest::UnModifyExUserFIO(tmp);
			}
			QStandardItem *item = new 	QStandardItem(tmp);
			model->setItem(row, col, item);
		}
		row++;
	}
		

	QTextCodec *StrCodec = QTextCodec::codecForName("Windows-1251");
	QString str1 = StrCodec->toUnicode("�.�.�.");
	model->setHeaderData(1, Qt::Horizontal, str1.toStdString().c_str());

	str1 = StrCodec->toUnicode("�");
	model->setHeaderData(2, Qt::Horizontal, str1.toStdString().c_str());

	str1 = StrCodec->toUnicode("��");
	model->setHeaderData(3, Qt::Horizontal, str1.toStdString().c_str());	

	return true;
}

//�������� �������� ���� ����� �-� ��� ������� ������������. � ������ ������ ������ ��������� � ���������� false.
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

	//�������� �������� ��� ��������� ����� ��
	int cnt = 0;

	QString Dh_OpenKey_;
	QString hCrcDH_;

	int Status = 0;
	while (query.next())
	{
		hCrcDH_= query.value("C").toString();
		Dh_OpenKey_= query.value("D").toString();

		Status = query.value("E").toInt(); //��������� ������.
		cnt++;
	}

	//��� �����.
	if (cnt == 0)
	{
		ShowError("������ � 718: ����������� ������.");
		return false;
	}
	
	//�������� ����� 1-� ������.
	if (cnt>1)
	{
		string h = "������ � 719: ������������� �����������.n=" + std::to_string(cnt)+".";
		ShowError(h);
		return false;
	}

	//������ �������
	if (Status == 0)
	{
		ShowError("������ � 720: ����������� ������.");
		return false;
	}

	//������ ������
	if (Dh_OpenKey_.isEmpty())
	{
		ShowError("������ � 721: ����������� ������.");
		return false;
	}

	//������ ������
	if (hCrcDH_.isEmpty())
	{
		ShowError("������ � 722: ����������� ������.");
		return false;
	}


	QString Crc; //����������� ����� ��� �������� ��������� �����.
	VanishChest::CrcDHKey(Crc, Dh_OpenKey_); //�������� ����������� �����.
	
	//�������� ����������� �����.
	if (Crc != hCrcDH_)
	{
		ShowError("������ � 723: ��������� ������������� ����.");
		return false;
	}


	QString DHkey; //�������� ���� ����� �� � ���������� ����.
	VanishChest::UnModifyDHKey(Dh_OpenKey_, DHkey);

	rHashOkDH = hCrcDH_.toStdString();
	Dh_OpenKey = DHkey.toStdString();

	return true;
}

string GranitDbI::getQIndex()
{
	return UserQIndex;
}

//��������� ������ ������������, �������� ������ ��������� �����(����� �������� �������).
bool GranitDbI::getSignOpenKey(string & Qindex, string & Qx, string & Qy, QString & FIO)
{
	QString _Qindex(Qindex.c_str()); //������ ��������� ����� ����������
	
	//��������� � ������ ����� �������� ������. ��� ������������ � ������.
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
	int cnt = 0; //���������� �������.
	while (query.next())
	{
		_FIO= query.value(0).toString(); //xFIO,t6.B,t6.C
		_Qx=query.value(1).toString();
		_Qy= query.value(2).toString();
		cnt++;
	}

	//��� �������.
	if (cnt == 0)
	{
		ShowError("������ 724: �� ������� ������ ����� �������� �������.");
		return false;
	}

	//������� ����� 1.
	if (cnt>1)
	{
		string h = "������ 725: ������������� ������ �� ������ ����� �������� �������.n="+std::to_string(cnt)+".";
		ShowError(h);
		return false;
	}

	QString FIO_ = VanishChest::UnModifyExUserFIO(_FIO); //�������������� � ���������� ���.
	
	//�������� �������� FIO,C,D
	FIO = FIO_;

	Qx = _Qx.toStdString(); 
	Qy = _Qy.toStdString();

	return true;
}

bool GranitDbI::findUserCert(string digest, string num, QByteArray &cert)
{
	//��������� � ������ ����� �������� ������. ��� ������������ � ������.
	QString sql = "select t3.* from TableT3 t3"\
		" where t3.K=:k and t3.L=:l and J=1;"; //J=1 ����������� ����������.

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

	
	int cnt = 0; //���������� �������.
	QByteArray res;

	while (query.next())
	{
	    res=query.value("B").toByteArray();
		cnt++;
	}

	cert = res;

	//��� �������.
	if (cnt == 0) return true;
	

	//������� ����� 1.
	if (cnt>1)
	{
		lastError= "������ 725AB: ������������� ������ �� ������ ����� �������� �������.n=" + std::to_string(cnt) + ".";
		return false;
	}
	
	return true;
}

bool GranitDbI::getDbDateEnd(QString &dtEnd)
{
	if (!OpenDb())
	{
		lastError="������ �� 700: �� ������� ������� ������ ������������.";
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
	QString mdtEnd= query.value("A").toString(); //���������������� ����.
	QString crc = query.value("B").toString();//����������� �����.

	//�������� ����������� �����.
	QString calcCrc = VanishChest::getCrcEndDbDate(mdtEnd);
	if (crc != calcCrc)
	{
		lastError = "Error", "Error 457Y: Bad CRC.";
		return false;
	}

	//������� ���� � ���������� ���.
	bool errFlag = false;
	QString _dtEnd=VanishChest::UnModifyEndDbDate(mdtEnd, errFlag);
	if (errFlag) //������.
	{
		lastError = "Error 458Y: Bad value.";
		return false;
	}

	dtEnd = _dtEnd;

	return true;
}

bool GranitDbI::checkDbDateEnd()
{
	//������� ���� ��������� �������� ��.
	QString dtEnd = "";
	if (!getDbDateEnd(dtEnd)) return false;
	
	//�������� ���� ��������� �������� ��.
	QDate DbEndDt = QDate::fromString(dtEnd, "dd.MM.yyyy");

	//�������� ���� �� ����������, ����� ������������ ���� ����� �� ������ � ��.
	if (!DbEndDt.isValid())
	{
		return false;
	}

	QDate nowDt = QDateTime::currentDateTime().date();
	if (nowDt > DbEndDt) //���� �������� �� �����.
	{
		return false;
	}
	
	return true;
}

string GranitDbI::getLastError()
{
	return lastError;
}




//�������� ������� ��������� ����� � ListSignOpenKey  �� ���������� �������. ���� ������� �� ������� ���������� -1.
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

//��������� ����������� ����� ����������� ����������� �� �� �� ��������� crc � ��� �����. � ������ ������ ������� ���������
//���������� false.
bool GranitDbI::CheckCertCrc(QByteArray & Cert, int Len, QString & crc)
{
	if (Cert.size() != Len) //�� ���������� �������� ������������.
		return false;

	QString crc_ = VanishChest::CrcUserCertificate(Cert); //�������� � ����������� ������ �����������.
	if(crc!=crc_) return false;

	return true;
}

//��������� ��������� �������� ������� �������� �����������. ������ ���������. � ������ ������� ���������� false.
bool GranitDbI::CheckCertUsePeriod(QString DtBegin, QString DtEnd)
{
	QDateTime dtb = QDateTime::fromString(DtBegin, "dd.MM.yyyy hh:mm:ss");
	QDateTime dte = QDateTime::fromString(DtEnd, "dd.MM.yyyy hh:mm:ss");

	QDateTime now = QDateTime::currentDateTime();
	
	if (now < dtb)
	{
		lastError = "������ �� 705: ���������� �� ����� �����������. ���� ������ �������� �����������: "+DtBegin.toStdString();
		return false;
	}


	if (now > dte)
	{
		lastError = "������ �� 706: ���������� ���������. ���� ��������� �������� �����������: " + DtEnd.toStdString();
		return false;
	}


	return true;
}

QString GranitDbI::GetSignOkFromDb(string & index, string & Qx, string & Qy, QString & FIO)
{
	/*
	try
	{
		QString Qindex(index.c_str()); //������ ��������� ����� ����������

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

		//��� �������
		if (query.record().count() == 0)
		{
			return StrCodec->toUnicode("� ���� ������ �� ��������� ���� �������� ������� ����������");
		}


		//�������� �������� FIO,C,D
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
		return StrCodec->toUnicode("� ������ GetSignOkFromDb �������� ����������") + e.what();
	}
	*/
	return "";
}

void GranitDbI::ShowError(string message)
{
	QTextCodec *StrCodec; //��� �������������� ���������
	StrCodec = QTextCodec::codecForName("Windows-1251"); //��������� ���������
	QString err = StrCodec->toUnicode(message.c_str());
	QString title= StrCodec->toUnicode("������.");
	QMessageBox::critical(0, title, err);
}
