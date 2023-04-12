/********************************************************************************
** Form generated from reading UI file 'AboutProgramForm.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTPROGRAMFORM_H
#define UI_ABOUTPROGRAMFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AboutProgramForm
{
public:
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButtonClose;
    QGroupBox *groupBox;
    QTextEdit *textEdit;
    QPushButton *pushButtonProductKey;

    void setupUi(QWidget *AboutProgramForm)
    {
        if (AboutProgramForm->objectName().isEmpty())
            AboutProgramForm->setObjectName(QStringLiteral("AboutProgramForm"));
        AboutProgramForm->resize(676, 418);
        label = new QLabel(AboutProgramForm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(111, 13, 501, 16));
        label->setStyleSheet(QLatin1String("font-family:Times New Roman;\n"
"font-style: normal;\n"
"font-weight: bold;\n"
"\n"
"font-size:16px;\n"
"color:#1B1BB3;"));
        label_2 = new QLabel(AboutProgramForm);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 50, 131, 301));
        label_2->setStyleSheet(QStringLiteral("background-image: url(\"Image/logo_cut.png\");"));
        pushButtonClose = new QPushButton(AboutProgramForm);
        pushButtonClose->setObjectName(QStringLiteral("pushButtonClose"));
        pushButtonClose->setGeometry(QRect(580, 380, 75, 23));
        pushButtonClose->setStyleSheet(QLatin1String("QPushButton \n"
"\n"
"{\n"
"\n"
"   border-style: solid;\n"
" \n"
"   border-radius: 10;\n"
"\n"
"    border-top          : 1px solid rgba(200, 200, 200, 255);\n"
"\n"
"    border-left         : 1px solid rgba(200, 200, 200, 255);\n"
"\n"
"    border-right        : 2px solid rgba(172, 172, 172);\n"
"\n"
"    border-bottom       : 2px solid rgba(172, 172, 172);\n"
"\n"
"   /*\n"
"    background-color    : qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(198, 198, 198), stop:1 rgba(225, 225, 225, 255));\n"
"*/\n"
"background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ffffff, stop: 0.1 #eaeaea, stop: 0.49 #eaeaea, stop: 0.5 #eaeaea, stop: 1 #ffffff);\n"
"font-size: 16px;\n"
"}\n"
"\n"
"\n"
"\n"
"QPushButton:hover \n"
"\n"
"{\n"
"/*\n"
"    background-color    : qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(225, 225, 225, 255), stop:1 rgba(163, 163, 163));\n"
"*/\n"
"    background-color    : qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(196, 228, 255), stop:1 rgba(225, 225, 225, 2"
                        "55));\n"
"}\n"
"\n"
"\n"
"\n"
"QPushButton:pressed \n"
"\n"
"{\n"
"\n"
"    padding-top         : 2px;\n"
"\n"
"    padding-left        : 2px;\n"
"\n"
"    border-top          : 2px solid rgba(117, 117, 117, 255);\n"
"\n"
"    border-left         : 2px solid rgba(117, 117, 117, 255);\n"
"\n"
"    border-right        : 1px solid rgba(200, 200, 200, 255);\n"
"\n"
"    border-bottom       : 1px solid rgba(200, 200, 200, 255);\n"
"\n"
"    background-color    : rgba(172, 172, 172);\n"
"\n"
"}\n"
"\n"
"\n"
"\n"
"QPushButton:disabled \n"
"\n"
"{\n"
"\n"
"    background-color    : qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(225, 225, 255, 255), stop:1 rgba(172, 172, 172));\n"
"\n"
"}"));
        groupBox = new QGroupBox(AboutProgramForm);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(160, 47, 501, 310));
        textEdit = new QTextEdit(groupBox);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(5, 3, 491, 300));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
        textEdit->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(11);
        textEdit->setFont(font);
        pushButtonProductKey = new QPushButton(AboutProgramForm);
        pushButtonProductKey->setObjectName(QStringLiteral("pushButtonProductKey"));
        pushButtonProductKey->setGeometry(QRect(20, 380, 131, 23));
        pushButtonProductKey->setStyleSheet(QString::fromUtf8("\n"
"QPushButton \n"
"{\n"
"	border: none;\n"
"	font-family:Times New Roman;\n"
"	font-style: normal;\n"
"	font-weight: bold;\n"
"	font-size:16px;\n"
"	color:#1B1BB3;\n"
"	text-decoration:underline;\n"
"     border-style: solid;\n"
"    border-radius: 10;\n"
"    border-top          : 1px solid rgba(200, 200, 200, 255);\n"
"    border-left         : 1px solid rgba(200, 200, 200, 255);\n"
"    border-right        : 2px solid rgba(172, 172, 172);\n"
"    border-bottom       : 2px solid rgba(172, 172, 172);\n"
"    border: none;\n"
"}\n"
"\n"
"\n"
"/*\n"
"\320\235\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\265 \320\272\321\203\321\200\321\201\320\276\321\200\320\260.\n"
"*/\n"
"QPushButton:hover \n"
"{\n"
"\n"
"    background-color    : qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(196, 228, 255), stop:1 rgba(225, 225, 225, 255));\n"
"font-style: solid;\n"
"    font-color    : qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(196, 228, 255), stop:1 rgba(225, 225, 225, 255));\n"
"color:#00"
                        "1BB3;\n"
"/*\n"
"    background-color    : qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(225, 225, 225, 255), stop:1 rgba(163, 163, 163));\n"
"\n"
"*/\n"
"\n"
"}\n"
"\n"
"\n"
"\n"
"QPushButton:pressed \n"
"\n"
"{\n"
"/*\n"
"    padding-top         : 2px;\n"
"\n"
"    padding-left        : 2px;\n"
"\n"
"    border-top          : 2px solid rgba(117, 117, 117, 255);\n"
"\n"
"    border-left         : 2px solid rgba(117, 117, 117, 255);\n"
"\n"
"    border-right        : 1px solid rgba(200, 200, 200, 255);\n"
"\n"
"    border-bottom       : 1px solid rgba(200, 200, 200, 255);\n"
"\n"
"    background-color    : rgba(172, 172, 172);\n"
"*/\n"
"}\n"
"\n"
"\n"
"\n"
"QPushButton:disabled \n"
"\n"
"{\n"
"\n"
"    background-color    : qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(225, 225, 255, 255), stop:1 rgba(172, 172, 172));\n"
"\n"
"}"));

        retranslateUi(AboutProgramForm);

        QMetaObject::connectSlotsByName(AboutProgramForm);
    } // setupUi

    void retranslateUi(QWidget *AboutProgramForm)
    {
        AboutProgramForm->setWindowTitle(QApplication::translate("AboutProgramForm", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265.", nullptr));
        label->setText(QApplication::translate("AboutProgramForm", "C\320\270\321\201\321\202\320\265\320\274\320\260 \320\232\321\200\320\270\320\277\321\202\320\276\320\263\321\200\320\260\321\204\320\270\321\207\320\265\321\201\320\272\320\276\320\271 \320\227\320\260\321\211\320\270\321\202\321\213 \320\230\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\320\270 \"\320\240\320\276\320\267\320\260-\320\234\"", nullptr));
        label_2->setText(QString());
        pushButtonClose->setText(QApplication::translate("AboutProgramForm", "\320\227\320\260\320\272\321\200\321\213\321\202\321\214", nullptr));
        groupBox->setTitle(QString());
        pushButtonProductKey->setText(QApplication::translate("AboutProgramForm", "\320\232\320\273\321\216\321\207 \320\277\321\200\320\276\320\264\321\203\320\272\321\202\320\260.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutProgramForm: public Ui_AboutProgramForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTPROGRAMFORM_H
