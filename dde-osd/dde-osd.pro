#-------------------------------------------------
#
# Project created by QtCreator 2015-11-03T10:28:12
#
#-------------------------------------------------

QT       += core gui dbus x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lX11 -lXext
TARGET = dde-osd
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        osd.cpp \
    dbus/volumedbus.cpp \
    dbus/layoutdbus.cpp \
    dbus/displaydbus.cpp \

HEADERS  += osd.h \
    dbus/volumedbus.h \
    dbus/layoutdbus.h \
    dbus/displaydbus.h \

RESOURCES += \
    image.qrc
