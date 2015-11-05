#-------------------------------------------------
#
# Project created by QtCreator 2015-10-09T14:05:35
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lightdm-deepin-greeter
TEMPLATE = app

PKGCONFIG += gsettings-qt dui liblightdm-qt5-3
CONFIG += c++11 link_pkgconfig

HEADERS  += \
    view/logowidget.h \
    view/switchframe.h \
    app/loginmanager.h \

SOURCES += main.cpp \
    view/logowidget.cpp \
    view/switchframe.cpp \
    app/loginmanager.cpp \



include(../common.pri)
include(../background/background.pri)
include(../widgets/widgets.pri)
include(../global_util/global_util.pri)
include(../cutelogger/cutelogger.pri)

target.path = $${PREFIX}/bin/
desktop_file.files = lightdm-deepin-greeter.desktop
desktop_file.path = $${PREFIX}/share/xgreeters/

INSTALLS += target desktop_file

RESOURCES += \
    image.qrc \
    qss.qrc
