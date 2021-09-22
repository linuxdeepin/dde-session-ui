#-------------------------------------------------
#
# Project created by QtCreator 2021-09-10T09:51:56
#
#-------------------------------------------------

include(../common.pri)

QT       += core gui widgets dtkwidget

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dde-hints-dialog
TEMPLATE = app

CONFIG += c++11 link_pkgconfig

SOURCES += main.cpp \
    hintsdialog.cpp \
    horizontalseperator.cpp

HEADERS  += \
    hintsdialog.h \
    horizontalseperator.h

target.path = $${PREFIX}/bin/
INSTALLS   += target
