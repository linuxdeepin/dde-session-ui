#-------------------------------------------------
#
# Project created by QtCreator 2015-11-11T12:48:52
#
#-------------------------------------------------

QT       += core gui dbus dtkwidget

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dde-lowpower
TEMPLATE = app
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp \
        window.cpp

HEADERS  += window.h

RESOURCES += \
    images.qrc

target.path = /usr/lib/deepin-daemon/
INSTALLS += target
