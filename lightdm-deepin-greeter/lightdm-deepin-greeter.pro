#-------------------------------------------------
#
# Project created by QtCreator 2015-10-09T14:05:35
#
#-------------------------------------------------
include(../common.pri)

QT       += core gui dbus x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lightdm-deepin-greeter
TEMPLATE = app

PKGCONFIG += gsettings-qt dui liblightdm-qt5-3 x11 xext xfixes xcursor
CONFIG += c++11 link_pkgconfig

include(../background/background.pri)
include(../widgets/widgets.pri)
include(../global_util/global_util.pri)
include(../cutelogger/cutelogger.pri)
include(../boxframe/boxframe.pri)

HEADERS  += \
    view/logowidget.h \
    view/switchframe.h \
    app/loginmanager.h \
    app/loginframe.h

SOURCES += main.cpp \
    view/logowidget.cpp \
    view/switchframe.cpp \
    app/loginmanager.cpp \
    app/loginframe.cpp \

target.path = $${PREFIX}/bin/
desktop_file.files = lightdm-deepin-greeter.desktop
desktop_file.path = $${PREFIX}/share/xgreeters/

INSTALLS += target desktop_file

RESOURCES += \
    image.qrc \
    logintheme.qrc
