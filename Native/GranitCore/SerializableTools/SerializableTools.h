#pragma once

#include <QObject>
#include <QVariant>
#include <QDomDocument>
#include <QMetaProperty>

class SerializableTools : public QObject
{
	Q_OBJECT

public:
	SerializableTools();
	~SerializableTools();

	//Сериализирует объект.
	template <class T>
	static void Serialize(QString &Result, T obj);

	template <class T>
	static void Deserialize(QByteArray *In, T *object);

};

template <class T>
inline void SerializableTools::Serialize(QString & Result, T obj)
{
	QDomDocument doc;
	doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""));

	QDomElement root = doc.createElement(obj.metaObject()->className()); //Имя класса

	doc.appendChild(root);//Добавляю корневой элемент

	QString propName = "";
	for (int i = 0;i < obj.metaObject()->propertyCount();i++)
	{
		QMetaProperty prop = obj.metaObject()->property(i);
		propName = prop.name();
		if (propName == "objectName") continue;
		QDomElement el = doc.createElement(propName);
		QVariant value = obj.property(propName.toUtf8().data());
		QDomText pstr = doc.createTextNode(value.toString());
		el.appendChild(pstr);
		root.appendChild(el);
	}

	//"Windows-1251"	
	//Сохраняю xml в строку
	QString out_str;
	QTextStream out(&out_str);
	out.setCodec("Windows-1251");
	doc.save(out, 4);

	Result = out_str;
}


template <class T>
inline void SerializableTools::Deserialize(QByteArray *In, T *object)
{
	QDomDocument doc;
	
	QString         errorString;
	int             errorLine;
	int             errorColumn;

	if (doc.setContent(*In, false, &errorString, &errorLine, &errorColumn))
	{
		QDomElement root = doc.documentElement();
		QString value;//Значение узла
					  //Цикл по свойствам класса	
		for (int i = 0;i < object->metaObject()->propertyCount();i++)
		{
			QMetaProperty prop = object->metaObject()->property(i);
			QString propName = prop.name();
			if (propName == "objectName") continue;

			//Листаю узлы в файле
			for (int i = 0; i < root.childNodes().count(); i++)
			{
				QDomElement node = root.childNodes().at(i).toElement();
				//Нашел узел с нужными свойствами
				if (node.nodeName() == propName)
				{
					value = node.toElement().text();
					object->setProperty(propName.toUtf8().data(), QVariant(value));

				}
			}
		}

	}
	
}