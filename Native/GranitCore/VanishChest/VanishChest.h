#pragma once
#include "hash3411\hash3411.h"
#include "mpirxx.h"
#include <QByteArray>
#include <QString>
#include <QCryptographicHash>

//Класс для сокрытия разных значений от посторонних глаз. Содержит функции преобразований.
class VanishChest
{
public:
	VanishChest();
	~VanishChest();
	
	//Модифицирует открытый ключ  схемы Диффи-Хэлмана. Массив может иметь любую размерность
	static void ModifyDHKey(QByteArray In, QByteArray &Out);

	//Из модифицированного состояния преобразовывает в нормальный вид открытый ключ  схемы Диффи-Хэлмана. 
	static void UnModifyDHKey(QByteArray In, QByteArray &Out);

	//Из модифицированного состояния преобразовывает в нормальный вид открытый ключ  схемы Диффи-Хэлмана. 
	static void UnModifyDHKey(QString &In, QString &Out);


	//Формирует специальную строку для проверки DHKey. Что то вроде контрольной суммы.
	static void CrcDHKey(QString &hash_str, QString &DHKey);
	//Формирует контрольную сумму для проверки ключа ЭЦП. Также используется как индекс для быстрого поиска строки  с необходимым
	//сертификатом используя открытый ключ ЭЦП.
	static QString CrcECKey(QString x,  QString y);

	//Вычисляет значение хэш фушкции с использованием алгоритма MD5 на основании значений строк x,y.
	static string  QkeyMD5(string x, string y);

	//Возвращает контрольную сумму пользовательского сертификата.
	static QString CrcUserCertificate(QByteArray cert);

	//Возвращает контрольную сумму системного сертификата.
	static QString CrcSystemCertificate(QByteArray cert);

	//Модифицирует строку с данными Ф И О пользователя в группе.
	static QString ModifyExUserFIO(QString &in);

	//Превращаю  строку с данными Ф И О пользователя в группе в нормальный вид.
	static QString UnModifyExUserFIO(QString &in);

	//Модифицирует дату окончания срока действия базы данных. Входная строка должна содержать 10 символов. 
	static QString ModifyEndDbDate(QString in);

	//Восстанавливает дату окончания срока действия базы данных. Входная строка должна содержать 20 символов. 
	static QString UnModifyEndDbDate(QString in,bool &err);

	//Тест преобразования даты окончания срока действия БД системы.
	bool TestVanichEndDbDate();

	//Возвращает контрольную сумму даты окончания действия БД системы.
	static QString getCrcEndDbDate(QString in);

	//Формирует строку, которая называется "Хеш ключа" в заявке(WORD документ).
	static QString algHashSignKey(QString x, QString y);

private:
	//Выполняет перестановку открытого ключа ЭЦП. Вспомогательная для CrcECKey.
	static void TranspECQ(QByteArray Qz);

};

