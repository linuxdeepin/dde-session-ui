include(../common.pri)
QT       += core gui dbus svg widgets

LIBS += -lX11 -lXext -lXfixes
TARGET = dde-welcome
TEMPLATE = app
CONFIG += c++11 link_pkgconfig
PKGCONFIG += dtkwidget dframeworkdbus gsettings-qt

include(../widgets/widgets.pri)
include(../global_util/global_util.pri)

SOURCES += \
    main.cpp \
    welcomeservice.cpp \
    updatecontent.cpp \
    mainwidget.cpp

HEADERS  += \
    welcomeservice.h \
    updatecontent.h \
    version.h \
    mainwidget.h

RESOURCES += \
    resources.qrc

target.path = /usr/lib/deepin-daemon/
INSTALLS   += target

service_file.path = /usr/share/dbus-1/services/
service_file.files = com.deepin.dde.welcome.service
INSTALLS += service_file
