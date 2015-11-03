#-------------------------------------------------
#
# Project created by QtCreator 2015-10-21T07:07:40
#
#-------------------------------------------------

QT       += core gui dbus x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dde-zone
TEMPLATE = app
CONFIG += link_pkgconfig c++11
LIBS += -lX11


SOURCES += main.cpp \
    hotzone.cpp \
    hoverwidget.cpp \
    dbus/dbuszone.cpp \
    pushbuttonlist.cpp \
    mainwindow.cpp

HEADERS  += \
    hotzone.h \
    hoverwidget.h \
    dbus/dbuszone.h \
    pushbuttonlist.h \
    mainwindow.h

RESOURCES += \
    image.qrc
