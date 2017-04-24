#-------------------------------------------------
#
# Project created by QtCreator 2015-10-21T07:07:40
#
#-------------------------------------------------

QT       += core gui dbus x11extras multimediawidgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dde-zone
TEMPLATE = app
CONFIG += link_pkgconfig c++11
PKGCONFIG += dtkwidget dtkbase dtkutil
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

target.path = /usr/lib/deepin-daemon/
INSTALLS += target

isEqual(DISABLE_DEMO_VIDEO, YES){
    DEFINES += DISABLE_DEMO_VIDEO
}
