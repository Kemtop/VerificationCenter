/********************************************************************************
** Form generated from reading UI file 'ProductKeyForm.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRODUCTKEYFORM_H
#define UI_PRODUCTKEYFORM_H

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

class Ui_ProductKeyForm
{
public:
    QLineEdit *lineEdit_1;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButtonOk;
    QPushButton *pushButtonCancel;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *labelShowError;

    void setupUi(QWidget *ProductKeyForm)
    {
        if (ProductKeyForm->objectName().isEmpty())
            ProductKeyForm->setObjectName(QStringLiteral("ProductKeyForm"));
        ProductKeyForm->setWindowModality(Qt::ApplicationModal);
        ProductKeyForm->resize(728, 449);
        lineEdit_1 = new QLineEdit(ProductKeyForm);
        lineEdit_1->setObjectName(QStringLiteral("lineEdit_1"));
        lineEdit_1->setGeometry(QRect(23, 220, 123, 32));
        QFont font;
        font.setFamily(QStringLiteral("Times New Roman"));
        font.setPointSize(18);
        font.setBold(true);
        font.setWeight(75);
        lineEdit_1->setFont(font);
        lineEdit_1->setInputMethodHints(Qt::ImhUppercaseOnly);
        lineEdit_1->setMaxLength(29);
        lineEdit_2 = new QLineEdit(ProductKeyForm);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(163, 220, 123, 32));
        lineEdit_2->setFont(font);
        lineEdit_2->setMaxLength(5);
        lineEdit_3 = new QLineEdit(ProductKeyForm);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(303, 220, 123, 32));
        lineEdit_3->setFont(font);
        lineEdit_3->setMaxLength(5);
        lineEdit_4 = new QLineEdit(ProductKeyForm);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(443, 220, 123, 32));
        lineEdit_4->setFont(font);
        lineEdit_4->setMaxLength(5);
        lineEdit_5 = new QLineEdit(ProductKeyForm);
        lineEdit_5->setObjectName(QStringLiteral("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(583, 220, 123, 32));
        lineEdit_5->setFont(font);
        lineEdit_5->setMaxLength(5);
        label = new QLabel(ProductKeyForm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 20, 301, 31));
        QFont font1;
        font1.setFamily(QStringLiteral("Times New Roman"));
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        label->setFont(font1);
        label->setStyleSheet(QLatin1String("font-family:Times New Roman;\n"
"font-style: normal;\n"
"font-weight: bold;\n"
"\n"
"font-size:26px;\n"
"color:#3A91E8;"));
        label_2 = new QLabel(ProductKeyForm);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 100, 691, 41));
        QFont font2;
        font2.setPointSize(12);
        label_2->setFont(font2);
        pushButtonOk = new QPushButton(ProductKeyForm);
        pushButtonOk->setObjectName(QStringLiteral("pushButtonOk"));
        pushButtonOk->setGeometry(QRect(480, 410, 111, 23));
        pushButtonOk->setStyleSheet(QLatin1String("QPushButton \n"
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
        pushButtonCancel = new QPushButton(ProductKeyForm);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(610, 410, 111, 23));
        pushButtonCancel->setStyleSheet(QLatin1String("QPushButton \n"
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
        label_3 = new QLabel(ProductKeyForm);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(150, 230, 16, 16));
        QFont font3;
        font3.setFamily(QStringLiteral("Terminal"));
        font3.setPointSize(18);
        font3.setBold(true);
        font3.setItalic(false);
        font3.setWeight(75);
        label_3->setFont(font3);
        label_4 = new QLabel(ProductKeyForm);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(290, 230, 16, 16));
        label_4->setFont(font3);
        label_5 = new QLabel(ProductKeyForm);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(430, 230, 16, 16));
        label_5->setFont(font3);
        label_6 = new QLabel(ProductKeyForm);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(570, 230, 16, 16));
        label_6->setFont(font3);
        labelShowError = new QLabel(ProductKeyForm);
        labelShowError->setObjectName(QStringLiteral("labelShowError"));
        labelShowError->setGeometry(QRect(30, 280, 671, 81));
        labelShowError->setFont(font1);
        labelShowError->setStyleSheet(QLatin1String("font-family:Times New Roman;\n"
"font-style: normal;\n"
"font-weight: bold;\n"
"\n"
"font-size:16px;\n"
"color:#FF0000;"));

        retranslateUi(ProductKeyForm);

        QMetaObject::connectSlotsByName(ProductKeyForm);
    } // setupUi

    void retranslateUi(QWidget *ProductKeyForm)
    {
        ProductKeyForm->setWindowTitle(QApplication::translate("ProductKeyForm", "\320\232\320\273\321\216\321\207 \320\277\321\200\320\276\320\264\321\203\320\272\321\202\320\260.", nullptr));
        lineEdit_1->setInputMask(QString());
        lineEdit_1->setText(QApplication::translate("ProductKeyForm", "WWWWW", nullptr));
        lineEdit_2->setText(QApplication::translate("ProductKeyForm", "WWWWW", nullptr));
        lineEdit_3->setText(QApplication::translate("ProductKeyForm", "WWWWW", nullptr));
        lineEdit_4->setText(QApplication::translate("ProductKeyForm", "WWWWW", nullptr));
        lineEdit_5->setText(QApplication::translate("ProductKeyForm", "WWWWW", nullptr));
        label->setText(QApplication::translate("ProductKeyForm", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\272\320\273\321\216\321\207 \320\277\321\200\320\276\320\264\321\203\320\272\321\202\320\260.", nullptr));
        label_2->setText(QApplication::translate("ProductKeyForm", "\320\232\320\273\321\216\321\207 \320\277\321\200\320\276\320\264\321\203\320\272\321\202\320\260 \320\270\320\274\320\265\320\265\321\202 \321\201\320\273\320\265\320\264\321\203\321\216\321\211\320\270\320\271 \321\204\320\276\321\200\320\274\320\260\321\202:  XXXXX-XXXXX-XXXXX-XXXXX-XXXXX.", nullptr));
        pushButtonOk->setText(QApplication::translate("ProductKeyForm", "\320\237\320\276\320\264\321\202\320\262\320\265\321\200\320\264\320\270\321\202\321\214", nullptr));
        pushButtonCancel->setText(QApplication::translate("ProductKeyForm", "\320\236\321\202\320\274\320\265\320\275\320\270\321\202\321\214", nullptr));
        label_3->setText(QApplication::translate("ProductKeyForm", "-", nullptr));
        label_4->setText(QApplication::translate("ProductKeyForm", "-", nullptr));
        label_5->setText(QApplication::translate("ProductKeyForm", "-", nullptr));
        label_6->setText(QApplication::translate("ProductKeyForm", "-", nullptr));
        labelShowError->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ProductKeyForm: public Ui_ProductKeyForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRODUCTKEYFORM_H
