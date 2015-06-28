#-------------------------------------------------
#
# Project created by QtCreator 2015-06-02T17:28:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = candycrush
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    menu.cpp

HEADERS  += \
    mainwindow.h \
    tle.h \
    sle.h \
    menu.h

FORMS    += \
    mainwindow.ui \
    menu.ui

RESOURCES += \
    res.qrc
