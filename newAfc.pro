QT       += core gui sql network
QMAKE_CXXFLAGS += -std=c++11

#DEFINES  += VERBOSE_LOG
#DEFINES  += VERBOSE_LOG_GATE

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = elin
TEMPLATE = app

INCLUDEPATH += $$PWD/QtSerialPort
INCLUDEPATH += $$PWD/crypto
INCLUDEPATH += $$PWD/QOnScreenKeyboard

LIBS += $$PWD/crypto/libcrypto.a
LIBS += $$PWD/QOnScreenKeyboard/libQOnScreenKeyboard.a

SOURCES += main.cpp \
    QtSerialPort/qserialport.cpp \
    QtSerialPort/qserialportinfo.cpp \
    QtSerialPort/qserialportinfo_unix.cpp \
    QtSerialPort/qserialport_unix.cpp \
    QtSerialPort/qttylocker_unix.cpp \
    DualiCardReader.cpp \
    MifareCard.cpp \
    MainWindow.cpp \
    MemoryBlock.cpp \
    DesfireCard.cpp \
    QrCode.cpp \
    HoneywellBarcodeReader.cpp \
    CityPayCard.cpp \
    Cryptography.cpp \
    CityPaySam.cpp \
    Utilities.cpp \
    CityWayCard.cpp \
    CityPayTransactionLog.cpp \
    CityPaySettlementManager.cpp \
    Scu.cpp \
    ActivityManager.cpp \
    ParameterManager.cpp \
    MetroBusiness.cpp \
    GateController.cpp \
    GpioSunXi.cpp \
    BuzzerController.cpp \
    PanelWidget.cpp \
    QFocusableLineEdit.cpp \
    QrTicket.cpp \
    QrDbManager.cpp \
    QrServerManager.cpp \
    QrKeyStore.cpp

HEADERS  +=\
    QtSerialPort/qserialport.h \
    QtSerialPort/qserialportglobal.h \
    QtSerialPort/qserialportinfo.h \
    QtSerialPort/qserialportinfo_p.h \
    QtSerialPort/qserialport_p.h \
    QtSerialPort/qserialport_unix_p.h \
    QtSerialPort/qttylocker_unix_p.h \
    CardReader.h \
    Result.h \
    DualiCardReader.h \
    MifareCard.h \
    CreditCard.h \
    CreditCardType.h \
    Business.h \
    MainWindow.h \
    MemoryBlock.h \
    Settings.h \
    DesfireCard.h \
    WindowManager.h \
    Device.h \
    BarcodeReader.h \
    Barcode.h \
    BarcodeType.h \
    QrCode.h \
    HoneywellBarcodeReader.h \
    CityPayCard.h \
    Cryptography.h \
    CityPaySam.h \
    Utilities.h \
    CityWayCard.h \
    TransactionLog.h \
    CityPayTransactionLog.h \
    SettlementManager.h \
    CityPaySettlementManager.h \
    Scu.h \
    ActivityManager.h \
    ParameterManager.h \
    MetroBusiness.h \
    GateController.h \
    GpioSunXi.h \
    BuzzerController.h \
    PanelWidget.h \
    QFocusableLineEdit.h \
    QrDbManager.h \
    QrTicket.h \
    QrServerManager.h \
    QrKeyStore.h

FORMS    += \
    PanelWidget.ui \
    MainWindow.ui

RESOURCES += \
    Resources/res.qrc

target.path = /root
INSTALLS += target
