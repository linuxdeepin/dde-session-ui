include(../libdde-auth/libdde-auth.pri)

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/authinterface.h \
    $$PWD/sessionbasewindow.h \
    $$PWD/userinputwidget.h \
    $$PWD/sessionbasemodel.h \
    $$PWD/userinfo.h \
    $$PWD/userframe.h \
    $$PWD/lockcontent.h \
    $$PWD/framedatabind.h \
    $$PWD/lockpasswordwidget.h

SOURCES += \
    $$PWD/authinterface.cpp \
    $$PWD/sessionbasewindow.cpp \
    $$PWD/userinputwidget.cpp \
    $$PWD/sessionbasemodel.cpp \
    $$PWD/userinfo.cpp \
    $$PWD/userframe.cpp \
    $$PWD/lockcontent.cpp \
    $$PWD/framedatabind.cpp \
    $$PWD/lockpasswordwidget.cpp
