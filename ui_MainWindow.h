/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

namespace Afc {

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *lblTransaction;
    QLabel *lblTransactionShadow;
    QLabel *lblLogo1;
    QWidget *widget;
    QLabel *lblLogo2;
    QWidget *widget_2;
    QLabel *lblTime;
    QLabel *lblLeft;
    QLabel *lblRight;
    QPushButton *btnPanel;

    void setupUi(QMainWindow *Afc__MainWindow)
    {
        if (Afc__MainWindow->objectName().isEmpty())
            Afc__MainWindow->setObjectName(QString::fromUtf8("Afc__MainWindow"));
        Afc__MainWindow->resize(800, 480);
        Afc__MainWindow->setStyleSheet(QString::fromUtf8(""));
        centralWidget = new QWidget(Afc__MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        lblTransaction = new QLabel(centralWidget);
        lblTransaction->setObjectName(QString::fromUtf8("lblTransaction"));
        lblTransaction->setGeometry(QRect(0, 0, 800, 480));
        QFont font;
        font.setFamily(QString::fromUtf8("Sahel Black FD"));
        font.setPointSize(45);
        lblTransaction->setFont(font);
        lblTransaction->setLayoutDirection(Qt::RightToLeft);
        lblTransaction->setStyleSheet(QString::fromUtf8("font-family: \"Sahel Black FD\";\n"
"font-size: 45pt;"));
        lblTransaction->setTextFormat(Qt::PlainText);
        lblTransaction->setAlignment(Qt::AlignCenter);
        lblTransactionShadow = new QLabel(centralWidget);
        lblTransactionShadow->setObjectName(QString::fromUtf8("lblTransactionShadow"));
        lblTransactionShadow->setGeometry(QRect(0, 208, 800, 151));
        lblTransactionShadow->setMinimumSize(QSize(1, 0));
        lblTransactionShadow->setFont(font);
        lblTransactionShadow->setStyleSheet(QString::fromUtf8("font-family: \"Sahel Black FD\";\n"
"font-size: 45pt;\n"
"color:rgba(90,90,90,100);"));
        lblTransactionShadow->setTextFormat(Qt::PlainText);
        lblTransactionShadow->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        lblLogo1 = new QLabel(centralWidget);
        lblLogo1->setObjectName(QString::fromUtf8("lblLogo1"));
        lblLogo1->setGeometry(QRect(86, 24, 57, 28));
        lblLogo1->setStyleSheet(QString::fromUtf8("font-family: \"Sahel Black FD\";\n"
"font-size: 18pt;"));
        lblLogo1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        widget = new QWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, -4, 69, 85));
        widget->setMinimumSize(QSize(64, 64));
        widget->setStyleSheet(QString::fromUtf8("image: url(:/images/logo_mapra.png);"));
        lblLogo2 = new QLabel(centralWidget);
        lblLogo2->setObjectName(QString::fromUtf8("lblLogo2"));
        lblLogo2->setGeometry(QRect(591, 24, 129, 28));
        lblLogo2->setMinimumSize(QSize(0, 0));
        lblLogo2->setStyleSheet(QString::fromUtf8("font-family: \"Sahel Black FD\";\n"
"font-size: 18pt;"));
        widget_2 = new QWidget(centralWidget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(724, 9, 64, 64));
        widget_2->setMinimumSize(QSize(64, 64));
        widget_2->setStyleSheet(QString::fromUtf8("image: url(:/images/logo_metro.png);"));
        lblTime = new QLabel(centralWidget);
        lblTime->setObjectName(QString::fromUtf8("lblTime"));
        lblTime->setGeometry(QRect(255, 440, 290, 40));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Sahel Black FD"));
        font1.setPointSize(20);
        lblTime->setFont(font1);
        lblTime->setStyleSheet(QString::fromUtf8("image: url(:/images/banner.png);\n"
"image-position:bottom;\n"
"font-family: \"Sahel Black FD\";\n"
"font-size: 20pt;\n"
""));
        lblTime->setAlignment(Qt::AlignCenter);
        lblLeft = new QLabel(centralWidget);
        lblLeft->setObjectName(QString::fromUtf8("lblLeft"));
        lblLeft->setGeometry(QRect(566, 418, 225, 40));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Liberation Serif Regular"));
        font2.setPointSize(18);
        lblLeft->setFont(font2);
        lblLeft->setStyleSheet(QString::fromUtf8("font-family:  \"Liberation Serif Regular\";\n"
"font-size: 18pt;"));
        lblLeft->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblRight = new QLabel(centralWidget);
        lblRight->setObjectName(QString::fromUtf8("lblRight"));
        lblRight->setGeometry(QRect(566, 444, 225, 40));
        lblRight->setFont(font2);
        lblRight->setStyleSheet(QString::fromUtf8("font-family:  \"Liberation Serif Regular\";\n"
"font-size: 18pt;"));
        lblRight->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        btnPanel = new QPushButton(centralWidget);
        btnPanel->setObjectName(QString::fromUtf8("btnPanel"));
        btnPanel->setGeometry(QRect(10, 436, 141, 43));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Sahel Black FD"));
        font3.setPointSize(22);
        btnPanel->setFont(font3);
        btnPanel->setFocusPolicy(Qt::NoFocus);
        btnPanel->setLayoutDirection(Qt::RightToLeft);
        btnPanel->setStyleSheet(QString::fromUtf8("background: transparent;\n"
"font-family: \"Sahel Black FD\";\n"
"font-size: 22pt;\n"
"border: 0;\n"
"outline: 0;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/icon_settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPanel->setIcon(icon);
        btnPanel->setIconSize(QSize(32, 32));
        Afc__MainWindow->setCentralWidget(centralWidget);

        retranslateUi(Afc__MainWindow);

        QMetaObject::connectSlotsByName(Afc__MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *Afc__MainWindow)
    {
        Afc__MainWindow->setWindowTitle(QApplication::translate("Afc::MainWindow", "Elin Card Reader", 0, QApplication::UnicodeUTF8));
        lblTransaction->setText(QApplication::translate("Afc::MainWindow", "\331\204\330\267\331\201\330\247 \330\250\331\204\333\214\330\252 \330\247\330\261\330\247\330\246\331\207 \330\257\331\207\333\214\330\257", 0, QApplication::UnicodeUTF8));
        lblTransactionShadow->setText(QApplication::translate("Afc::MainWindow", "\331\204\330\267\331\201\330\247 \330\250\331\204\333\214\330\252 \330\247\330\261\330\247\330\246\331\207 \330\257\331\207\333\214\330\257", 0, QApplication::UnicodeUTF8));
        lblLogo1->setText(QApplication::translate("Afc::MainWindow", "\331\205\331\276\330\261\330\247", 0, QApplication::UnicodeUTF8));
        lblLogo2->setText(QApplication::translate("Afc::MainWindow", "\331\205\330\252\330\261\331\210 \330\264\333\214\330\261\330\247\330\262", 0, QApplication::UnicodeUTF8));
        lblTime->setText(QApplication::translate("Afc::MainWindow", "000/00/00 | 00:00:00", 0, QApplication::UnicodeUTF8));
        lblLeft->setText(QApplication::translate("Afc::MainWindow", "000-0-0000", 0, QApplication::UnicodeUTF8));
        lblRight->setText(QApplication::translate("Afc::MainWindow", "000-0-0000", 0, QApplication::UnicodeUTF8));
        btnPanel->setText(QApplication::translate("Afc::MainWindow", "\330\252\331\206\330\270\333\214\331\205\330\247\330\252", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

} // namespace Afc

namespace Afc {
namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui
} // namespace Afc

#endif // UI_MAINWINDOW_H
