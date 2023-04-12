/********************************************************************************
** Form generated from reading UI file 'SelectKeyForm.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTKEYFORM_H
#define UI_SELECTKEYFORM_H

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

class Ui_SelectKeyForm
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEditKeyPath;
    QPushButton *pushButtonOk;
    QPushButton *pushButtonCancel;
    QPushButton *pushButtonSelectKeyPath;

    void setupUi(QWidget *SelectKeyForm)
    {
        if (SelectKeyForm->objectName().isEmpty())
            SelectKeyForm->setObjectName(QStringLiteral("SelectKeyForm"));
        SelectKeyForm->setWindowModality(Qt::ApplicationModal);
        SelectKeyForm->resize(495, 305);
        SelectKeyForm->setMaximumSize(QSize(495, 305));
        QFont font;
        font.setPointSize(11);
        SelectKeyForm->setFont(font);
        label = new QLabel(SelectKeyForm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 10, 111, 80));
        label->setStyleSheet(QStringLiteral("background-image:url(\"Image/secret_success.png\");"));
        label_2 = new QLabel(SelectKeyForm);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(50, 137, 271, 21));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        label_2->setFont(font1);
        label_3 = new QLabel(SelectKeyForm);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(160, 30, 321, 31));
        QFont font2;
        font2.setFamily(QStringLiteral("Tahoma"));
        font2.setBold(true);
        font2.setItalic(false);
        font2.setWeight(75);
        label_3->setFont(font2);
        label_3->setStyleSheet(QLatin1String("font-family:Tahoma;\n"
"font-style: normal;\n"
"font-size: 12pt;\n"
"font-weight: bold;\n"
"\n"
"font-size:17px;\n"
"color:blue;"));
        lineEditKeyPath = new QLineEdit(SelectKeyForm);
        lineEditKeyPath->setObjectName(QStringLiteral("lineEditKeyPath"));
        lineEditKeyPath->setGeometry(QRect(50, 160, 381, 20));
        lineEditKeyPath->setReadOnly(true);
        pushButtonOk = new QPushButton(SelectKeyForm);
        pushButtonOk->setObjectName(QStringLiteral("pushButtonOk"));
        pushButtonOk->setGeometry(QRect(50, 265, 91, 30));
        pushButtonOk->setFont(font);
        pushButtonOk->setStyleSheet(QLatin1String("text-align:center;\n"
"background-image: url(\"Image/ok.png\");\n"
"background-repeat: no-repeat;\n"
"background-position: left \n"
""));
        pushButtonCancel = new QPushButton(SelectKeyForm);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(350, 264, 91, 30));
        pushButtonCancel->setFont(font);
        pushButtonCancel->setStyleSheet(QLatin1String("text-align:center;\n"
"background-image: url(\"Image/cancel.png\");\n"
"background-repeat: no-repeat;\n"
"background-position: left \n"
""));
        pushButtonSelectKeyPath = new QPushButton(SelectKeyForm);
        pushButtonSelectKeyPath->setObjectName(QStringLiteral("pushButtonSelectKeyPath"));
        pushButtonSelectKeyPath->setGeometry(QRect(440, 160, 25, 21));
        pushButtonSelectKeyPath->setStyleSheet(QLatin1String("text-align:center;\n"
"background-image: url(\"Image/open-folder-yellow.png\");\n"
"background-repeat: no-repeat;\n"
"background-position: left \n"
""));

        retranslateUi(SelectKeyForm);

        QMetaObject::connectSlotsByName(SelectKeyForm);
    } // setupUi

    void retranslateUi(QWidget *SelectKeyForm)
    {
        SelectKeyForm->setWindowTitle(QApplication::translate("SelectKeyForm", "\320\222\321\213\320\261\320\276\321\200 \321\201\320\265\320\272\321\200\320\265\321\202\320\275\320\276\320\263\320\276 \320\272\320\273\321\216\321\207\320\260", nullptr));
        label->setText(QString());
        label_2->setText(QApplication::translate("SelectKeyForm", "\320\237\321\203\321\202\321\214 \320\272 \321\204\320\260\320\271\320\273\321\203 \321\201\320\265\320\272\321\200\320\265\321\202\320\275\320\276\320\263\320\276 \320\272\320\273\321\216\321\207\320\260:", nullptr));
        label_3->setText(QApplication::translate("SelectKeyForm", "\320\222\321\213\320\261\320\265\321\200\320\270\321\202\320\265 \321\204\320\260\320\271\320\273 \321\201\320\265\320\272\321\200\320\265\321\202\320\275\320\276\320\263\320\276 \320\272\320\273\321\216\321\207\320\260", nullptr));
        pushButtonOk->setText(QApplication::translate("SelectKeyForm", "\320\236\320\272", nullptr));
        pushButtonCancel->setText(QApplication::translate("SelectKeyForm", "    \320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
        pushButtonSelectKeyPath->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SelectKeyForm: public Ui_SelectKeyForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTKEYFORM_H
