/********************************************************************************
** Form generated from reading UI file 'MainForm.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINFORM_H
#define UI_MAINFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainForm
{
public:
    QPushButton *pushButtonOK;
    QRadioButton *radioButtonUser;
    QRadioButton *radioButtonExpert;
    QLineEdit *lineEditPassword;
    QLabel *labelPasword;

    void setupUi(QWidget *MainForm)
    {
        if (MainForm->objectName().isEmpty())
            MainForm->setObjectName(QStringLiteral("MainForm"));
        MainForm->resize(485, 300);
        MainForm->setMaximumSize(QSize(485, 300));
        QFont font;
        font.setPointSize(11);
        MainForm->setFont(font);
        pushButtonOK = new QPushButton(MainForm);
        pushButtonOK->setObjectName(QStringLiteral("pushButtonOK"));
        pushButtonOK->setGeometry(QRect(210, 260, 75, 23));
        radioButtonUser = new QRadioButton(MainForm);
        radioButtonUser->setObjectName(QStringLiteral("radioButtonUser"));
        radioButtonUser->setGeometry(QRect(110, 50, 271, 17));
        radioButtonExpert = new QRadioButton(MainForm);
        radioButtonExpert->setObjectName(QStringLiteral("radioButtonExpert"));
        radioButtonExpert->setGeometry(QRect(110, 100, 251, 17));
        lineEditPassword = new QLineEdit(MainForm);
        lineEditPassword->setObjectName(QStringLiteral("lineEditPassword"));
        lineEditPassword->setGeometry(QRect(180, 160, 161, 20));
        lineEditPassword->setEchoMode(QLineEdit::Password);
        labelPasword = new QLabel(MainForm);
        labelPasword->setObjectName(QStringLiteral("labelPasword"));
        labelPasword->setGeometry(QRect(110, 160, 71, 16));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        labelPasword->setFont(font1);

        retranslateUi(MainForm);

        QMetaObject::connectSlotsByName(MainForm);
    } // setupUi

    void retranslateUi(QWidget *MainForm)
    {
        MainForm->setWindowTitle(QApplication::translate("MainForm", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265 USB \320\272\320\273\321\216\321\207\320\276\320\274 \320\241\320\232\320\227\320\230 \"\320\223\321\200\320\260\320\275\320\270\321\202-\320\232\".", nullptr));
        pushButtonOK->setText(QApplication::translate("MainForm", "OK", nullptr));
        radioButtonUser->setText(QApplication::translate("MainForm", "\320\240\320\265\320\266\320\270\320\274 \320\277\321\200\320\276\321\201\321\202\320\276\320\263\320\276 \320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\217.", nullptr));
        radioButtonExpert->setText(QApplication::translate("MainForm", "\320\240\320\265\320\266\320\270\320\274 \321\215\320\272\321\201\320\277\320\265\321\200\321\202\320\260.", nullptr));
        labelPasword->setText(QApplication::translate("MainForm", "\320\237\320\260\321\200\320\276\320\273\321\214:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainForm: public Ui_MainForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFORM_H
