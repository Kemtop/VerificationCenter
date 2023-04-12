#include "MainForm.h"
#include "eng.h"

MainForm::MainForm(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.radioButtonUser->setChecked(true); //Простой пользователь по умолчанию.
	ui.lineEditPassword->setVisible(false);
	ui.labelPasword->setVisible(false);
	connect(ui.radioButtonExpert, SIGNAL(toggled(bool)),this, SLOT(slotToggledExpert(bool)));
	connect(ui.pushButtonOK, SIGNAL(pressed()), this, SLOT(slotpushButtonOK()));
	mode = 0;
	attemp = 0;
}

MainForm::~MainForm()
{
}

int MainForm::getMode()
{
	return mode;
}

void MainForm::slotpushButtonOK()
{
	if (ui.radioButtonExpert->isChecked())
	{
		QString in = ui.lineEditPassword->text();
		in = in.trimmed();

		if (in != ung)
		{
			attemp++;
			QMessageBox::information(this, "", ung1);
			if (attemp > ung3)
			{
				QMessageBox::information(this, "", ung2);
				reject();
			}

			return;
		}
		mode = 1;
	}
	else
	{
		mode = 0;
	}
		
	accept();
}


void MainForm::slotToggledExpert(bool checked)
{
	if (checked)
	{
		ui.lineEditPassword->setVisible(true);
		ui.labelPasword->setVisible(true);
	}
	else
	{
		ui.lineEditPassword->setVisible(false);
		ui.labelPasword->setVisible(false);
	}

}