#-------------------------------------------------
#
# Project created by QtCreator 2017-05-10T09:51:56
#
#-------------------------------------------------

include(../common.pri)

QT       += core gui widgets dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dde-warning-dialog
TEMPLATE = app

CONFIG += c++11 link_pkgconfig
PKGCONFIG += dtkbase dtkwidget dtkutil

SOURCES += main.cpp \
    warningdialog.cpp

HEADERS  += \
    warningdialog.h

target.path = /usr/lib/deepin-daemon/
INSTALLS   += target
