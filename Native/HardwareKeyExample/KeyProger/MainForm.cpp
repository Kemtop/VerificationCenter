#include "MainForm.h"
#include "config.h"

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

//Нажатие на кнопку «Ок».
void MainForm::slotpushButtonOK()
{
	if (ui.radioButtonExpert->isChecked())
	{
		QString in = ui.lineEditPassword->text();
		in = in.trimmed();

		if (in != EXPERT_PASSWORD)
		{
			attemp++;
			QMessageBox::information(this, "", "Invalid password!");
			if (attemp > MAX_ATTEMPT)
			{
				QMessageBox::information(this, "", "Too many bad password! Program will close!");
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

//Переключение формы в режим «Эксперт».
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