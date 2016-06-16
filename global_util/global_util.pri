INCLUDEPATH += $$PWD

QT  += xml

HEADERS += \
    $$PWD/util_updateui.h \
    $$PWD/util_signalmanager.h \
    $$PWD/constants.h \
    $$PWD/util_settings.h \
    $$PWD/xkbparser.h \
    $$PWD/dbus/dbusdisplaymanager.h \
    $$PWD/dbus/dbuslogin1manager.h \
    $$PWD/dbus/dbusvariant.h \
    $$PWD/dbus/dbussessionmanager.h \
    $$PWD/dbus/displayinterface.h \
    $$PWD/dbus/dbushotzone.h \
    $$PWD/dbus/dbuslockservice.h \
    $$PWD/util_file.h \
    $$PWD/accountsutils.h


SOURCES += \
    $$PWD/util_updateui.cpp \
    $$PWD/util_signalmanager.cpp \
    $$PWD/util_settings.cpp \
    $$PWD/xkbparser.cpp \
    $$PWD/dbus/dbusdisplaymanager.cpp \
    $$PWD/dbus/dbuslogin1manager.cpp \
    $$PWD/dbus/dbusvariant.cpp \
    $$PWD/dbus/dbussessionmanager.cpp \
    $$PWD/dbus/displayinterface.cpp \
    $$PWD/dbus/dbushotzone.cpp \
    $$PWD/dbus/dbuslockservice.cpp \
    $$PWD/util_file.cpp \
    $$PWD/accountsutils.cpp


RESOURCES += \
    $$PWD/commonimage.qrc

DISTFILES += \
    $$PWD/dbus/com.deepin.daemon.Zone.xml

