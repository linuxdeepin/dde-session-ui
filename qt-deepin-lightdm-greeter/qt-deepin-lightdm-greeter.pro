#-------------------------------------------------
#
# Project created by QtCreator 2015-10-09T14:05:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-deepin-lightdm-greeter
TEMPLATE = app

PKGCONFIG += gsettings-qt dui liblightdm-qt5-3
CONFIG += c++11 link_pkgconfig

SOURCES += main.cpp\
    app/sessionlogin.cpp \
    view/logowidget.cpp \
    view/switchframe.cpp

HEADERS  += \
    app/sessionlogin.h \
    view/switchframe.h \
    view/logowidget.h

include(../background/background.pri)
include(../widgets/widgets.pri)
include(../global_util/global_util.pri)
include(../cutelogger/cutelogger.pri)

target.path = $${PREFIX}/bin/
INSTALLS += target

RESOURCES += \
    image.qrc \
    qss.qrc
