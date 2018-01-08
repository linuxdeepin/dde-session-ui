QT       += core gui widgets dbus x11extras

TARGET = dmemory-warning-dialog
TEMPLATE = app

CONFIG += c++14 link_pkgconfig
PKGCONFIG += dtkwidget

HEADERS += \
    src/buttondelegate.h \
    src/dmemorywarningdialog.h \
    src/processinfodelegate.h \
    src/processinfomanager.h \
    src/processinfomodel.h \
    src/processinfotable.h

SOURCES += \
    src/buttondelegate.cpp \
    src/dmemorywarningdialog.cpp \
    src/main.cpp \
    src/processinfodelegate.cpp \
    src/processinfomanager.cpp \
    src/processinfomodel.cpp \
    src/processinfotable.cpp

target.path = /usr/bin
INSTALLS   += target
