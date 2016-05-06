#-------------------------------------------------
#
# Project created by QtCreator 2015-11-03T10:28:12
#
#-------------------------------------------------

QT       += core gui dbus x11extras svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lX11 -lXext
TARGET = dde-osd
TEMPLATE = app
CONFIG += c++11 link_pkgconfig
PKGCONFIG += gtk+-2.0


SOURCES += main.cpp\
        osd.cpp \
    dbus/volumedbus.cpp \
    dbus/layoutdbus.cpp \
    dbus/displaydbus.cpp \
    switchlayout.cpp \
    switchmonitor.cpp \
    switchnormal.cpp

HEADERS  += osd.h \
    dbus/volumedbus.h \
    dbus/layoutdbus.h \
    dbus/displaydbus.h \
    switchlayout.h \
    switchmonitor.h \
    switchnormal.h

RESOURCES += \
    image.qrc

target.path = /usr/lib/deepin-daemon/
INSTALLS   += target

service_file.path = /usr/share/dbus-1/services/
service_file.files = com.deepin.dde.osd.service
INSTALLS += service_file
