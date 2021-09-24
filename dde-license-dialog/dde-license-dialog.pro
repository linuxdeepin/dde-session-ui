#-------------------------------------------------
#
# Project created by QtCreator 2019-07-02T13:43:16
#
#-------------------------------------------------

QT       += core gui dtkwidget concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dde-license-dialog
TEMPLATE = app

CONFIG += link_pkgconfig
CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        content.cpp \
        main.cpp \
        mainwindow.cpp \
    ../common/accessibilitycheckerex.cpp

HEADERS += \
        content.h \
        mainwindow.h \
    ../common/accessibilitycheckerex.h

isEmpty(PREFIX){
    PREFIX = /usr
}

target.path = $${PREFIX}/bin/
INSTALLS += target
