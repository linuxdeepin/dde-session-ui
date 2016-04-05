#-------------------------------------------------
#
# Project created by QtCreator 2016-03-11T10:40:07
#
#-------------------------------------------------

QT       += core gui dbus concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dde-wallpaper-chooser
TEMPLATE = app
CONFIG += c++11 link_pkgconfig
PKGCONFIG += gsettings-qt

SOURCES += main.cpp\
        frame.cpp \
    dbus/appearancedaemon_interface.cpp \
    wallpaperlist.cpp \
    wallpaperitem.cpp \
    thumbnailmanager.cpp \
    button.cpp \
    dbus/deepin_wm.cpp

HEADERS  += frame.h \
    dbus/appearancedaemon_interface.h \
    wallpaperlist.h \
    wallpaperitem.h \
    constants.h \
    thumbnailmanager.h \
    button.h \
    dbus/deepin_wm.h

RESOURCES += \
    images.qrc

target.path = /usr/lib/deepin-daemon/
INSTALLS += target
