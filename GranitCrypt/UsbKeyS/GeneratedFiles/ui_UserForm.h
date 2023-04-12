/********************************************************************************
** Form generated from reading UI file 'UserForm.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERFORM_H
#define UI_USERFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserForm
{
public:
    QLineEdit *lineEditProductKey;
    QLabel *labelKeyInfo;
    QPushButton *pushButtonSendProductKey;
    QPushButton *pushButtonSetRSA;
    QStatusBar *statusBar;

    void setupUi(QWidget *UserForm)
    {
        if (UserForm->objectName().isEmpty())
            UserForm->setObjectName(QStringLiteral("UserForm"));
        UserForm->resize(485, 300);
        UserForm->setMaximumSize(QSize(485, 300));
        QFont font;
        font.setPointSize(11);
        UserForm->setFont(font);
        lineEditProductKey = new QLineEdit(UserForm);
        lineEditProductKey->setObjectName(QStringLiteral("lineEditProductKey"));
        lineEditProductKey->setGeometry(QRect(50, 40, 381, 20));
        labelKeyInfo = new QLabel(UserForm);
        labelKeyInfo->setObjectName(QStringLiteral("labelKeyInfo"));
        labelKeyInfo->setGeometry(QRect(190, 20, 141, 16));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        labelKeyInfo->setFont(font1);
        pushButtonSendProductKey = new QPushButton(UserForm);
        pushButtonSendProductKey->setObjectName(QStringLiteral("pushButtonSendProductKey"));
        pushButtonSendProductKey->setGeometry(QRect(30, 80, 421, 31));
        pushButtonSetRSA = new QPushButton(UserForm);
        pushButtonSetRSA->setObjectName(QStringLiteral("pushButtonSetRSA"));
        pushButtonSetRSA->setGeometry(QRect(30, 190, 421, 31));
        statusBar = new QStatusBar(UserForm);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusBar->setGeometry(QRect(0, 280, 485, 21));

        retranslateUi(UserForm);

        QMetaObject::connectSlotsByName(UserForm);
    } // setupUi

    void retranslateUi(QWidget *UserForm)
    {
        UserForm->setWindowTitle(QApplication::translate("UserForm", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265 USB \320\272\320\273\321\216\321\207\320\265\320\274.", nullptr));
        lineEditProductKey->setText(QApplication::translate("UserForm", "WWWWW-WWWWW-WWWWW-WWWWW-WWWWW", nullptr));
        labelKeyInfo->setText(QApplication::translate("UserForm", "\320\232\320\273\321\216\321\207 \320\277\321\200\320\276\320\264\321\203\320\272\321\202\320\260.", nullptr));
        pushButtonSendProductKey->setText(QApplication::translate("UserForm", "\320\236\321\202\320\277\321\200\320\260\320\262\320\270\321\202\321\214 \320\272\320\273\321\216\321\207 \320\277\321\200\320\276\320\264\321\203\320\272\321\202\320\260.", nullptr));
        pushButtonSetRSA->setText(QApplication::translate("UserForm", "\320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\270\321\202\321\214 \320\272\320\273\321\216\321\207 \321\210\320\270\321\204\321\200\320\276\320\262\320\260\320\275\320\270\321\217 \320\272\320\260\320\275\320\260\320\273\321\214\320\275\320\276\320\263\320\276 \321\203\321\200\320\276\320\262\320\275\321\217.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserForm: public Ui_UserForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERFORM_H
