isEmpty(PREFIX){
    PREFIX = /usr
}
QT += svg

HEADERS += \
    $$PWD/bubble.h \
    $$PWD/bubblemanager.h \
    $$PWD/notifications_dbus_adaptor.h \
    $$PWD/dbus_daemon_interface.h \
    $$PWD/notificationentity.h \
    $$PWD/dbuslogin1manager.h \
    $$PWD/actionbutton.h \
    $$PWD/appicon.h\
    $$PWD/dbusdock_interface.h \
    $$PWD/persistence.h \
    $$PWD/appbody.h \
    $$PWD/icondata.h \
    $$PWD/button.h \
    $$PWD/iconbutton.h \
    $$PWD/appbodylabel.h \
    $$PWD/constants.h \
    $$PWD/bubbletool.h \
    $$PWD/dbusdisplay.h \
    $$PWD/dbusdock.h \
    $$PWD/dockrect.h \
    $$PWD/notifyplugininterface.h

SOURCES += \
    $$PWD/bubble.cpp \
    $$PWD/bubblemanager.cpp \
    $$PWD/notifications_dbus_adaptor.cpp \
    $$PWD/dbus_daemon_interface.cpp \
    $$PWD/notificationentity.cpp \
    $$PWD/dbuslogin1manager.cpp \
    $$PWD/actionbutton.cpp \
    $$PWD/appicon.cpp\
    $$PWD/dbusdock_interface.cpp \
    $$PWD/persistence.cpp \
    $$PWD/appbody.cpp \
    $$PWD/icondata.cpp \
    $$PWD/button.cpp \
    $$PWD/iconbutton.cpp \
    $$PWD/appbodylabel.cpp \
    $$PWD/bubbletool.cpp \
    $$PWD/dbusdisplay.cpp \
    $$PWD/dbusdock.cpp \
    $$PWD/dockrect.cpp \
    $$PWD/notifyplugininterface.cpp

orgDBus.input = $$PWD/files/com.deepin.dde.freedesktop.Notification.service.in
orgDBus.output = $$OUT_PWD/files/com.deepin.dde.freedesktop.Notification.service

ddeDBus.input = $$PWD/files/com.deepin.dde.Notification.service.in
ddeDBus.output = $$OUT_PWD/files/com.deepin.dde.Notification.service

QMAKE_SUBSTITUTES += orgDBus ddeDBus
QMAKE_CLEAN       += $${orgDBus.output} $${ddeDBus.output}

dbus.path = $${PREFIX}/share/dbus-1/services/
dbus.files += $$OUT_PWD/files/com.deepin.dde.freedesktop.Notification.service
dbus.files += $$OUT_PWD/files/com.deepin.dde.Notification.service

INSTALLS += dbus
