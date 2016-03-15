#-------------------------------------------------
#
# Project created by QtCreator 2016-03-11T10:40:07
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dde-wallpaper-chooser
TEMPLATE = app


SOURCES += main.cpp\
        frame.cpp \
    dbus/appearancedaemon_interface.cpp \
    wallpaperlist.cpp \
    wallpaperitem.cpp \
    thumbnailmanager.cpp

HEADERS  += frame.h \
    dbus/appearancedaemon_interface.h \
    wallpaperlist.h \
    wallpaperitem.h \
    constants.h \
    thumbnailmanager.h
