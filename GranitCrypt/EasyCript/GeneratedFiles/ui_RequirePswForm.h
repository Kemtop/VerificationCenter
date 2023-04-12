/********************************************************************************
** Form generated from reading UI file 'RequirePswForm.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REQUIREPSWFORM_H
#define UI_REQUIREPSWFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RequirePswForm
{
public:
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEditPswd;
    QPushButton *pushButtonOk;
    QPushButton *pushButtonCancel;
    QLabel *label_3;

    void setupUi(QWidget *RequirePswForm)
    {
        if (RequirePswForm->objectName().isEmpty())
            RequirePswForm->setObjectName(QStringLiteral("RequirePswForm"));
        RequirePswForm->resize(400, 247);
        QFont font;
        font.setPointSize(11);
        RequirePswForm->setFont(font);
        label = new QLabel(RequirePswForm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(96, 36, 291, 16));
        label->setStyleSheet(QLatin1String("font-family:Tahoma;\n"
"font-style: normal;\n"
"font-size: 12pt;\n"
"font-weight: bold;\n"
"\n"
"font-size:14px;\n"
"color:blue;"));
        label_2 = new QLabel(RequirePswForm);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(24, 121, 71, 16));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        label_2->setFont(font1);
        lineEditPswd = new QLineEdit(RequirePswForm);
        lineEditPswd->setObjectName(QStringLiteral("lineEditPswd"));
        lineEditPswd->setGeometry(QRect(90, 120, 271, 20));
        lineEditPswd->setEchoMode(QLineEdit::Password);
        pushButtonOk = new QPushButton(RequirePswForm);
        pushButtonOk->setObjectName(QStringLiteral("pushButtonOk"));
        pushButtonOk->setGeometry(QRect(30, 200, 91, 30));
        pushButtonOk->setFont(font);
        pushButtonOk->setStyleSheet(QLatin1String("text-align:center;\n"
"background-image: url(\"Image/ok.png\");\n"
"background-repeat: no-repeat;\n"
"background-position: left \n"
""));
        pushButtonCancel = new QPushButton(RequirePswForm);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(280, 200, 91, 30));
        pushButtonCancel->setFont(font);
        pushButtonCancel->setStyleSheet(QLatin1String("text-align:center;\n"
"background-image: url(\"Image/cancel.png\");\n"
"background-repeat: no-repeat;\n"
"background-position: left \n"
""));
        label_3 = new QLabel(RequirePswForm);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(5, 3, 76, 91));
        label_3->setStyleSheet(QStringLiteral("background-image: url(\"Image/passw.png\");"));

        retranslateUi(RequirePswForm);

        QMetaObject::connectSlotsByName(RequirePswForm);
    } // setupUi

    void retranslateUi(QWidget *RequirePswForm)
    {
        RequirePswForm->setWindowTitle(QApplication::translate("RequirePswForm", "RequirePswForm", nullptr));
        label->setText(QApplication::translate("RequirePswForm", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\277\320\260\321\200\320\276\320\273\321\214 \320\264\320\273\321\217 \321\201\320\265\320\272\321\200\320\265\321\202\320\275\320\276\320\263\320\276 \320\272\320\273\321\216\321\207\320\260", nullptr));
        label_2->setText(QApplication::translate("RequirePswForm", "\320\237\320\260\321\200\320\276\320\273\321\214:", nullptr));
        pushButtonOk->setText(QApplication::translate("RequirePswForm", "Ok", nullptr));
        pushButtonCancel->setText(QApplication::translate("RequirePswForm", "    \320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
        label_3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RequirePswForm: public Ui_RequirePswForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REQUIREPSWFORM_H
