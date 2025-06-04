/********************************************************************************
** Form generated from reading UI file 'PanelWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PANELWIDGET_H
#define UI_PANELWIDGET_H

#include <QtCore/QDate>
#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "QFocusableLineEdit.h"

namespace Afc {

class Ui_PanelWidget
{
public:
    QGridLayout *gridLayout_7;
    QTabWidget *panelTab;
    QWidget *tabMain;
    QGridLayout *gridLayout_2;
    QPushButton *btnExitPanel;
    QPushButton *btnOperator;
    QPushButton *btnGateMode;
    QSpacerItem *verticalSpacer_3;
    QWidget *tabGateModes;
    QGridLayout *gridLayout_3;
    QSpacerItem *verticalSpacer_7;
    QPushButton *btnExitGateMode;
    QRadioButton *radioGateMode3;
    QRadioButton *radioGateMode2;
    QRadioButton *radioGateMode1;
    QRadioButton *radioGateMode4;
    QRadioButton *radioGateMode5;
    QRadioButton *radioGateMode6;
    QRadioButton *radioGateMode9;
    QRadioButton *radioGateMode8;
    QRadioButton *radioGateMode7;
    QRadioButton *radioGateModeAuto;
    QWidget *tabOperatorLogin;
    QGridLayout *gridLayout_4;
    QPushButton *btnExitLogin;
    QFocusableLineEdit *txtPassword;
    QSpacerItem *verticalSpacer;
    QPushButton *btnLogin;
    QLabel *label;
    QSpacerItem *verticalSpacer_2;
    QWidget *tabOperator;
    QGridLayout *gridLayout_5;
    QPushButton *btnDateSettings;
    QPushButton *btnSamSettings;
    QPushButton *btnIpSettings;
    QPushButton *btnFileOperations;
    QPushButton *btnGateSettings;
    QSpacerItem *verticalSpacer_4;
    QPushButton *btnExitOperator;
    QWidget *tabIpSettings;
    QGridLayout *gridLayout;
    QPushButton *radioReaderB;
    QPushButton *btnSaveIp;
    QSpacerItem *verticalSpacer_5;
    QLabel *label_3;
    QPushButton *radioReaderA;
    QLabel *label_4;
    QPushButton *btnExitIpSettings;
    QFocusableLineEdit *txtIp;
    QLabel *label_2;
    QFocusableLineEdit *txtStationId;
    QLabel *label_5;
    QFocusableLineEdit *txtQrIp;
    QWidget *tabDateSettings;
    QGridLayout *gridLayout_8;
    QPushButton *btnSaveDate;
    QPushButton *btnExitDateSettings;
    QSpacerItem *verticalSpacer_6;
    QLabel *label_7;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout_5;
    QFocusableLineEdit *txtDateNew;
    QFocusableLineEdit *txtTimeNew;
    QDateTimeEdit *dateTimeCurrent;
    QSpacerItem *horizontalSpacer_2;
    QWidget *tabGateSettings;
    QGridLayout *gridLayout_14;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_13;
    QLabel *lblGateEntryCounter;
    QLabel *label_9;
    QLabel *lblGateExitCounter;
    QLabel *label_10;
    QSpacerItem *verticalSpacer_16;
    QLabel *lblLastGateReset;
    QSpacerItem *verticalSpacer_18;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *btnExitGateSettings;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btnClearGateCounters;
    QWidget *tabSamSettings;
    QGridLayout *gridLayout_6;
    QSpinBox *spinSamSlot;
    QPushButton *btnSaveSam;
    QLabel *label_12;
    QLabel *label_11;
    QPushButton *btnExitSamSettings;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_8;
    QFocusableLineEdit *txtSamKey;
    QSpacerItem *verticalSpacer_17;
    QLabel *lblSamModuleError;
    QLabel *lblSamCityPayError;
    QLabel *lblSamCityWayError;
    QWidget *tabFileOperation;
    QGridLayout *gridLayout_9;
    QPushButton *btnExitFileOperations;
    QPushButton *btnTransactions;
    QPushButton *btnParameters;
    QPushButton *btnLogs;
    QSpacerItem *verticalSpacer_9;
    QWidget *tabParameters;
    QGridLayout *gridLayout_10;
    QHBoxLayout *horizontalLayout;
    QLabel *lblUsbParamVersion;
    QLabel *label_14;
    QPushButton *btnExitParameters;
    QProgressBar *progressParameters;
    QLabel *lblUsbParamStatus;
    QSpacerItem *verticalSpacer_10;
    QPushButton *btnLoadParameters;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lblCurrentParamVersion;
    QLabel *label_16;
    QLabel *lblUsbParamStructure;
    QSpacerItem *verticalSpacer_13;
    QWidget *tabTransactions;
    QGridLayout *gridLayout_11;
    QSpacerItem *verticalSpacer_11;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lblUsbTransactionsCount;
    QLabel *label_17;
    QPushButton *btnExitTransactions;
    QPushButton *btnSaveTransactions;
    QProgressBar *progressTransactions;
    QLabel *lblUsbTransactionStatus;
    QSpacerItem *verticalSpacer_12;
    QWidget *tabLogs;
    QGridLayout *gridLayout_12;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lblUsbLogsCount;
    QLabel *label_18;
    QLabel *lblUsbLogStatus;
    QProgressBar *progressLogs;
    QSpacerItem *verticalSpacer_15;
    QPushButton *btnExitLog;
    QPushButton *btnSaveLogs;
    QSpacerItem *verticalSpacer_14;

    void setupUi(QWidget *Afc__PanelWidget)
    {
        if (Afc__PanelWidget->objectName().isEmpty())
            Afc__PanelWidget->setObjectName(QString::fromUtf8("Afc__PanelWidget"));
        Afc__PanelWidget->resize(801, 480);
        Afc__PanelWidget->setStyleSheet(QString::fromUtf8("color: black;\n"
"background-color: rgb(210, 210, 210);\n"
"font-family: \"Sahel FD\";\n"
"font-size: 20pt;\n"
"outline: 0;"));
        gridLayout_7 = new QGridLayout(Afc__PanelWidget);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        panelTab = new QTabWidget(Afc__PanelWidget);
        panelTab->setObjectName(QString::fromUtf8("panelTab"));
        panelTab->setStyleSheet(QString::fromUtf8("QTabWidget{\n"
"border: 0;\n"
"}\n"
"QTabWidget::pane{\n"
"border: 0;\n"
"}\n"
"QTabWidget::tab-bar{\n"
"border: 0;\n"
"}\n"
"QTabBar::tab {\n"
"border: 0;\n"
"width: 0px;\n"
"height: 0px;\n"
"}\n"
""));
        panelTab->setTabShape(QTabWidget::Rounded);
        panelTab->setUsesScrollButtons(true);
        tabMain = new QWidget();
        tabMain->setObjectName(QString::fromUtf8("tabMain"));
        tabMain->setStyleSheet(QString::fromUtf8(""));
        gridLayout_2 = new QGridLayout(tabMain);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        btnExitPanel = new QPushButton(tabMain);
        btnExitPanel->setObjectName(QString::fromUtf8("btnExitPanel"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnExitPanel->sizePolicy().hasHeightForWidth());
        btnExitPanel->setSizePolicy(sizePolicy);
        btnExitPanel->setMinimumSize(QSize(0, 80));
        btnExitPanel->setMaximumSize(QSize(16777215, 80));
        btnExitPanel->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_2->addWidget(btnExitPanel, 2, 0, 1, 1);

        btnOperator = new QPushButton(tabMain);
        btnOperator->setObjectName(QString::fromUtf8("btnOperator"));
        sizePolicy.setHeightForWidth(btnOperator->sizePolicy().hasHeightForWidth());
        btnOperator->setSizePolicy(sizePolicy);
        btnOperator->setMinimumSize(QSize(0, 80));
        btnOperator->setMaximumSize(QSize(16777215, 80));
        btnOperator->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_2->addWidget(btnOperator, 0, 1, 1, 1);

        btnGateMode = new QPushButton(tabMain);
        btnGateMode->setObjectName(QString::fromUtf8("btnGateMode"));
        sizePolicy.setHeightForWidth(btnGateMode->sizePolicy().hasHeightForWidth());
        btnGateMode->setSizePolicy(sizePolicy);
        btnGateMode->setMinimumSize(QSize(0, 80));
        btnGateMode->setMaximumSize(QSize(16777215, 80));
        btnGateMode->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_2->addWidget(btnGateMode, 0, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_3, 1, 0, 1, 1);

        panelTab->addTab(tabMain, QString());
        tabGateModes = new QWidget();
        tabGateModes->setObjectName(QString::fromUtf8("tabGateModes"));
        tabGateModes->setStyleSheet(QString::fromUtf8(""));
        gridLayout_3 = new QGridLayout(tabGateModes);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_7, 3, 0, 1, 3);

        btnExitGateMode = new QPushButton(tabGateModes);
        btnExitGateMode->setObjectName(QString::fromUtf8("btnExitGateMode"));
        sizePolicy.setHeightForWidth(btnExitGateMode->sizePolicy().hasHeightForWidth());
        btnExitGateMode->setSizePolicy(sizePolicy);
        btnExitGateMode->setMinimumSize(QSize(0, 80));
        btnExitGateMode->setMaximumSize(QSize(16777215, 80));
        btnExitGateMode->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_3->addWidget(btnExitGateMode, 4, 0, 1, 1);

        radioGateMode3 = new QRadioButton(tabGateModes);
        radioGateMode3->setObjectName(QString::fromUtf8("radioGateMode3"));
        radioGateMode3->setMinimumSize(QSize(0, 80));
        radioGateMode3->setMaximumSize(QSize(16777215, 80));
        radioGateMode3->setStyleSheet(QString::fromUtf8("QRadioButton\n"
"{\n"
"border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"QRadioButton:checked\n"
"{\n"
"background-color: rgb(0, 255, 0);\n"
"}\n"
"QRadioButton::indicator\n"
"{\n"
"border: 0;\n"
"width: 63px;\n"
"}"));
        radioGateMode3->setIconSize(QSize(16, 16));
        radioGateMode3->setCheckable(true);
        radioGateMode3->setChecked(false);

        gridLayout_3->addWidget(radioGateMode3, 0, 0, 1, 1);

        radioGateMode2 = new QRadioButton(tabGateModes);
        radioGateMode2->setObjectName(QString::fromUtf8("radioGateMode2"));
        radioGateMode2->setMinimumSize(QSize(0, 80));
        radioGateMode2->setMaximumSize(QSize(16777215, 80));
        radioGateMode2->setStyleSheet(QString::fromUtf8("QRadioButton\n"
"{\n"
"border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"QRadioButton:checked\n"
"{\n"
"background-color: rgb(0, 255, 0);\n"
"}\n"
"QRadioButton::indicator\n"
"{\n"
"border: 0;\n"
"width: 66px;\n"
"}"));
        radioGateMode2->setIconSize(QSize(16, 16));
        radioGateMode2->setCheckable(true);

        gridLayout_3->addWidget(radioGateMode2, 0, 1, 1, 1);

        radioGateMode1 = new QRadioButton(tabGateModes);
        radioGateMode1->setObjectName(QString::fromUtf8("radioGateMode1"));
        radioGateMode1->setMinimumSize(QSize(0, 80));
        radioGateMode1->setMaximumSize(QSize(16777215, 80));
        radioGateMode1->setStyleSheet(QString::fromUtf8("QRadioButton\n"
"{\n"
"border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"QRadioButton:checked\n"
"{\n"
"background-color: rgb(0, 255, 0);\n"
"}\n"
"QRadioButton::indicator\n"
"{\n"
"border: 0;\n"
"width: 68px;\n"
"}"));
        radioGateMode1->setIconSize(QSize(16, 16));
        radioGateMode1->setCheckable(true);

        gridLayout_3->addWidget(radioGateMode1, 0, 2, 1, 1);

        radioGateMode4 = new QRadioButton(tabGateModes);
        radioGateMode4->setObjectName(QString::fromUtf8("radioGateMode4"));
        radioGateMode4->setMinimumSize(QSize(0, 80));
        radioGateMode4->setMaximumSize(QSize(16777215, 80));
        radioGateMode4->setStyleSheet(QString::fromUtf8("QRadioButton\n"
"{\n"
"border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"QRadioButton:checked\n"
"{\n"
"background-color: rgb(0, 255, 0);\n"
"}\n"
"QRadioButton::indicator\n"
"{\n"
"border: 0;\n"
"width: 76px;\n"
"}"));
        radioGateMode4->setIconSize(QSize(16, 16));
        radioGateMode4->setCheckable(true);

        gridLayout_3->addWidget(radioGateMode4, 1, 2, 1, 1);

        radioGateMode5 = new QRadioButton(tabGateModes);
        radioGateMode5->setObjectName(QString::fromUtf8("radioGateMode5"));
        radioGateMode5->setMinimumSize(QSize(0, 80));
        radioGateMode5->setMaximumSize(QSize(16777215, 80));
        radioGateMode5->setStyleSheet(QString::fromUtf8("QRadioButton\n"
"{\n"
"border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"QRadioButton:checked\n"
"{\n"
"background-color: rgb(0, 255, 0);\n"
"}\n"
"QRadioButton::indicator\n"
"{\n"
"border: 0;\n"
"width: 70px;\n"
"}"));
        radioGateMode5->setIconSize(QSize(16, 16));
        radioGateMode5->setCheckable(true);

        gridLayout_3->addWidget(radioGateMode5, 1, 1, 1, 1);

        radioGateMode6 = new QRadioButton(tabGateModes);
        radioGateMode6->setObjectName(QString::fromUtf8("radioGateMode6"));
        radioGateMode6->setMinimumSize(QSize(0, 80));
        radioGateMode6->setMaximumSize(QSize(16777215, 80));
        radioGateMode6->setStyleSheet(QString::fromUtf8("QRadioButton\n"
"{\n"
"border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"QRadioButton:checked\n"
"{\n"
"background-color: rgb(0, 255, 0);\n"
"}\n"
"QRadioButton::indicator\n"
"{\n"
"border: 0;\n"
"width: 80px;\n"
"}"));
        radioGateMode6->setIconSize(QSize(16, 16));
        radioGateMode6->setCheckable(true);

        gridLayout_3->addWidget(radioGateMode6, 1, 0, 1, 1);

        radioGateMode9 = new QRadioButton(tabGateModes);
        radioGateMode9->setObjectName(QString::fromUtf8("radioGateMode9"));
        radioGateMode9->setMinimumSize(QSize(0, 80));
        radioGateMode9->setMaximumSize(QSize(16777215, 80));
        radioGateMode9->setStyleSheet(QString::fromUtf8("QRadioButton\n"
"{\n"
"border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"QRadioButton:checked\n"
"{\n"
"background-color: rgb(0, 255, 0);\n"
"}\n"
"QRadioButton::indicator\n"
"{\n"
"border: 0;\n"
"width: 70px;\n"
"}"));
        radioGateMode9->setIconSize(QSize(16, 16));
        radioGateMode9->setCheckable(true);

        gridLayout_3->addWidget(radioGateMode9, 2, 0, 1, 1);

        radioGateMode8 = new QRadioButton(tabGateModes);
        radioGateMode8->setObjectName(QString::fromUtf8("radioGateMode8"));
        radioGateMode8->setMinimumSize(QSize(0, 80));
        radioGateMode8->setMaximumSize(QSize(16777215, 80));
        radioGateMode8->setStyleSheet(QString::fromUtf8("QRadioButton\n"
"{\n"
"border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"QRadioButton:checked\n"
"{\n"
"background-color: rgb(0, 255, 0);\n"
"}\n"
"QRadioButton::indicator\n"
"{\n"
"border: 0;\n"
"width: 74px;\n"
"}"));
        radioGateMode8->setIconSize(QSize(16, 16));
        radioGateMode8->setCheckable(true);

        gridLayout_3->addWidget(radioGateMode8, 2, 1, 1, 1);

        radioGateMode7 = new QRadioButton(tabGateModes);
        radioGateMode7->setObjectName(QString::fromUtf8("radioGateMode7"));
        radioGateMode7->setMinimumSize(QSize(0, 80));
        radioGateMode7->setMaximumSize(QSize(16777215, 80));
        radioGateMode7->setStyleSheet(QString::fromUtf8("QRadioButton\n"
"{\n"
"border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"QRadioButton:checked\n"
"{\n"
"background-color: rgb(0, 255, 0);\n"
"}\n"
"QRadioButton::indicator\n"
"{\n"
"border: 0;\n"
"width: 72px;\n"
"}"));
        radioGateMode7->setIconSize(QSize(16, 16));
        radioGateMode7->setCheckable(true);

        gridLayout_3->addWidget(radioGateMode7, 2, 2, 1, 1);

        radioGateModeAuto = new QRadioButton(tabGateModes);
        radioGateModeAuto->setObjectName(QString::fromUtf8("radioGateModeAuto"));
        radioGateModeAuto->setMinimumSize(QSize(0, 80));
        radioGateModeAuto->setMaximumSize(QSize(16777215, 80));
        radioGateModeAuto->setStyleSheet(QString::fromUtf8("QRadioButton\n"
"{\n"
"border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"QRadioButton:checked\n"
"{\n"
"background-color: rgb(0, 255, 0);\n"
"}\n"
"QRadioButton::indicator\n"
"{\n"
"border: 0;\n"
"width: 58px;\n"
"}"));
        radioGateModeAuto->setIconSize(QSize(16, 16));
        radioGateModeAuto->setCheckable(true);
        radioGateModeAuto->setChecked(true);

        gridLayout_3->addWidget(radioGateModeAuto, 4, 2, 1, 1);

        panelTab->addTab(tabGateModes, QString());
        tabOperatorLogin = new QWidget();
        tabOperatorLogin->setObjectName(QString::fromUtf8("tabOperatorLogin"));
        gridLayout_4 = new QGridLayout(tabOperatorLogin);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        btnExitLogin = new QPushButton(tabOperatorLogin);
        btnExitLogin->setObjectName(QString::fromUtf8("btnExitLogin"));
        btnExitLogin->setMinimumSize(QSize(0, 80));
        btnExitLogin->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_4->addWidget(btnExitLogin, 4, 0, 1, 1);

        txtPassword = new QFocusableLineEdit(tabOperatorLogin);
        txtPassword->setObjectName(QString::fromUtf8("txtPassword"));
        txtPassword->setMinimumSize(QSize(0, 80));
        txtPassword->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        txtPassword->setEchoMode(QLineEdit::Password);

        gridLayout_4->addWidget(txtPassword, 2, 0, 1, 3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer, 3, 0, 1, 3);

        btnLogin = new QPushButton(tabOperatorLogin);
        btnLogin->setObjectName(QString::fromUtf8("btnLogin"));
        btnLogin->setMinimumSize(QSize(0, 80));
        btnLogin->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_4->addWidget(btnLogin, 4, 2, 1, 1);

        label = new QLabel(tabOperatorLogin);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(0, 50));

        gridLayout_4->addWidget(label, 1, 0, 1, 3);

        verticalSpacer_2 = new QSpacerItem(20, 90, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_4->addItem(verticalSpacer_2, 0, 1, 1, 1);

        panelTab->addTab(tabOperatorLogin, QString());
        tabOperator = new QWidget();
        tabOperator->setObjectName(QString::fromUtf8("tabOperator"));
        gridLayout_5 = new QGridLayout(tabOperator);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        btnDateSettings = new QPushButton(tabOperator);
        btnDateSettings->setObjectName(QString::fromUtf8("btnDateSettings"));
        sizePolicy.setHeightForWidth(btnDateSettings->sizePolicy().hasHeightForWidth());
        btnDateSettings->setSizePolicy(sizePolicy);
        btnDateSettings->setMinimumSize(QSize(0, 80));
        btnDateSettings->setMaximumSize(QSize(16777215, 80));
        btnDateSettings->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_5->addWidget(btnDateSettings, 0, 0, 1, 1);

        btnSamSettings = new QPushButton(tabOperator);
        btnSamSettings->setObjectName(QString::fromUtf8("btnSamSettings"));
        sizePolicy.setHeightForWidth(btnSamSettings->sizePolicy().hasHeightForWidth());
        btnSamSettings->setSizePolicy(sizePolicy);
        btnSamSettings->setMinimumSize(QSize(0, 80));
        btnSamSettings->setMaximumSize(QSize(16777215, 80));
        btnSamSettings->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_5->addWidget(btnSamSettings, 1, 0, 1, 1);

        btnIpSettings = new QPushButton(tabOperator);
        btnIpSettings->setObjectName(QString::fromUtf8("btnIpSettings"));
        sizePolicy.setHeightForWidth(btnIpSettings->sizePolicy().hasHeightForWidth());
        btnIpSettings->setSizePolicy(sizePolicy);
        btnIpSettings->setMinimumSize(QSize(0, 80));
        btnIpSettings->setMaximumSize(QSize(16777215, 80));
        btnIpSettings->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_5->addWidget(btnIpSettings, 0, 1, 1, 1);

        btnFileOperations = new QPushButton(tabOperator);
        btnFileOperations->setObjectName(QString::fromUtf8("btnFileOperations"));
        btnFileOperations->setEnabled(false);
        sizePolicy.setHeightForWidth(btnFileOperations->sizePolicy().hasHeightForWidth());
        btnFileOperations->setSizePolicy(sizePolicy);
        btnFileOperations->setMinimumSize(QSize(0, 80));
        btnFileOperations->setMaximumSize(QSize(16777215, 80));
        btnFileOperations->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"QPushButton:disabled\n"
"{\n"
"background-color: rgb(210, 210, 210);\n"
"}"));

        gridLayout_5->addWidget(btnFileOperations, 2, 1, 1, 1);

        btnGateSettings = new QPushButton(tabOperator);
        btnGateSettings->setObjectName(QString::fromUtf8("btnGateSettings"));
        btnGateSettings->setEnabled(true);
        sizePolicy.setHeightForWidth(btnGateSettings->sizePolicy().hasHeightForWidth());
        btnGateSettings->setSizePolicy(sizePolicy);
        btnGateSettings->setMinimumSize(QSize(0, 80));
        btnGateSettings->setMaximumSize(QSize(16777215, 80));
        btnGateSettings->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"QPushButton:disabled\n"
"{\n"
"background-color: rgb(210, 210, 210);\n"
"}"));

        gridLayout_5->addWidget(btnGateSettings, 1, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_4, 3, 0, 1, 1);

        btnExitOperator = new QPushButton(tabOperator);
        btnExitOperator->setObjectName(QString::fromUtf8("btnExitOperator"));
        sizePolicy.setHeightForWidth(btnExitOperator->sizePolicy().hasHeightForWidth());
        btnExitOperator->setSizePolicy(sizePolicy);
        btnExitOperator->setMinimumSize(QSize(0, 80));
        btnExitOperator->setMaximumSize(QSize(16777215, 80));
        btnExitOperator->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_5->addWidget(btnExitOperator, 4, 0, 1, 1);

        panelTab->addTab(tabOperator, QString());
        tabIpSettings = new QWidget();
        tabIpSettings->setObjectName(QString::fromUtf8("tabIpSettings"));
        gridLayout = new QGridLayout(tabIpSettings);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        radioReaderB = new QPushButton(tabIpSettings);
        radioReaderB->setObjectName(QString::fromUtf8("radioReaderB"));
        radioReaderB->setEnabled(false);
        radioReaderB->setMinimumSize(QSize(0, 80));
        radioReaderB->setMaximumSize(QSize(16777215, 80));
        radioReaderB->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"QPushButton:checked\n"
"{\n"
"background-color: rgb(0, 255, 0);\n"
"}"));
        radioReaderB->setCheckable(true);

        gridLayout->addWidget(radioReaderB, 3, 1, 1, 1);

        btnSaveIp = new QPushButton(tabIpSettings);
        btnSaveIp->setObjectName(QString::fromUtf8("btnSaveIp"));
        sizePolicy.setHeightForWidth(btnSaveIp->sizePolicy().hasHeightForWidth());
        btnSaveIp->setSizePolicy(sizePolicy);
        btnSaveIp->setMinimumSize(QSize(0, 80));
        btnSaveIp->setMaximumSize(QSize(16777215, 80));
        btnSaveIp->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout->addWidget(btnSaveIp, 6, 2, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_5, 5, 0, 1, 3);

        label_3 = new QLabel(tabIpSettings);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setLayoutDirection(Qt::LeftToRight);
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_3, 1, 2, 1, 1);

        radioReaderA = new QPushButton(tabIpSettings);
        radioReaderA->setObjectName(QString::fromUtf8("radioReaderA"));
        radioReaderA->setEnabled(false);
        radioReaderA->setMinimumSize(QSize(0, 80));
        radioReaderA->setMaximumSize(QSize(16777215, 80));
        radioReaderA->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"QPushButton:checked\n"
"{\n"
"background-color: rgb(0, 255, 0);\n"
"}"));
        radioReaderA->setCheckable(true);
        radioReaderA->setChecked(true);

        gridLayout->addWidget(radioReaderA, 3, 0, 1, 1);

        label_4 = new QLabel(tabIpSettings);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 2, 1, 1);

        btnExitIpSettings = new QPushButton(tabIpSettings);
        btnExitIpSettings->setObjectName(QString::fromUtf8("btnExitIpSettings"));
        sizePolicy.setHeightForWidth(btnExitIpSettings->sizePolicy().hasHeightForWidth());
        btnExitIpSettings->setSizePolicy(sizePolicy);
        btnExitIpSettings->setMinimumSize(QSize(0, 80));
        btnExitIpSettings->setMaximumSize(QSize(16777215, 80));
        btnExitIpSettings->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout->addWidget(btnExitIpSettings, 6, 0, 1, 1);

        txtIp = new QFocusableLineEdit(tabIpSettings);
        txtIp->setObjectName(QString::fromUtf8("txtIp"));
        txtIp->setMinimumSize(QSize(0, 80));
        txtIp->setMaximumSize(QSize(16777215, 80));
        txtIp->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 57 regular 11pt \"LiberationSerif-Regular\";"));
        txtIp->setFrame(true);

        gridLayout->addWidget(txtIp, 1, 0, 1, 2);

        label_2 = new QLabel(tabIpSettings);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        txtStationId = new QFocusableLineEdit(tabIpSettings);
        txtStationId->setObjectName(QString::fromUtf8("txtStationId"));
        txtStationId->setEnabled(false);
        txtStationId->setMinimumSize(QSize(0, 80));
        txtStationId->setMaximumSize(QSize(16777215, 80));
        txtStationId->setStyleSheet(QString::fromUtf8("QFocusableLineEdit\n"
"{\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"QFocusableLineEdit:disabled\n"
"{\n"
"background-color: rgb(210, 210, 210);\n"
"}"));
        txtStationId->setFrame(true);

        gridLayout->addWidget(txtStationId, 0, 0, 1, 2);

        label_5 = new QLabel(tabIpSettings);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_5, 2, 2, 1, 1);

        txtQrIp = new QFocusableLineEdit(tabIpSettings);
        txtQrIp->setObjectName(QString::fromUtf8("txtQrIp"));
        txtQrIp->setMinimumSize(QSize(0, 80));
        txtQrIp->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 57 regular 11pt \"LiberationSerif-Regular\";"));

        gridLayout->addWidget(txtQrIp, 2, 0, 1, 2);

        panelTab->addTab(tabIpSettings, QString());
        tabDateSettings = new QWidget();
        tabDateSettings->setObjectName(QString::fromUtf8("tabDateSettings"));
        gridLayout_8 = new QGridLayout(tabDateSettings);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        gridLayout_8->setContentsMargins(0, 0, 0, 0);
        btnSaveDate = new QPushButton(tabDateSettings);
        btnSaveDate->setObjectName(QString::fromUtf8("btnSaveDate"));
        sizePolicy.setHeightForWidth(btnSaveDate->sizePolicy().hasHeightForWidth());
        btnSaveDate->setSizePolicy(sizePolicy);
        btnSaveDate->setMinimumSize(QSize(300, 80));
        btnSaveDate->setMaximumSize(QSize(300, 80));
        btnSaveDate->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_8->addWidget(btnSaveDate, 3, 2, 1, 1);

        btnExitDateSettings = new QPushButton(tabDateSettings);
        btnExitDateSettings->setObjectName(QString::fromUtf8("btnExitDateSettings"));
        sizePolicy.setHeightForWidth(btnExitDateSettings->sizePolicy().hasHeightForWidth());
        btnExitDateSettings->setSizePolicy(sizePolicy);
        btnExitDateSettings->setMinimumSize(QSize(300, 80));
        btnExitDateSettings->setMaximumSize(QSize(300, 80));
        btnExitDateSettings->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_8->addWidget(btnExitDateSettings, 3, 0, 1, 1);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_8->addItem(verticalSpacer_6, 2, 0, 1, 3);

        label_7 = new QLabel(tabDateSettings);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_8->addWidget(label_7, 1, 2, 1, 1);

        label_6 = new QLabel(tabDateSettings);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_8->addWidget(label_6, 0, 2, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        txtDateNew = new QFocusableLineEdit(tabDateSettings);
        txtDateNew->setObjectName(QString::fromUtf8("txtDateNew"));
        txtDateNew->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(txtDateNew->sizePolicy().hasHeightForWidth());
        txtDateNew->setSizePolicy(sizePolicy1);
        txtDateNew->setMinimumSize(QSize(0, 80));
        txtDateNew->setMaximumSize(QSize(16777215, 16777215));
        txtDateNew->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 57 regular 11pt \"LiberationSerif-Regular\";"));
        txtDateNew->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(txtDateNew);

        txtTimeNew = new QFocusableLineEdit(tabDateSettings);
        txtTimeNew->setObjectName(QString::fromUtf8("txtTimeNew"));
        txtTimeNew->setEnabled(true);
        sizePolicy1.setHeightForWidth(txtTimeNew->sizePolicy().hasHeightForWidth());
        txtTimeNew->setSizePolicy(sizePolicy1);
        txtTimeNew->setMinimumSize(QSize(0, 80));
        txtTimeNew->setMaximumSize(QSize(16777215, 16777215));
        txtTimeNew->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 57 regular 11pt \"LiberationSerif-Regular\";"));
        txtTimeNew->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(txtTimeNew);


        gridLayout_8->addLayout(horizontalLayout_5, 1, 0, 1, 1);

        dateTimeCurrent = new QDateTimeEdit(tabDateSettings);
        dateTimeCurrent->setObjectName(QString::fromUtf8("dateTimeCurrent"));
        sizePolicy1.setHeightForWidth(dateTimeCurrent->sizePolicy().hasHeightForWidth());
        dateTimeCurrent->setSizePolicy(sizePolicy1);
        dateTimeCurrent->setMinimumSize(QSize(200, 80));
        QFont font;
        font.setFamily(QString::fromUtf8("11pt LiberationSerif-Regular"));
        font.setPointSize(20);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(7);
        dateTimeCurrent->setFont(font);
        dateTimeCurrent->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 57 regular 11pt \"LiberationSerif-Regular\";\n"
""));
        dateTimeCurrent->setLocale(QLocale(QLocale::Persian, QLocale::Iran));
        dateTimeCurrent->setAlignment(Qt::AlignCenter);
        dateTimeCurrent->setReadOnly(true);
        dateTimeCurrent->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dateTimeCurrent->setKeyboardTracking(true);
        dateTimeCurrent->setMaximumDate(QDate(3000, 12, 31));
        dateTimeCurrent->setMinimumDate(QDate(2000, 12, 2));
        dateTimeCurrent->setCalendarPopup(false);
        dateTimeCurrent->setTimeSpec(Qt::UTC);

        gridLayout_8->addWidget(dateTimeCurrent, 0, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(150, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_2, 1, 1, 1, 1);

        panelTab->addTab(tabDateSettings, QString());
        tabGateSettings = new QWidget();
        tabGateSettings->setObjectName(QString::fromUtf8("tabGateSettings"));
        gridLayout_14 = new QGridLayout(tabGateSettings);
        gridLayout_14->setSpacing(0);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        gridLayout_14->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout_13 = new QGridLayout();
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        lblGateEntryCounter = new QLabel(tabGateSettings);
        lblGateEntryCounter->setObjectName(QString::fromUtf8("lblGateEntryCounter"));
        lblGateEntryCounter->setMinimumSize(QSize(0, 0));
        lblGateEntryCounter->setMaximumSize(QSize(200, 16777215));
        lblGateEntryCounter->setStyleSheet(QString::fromUtf8("font-size: 26px;"));

        gridLayout_13->addWidget(lblGateEntryCounter, 0, 0, 1, 1);

        label_9 = new QLabel(tabGateSettings);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(0, 0));
        label_9->setMaximumSize(QSize(16777215, 16777215));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Sahel FD"));
        label_9->setFont(font1);
        label_9->setStyleSheet(QString::fromUtf8("font-size: 26px;"));

        gridLayout_13->addWidget(label_9, 0, 1, 1, 1);

        lblGateExitCounter = new QLabel(tabGateSettings);
        lblGateExitCounter->setObjectName(QString::fromUtf8("lblGateExitCounter"));
        lblGateExitCounter->setMinimumSize(QSize(0, 0));
        lblGateExitCounter->setMaximumSize(QSize(200, 16777215));
        lblGateExitCounter->setStyleSheet(QString::fromUtf8("font-size: 26px;"));

        gridLayout_13->addWidget(lblGateExitCounter, 1, 0, 1, 1);

        label_10 = new QLabel(tabGateSettings);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMinimumSize(QSize(0, 0));
        label_10->setMaximumSize(QSize(16777215, 16777215));
        label_10->setFont(font1);
        label_10->setStyleSheet(QString::fromUtf8("font-size: 26px;"));

        gridLayout_13->addWidget(label_10, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout_13);

        verticalSpacer_16 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_16);

        lblLastGateReset = new QLabel(tabGateSettings);
        lblLastGateReset->setObjectName(QString::fromUtf8("lblLastGateReset"));

        verticalLayout->addWidget(lblLastGateReset);

        verticalSpacer_18 = new QSpacerItem(310, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_18);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        btnExitGateSettings = new QPushButton(tabGateSettings);
        btnExitGateSettings->setObjectName(QString::fromUtf8("btnExitGateSettings"));
        sizePolicy.setHeightForWidth(btnExitGateSettings->sizePolicy().hasHeightForWidth());
        btnExitGateSettings->setSizePolicy(sizePolicy);
        btnExitGateSettings->setMinimumSize(QSize(300, 80));
        btnExitGateSettings->setMaximumSize(QSize(99999, 80));
        btnExitGateSettings->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        horizontalLayout_6->addWidget(btnExitGateSettings);

        horizontalSpacer_3 = new QSpacerItem(144, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);

        btnClearGateCounters = new QPushButton(tabGateSettings);
        btnClearGateCounters->setObjectName(QString::fromUtf8("btnClearGateCounters"));
        sizePolicy.setHeightForWidth(btnClearGateCounters->sizePolicy().hasHeightForWidth());
        btnClearGateCounters->setSizePolicy(sizePolicy);
        btnClearGateCounters->setMinimumSize(QSize(300, 80));
        btnClearGateCounters->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        horizontalLayout_6->addWidget(btnClearGateCounters);


        verticalLayout->addLayout(horizontalLayout_6);


        gridLayout_14->addLayout(verticalLayout, 0, 0, 1, 1);

        panelTab->addTab(tabGateSettings, QString());
        tabSamSettings = new QWidget();
        tabSamSettings->setObjectName(QString::fromUtf8("tabSamSettings"));
        gridLayout_6 = new QGridLayout(tabSamSettings);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_6->setContentsMargins(0, 0, 0, 0);
        spinSamSlot = new QSpinBox(tabSamSettings);
        spinSamSlot->setObjectName(QString::fromUtf8("spinSamSlot"));
        spinSamSlot->setMinimumSize(QSize(0, 80));
        spinSamSlot->setMaximumSize(QSize(100, 16777215));
        spinSamSlot->setStyleSheet(QString::fromUtf8("background: white;\n"
"font: 57 regular 11pt \"LiberationSerif-Regular\";"));
        spinSamSlot->setAlignment(Qt::AlignCenter);
        spinSamSlot->setMaximum(3);

        gridLayout_6->addWidget(spinSamSlot, 1, 0, 1, 1);

        btnSaveSam = new QPushButton(tabSamSettings);
        btnSaveSam->setObjectName(QString::fromUtf8("btnSaveSam"));
        sizePolicy.setHeightForWidth(btnSaveSam->sizePolicy().hasHeightForWidth());
        btnSaveSam->setSizePolicy(sizePolicy);
        btnSaveSam->setMinimumSize(QSize(300, 80));
        btnSaveSam->setMaximumSize(QSize(16777215, 80));
        btnSaveSam->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_6->addWidget(btnSaveSam, 7, 2, 1, 1);

        label_12 = new QLabel(tabSamSettings);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_12, 0, 2, 1, 1);

        label_11 = new QLabel(tabSamSettings);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_11, 1, 2, 1, 1);

        btnExitSamSettings = new QPushButton(tabSamSettings);
        btnExitSamSettings->setObjectName(QString::fromUtf8("btnExitSamSettings"));
        sizePolicy.setHeightForWidth(btnExitSamSettings->sizePolicy().hasHeightForWidth());
        btnExitSamSettings->setSizePolicy(sizePolicy);
        btnExitSamSettings->setMinimumSize(QSize(300, 80));
        btnExitSamSettings->setMaximumSize(QSize(256, 80));
        btnExitSamSettings->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_6->addWidget(btnExitSamSettings, 7, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer, 7, 1, 1, 1);

        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer_8, 6, 0, 1, 3);

        txtSamKey = new QFocusableLineEdit(tabSamSettings);
        txtSamKey->setObjectName(QString::fromUtf8("txtSamKey"));
        txtSamKey->setMinimumSize(QSize(0, 80));
        txtSamKey->setMaximumSize(QSize(550, 80));
        txtSamKey->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 57 regular 11pt \"LiberationSerif-Regular\";"));
        txtSamKey->setMaxLength(47);

        gridLayout_6->addWidget(txtSamKey, 0, 0, 1, 2);

        verticalSpacer_17 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_6->addItem(verticalSpacer_17, 2, 0, 1, 3);

        lblSamModuleError = new QLabel(tabSamSettings);
        lblSamModuleError->setObjectName(QString::fromUtf8("lblSamModuleError"));
        lblSamModuleError->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));

        gridLayout_6->addWidget(lblSamModuleError, 3, 0, 1, 3);

        lblSamCityPayError = new QLabel(tabSamSettings);
        lblSamCityPayError->setObjectName(QString::fromUtf8("lblSamCityPayError"));
        lblSamCityPayError->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));

        gridLayout_6->addWidget(lblSamCityPayError, 4, 0, 1, 3);

        lblSamCityWayError = new QLabel(tabSamSettings);
        lblSamCityWayError->setObjectName(QString::fromUtf8("lblSamCityWayError"));
        lblSamCityWayError->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));

        gridLayout_6->addWidget(lblSamCityWayError, 5, 0, 1, 3);

        panelTab->addTab(tabSamSettings, QString());
        tabFileOperation = new QWidget();
        tabFileOperation->setObjectName(QString::fromUtf8("tabFileOperation"));
        gridLayout_9 = new QGridLayout(tabFileOperation);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        gridLayout_9->setContentsMargins(-1, 0, 0, 0);
        btnExitFileOperations = new QPushButton(tabFileOperation);
        btnExitFileOperations->setObjectName(QString::fromUtf8("btnExitFileOperations"));
        sizePolicy.setHeightForWidth(btnExitFileOperations->sizePolicy().hasHeightForWidth());
        btnExitFileOperations->setSizePolicy(sizePolicy);
        btnExitFileOperations->setMinimumSize(QSize(0, 80));
        btnExitFileOperations->setMaximumSize(QSize(16777215, 80));
        btnExitFileOperations->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_9->addWidget(btnExitFileOperations, 3, 0, 1, 1);

        btnTransactions = new QPushButton(tabFileOperation);
        btnTransactions->setObjectName(QString::fromUtf8("btnTransactions"));
        sizePolicy.setHeightForWidth(btnTransactions->sizePolicy().hasHeightForWidth());
        btnTransactions->setSizePolicy(sizePolicy);
        btnTransactions->setMinimumSize(QSize(0, 80));
        btnTransactions->setMaximumSize(QSize(16777215, 80));
        btnTransactions->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_9->addWidget(btnTransactions, 0, 0, 1, 1);

        btnParameters = new QPushButton(tabFileOperation);
        btnParameters->setObjectName(QString::fromUtf8("btnParameters"));
        sizePolicy.setHeightForWidth(btnParameters->sizePolicy().hasHeightForWidth());
        btnParameters->setSizePolicy(sizePolicy);
        btnParameters->setMinimumSize(QSize(0, 80));
        btnParameters->setMaximumSize(QSize(16777215, 80));
        btnParameters->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_9->addWidget(btnParameters, 0, 1, 1, 1);

        btnLogs = new QPushButton(tabFileOperation);
        btnLogs->setObjectName(QString::fromUtf8("btnLogs"));
        sizePolicy.setHeightForWidth(btnLogs->sizePolicy().hasHeightForWidth());
        btnLogs->setSizePolicy(sizePolicy);
        btnLogs->setMinimumSize(QSize(0, 80));
        btnLogs->setMaximumSize(QSize(16777215, 80));
        btnLogs->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_9->addWidget(btnLogs, 1, 1, 1, 1);

        verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_9, 2, 0, 1, 2);

        panelTab->addTab(tabFileOperation, QString());
        tabParameters = new QWidget();
        tabParameters->setObjectName(QString::fromUtf8("tabParameters"));
        gridLayout_10 = new QGridLayout(tabParameters);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        gridLayout_10->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lblUsbParamVersion = new QLabel(tabParameters);
        lblUsbParamVersion->setObjectName(QString::fromUtf8("lblUsbParamVersion"));
        lblUsbParamVersion->setMinimumSize(QSize(0, 80));
        lblUsbParamVersion->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(lblUsbParamVersion);

        label_14 = new QLabel(tabParameters);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setMinimumSize(QSize(0, 80));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_14);


        gridLayout_10->addLayout(horizontalLayout, 1, 1, 1, 1);

        btnExitParameters = new QPushButton(tabParameters);
        btnExitParameters->setObjectName(QString::fromUtf8("btnExitParameters"));
        sizePolicy.setHeightForWidth(btnExitParameters->sizePolicy().hasHeightForWidth());
        btnExitParameters->setSizePolicy(sizePolicy);
        btnExitParameters->setMinimumSize(QSize(0, 80));
        btnExitParameters->setMaximumSize(QSize(16777215, 80));
        btnExitParameters->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_10->addWidget(btnExitParameters, 5, 0, 1, 1);

        progressParameters = new QProgressBar(tabParameters);
        progressParameters->setObjectName(QString::fromUtf8("progressParameters"));
        progressParameters->setStyleSheet(QString::fromUtf8(""));
        progressParameters->setLocale(QLocale(QLocale::Persian, QLocale::Iran));
        progressParameters->setValue(0);

        gridLayout_10->addWidget(progressParameters, 3, 0, 1, 2);

        lblUsbParamStatus = new QLabel(tabParameters);
        lblUsbParamStatus->setObjectName(QString::fromUtf8("lblUsbParamStatus"));
        lblUsbParamStatus->setMinimumSize(QSize(0, 80));
        lblUsbParamStatus->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);\n"
"border: 4px solid white;\n"
"border-radius: 8px;"));
        lblUsbParamStatus->setAlignment(Qt::AlignCenter);

        gridLayout_10->addWidget(lblUsbParamStatus, 0, 1, 1, 1);

        verticalSpacer_10 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_10->addItem(verticalSpacer_10, 4, 0, 1, 2);

        btnLoadParameters = new QPushButton(tabParameters);
        btnLoadParameters->setObjectName(QString::fromUtf8("btnLoadParameters"));
        sizePolicy.setHeightForWidth(btnLoadParameters->sizePolicy().hasHeightForWidth());
        btnLoadParameters->setSizePolicy(sizePolicy);
        btnLoadParameters->setMinimumSize(QSize(0, 80));
        btnLoadParameters->setMaximumSize(QSize(16777215, 80));
        btnLoadParameters->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_10->addWidget(btnLoadParameters, 5, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lblCurrentParamVersion = new QLabel(tabParameters);
        lblCurrentParamVersion->setObjectName(QString::fromUtf8("lblCurrentParamVersion"));
        lblCurrentParamVersion->setMinimumSize(QSize(0, 80));
        lblCurrentParamVersion->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(lblCurrentParamVersion);

        label_16 = new QLabel(tabParameters);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setMinimumSize(QSize(0, 80));
        label_16->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label_16);


        gridLayout_10->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        lblUsbParamStructure = new QLabel(tabParameters);
        lblUsbParamStructure->setObjectName(QString::fromUtf8("lblUsbParamStructure"));
        lblUsbParamStructure->setMinimumSize(QSize(0, 80));
        lblUsbParamStructure->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);\n"
"border: 4px solid white;\n"
"border-radius: 8px;"));
        lblUsbParamStructure->setAlignment(Qt::AlignCenter);

        gridLayout_10->addWidget(lblUsbParamStructure, 0, 0, 1, 1);

        verticalSpacer_13 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_10->addItem(verticalSpacer_13, 2, 0, 1, 1);

        panelTab->addTab(tabParameters, QString());
        tabTransactions = new QWidget();
        tabTransactions->setObjectName(QString::fromUtf8("tabTransactions"));
        gridLayout_11 = new QGridLayout(tabTransactions);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        gridLayout_11->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_11 = new QSpacerItem(779, 115, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_11->addItem(verticalSpacer_11, 4, 0, 1, 2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lblUsbTransactionsCount = new QLabel(tabTransactions);
        lblUsbTransactionsCount->setObjectName(QString::fromUtf8("lblUsbTransactionsCount"));
        lblUsbTransactionsCount->setMinimumSize(QSize(0, 80));
        lblUsbTransactionsCount->setLocale(QLocale(QLocale::Persian, QLocale::Iran));
        lblUsbTransactionsCount->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(lblUsbTransactionsCount);

        label_17 = new QLabel(tabTransactions);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setMinimumSize(QSize(0, 80));
        label_17->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label_17);


        gridLayout_11->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        btnExitTransactions = new QPushButton(tabTransactions);
        btnExitTransactions->setObjectName(QString::fromUtf8("btnExitTransactions"));
        sizePolicy.setHeightForWidth(btnExitTransactions->sizePolicy().hasHeightForWidth());
        btnExitTransactions->setSizePolicy(sizePolicy);
        btnExitTransactions->setMinimumSize(QSize(0, 80));
        btnExitTransactions->setMaximumSize(QSize(16777215, 80));
        btnExitTransactions->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_11->addWidget(btnExitTransactions, 5, 0, 1, 1);

        btnSaveTransactions = new QPushButton(tabTransactions);
        btnSaveTransactions->setObjectName(QString::fromUtf8("btnSaveTransactions"));
        sizePolicy.setHeightForWidth(btnSaveTransactions->sizePolicy().hasHeightForWidth());
        btnSaveTransactions->setSizePolicy(sizePolicy);
        btnSaveTransactions->setMinimumSize(QSize(0, 80));
        btnSaveTransactions->setMaximumSize(QSize(16777215, 80));
        btnSaveTransactions->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_11->addWidget(btnSaveTransactions, 5, 1, 1, 1);

        progressTransactions = new QProgressBar(tabTransactions);
        progressTransactions->setObjectName(QString::fromUtf8("progressTransactions"));
        progressTransactions->setStyleSheet(QString::fromUtf8(""));
        progressTransactions->setLocale(QLocale(QLocale::Persian, QLocale::Iran));
        progressTransactions->setValue(0);

        gridLayout_11->addWidget(progressTransactions, 3, 0, 1, 2);

        lblUsbTransactionStatus = new QLabel(tabTransactions);
        lblUsbTransactionStatus->setObjectName(QString::fromUtf8("lblUsbTransactionStatus"));
        lblUsbTransactionStatus->setMinimumSize(QSize(0, 80));
        lblUsbTransactionStatus->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);\n"
"border: 4px solid white;\n"
"border-radius: 8px;"));
        lblUsbTransactionStatus->setAlignment(Qt::AlignCenter);

        gridLayout_11->addWidget(lblUsbTransactionStatus, 0, 1, 1, 1);

        verticalSpacer_12 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_11->addItem(verticalSpacer_12, 2, 0, 1, 2);

        panelTab->addTab(tabTransactions, QString());
        tabLogs = new QWidget();
        tabLogs->setObjectName(QString::fromUtf8("tabLogs"));
        gridLayout_12 = new QGridLayout(tabLogs);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        gridLayout_12->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        lblUsbLogsCount = new QLabel(tabLogs);
        lblUsbLogsCount->setObjectName(QString::fromUtf8("lblUsbLogsCount"));
        lblUsbLogsCount->setMinimumSize(QSize(0, 80));
        lblUsbLogsCount->setLocale(QLocale(QLocale::Persian, QLocale::Iran));
        lblUsbLogsCount->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(lblUsbLogsCount);

        label_18 = new QLabel(tabLogs);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setMinimumSize(QSize(0, 80));
        label_18->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label_18);


        gridLayout_12->addLayout(horizontalLayout_4, 0, 0, 1, 1);

        lblUsbLogStatus = new QLabel(tabLogs);
        lblUsbLogStatus->setObjectName(QString::fromUtf8("lblUsbLogStatus"));
        lblUsbLogStatus->setMinimumSize(QSize(0, 80));
        lblUsbLogStatus->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);\n"
"border: 4px solid white;\n"
"border-radius: 8px;"));
        lblUsbLogStatus->setAlignment(Qt::AlignCenter);

        gridLayout_12->addWidget(lblUsbLogStatus, 0, 1, 1, 1);

        progressLogs = new QProgressBar(tabLogs);
        progressLogs->setObjectName(QString::fromUtf8("progressLogs"));
        progressLogs->setStyleSheet(QString::fromUtf8(""));
        progressLogs->setLocale(QLocale(QLocale::Persian, QLocale::Iran));
        progressLogs->setValue(0);

        gridLayout_12->addWidget(progressLogs, 2, 0, 1, 2);

        verticalSpacer_15 = new QSpacerItem(779, 115, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_12->addItem(verticalSpacer_15, 3, 0, 1, 2);

        btnExitLog = new QPushButton(tabLogs);
        btnExitLog->setObjectName(QString::fromUtf8("btnExitLog"));
        sizePolicy.setHeightForWidth(btnExitLog->sizePolicy().hasHeightForWidth());
        btnExitLog->setSizePolicy(sizePolicy);
        btnExitLog->setMinimumSize(QSize(0, 80));
        btnExitLog->setMaximumSize(QSize(16777215, 80));
        btnExitLog->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_12->addWidget(btnExitLog, 4, 0, 1, 1);

        btnSaveLogs = new QPushButton(tabLogs);
        btnSaveLogs->setObjectName(QString::fromUtf8("btnSaveLogs"));
        sizePolicy.setHeightForWidth(btnSaveLogs->sizePolicy().hasHeightForWidth());
        btnSaveLogs->setSizePolicy(sizePolicy);
        btnSaveLogs->setMinimumSize(QSize(0, 80));
        btnSaveLogs->setMaximumSize(QSize(16777215, 80));
        btnSaveLogs->setStyleSheet(QString::fromUtf8("border: 4px solid black;\n"
"border-radius: 8px;\n"
"background-color: rgb(255, 255, 255);"));

        gridLayout_12->addWidget(btnSaveLogs, 4, 1, 1, 1);

        verticalSpacer_14 = new QSpacerItem(779, 116, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_12->addItem(verticalSpacer_14, 1, 0, 1, 2);

        panelTab->addTab(tabLogs, QString());

        gridLayout_7->addWidget(panelTab, 0, 0, 1, 1);


        retranslateUi(Afc__PanelWidget);

        panelTab->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(Afc__PanelWidget);
    } // setupUi

    void retranslateUi(QWidget *Afc__PanelWidget)
    {
        Afc__PanelWidget->setWindowTitle(QApplication::translate("Afc::PanelWidget", "Form", 0, QApplication::UnicodeUTF8));
        btnExitPanel->setText(QApplication::translate("Afc::PanelWidget", "\330\256\330\261\331\210\330\254", 0, QApplication::UnicodeUTF8));
        btnOperator->setText(QApplication::translate("Afc::PanelWidget", "\330\247\331\276\330\261\330\247\330\252\331\210\330\261", 0, QApplication::UnicodeUTF8));
        btnGateMode->setText(QApplication::translate("Afc::PanelWidget", "\330\252\330\272\333\214\333\214\330\261 \330\252\331\206\330\270\333\214\331\205\330\247\330\252 \332\257\333\214\330\252", 0, QApplication::UnicodeUTF8));
        panelTab->setTabText(panelTab->indexOf(tabMain), QString());
        btnExitGateMode->setText(QApplication::translate("Afc::PanelWidget", "\330\256\330\261\331\210\330\254", 0, QApplication::UnicodeUTF8));
        radioGateMode3->setText(QApplication::translate("Afc::PanelWidget", "\332\251\331\206\330\252\330\261\331\204 - \332\251\331\206\330\252\330\261\331\204", 0, QApplication::UnicodeUTF8));
        radioGateMode2->setText(QApplication::translate("Afc::PanelWidget", "\330\250\331\204\330\247\332\251 - \332\251\331\206\330\252\330\261\331\204", 0, QApplication::UnicodeUTF8));
        radioGateMode1->setText(QApplication::translate("Afc::PanelWidget", "\332\251\331\206\330\252\330\261\331\204- \330\250\331\204\330\247\332\251", 0, QApplication::UnicodeUTF8));
        radioGateMode4->setText(QApplication::translate("Afc::PanelWidget", "\330\250\331\204\330\247\332\251 - \330\242\330\262\330\247\330\257", 0, QApplication::UnicodeUTF8));
        radioGateMode5->setText(QApplication::translate("Afc::PanelWidget", "\330\250\331\204\330\247\332\251 - \330\250\331\204\330\247\332\251", 0, QApplication::UnicodeUTF8));
        radioGateMode6->setText(QApplication::translate("Afc::PanelWidget", "\330\242\330\262\330\247\330\257 - \330\242\330\262\330\247\330\257", 0, QApplication::UnicodeUTF8));
        radioGateMode9->setText(QApplication::translate("Afc::PanelWidget", "\330\242\330\262\330\247\330\257 - \332\251\331\206\330\252\330\261\331\204", 0, QApplication::UnicodeUTF8));
        radioGateMode8->setText(QApplication::translate("Afc::PanelWidget", "\330\242\330\262\330\247\330\257 - \330\250\331\204\330\247\332\251", 0, QApplication::UnicodeUTF8));
        radioGateMode7->setText(QApplication::translate("Afc::PanelWidget", "\332\251\331\206\330\252\330\261\331\204 - \330\242\330\262\330\247\330\257", 0, QApplication::UnicodeUTF8));
        radioGateModeAuto->setText(QApplication::translate("Afc::PanelWidget", "\331\210\330\266\330\271\333\214\330\252 \330\256\331\210\330\257\332\251\330\247\330\261", 0, QApplication::UnicodeUTF8));
        panelTab->setTabText(panelTab->indexOf(tabGateModes), QString());
        btnExitLogin->setText(QApplication::translate("Afc::PanelWidget", "\330\256\330\261\331\210\330\254", 0, QApplication::UnicodeUTF8));
        btnLogin->setText(QApplication::translate("Afc::PanelWidget", "\330\252\330\247\333\214\333\214\330\257", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Afc::PanelWidget", "\330\261\331\205\330\262 \330\271\330\250\331\210\330\261:", 0, QApplication::UnicodeUTF8));
        panelTab->setTabText(panelTab->indexOf(tabOperatorLogin), QApplication::translate("Afc::PanelWidget", "Page", 0, QApplication::UnicodeUTF8));
        btnDateSettings->setText(QApplication::translate("Afc::PanelWidget", "\330\252\331\206\330\270\333\214\331\205\330\247\330\252 \330\262\331\205\330\247\331\206 \331\210 \330\252\330\247\330\261\333\214\330\256", 0, QApplication::UnicodeUTF8));
        btnSamSettings->setText(QApplication::translate("Afc::PanelWidget", "SAM \330\252\331\206\330\270\333\214\331\205\330\247\330\252", 0, QApplication::UnicodeUTF8));
        btnIpSettings->setText(QApplication::translate("Afc::PanelWidget", "IP \330\252\331\206\330\270\333\214\331\205\330\247\330\252", 0, QApplication::UnicodeUTF8));
        btnFileOperations->setText(QApplication::translate("Afc::PanelWidget", "\330\271\331\205\331\204\333\214\330\247\330\252 \331\201\330\247\333\214\331\204", 0, QApplication::UnicodeUTF8));
        btnGateSettings->setText(QApplication::translate("Afc::PanelWidget", "\330\252\331\206\330\270\333\214\331\205\330\247\330\252 \332\257\333\214\330\252", 0, QApplication::UnicodeUTF8));
        btnExitOperator->setText(QApplication::translate("Afc::PanelWidget", "\330\256\330\261\331\210\330\254", 0, QApplication::UnicodeUTF8));
        panelTab->setTabText(panelTab->indexOf(tabOperator), QApplication::translate("Afc::PanelWidget", "Page", 0, QApplication::UnicodeUTF8));
        radioReaderB->setText(QApplication::translate("Afc::PanelWidget", "B", 0, QApplication::UnicodeUTF8));
        btnSaveIp->setText(QApplication::translate("Afc::PanelWidget", "\330\260\330\256\333\214\330\261\331\207", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Afc::PanelWidget", ":IP", 0, QApplication::UnicodeUTF8));
        radioReaderA->setText(QApplication::translate("Afc::PanelWidget", "A", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Afc::PanelWidget", "\331\206\331\210\330\271 \332\251\330\247\330\261\330\252\330\256\331\210\330\247\331\206:", 0, QApplication::UnicodeUTF8));
        btnExitIpSettings->setText(QApplication::translate("Afc::PanelWidget", "\330\256\330\261\331\210\330\254", 0, QApplication::UnicodeUTF8));
        txtIp->setInputMask(QString());
        txtIp->setText(QString());
        txtIp->setPlaceholderText(QString());
        label_2->setText(QApplication::translate("Afc::PanelWidget", "\332\251\330\257 \330\247\333\214\330\263\330\252\332\257\330\247\331\207:", 0, QApplication::UnicodeUTF8));
        txtStationId->setInputMask(QString());
        txtStationId->setText(QString());
        txtStationId->setPlaceholderText(QString());
        label_5->setText(QApplication::translate("Afc::PanelWidget", "\330\242\330\257\330\261\330\263 \330\263\330\261\331\210\330\261  QR:", 0, QApplication::UnicodeUTF8));
        panelTab->setTabText(panelTab->indexOf(tabIpSettings), QApplication::translate("Afc::PanelWidget", "Page", 0, QApplication::UnicodeUTF8));
        btnSaveDate->setText(QApplication::translate("Afc::PanelWidget", "\330\260\330\256\333\214\330\261\331\207", 0, QApplication::UnicodeUTF8));
        btnExitDateSettings->setText(QApplication::translate("Afc::PanelWidget", "\330\256\330\261\331\210\330\254", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Afc::PanelWidget", "\330\262\331\205\330\247\331\206 \330\254\330\257\333\214\330\257:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Afc::PanelWidget", "\330\262\331\205\330\247\331\206 \331\201\330\271\331\204\333\214:", 0, QApplication::UnicodeUTF8));
        txtDateNew->setInputMask(QApplication::translate("Afc::PanelWidget", "0000/00/00", 0, QApplication::UnicodeUTF8));
        txtTimeNew->setInputMask(QApplication::translate("Afc::PanelWidget", "00:00", 0, QApplication::UnicodeUTF8));
        dateTimeCurrent->setDisplayFormat(QApplication::translate("Afc::PanelWidget", "yyyy/MM/dd HH:mm", 0, QApplication::UnicodeUTF8));
        panelTab->setTabText(panelTab->indexOf(tabDateSettings), QApplication::translate("Afc::PanelWidget", "Page", 0, QApplication::UnicodeUTF8));
        lblGateEntryCounter->setText(QApplication::translate("Afc::PanelWidget", "0", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("Afc::PanelWidget", "\331\210\330\261\331\210\330\257:", 0, QApplication::UnicodeUTF8));
        lblGateExitCounter->setText(QApplication::translate("Afc::PanelWidget", "0", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("Afc::PanelWidget", "\330\256\330\261\331\210\330\254:", 0, QApplication::UnicodeUTF8));
        lblLastGateReset->setText(QString());
        btnExitGateSettings->setText(QApplication::translate("Afc::PanelWidget", "\330\256\330\261\331\210\330\254", 0, QApplication::UnicodeUTF8));
        btnClearGateCounters->setText(QApplication::translate("Afc::PanelWidget", "\331\276\330\247\332\251 \332\251\330\261\330\257\331\206 \330\242\331\205\330\247\330\261", 0, QApplication::UnicodeUTF8));
        panelTab->setTabText(panelTab->indexOf(tabGateSettings), QApplication::translate("Afc::PanelWidget", "Page", 0, QApplication::UnicodeUTF8));
        btnSaveSam->setText(QApplication::translate("Afc::PanelWidget", "\330\260\330\256\333\214\330\261\331\207", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("Afc::PanelWidget", ":SAM \332\251\331\204\333\214\330\257", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("Afc::PanelWidget", "   :SAM \331\205\330\255\331\204 \331\202\330\261\330\247\330\261\332\257\333\214\330\261\333\214", 0, QApplication::UnicodeUTF8));
        btnExitSamSettings->setText(QApplication::translate("Afc::PanelWidget", "\330\256\330\261\331\210\330\254", 0, QApplication::UnicodeUTF8));
        txtSamKey->setInputMask(QApplication::translate("Afc::PanelWidget", "HH-HH-HH-HH-HH-HH-HH-HH-HH-HH-HH-HH-HH-HH-HH-HH", 0, QApplication::UnicodeUTF8));
        txtSamKey->setText(QApplication::translate("Afc::PanelWidget", "---------------", 0, QApplication::UnicodeUTF8));
        lblSamModuleError->setText(QString());
        lblSamCityPayError->setText(QString());
        lblSamCityWayError->setText(QString());
        panelTab->setTabText(panelTab->indexOf(tabSamSettings), QApplication::translate("Afc::PanelWidget", "Page", 0, QApplication::UnicodeUTF8));
        btnExitFileOperations->setText(QApplication::translate("Afc::PanelWidget", "\330\256\330\261\331\210\330\254", 0, QApplication::UnicodeUTF8));
        btnTransactions->setText(QApplication::translate("Afc::PanelWidget", "\330\252\330\256\331\204\333\214\331\207 \330\252\330\261\330\247\332\251\331\206\330\264", 0, QApplication::UnicodeUTF8));
        btnParameters->setText(QApplication::translate("Afc::PanelWidget", "\330\250\330\247\330\261\332\257\330\260\330\247\330\261\333\214 \331\276\330\247\330\261\330\247\331\205\330\252\330\261\331\207\330\247", 0, QApplication::UnicodeUTF8));
        btnLogs->setText(QApplication::translate("Afc::PanelWidget", "\330\252\330\256\331\204\333\214\331\207 \331\204\330\247\332\257", 0, QApplication::UnicodeUTF8));
        panelTab->setTabText(panelTab->indexOf(tabFileOperation), QApplication::translate("Afc::PanelWidget", "Page", 0, QApplication::UnicodeUTF8));
        lblUsbParamVersion->setText(QApplication::translate("Afc::PanelWidget", "0", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("Afc::PanelWidget", ":USB \331\206\330\263\330\256\331\207 \331\276\330\247\330\261\330\247\331\205\330\252\330\261", 0, QApplication::UnicodeUTF8));
        btnExitParameters->setText(QApplication::translate("Afc::PanelWidget", "\330\256\330\261\331\210\330\254", 0, QApplication::UnicodeUTF8));
        lblUsbParamStatus->setText(QApplication::translate("Afc::PanelWidget", "USB \331\210\330\266\330\271\333\214\330\252 \330\247\330\252\330\265\330\247\331\204", 0, QApplication::UnicodeUTF8));
        btnLoadParameters->setText(QApplication::translate("Afc::PanelWidget", "\330\250\330\247\330\261\332\257\330\260\330\247\330\261\333\214 \331\276\330\247\330\261\330\247\331\205\330\252\330\261", 0, QApplication::UnicodeUTF8));
        lblCurrentParamVersion->setText(QApplication::translate("Afc::PanelWidget", "0", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("Afc::PanelWidget", "\331\206\330\263\330\256\331\207 \331\276\330\247\330\261\330\247\331\205\330\252\330\261 \331\201\330\271\331\204\333\214:", 0, QApplication::UnicodeUTF8));
        lblUsbParamStructure->setText(QApplication::translate("Afc::PanelWidget", "\330\263\330\247\330\256\330\252\330\247\330\261 \331\201\330\247\333\214\331\204", 0, QApplication::UnicodeUTF8));
        panelTab->setTabText(panelTab->indexOf(tabParameters), QApplication::translate("Afc::PanelWidget", "Page", 0, QApplication::UnicodeUTF8));
        lblUsbTransactionsCount->setText(QApplication::translate("Afc::PanelWidget", "0", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("Afc::PanelWidget", "\330\252\330\271\330\257\330\247\330\257 \331\201\330\247\333\214\331\204 \331\207\330\247\333\214 \330\252\330\261\330\247\332\251\331\206\330\264:", 0, QApplication::UnicodeUTF8));
        btnExitTransactions->setText(QApplication::translate("Afc::PanelWidget", "\330\256\330\261\331\210\330\254", 0, QApplication::UnicodeUTF8));
        btnSaveTransactions->setText(QApplication::translate("Afc::PanelWidget", "\330\252\330\256\331\204\333\214\331\207 \330\252\330\261\330\247\332\251\331\206\330\264", 0, QApplication::UnicodeUTF8));
        lblUsbTransactionStatus->setText(QApplication::translate("Afc::PanelWidget", "USB \331\210\330\266\330\271\333\214\330\252 \330\247\330\252\330\265\330\247\331\204", 0, QApplication::UnicodeUTF8));
        panelTab->setTabText(panelTab->indexOf(tabTransactions), QApplication::translate("Afc::PanelWidget", "Page", 0, QApplication::UnicodeUTF8));
        lblUsbLogsCount->setText(QApplication::translate("Afc::PanelWidget", "0", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("Afc::PanelWidget", "\330\252\330\271\330\257\330\247\330\257 \331\201\330\247\333\214\331\204 \331\207\330\247\333\214 \331\204\330\247\332\257:", 0, QApplication::UnicodeUTF8));
        lblUsbLogStatus->setText(QApplication::translate("Afc::PanelWidget", "USB \331\210\330\266\330\271\333\214\330\252 \330\247\330\252\330\265\330\247\331\204", 0, QApplication::UnicodeUTF8));
        btnExitLog->setText(QApplication::translate("Afc::PanelWidget", "\330\256\330\261\331\210\330\254", 0, QApplication::UnicodeUTF8));
        btnSaveLogs->setText(QApplication::translate("Afc::PanelWidget", "\330\252\330\256\331\204\333\214\331\207 \331\204\330\247\332\257", 0, QApplication::UnicodeUTF8));
        panelTab->setTabText(panelTab->indexOf(tabLogs), QApplication::translate("Afc::PanelWidget", "Page", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

} // namespace Afc

namespace Afc {
namespace Ui {
    class PanelWidget: public Ui_PanelWidget {};
} // namespace Ui
} // namespace Afc

#endif // UI_PANELWIDGET_H
