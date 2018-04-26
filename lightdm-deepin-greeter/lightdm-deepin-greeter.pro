#-------------------------------------------------
#
# Project created by QtCreator 2015-10-09T14:05:35
#
#-------------------------------------------------
include(../common.pri)
load(deepin_qt)

QT       += core gui dbus x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lightdm-deepin-greeter
TEMPLATE = app

PKGCONFIG += gsettings-qt liblightdm-qt5-3 x11 xext xfixes xcursor dtkwidget
CONFIG += c++14 link_pkgconfig

include(../widgets/widgets.pri)
include(../global_util/global_util.pri)
include(../boxframe/boxframe.pri)

HEADERS  += \
    view/logowidget.h \
    app/loginmanager.h \
    loginwindow.h

SOURCES += main.cpp \
    view/logowidget.cpp \
    app/loginmanager.cpp \
    loginwindow.cpp

contains(DEFINES,WITH_SHENWEI_PLATFORM){
    SHENWEI_PLATFORM = YES
    message("SHENWEI_PLATFORM defined")
} else {
    message("SHENWEI_PLATFORM not defined")
}

isEqual(SHENWEI_PLATFORM, YES) {
    DEFINES += SHENWEI_PLATFORM
}　else {
    DEFINES -= SHENWEI_PLATFORM
}

isEqual(USE_CURSOR_LOADING_ANI, YES) {
    DEFINES += USE_CURSOR_LOADING_ANI
}

target.path = $${PREFIX}/bin/
desktop_file.files = lightdm-deepin-greeter.desktop
desktop_file.path = $${PREFIX}/share/xgreeters/

INSTALLS += target desktop_file

RESOURCES += \
    image.qrc \
    logintheme.qrc

deepin_professional {
    host_mips64: {
        DEFINES += DISABLE_ANIMATIONS
        DEFINES += SHUTDOWN_NO_QUIT
        DEFINES += LOCK_NO_QUIT
        DEFINES += DISABLE_TEXT_SHADOW
        DEFINES += DISABLE_TEXT_SHADOW
        DEFINES += DISABLE_ANIMATIONS
        DEFINES += USE_CURSOR_LOADING_ANI
    }
}

