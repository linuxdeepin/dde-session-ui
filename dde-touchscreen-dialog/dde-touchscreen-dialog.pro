#-------------------------------------------------
#
# Project created by QtCreator 2020-03-18T13:00:44
#
#-------------------------------------------------

QT       += core gui widgets dtkwidget x11extras

TARGET = dde-touchscreen-dialog
TEMPLATE = app
CONFIG += link_pkgconfig c++11
PKGCONFIG += dframeworkdbus

LIBS += -lX11 -lXext

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    main.cpp \
    touchscreensetting.cpp \
    recognizedialog.cpp \
    monitorindicator.cpp \
    ../common/accessibilitycheckerex.cpp

HEADERS += \
    touchscreensetting.h \
    recognizedialog.h \
    monitorindicator.h \
    ../common/accessibilitycheckerex.h

target.path = /usr/lib/deepin-daemon/
INSTALLS   += target
