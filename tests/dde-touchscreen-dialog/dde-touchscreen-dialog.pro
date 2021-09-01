QT       += widgets core gui dbus gui-private x11extras

TARGET = ut_dde-touchscreen-dialog
TEMPLATE = app

include(../testcase.prf)

PKGCONFIG += dtkwidget dtkgui gio-qt dframeworkdbus gsettings-qt
CONFIG += c++11 link_pkgconfig
CONFIG -= app_bundle

LIBS += -lgtest -lX11 -lXext

DEFINES += QT_DEPRECATED_WARNINGS

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../../dde-touchscreen-dialog/src.pri)

SOURCES += \
    main.cpp \
    ut_monitorindicator.cpp \
    ut_recognizedialog.cpp \
    ut_touchscreensetting.cpp

INCLUDEPATH += ../../dde-touchscreen-dialog

RESOURCES +=
