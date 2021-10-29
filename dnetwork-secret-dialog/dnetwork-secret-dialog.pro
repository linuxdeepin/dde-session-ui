QT       += core gui widgets dtkwidget

TARGET = dnetwork-secret-dialog
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11 link_pkgconfig

SOURCES += \
        main.cpp \
    networksecretdialog.cpp \
    ../common/accessibilitycheckerex.cpp \
    networkdialog.cpp \

HEADERS += \
    networksecretdialog.h \
    ../common/accessibilitycheckerex.h \
    networkdialog.h \

RESOURCES += \
    resources.qrc

target.path = /usr/lib/deepin-daemon/
INSTALLS   += target
