#-------------------------------------------------
#
# Project created by QtCreator 2016-11-24T15:16:05
#
#-------------------------------------------------

QT       += widgets
QMAKE_CXXFLAGS += -std=c++11

TARGET = QOnScreenKeyboard
TEMPLATE = lib
CONFIG += staticlib


DEFINES += QONSCREENKEYBOARD_LIBRARY

SOURCES += qonscreenkeyboard.cpp \
    numpadkeyboard.cpp \
    fullkeyboard.cpp

HEADERS += qonscreenkeyboard.h\
        numpadkeyboard.h \
        IKeyboard.h \
        fullkeyboard.h

FORMS += \
    numpadkeyboard.ui \
    fullkeyboard.ui
