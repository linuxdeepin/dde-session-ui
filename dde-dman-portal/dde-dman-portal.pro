include(../common.pri)

QT += core dbus
QT -= gui

TEMPLATE = app

CONFIG += c++11 link_pkgconfig
PKGCONFIG += dtkcore

TARGET = dde-dman-portal

HEADERS += \
    dmanproxy.h

SOURCES += main.cpp \
    dmanproxy.cpp

service.path = $${PREFIX}/share/dbus-1/services
service.files = $$PWD/com.deepin.dman.service

binary.path =  $${PREFIX}/lib/deepin-daemon
binary.files = $${OUT_PWD}/$${TARGET}

INSTALLS += binary service

