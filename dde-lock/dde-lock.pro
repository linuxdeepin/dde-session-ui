include(../common.pri)

QT       += core gui widgets

TARGET = dde-lock
TEMPLATE = app

CONFIG += c++11 link_pkgconfig dbus
PKGCONFIG += gsettings-qt

include(../background/background.pri)
include(../widgets/widgets.pri)
include(../global_util/global_util.pri)

SOURCES += main.cpp \
    lockframe.cpp \
    timewidget.cpp

HEADERS  += \ 
    lockframe.h \
    timewidget.h

target.path = $${PREFIX}/bin/
INSTALLS += target

RESOURCES += \
    qss.qrc
