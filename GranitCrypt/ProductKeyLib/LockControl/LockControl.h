#pragma once
#include <qlist>
#include <QAction>

class LockControl
{
public:
	LockControl();
	~LockControl();

	//�������� ������� ����������� ����������.
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
	//������ ��������� ���������� ����������.
	//static 
		QList<QAction*> LockAction;
};

