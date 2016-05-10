#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T10:27:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = session-ui-guardien
TEMPLATE = app
CONFIG += c++11

DISTFILES += session-ui-guardien.desktop

SOURCES += main.cpp \
    guardien.cpp

HEADERS += \
    guardien.h

target.path = /usr/bin

desktop.files = session-ui-guardien.desktop
desktop.path = /etc/xdg/autostart/

INSTALLS += target desktop
