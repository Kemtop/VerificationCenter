#include "UsbKeyS.h"
#include "MainForm.h"
#include "UserForm.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	MainForm mForm;

	if (mForm.exec() != QDialog::Accepted) return 0; //Пользователь закрыл форму.
	
	int b = mForm.getMode();

	if (b)
	{
		UsbKeyS w;
		w.show();
		return a.exec();
	}
	else
	{
		UserForm F;
		F.show();
		return a.exec();
	}
 
	
}
