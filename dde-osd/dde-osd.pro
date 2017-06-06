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
PKGCONFIG += gtk+-2.0 dframeworkdbus dtkwidget dtkbase

SOURCES += main.cpp \
    container.cpp \
    manager.cpp \
    listview.cpp \
    delegate.cpp \
    model.cpp \
    osdprovider.cpp \
    audioprovider.cpp \
    common.cpp \
    brightnessprovider.cpp \
    kblayoutprovider.cpp \
    displaymodeprovider.cpp \
    indicatorprovider.cpp \
    wmstateprovider.cpp \
    kblayoutindicator.cpp

HEADERS  += container.h \
    manager.h \
    listview.h \
    delegate.h \
    model.h \
    common.h \
    osdprovider.h \
    audioprovider.h \
    brightnessprovider.h \
    kblayoutprovider.h \
    displaymodeprovider.h \
    indicatorprovider.h \
    wmstateprovider.h \
    kblayoutindicator.h

RESOURCES += \
    image.qrc \
    theme.qrc

target.path = /usr/lib/deepin-daemon/
INSTALLS   += target

service_file.path = /usr/share/dbus-1/services/
service_file.files = com.deepin.dde.osd.service
INSTALLS += service_file

autostart_file.path = /etc/xdg/autostart/
autostart_file.files = dde-osd_autostart.desktop
INSTALLS += autostart_file
