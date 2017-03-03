#-------------------------------------------------
#
# Project created by QtCreator 2015-10-09T14:27:44
#
#-------------------------------------------------
INCLUDEPATH += $$PWD
QT += x11extras

PKGCONFIG   += liblightdm-qt5-3 dtkbase dtkwidget x11 xi

HEADERS += \
    $$PWD/useravatar.h \
    $$PWD/userbutton.h \
    $$PWD/passwdedit.h \
    $$PWD/sessionwidget.h \
    $$PWD/userwidget.h \
    $$PWD/userbreathinglabel.h \
    $$PWD/rounditembutton.h \
    $$PWD/kblayoutwidget.h \
    $$PWD/shutdownwidget.h \
    $$PWD/darrowrectangle.h \
    $$PWD/errortooltip.h \
    $$PWD/keyboardmonitor.h



SOURCES += \
    $$PWD/useravatar.cpp \
    $$PWD/userbutton.cpp \
    $$PWD/passwdedit.cpp \
    $$PWD/sessionwidget.cpp \
    $$PWD/userwidget.cpp \
    $$PWD/userbreathinglabel.cpp \
    $$PWD/rounditembutton.cpp \
    $$PWD/kblayoutwidget.cpp \
    $$PWD/shutdownwidget.cpp \
    $$PWD/darrowrectangle.cpp \
    $$PWD/errortooltip.cpp \
    $$PWD/keyboardmonitor.cpp

RESOURCES += \
    $$PWD/widgetstheme.qrc \
    $$PWD/widgetsimages.qrc

DISTFILES +=

