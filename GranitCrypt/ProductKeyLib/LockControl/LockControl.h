#pragma once
#include <qlist>
#include <QAction>

class LockControl
{
public:
	LockControl();
	~LockControl();

	//Передает контрол подележащий блокировке.
	//static
		void setControlToLock(QAction*control)
	{
		LockAction.append(control);
	}

		void  LockControls()
		{
			for (int i = 0; i < LockAction.size(); ++i)
			{
				LockAction[i]->setEnabled(0);
			}
		}

private:
	//Список контролов подлежащим блокировке.
	//static 
		QList<QAction*> LockAction;
};

