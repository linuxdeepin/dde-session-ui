#-------------------------------------------------
#
# Project created by QtCreator 2017-05-25T16:56:48
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dde-wm-chooser
TEMPLATE = app

CONFIG += c++11 link_pkgconfig
PKGCONFIG += dtkbase dtkwidget dtkutil

include(../boxframe/boxframe.pri)

SOURCES += main.cpp\
    wmchooser.cpp \
    wmframe.cpp

HEADERS  += \
    wmchooser.h \
    wmframe.h

target.path = /usr/bin/
INSTALLS   += target
