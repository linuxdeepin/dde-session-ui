
QT       += core gui dbus svg widgets

LIBS += -lX11 -lXext -lXfixes
TARGET = dde-welcome
TEMPLATE = app
CONFIG += c++11 link_pkgconfig
PKGCONFIG += dtkwidget dtkbase

SOURCES += \
    main.cpp \
    welcome.cpp \
    welcomeservice.cpp

HEADERS  += \
    welcome.h \
    welcomeservice.h

RESOURCES += \
    resources.qrc

target.path = /usr/lib/deepin-daemon/
INSTALLS   += target

service_file.path = /usr/share/dbus-1/services/
service_file.files = com.deepin.dde.welcome.service
INSTALLS += service_file
