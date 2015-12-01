#-------------------------------------------------
#
# Project created by QtCreator 2015-11-11T12:48:52
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dde-lowpower
TEMPLATE = app


SOURCES += main.cpp \
        window.cpp

HEADERS  += window.h

RESOURCES += \
    images.qrc

target.path = /usr/lib/deepin-daemon/
INSTALLS += target
