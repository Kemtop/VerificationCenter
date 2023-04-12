/********************************************************************************
** Form generated from reading UI file 'UsbKeyS.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USBKEYS_H
#define UI_USBKEYS_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UsbKeySClass
{
public:
    QWidget *centralWidget;
    QPushButton *BtnGetSerial;
    QLineEdit *TbSerial;
    QPushButton *BtnSetSerial;
    QDateEdit *dateEdit;
    QPushButton *BtnGetDate;
    QPushButton *BtnSetDate;
    QPushButton *BtnKeyGen;
    QLineEdit *TbHWSerial;
    QPushButton *BtnGetHWSerial;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *UsbKeySClass)
    {
        if (UsbKeySClass->objectName().isEmpty())
            UsbKeySClass->setObjectName(QStringLiteral("UsbKeySClass"));
        UsbKeySClass->resize(305, 248);
        UsbKeySClass->setMaximumSize(QSize(305, 248));
        centralWidget = new QWidget(UsbKeySClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        BtnGetSerial = new QPushButton(centralWidget);
        BtnGetSerial->setObjectName(QStringLiteral("BtnGetSerial"));
        BtnGetSerial->setGeometry(QRect(10, 40, 75, 23));
        TbSerial = new QLineEdit(centralWidget);
        TbSerial->setObjectName(QStringLiteral("TbSerial"));
        TbSerial->setGeometry(QRect(10, 10, 281, 21));
        TbSerial->setMaxLength(29);
        TbSerial->setAlignment(Qt::AlignCenter);
        BtnSetSerial = new QPushButton(centralWidget);
        BtnSetSerial->setObjectName(QStringLiteral("BtnSetSerial"));
        BtnSetSerial->setGeometry(QRect(210, 40, 75, 23));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        BtnSetSerial->setFont(font);
        dateEdit = new QDateEdit(centralWidget);
        dateEdit->setObjectName(QStringLiteral("dateEdit"));
        dateEdit->setGeometry(QRect(130, 140, 110, 22));
        dateEdit->setDateTime(QDateTime(QDate(2019, 1, 1), QTime(0, 0, 0)));
        dateEdit->setMinimumDate(QDate(2019, 1, 1));
        dateEdit->setCurrentSection(QDateTimeEdit::DaySection);
        dateEdit->setCalendarPopup(true);
        BtnGetDate = new QPushButton(centralWidget);
        BtnGetDate->setObjectName(QStringLiteral("BtnGetDate"));
        BtnGetDate->setGeometry(QRect(10, 130, 91, 23));
        BtnSetDate = new QPushButton(centralWidget);
        BtnSetDate->setObjectName(QStringLiteral("BtnSetDate"));
        BtnSetDate->setGeometry(QRect(10, 160, 91, 23));
        BtnSetDate->setFont(font);
        BtnKeyGen = new QPushButton(centralWidget);
        BtnKeyGen->setObjectName(QStringLiteral("BtnKeyGen"));
        BtnKeyGen->setGeometry(QRect(10, 200, 281, 23));
        BtnKeyGen->setFont(font);
        TbHWSerial = new QLineEdit(centralWidget);
        TbHWSerial->setObjectName(QStringLiteral("TbHWSerial"));
        TbHWSerial->setGeometry(QRect(10, 70, 281, 20));
        TbHWSerial->setAlignment(Qt::AlignCenter);
        TbHWSerial->setReadOnly(true);
        BtnGetHWSerial = new QPushButton(centralWidget);
        BtnGetHWSerial->setObjectName(QStringLiteral("BtnGetHWSerial"));
        BtnGetHWSerial->setGeometry(QRect(90, 100, 121, 23));
        UsbKeySClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(UsbKeySClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        UsbKeySClass->setStatusBar(statusBar);

        retranslateUi(UsbKeySClass);

        QMetaObject::connectSlotsByName(UsbKeySClass);
    } // setupUi

    void retranslateUi(QMainWindow *UsbKeySClass)
    {
        UsbKeySClass->setWindowTitle(QApplication::translate("UsbKeySClass", "UsbKeyS", nullptr));
        BtnGetSerial->setText(QApplication::translate("UsbKeySClass", "Get Serial", nullptr));
        TbSerial->setInputMask(QApplication::translate("UsbKeySClass", "NNNNN-NNNNN-NNNNN-NNNNN-NNNNN", nullptr));
        TbSerial->setText(QApplication::translate("UsbKeySClass", "00000-00000-00000-00000-00000", nullptr));
        BtnSetSerial->setText(QApplication::translate("UsbKeySClass", "Set Serial", nullptr));
        BtnGetDate->setText(QApplication::translate("UsbKeySClass", "Get Last Date", nullptr));
        BtnSetDate->setText(QApplication::translate("UsbKeySClass", "Set Last Date", nullptr));
        BtnKeyGen->setText(QApplication::translate("UsbKeySClass", "Load New RSA Key", nullptr));
        BtnGetHWSerial->setText(QApplication::translate("UsbKeySClass", "Get HW Serial", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UsbKeySClass: public Ui_UsbKeySClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USBKEYS_H
