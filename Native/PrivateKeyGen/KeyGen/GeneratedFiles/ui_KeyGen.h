/********************************************************************************
** Form generated from reading UI file 'KeyGen.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KEYGEN_H
#define UI_KEYGEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KeyGenClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QLineEdit *lineEditKeyPath;
    QPushButton *pushButtonW1Cancel;
    QLabel *labelKeyPathName;
    QPushButton *pushButtonSelectReqPath;
    QPushButton *pushButtonW1Next;
    QLineEdit *lineEditRequestPath;
    QLabel *labelTitleSelReq;
    QPushButton *pushButtonSelectKeyPath;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLineEdit *lineEditFamiliya;
    QLineEdit *lineEditImia;
    QLineEdit *lineEditOtchestvo;
    QLabel *label_14;
    QLabel *label_15;
    QLineEdit *lineEditOrgName;
    QLineEdit *lineEditEGRUL;
    QLabel *label_17;
    QLineEdit *lineEditGorod;
    QLabel *label_18;
    QLineEdit *lineEditAdress;
    QLabel *label_19;
    QLineEdit *lineEditTelefon;
    QLabel *label_20;
    QLineEdit *lineEditPochta;
    QLabel *label_21;
    QLineEdit *lineEditPodrazdelenie;
    QLineEdit *lineEditRayon;
    QLabel *label_22;
    QWidget *page_3;
    QPushButton *pushButtonW2Next;
    QLabel *label_4;
    QProgressBar *progressBarW3;
    QLabel *label_5;
    QLabel *labelW3EndTime;
    QLabel *labelAnimation;
    QWidget *page_4;
    QLabel *label_6;
    QPushButton *pushButtonW3Next;
    QLabel *label_7;
    QLineEdit *lineEditW4Password;
    QLineEdit *lineEditW4PasswordReply;
    QLabel *label_8;
    QLabel *label_9;
    QWidget *page_5;
    QPushButton *pushButtonW5PrintZayavka;
    QTextEdit *textEditW5Message;

    void setupUi(QMainWindow *KeyGenClass)
    {
        if (KeyGenClass->objectName().isEmpty())
            KeyGenClass->setObjectName(QStringLiteral("KeyGenClass"));
        KeyGenClass->resize(1000, 618);
        KeyGenClass->setMinimumSize(QSize(428, 264));
        KeyGenClass->setMaximumSize(QSize(1000, 618));
        QFont font;
        font.setPointSize(12);
        KeyGenClass->setFont(font);
        KeyGenClass->setAcceptDrops(false);
        KeyGenClass->setDocumentMode(false);
        centralWidget = new QWidget(KeyGenClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(4, 1, 1, 1);
        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        lineEditKeyPath = new QLineEdit(page);
        lineEditKeyPath->setObjectName(QStringLiteral("lineEditKeyPath"));
        lineEditKeyPath->setGeometry(QRect(460, 470, 491, 22));
        lineEditKeyPath->setReadOnly(true);
        pushButtonW1Cancel = new QPushButton(page);
        pushButtonW1Cancel->setObjectName(QStringLiteral("pushButtonW1Cancel"));
        pushButtonW1Cancel->setGeometry(QRect(900, 580, 80, 23));
        pushButtonW1Cancel->setMinimumSize(QSize(80, 22));
        pushButtonW1Cancel->setMaximumSize(QSize(16777215, 16777215));
        pushButtonW1Cancel->setStyleSheet(QLatin1String("QPushButton \n"
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
        labelKeyPathName = new QLabel(page);
        labelKeyPathName->setObjectName(QStringLiteral("labelKeyPathName"));
        labelKeyPathName->setGeometry(QRect(168, 470, 301, 21));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        labelKeyPathName->setFont(font1);
        pushButtonSelectReqPath = new QPushButton(page);
        pushButtonSelectReqPath->setObjectName(QStringLiteral("pushButtonSelectReqPath"));
        pushButtonSelectReqPath->setGeometry(QRect(960, 520, 23, 23));
        pushButtonSelectReqPath->setStyleSheet(QLatin1String("text-align:left;\n"
"background-image: url(\"Image/open_folder.png\");\n"
"background-repeat: no-repeat;\n"
"background-position: left \n"
""));
        pushButtonW1Next = new QPushButton(page);
        pushButtonW1Next->setObjectName(QStringLiteral("pushButtonW1Next"));
        pushButtonW1Next->setGeometry(QRect(800, 580, 80, 23));
        pushButtonW1Next->setMinimumSize(QSize(80, 22));
        QFont font2;
        pushButtonW1Next->setFont(font2);
        pushButtonW1Next->setStyleSheet(QLatin1String("QPushButton \n"
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
        lineEditRequestPath = new QLineEdit(page);
        lineEditRequestPath->setObjectName(QStringLiteral("lineEditRequestPath"));
        lineEditRequestPath->setGeometry(QRect(460, 520, 491, 22));
        lineEditRequestPath->setReadOnly(true);
        labelTitleSelReq = new QLabel(page);
        labelTitleSelReq->setObjectName(QStringLiteral("labelTitleSelReq"));
        labelTitleSelReq->setGeometry(QRect(11, 515, 451, 31));
        labelTitleSelReq->setFont(font1);
        pushButtonSelectKeyPath = new QPushButton(page);
        pushButtonSelectKeyPath->setObjectName(QStringLiteral("pushButtonSelectKeyPath"));
        pushButtonSelectKeyPath->setGeometry(QRect(960, 470, 23, 20));
        pushButtonSelectKeyPath->setStyleSheet(QLatin1String("text-align:left;\n"
"background-image: url(\"Image/open-folder-yellow.png\");\n"
"background-repeat: no-repeat;\n"
"background-position: left \n"
""));
        label_10 = new QLabel(page);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(230, 0, 501, 31));
        QFont font3;
        font3.setFamily(QStringLiteral("TimesNewRoman"));
        font3.setBold(true);
        font3.setItalic(false);
        font3.setWeight(75);
        label_10->setFont(font3);
        label_10->setStyleSheet(QLatin1String("font-family:TimesNewRoman;\n"
"font-style: normal;\n"
"font-weight: bold;\n"
"\n"
"font-size:15px;\n"
"color:#1B1BB3;"));
        label_11 = new QLabel(page);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(30, 70, 81, 21));
        label_11->setFont(font1);
        label_12 = new QLabel(page);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(70, 113, 47, 13));
        label_12->setFont(font1);
        label_13 = new QLabel(page);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(26, 150, 91, 21));
        label_13->setFont(font1);
        lineEditFamiliya = new QLineEdit(page);
        lineEditFamiliya->setObjectName(QStringLiteral("lineEditFamiliya"));
        lineEditFamiliya->setGeometry(QRect(120, 72, 201, 22));
        lineEditImia = new QLineEdit(page);
        lineEditImia->setObjectName(QStringLiteral("lineEditImia"));
        lineEditImia->setGeometry(QRect(120, 110, 201, 22));
        lineEditOtchestvo = new QLineEdit(page);
        lineEditOtchestvo->setObjectName(QStringLiteral("lineEditOtchestvo"));
        lineEditOtchestvo->setGeometry(QRect(120, 150, 201, 22));
        label_14 = new QLabel(page);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(600, 140, 71, 16));
        label_14->setFont(font1);
        label_15 = new QLabel(page);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(530, 70, 241, 16));
        label_15->setFont(font1);
        lineEditOrgName = new QLineEdit(page);
        lineEditOrgName->setObjectName(QStringLiteral("lineEditOrgName"));
        lineEditOrgName->setGeometry(QRect(380, 110, 601, 22));
        lineEditEGRUL = new QLineEdit(page);
        lineEditEGRUL->setObjectName(QStringLiteral("lineEditEGRUL"));
        lineEditEGRUL->setGeometry(QRect(530, 160, 201, 22));
        label_17 = new QLabel(page);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(360, 274, 61, 31));
        label_17->setFont(font1);
        lineEditGorod = new QLineEdit(page);
        lineEditGorod->setObjectName(QStringLiteral("lineEditGorod"));
        lineEditGorod->setGeometry(QRect(427, 280, 201, 22));
        label_18 = new QLabel(page);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(57, 328, 71, 21));
        label_18->setFont(font1);
        lineEditAdress = new QLineEdit(page);
        lineEditAdress->setObjectName(QStringLiteral("lineEditAdress"));
        lineEditAdress->setGeometry(QRect(120, 330, 571, 22));
        label_19 = new QLabel(page);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(10, 380, 191, 31));
        label_19->setFont(font1);
        lineEditTelefon = new QLineEdit(page);
        lineEditTelefon->setObjectName(QStringLiteral("lineEditTelefon"));
        lineEditTelefon->setGeometry(QRect(200, 387, 211, 22));
        label_20 = new QLabel(page);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(450, 381, 181, 31));
        label_20->setFont(font1);
        lineEditPochta = new QLineEdit(page);
        lineEditPochta->setObjectName(QStringLiteral("lineEditPochta"));
        lineEditPochta->setGeometry(QRect(626, 387, 210, 22));
        label_21 = new QLabel(page);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(250, 210, 151, 21));
        label_21->setFont(font1);
        lineEditPodrazdelenie = new QLineEdit(page);
        lineEditPodrazdelenie->setObjectName(QStringLiteral("lineEditPodrazdelenie"));
        lineEditPodrazdelenie->setGeometry(QRect(392, 212, 201, 22));
        lineEditRayon = new QLineEdit(page);
        lineEditRayon->setObjectName(QStringLiteral("lineEditRayon"));
        lineEditRayon->setGeometry(QRect(120, 280, 201, 22));
        label_22 = new QLabel(page);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(54, 281, 61, 16));
        label_22->setFont(font1);
        stackedWidget->addWidget(page);
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
        pushButtonW2Next = new QPushButton(page_3);
        pushButtonW2Next->setObjectName(QStringLiteral("pushButtonW2Next"));
        pushButtonW2Next->setGeometry(QRect(900, 580, 80, 23));
        pushButtonW2Next->setMinimumSize(QSize(80, 22));
        pushButtonW2Next->setFont(font2);
        pushButtonW2Next->setStyleSheet(QLatin1String("QPushButton \n"
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
        label_4 = new QLabel(page_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(406, 30, 591, 71));
        QFont font4;
        font4.setFamily(QStringLiteral("Tahoma"));
        font4.setPointSize(28);
        font4.setBold(true);
        font4.setItalic(false);
        font4.setWeight(75);
        label_4->setFont(font4);
        label_4->setStyleSheet(QLatin1String("font-family:Tahoma;\n"
"font-style: normal;\n"
"font-size: 28pt;\n"
"font-weight: bold;\n"
"\n"
"\n"
"color:blue;"));
        progressBarW3 = new QProgressBar(page_3);
        progressBarW3->setObjectName(QStringLiteral("progressBarW3"));
        progressBarW3->setGeometry(QRect(10, 498, 971, 41));
        progressBarW3->setValue(24);
        label_5 = new QLabel(page_3);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(490, 160, 451, 51));
        QFont font5;
        font5.setPointSize(25);
        font5.setBold(false);
        font5.setWeight(50);
        label_5->setFont(font5);
        labelW3EndTime = new QLabel(page_3);
        labelW3EndTime->setObjectName(QStringLiteral("labelW3EndTime"));
        labelW3EndTime->setGeometry(QRect(10, 544, 141, 21));
        labelAnimation = new QLabel(page_3);
        labelAnimation->setObjectName(QStringLiteral("labelAnimation"));
        labelAnimation->setGeometry(QRect(3, 10, 400, 400));
        labelAnimation->setStyleSheet(QStringLiteral(""));
        stackedWidget->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName(QStringLiteral("page_4"));
        label_6 = new QLabel(page_4);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(300, 100, 661, 51));
        label_6->setFont(font5);
        pushButtonW3Next = new QPushButton(page_4);
        pushButtonW3Next->setObjectName(QStringLiteral("pushButtonW3Next"));
        pushButtonW3Next->setGeometry(QRect(900, 570, 80, 23));
        pushButtonW3Next->setMinimumSize(QSize(80, 22));
        pushButtonW3Next->setFont(font2);
        pushButtonW3Next->setStyleSheet(QLatin1String("QPushButton \n"
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
        label_7 = new QLabel(page_4);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(0, 12, 291, 291));
        label_7->setStyleSheet(QStringLiteral("background-image:url(\"Image/Lock.png\");"));
        lineEditW4Password = new QLineEdit(page_4);
        lineEditW4Password->setObjectName(QStringLiteral("lineEditW4Password"));
        lineEditW4Password->setGeometry(QRect(370, 336, 301, 20));
        lineEditW4Password->setEchoMode(QLineEdit::Password);
        lineEditW4PasswordReply = new QLineEdit(page_4);
        lineEditW4PasswordReply->setObjectName(QStringLiteral("lineEditW4PasswordReply"));
        lineEditW4PasswordReply->setGeometry(QRect(370, 392, 301, 20));
        lineEditW4PasswordReply->setEchoMode(QLineEdit::Password);
        label_8 = new QLabel(page_4);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(371, 313, 81, 21));
        QFont font6;
        font6.setPointSize(11);
        font6.setBold(true);
        font6.setWeight(75);
        label_8->setFont(font6);
        label_9 = new QLabel(page_4);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(370, 368, 321, 21));
        label_9->setFont(font6);
        label_9->setWordWrap(true);
        stackedWidget->addWidget(page_4);
        page_5 = new QWidget();
        page_5->setObjectName(QStringLiteral("page_5"));
        pushButtonW5PrintZayavka = new QPushButton(page_5);
        pushButtonW5PrintZayavka->setObjectName(QStringLiteral("pushButtonW5PrintZayavka"));
        pushButtonW5PrintZayavka->setGeometry(QRect(410, 580, 181, 23));
        pushButtonW5PrintZayavka->setMinimumSize(QSize(80, 22));
        pushButtonW5PrintZayavka->setFont(font2);
        pushButtonW5PrintZayavka->setStyleSheet(QLatin1String("QPushButton \n"
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
        textEditW5Message = new QTextEdit(page_5);
        textEditW5Message->setObjectName(QStringLiteral("textEditW5Message"));
        textEditW5Message->setGeometry(QRect(10, 16, 971, 551));
        QFont font7;
        font7.setPointSize(12);
        font7.setBold(false);
        font7.setWeight(50);
        textEditW5Message->setFont(font7);
        textEditW5Message->setStyleSheet(QLatin1String(" QTextEdit {\n"
"/* \n"
"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                   stop: 0 #E0E0E0, stop: 1 #FFFFFF);\n"
"*/\n"
"background-color:white;\n"
" border: 2px solid gray;\n"
" border-radius: 5px;\n"
" margin-top: 1ex; /* leave space at the top for the title */\n"
"}"));
        stackedWidget->addWidget(page_5);

        horizontalLayout->addWidget(stackedWidget);

        KeyGenClass->setCentralWidget(centralWidget);
        QWidget::setTabOrder(lineEditFamiliya, lineEditImia);
        QWidget::setTabOrder(lineEditImia, lineEditOtchestvo);
        QWidget::setTabOrder(lineEditOtchestvo, lineEditOrgName);
        QWidget::setTabOrder(lineEditOrgName, lineEditEGRUL);
        QWidget::setTabOrder(lineEditEGRUL, lineEditPodrazdelenie);
        QWidget::setTabOrder(lineEditPodrazdelenie, lineEditRayon);
        QWidget::setTabOrder(lineEditRayon, lineEditGorod);
        QWidget::setTabOrder(lineEditGorod, lineEditAdress);
        QWidget::setTabOrder(lineEditAdress, lineEditTelefon);
        QWidget::setTabOrder(lineEditTelefon, lineEditPochta);
        QWidget::setTabOrder(lineEditPochta, lineEditKeyPath);
        QWidget::setTabOrder(lineEditKeyPath, pushButtonSelectKeyPath);
        QWidget::setTabOrder(pushButtonSelectKeyPath, lineEditRequestPath);
        QWidget::setTabOrder(lineEditRequestPath, pushButtonSelectReqPath);
        QWidget::setTabOrder(pushButtonSelectReqPath, pushButtonW1Next);
        QWidget::setTabOrder(pushButtonW1Next, pushButtonW1Cancel);
        QWidget::setTabOrder(pushButtonW1Cancel, pushButtonW2Next);
        QWidget::setTabOrder(pushButtonW2Next, lineEditW4Password);
        QWidget::setTabOrder(lineEditW4Password, lineEditW4PasswordReply);
        QWidget::setTabOrder(lineEditW4PasswordReply, pushButtonW3Next);
        QWidget::setTabOrder(pushButtonW3Next, textEditW5Message);
        QWidget::setTabOrder(textEditW5Message, pushButtonW5PrintZayavka);

        retranslateUi(KeyGenClass);

        stackedWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(KeyGenClass);
    } // setupUi

    void retranslateUi(QMainWindow *KeyGenClass)
    {
        KeyGenClass->setWindowTitle(QApplication::translate("KeyGenClass", "\"\320\223\321\200\320\260\320\275\320\270\321\202\320\232\" \320\223\320\265\320\275\320\265\321\200\320\260\321\202\320\276\321\200 \321\201\320\265\320\272\321\200\320\265\321\202\320\275\320\276\320\263\320\276 \320\272\320\273\321\216\321\207\320\260 ", nullptr));
        pushButtonW1Cancel->setText(QApplication::translate("KeyGenClass", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
        labelKeyPathName->setText(QApplication::translate("KeyGenClass", "\320\237\321\203\321\202\321\214 \320\272 \320\272\320\260\321\202\320\260\320\273\320\276\320\263\321\203 \321\205\321\200\320\260\320\275\320\265\320\275\320\270\321\217 \320\272\320\273\321\216\321\207\320\260:", nullptr));
        pushButtonSelectReqPath->setText(QString());
        pushButtonW1Next->setText(QApplication::translate("KeyGenClass", "\320\224\320\260\320\273\320\265\320\265 >", nullptr));
        lineEditRequestPath->setText(QApplication::translate("KeyGenClass", "hfhfhfhfhtrtTTRrr", nullptr));
        labelTitleSelReq->setText(QApplication::translate("KeyGenClass", "\320\237\321\203\321\202\321\214 \320\272 \320\272\320\260\321\202\320\260\320\273\320\276\320\263\321\203 \321\201\320\276\321\205\321\200\320\260\320\275\320\265\320\275\320\270\321\217 \320\267\320\260\320\277\321\200\320\276\321\201\320\260 \320\275\320\260 \321\201\320\265\321\200\321\202\320\270\321\204\320\270\320\272\320\260\321\202:", nullptr));
        pushButtonSelectKeyPath->setText(QString());
        label_10->setText(QApplication::translate("KeyGenClass", "\320\224\320\273\321\217 \321\201\320\276\320\267\320\264\320\260\320\275\320\270\321\217 \321\201\320\265\320\272\321\200\320\265\321\202\320\275\320\276\320\263\320\276 \320\272\320\273\321\216\321\207\320\260 \320\267\320\260\320\277\320\276\320\273\320\275\320\270\321\202\320\265 \321\204\320\276\321\200\320\274\321\203 \321\201 \320\264\320\260\320\275\320\275\321\213\320\274\320\270.", nullptr));
        label_11->setText(QApplication::translate("KeyGenClass", "\320\244\320\260\320\274\320\270\320\273\320\270\321\217:", nullptr));
        label_12->setText(QApplication::translate("KeyGenClass", "\320\230\320\274\321\217:", nullptr));
        label_13->setText(QApplication::translate("KeyGenClass", "\320\236\321\202\321\207\320\265\321\201\321\202\320\262\320\276:", nullptr));
        label_14->setText(QApplication::translate("KeyGenClass", "\320\225\320\223\320\240\320\256\320\233:", nullptr));
        label_15->setText(QApplication::translate("KeyGenClass", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265 \320\276\321\200\320\263\320\260\320\275\320\270\320\267\320\260\321\206\320\270\320\270:", nullptr));
        label_17->setText(QApplication::translate("KeyGenClass", "\320\223\320\276\321\200\320\276\320\264:", nullptr));
        label_18->setText(QApplication::translate("KeyGenClass", "\320\220\320\264\321\200\320\265\321\201:", nullptr));
        label_19->setText(QApplication::translate("KeyGenClass", "\320\232\320\276\320\275\321\202\320\260\320\272\321\202\320\275\321\213\320\271 \321\202\320\265\320\273\320\265\321\204\320\276\320\275:", nullptr));
        label_20->setText(QApplication::translate("KeyGenClass", "\320\255\320\273\320\265\320\272\321\202\321\200\320\276\320\275\320\275\320\260\321\217 \320\277\320\276\321\207\321\202\320\260:", nullptr));
        label_21->setText(QApplication::translate("KeyGenClass", "\320\237\320\276\320\264\321\200\320\260\320\267\320\264\320\265\320\273\320\265\320\275\320\270\320\265:", nullptr));
        label_22->setText(QApplication::translate("KeyGenClass", "\320\240\320\260\320\271\320\276\320\275:", nullptr));
        pushButtonW2Next->setText(QApplication::translate("KeyGenClass", "\320\224\320\260\320\273\320\265\320\265 >", nullptr));
        label_4->setText(QApplication::translate("KeyGenClass", "\320\223\320\265\320\275\320\265\321\200\320\260\321\206\320\270\321\217 \321\201\320\265\320\272\321\200\320\265\321\202\320\275\320\276\320\263\320\276 \320\272\320\273\321\216\321\207\320\260", nullptr));
        label_5->setText(QApplication::translate("KeyGenClass", "\320\235\320\260\320\266\320\270\320\274\320\260\320\271\321\202\320\265 \320\273\321\216\320\261\321\213\320\265 \320\272\320\273\320\260\320\262\320\270\321\210\320\270", nullptr));
        labelW3EndTime->setText(QApplication::translate("KeyGenClass", "\320\236\321\201\321\202\320\260\320\273\320\276\321\201\321\214 4:59", nullptr));
        labelAnimation->setText(QApplication::translate("KeyGenClass", "TextLabel", nullptr));
        label_6->setText(QApplication::translate("KeyGenClass", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\277\320\260\321\200\320\276\320\273\321\214 \320\267\320\260\321\211\320\270\321\202\321\213 \321\201\320\265\320\272\321\200\320\265\321\202\320\275\320\276\320\263\320\276 \320\272\320\273\321\216\321\207\320\260.", nullptr));
        pushButtonW3Next->setText(QApplication::translate("KeyGenClass", "\320\224\320\260\320\273\320\265\320\265 >", nullptr));
        label_7->setText(QString());
        label_8->setText(QApplication::translate("KeyGenClass", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
        label_9->setText(QApplication::translate("KeyGenClass", "\320\237\320\276\320\264\321\202\320\262\320\265\321\200\320\266\320\264\320\265\320\275\320\270\320\265 \320\277\320\260\321\200\320\276\320\273\321\217", nullptr));
        pushButtonW5PrintZayavka->setText(QApplication::translate("KeyGenClass", "\320\237\320\265\321\207\320\260\321\202\321\214 \320\267\320\260\321\217\320\262\320\272\320\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class KeyGenClass: public Ui_KeyGenClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KEYGEN_H
