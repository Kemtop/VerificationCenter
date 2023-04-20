#include "stdafx.h"
#include "GranitCrypt.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QIcon icon("Image/app.png");
	
	
	//QThread Thread1; //Объект для запуска потока.
    GranitCrypt w;
	w.move(QApplication::desktop()->screen()->rect().center() - w.rect().center());
	w.setWindowIcon(icon);
    w.show();

    return a.exec();
}
