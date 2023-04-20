/********************************************************************************
** Form generated from reading UI file 'GranitCrypt.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRANITCRYPT_H
#define UI_GRANITCRYPT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GranitCryptClass
{
public:
    QAction *actionOpenFile;
    QAction *actionExit;
    QAction *actionHelp;
    QAction *actionDbPath;
    QAction *actionResetSetings;
    QAction *actiondffd;
    QAction *actionSaveCert;
    QWidget *centralWidget;
    QPushButton *pushButtonDecrypt;
    QProgressBar *progressBarAll;
    QPushButton *pushButtonCrypt;
    QLabel *labelProgressFile;
    QPlainTextEdit *plainTextEditFiles;
    QProgressBar *progressBarFile;
    QLabel *labelProgressAll;
    QPushButton *pushButtonSelectKey;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menuHelp;
    QMenu *menuSettings;

    void setupUi(QMainWindow *GranitCryptClass)
    {
        if (GranitCryptClass->objectName().isEmpty())
            GranitCryptClass->setObjectName(QStringLiteral("GranitCryptClass"));
        GranitCryptClass->resize(1000, 618);
        GranitCryptClass->setMaximumSize(QSize(1000, 618));
        QFont font;
        font.setPointSize(11);
        GranitCryptClass->setFont(font);
        GranitCryptClass->setStyleSheet(QStringLiteral(""));
        actionOpenFile = new QAction(GranitCryptClass);
        actionOpenFile->setObjectName(QStringLiteral("actionOpenFile"));
        actionExit = new QAction(GranitCryptClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionHelp = new QAction(GranitCryptClass);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        actionDbPath = new QAction(GranitCryptClass);
        actionDbPath->setObjectName(QStringLiteral("actionDbPath"));
        actionDbPath->setFont(font);
        actionResetSetings = new QAction(GranitCryptClass);
        actionResetSetings->setObjectName(QStringLiteral("actionResetSetings"));
        actionResetSetings->setFont(font);
        actiondffd = new QAction(GranitCryptClass);
        actiondffd->setObjectName(QStringLiteral("actiondffd"));
        actionSaveCert = new QAction(GranitCryptClass);
        actionSaveCert->setObjectName(QStringLiteral("actionSaveCert"));
        centralWidget = new QWidget(GranitCryptClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButtonDecrypt = new QPushButton(centralWidget);
        pushButtonDecrypt->setObjectName(QStringLiteral("pushButtonDecrypt"));
        pushButtonDecrypt->setGeometry(QRect(850, 550, 132, 31));
        pushButtonDecrypt->setFont(font);
        pushButtonDecrypt->setStyleSheet(QLatin1String("text-align:right;\n"
"background-image: url(\"Image/decrypted\");\n"
"background-repeat: no-repeat;\n"
"background-position: left "));
        progressBarAll = new QProgressBar(centralWidget);
        progressBarAll->setObjectName(QStringLiteral("progressBarAll"));
        progressBarAll->setGeometry(QRect(20, 510, 951, 23));
        progressBarAll->setValue(24);
        pushButtonCrypt = new QPushButton(centralWidget);
        pushButtonCrypt->setObjectName(QStringLiteral("pushButtonCrypt"));
        pushButtonCrypt->setGeometry(QRect(704, 550, 132, 31));
        pushButtonCrypt->setFont(font);
        pushButtonCrypt->setStyleSheet(QLatin1String("text-align:right;\n"
"background-image: url(\"Image/encrypted.png\");\n"
"background-repeat: no-repeat;\n"
"background-position: left "));
        labelProgressFile = new QLabel(centralWidget);
        labelProgressFile->setObjectName(QStringLiteral("labelProgressFile"));
        labelProgressFile->setGeometry(QRect(20, 438, 911, 21));
        plainTextEditFiles = new QPlainTextEdit(centralWidget);
        plainTextEditFiles->setObjectName(QStringLiteral("plainTextEditFiles"));
        plainTextEditFiles->setGeometry(QRect(10, 0, 981, 431));
        plainTextEditFiles->setReadOnly(true);
        progressBarFile = new QProgressBar(centralWidget);
        progressBarFile->setObjectName(QStringLiteral("progressBarFile"));
        progressBarFile->setGeometry(QRect(20, 460, 951, 23));
        progressBarFile->setValue(24);
        progressBarFile->setTextVisible(true);
        labelProgressAll = new QLabel(centralWidget);
        labelProgressAll->setObjectName(QStringLiteral("labelProgressAll"));
        labelProgressAll->setGeometry(QRect(20, 490, 921, 16));
        pushButtonSelectKey = new QPushButton(centralWidget);
        pushButtonSelectKey->setObjectName(QStringLiteral("pushButtonSelectKey"));
        pushButtonSelectKey->setGeometry(QRect(20, 550, 132, 31));
        pushButtonSelectKey->setFont(font);
        pushButtonSelectKey->setStyleSheet(QStringLiteral(""));
        GranitCryptClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(GranitCryptClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1000, 26));
        menuBar->setFont(font);
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        menu_File->setFont(font);
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuHelp->setFont(font);
        menuSettings = new QMenu(menuBar);
        menuSettings->setObjectName(QStringLiteral("menuSettings"));
        GranitCryptClass->setMenuBar(menuBar);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menuSettings->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menu_File->addAction(actionOpenFile);
        menu_File->addAction(actionExit);
        menuHelp->addAction(actionHelp);
        menuSettings->addAction(actionDbPath);
        menuSettings->addAction(actionResetSetings);

        retranslateUi(GranitCryptClass);

        QMetaObject::connectSlotsByName(GranitCryptClass);
    } // setupUi

    void retranslateUi(QMainWindow *GranitCryptClass)
    {
        GranitCryptClass->setWindowTitle(QApplication::translate("GranitCryptClass", "\320\241\320\270\321\201\321\202\320\265\320\274\320\260 \320\267\320\260\321\211\320\270\321\202\320\260 \320\264\320\260\320\275\320\275\321\213\321\205 \"\320\240\320\276\320\267\320\260-\320\234\" MIT License", nullptr));
        actionOpenFile->setText(QApplication::translate("GranitCryptClass", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", nullptr));
        actionExit->setText(QApplication::translate("GranitCryptClass", "\320\222\321\213\321\205\320\276\320\264", nullptr));
        actionHelp->setText(QApplication::translate("GranitCryptClass", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", nullptr));
        actionDbPath->setText(QApplication::translate("GranitCryptClass", "\320\241\320\263\320\265\320\275\320\265\321\200\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \320\272\320\273\321\216\321\207", nullptr));
        actionResetSetings->setText(QApplication::translate("GranitCryptClass", "\320\241\320\261\321\200\320\276\321\201 \320\275\320\260\321\201\321\202\321\200\320\276\320\265\320\272", nullptr));
        actiondffd->setText(QApplication::translate("GranitCryptClass", "dffd", nullptr));
        actionSaveCert->setText(QApplication::translate("GranitCryptClass", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \321\201\320\265\321\200\321\202\320\270\321\204\320\270\320\272\320\260\321\202\321\213", nullptr));
        pushButtonDecrypt->setText(QApplication::translate("GranitCryptClass", "\320\240\320\260\321\201\321\210\320\270\321\204\321\200\320\276\320\262\320\260\321\202\321\214 ", nullptr));
        pushButtonCrypt->setText(QApplication::translate("GranitCryptClass", "\320\227\320\260\321\210\320\270\321\204\321\200\320\276\320\262\320\260\321\202\321\214 ", nullptr));
        labelProgressFile->setText(QApplication::translate("GranitCryptClass", "labelProgressFile", nullptr));
        labelProgressAll->setText(QApplication::translate("GranitCryptClass", "labelProgressAll", nullptr));
        pushButtonSelectKey->setText(QApplication::translate("GranitCryptClass", "Test", nullptr));
        menu_File->setTitle(QApplication::translate("GranitCryptClass", "\320\244\320\260\320\271\320\273", nullptr));
        menuHelp->setTitle(QApplication::translate("GranitCryptClass", "\320\241\320\277\321\200\320\260\320\262\320\272\320\260", nullptr));
        menuSettings->setTitle(QApplication::translate("GranitCryptClass", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GranitCryptClass: public Ui_GranitCryptClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRANITCRYPT_H
