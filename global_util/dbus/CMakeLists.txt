# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(DBUS_SOURCE)

set_source_files_properties(
    ${CMAKE_CURRENT_LIST_DIR}/../../xml/org.desktopspec.ObjectManager1.xml
    PROPERTIES
    INCLUDE
       ${CMAKE_CURRENT_LIST_DIR}/types/objectmanager_type.h
    CLASSNAME
        ApplicationObjectManager1
)

qt_add_dbus_interface(
    DBUS_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/../../xml/org.desktopspec.ObjectManager1.xml
    objectmanager_interface
)


dtk_add_dbus_interface(
    DBUS_SOURCE
    ../../xml/org.deepin.dde.Audio1.xml
    org_deepin_dde_audio1
)

dtk_add_dbus_interface(
    DBUS_SOURCE
    ../../xml/org.deepin.dde.Audio1.Sink.xml
    org_deepin_dde_audio1_sink
)

dtk_add_dbus_interface(
    DBUS_SOURCE
    ../../xml/org.deepin.dde.Accounts1.xml
    org_deepin_dde_accounts1
)

dtk_add_dbus_interface(
    DBUS_SOURCE
    ../../xml/org.deepin.dde.Accounts1.User.xml
    org_deepin_dde_accounts1_user
)

dtk_add_dbus_interface(
    DBUS_SOURCE
    ../../xml/com.deepin.chromeExtension.TabsLimit.xml
    com_deepin_chromeextension_tabslimit
)

dtk_add_dbus_interface(
    DBUS_SOURCE
    ../../xml/org.deepin.dde.Display1.xml
    org_deepin_dde_display1
)

dtk_add_dbus_interface(
    DBUS_SOURCE
    ../../xml/org.deepin.dde.Display1.Monitor.xml
    org_deepin_dde_display1_monitor
)

dtk_add_dbus_interface(
    DBUS_SOURCE
    ../../xml/org.deepin.dde.Appearance1.xml
    org_deepin_dde_appearance1
)

dtk_add_dbus_interface(
    DBUS_SOURCE
    ../../xml/org.deepin.dde.Bluetooth1.xml
    org_deepin_dde_bluetooth1
)

dtk_add_dbus_interface(
    DBUS_SOURCE
    ../../xml/org.deepin.dde.daemon.Dock1.xml
    org_deepin_dde_daemon_dock1
)

dtk_add_dbus_interface(
    DBUS_SOURCE
    ../../xml/org.deepin.dde.Gesture1.xml
    org_deepin_dde_gesture1
)

dtk_add_dbus_interface(
    DBUS_SOURCE
    ../../xml/org.deepin.dde.ImageBlur1.xml
    org_deepin_dde_imageblur1
)

dtk_add_dbus_interface(
    DBUS_SOURCE
    ../../xml/org.deepin.dde.InputDevice1.Keyboard.xml
    org_deepin_dde_inputdevice1_keyboard
)

dtk_add_dbus_interface(
    DBUS_SOURCE
    ../../xml/org.deepin.dde.SessionManager1.xml
    org_deepin_dde_sessionmanager1
)

dtk_add_dbus_interface(
    DBUS_SOURCE
    ../../xml/org.deepin.dde.SoundEffect1.xml
    org_deepin_dde_soundeffect1
)

file(GLOB_RECURSE TYPES_SRCS
    "types/*.h"
    "types/*.cpp"
)

set(SESSION_UI_DBUS_INTERFACE_SRCS
    ${DBUS_SOURCE}
    ${TYPES_SRCS}
)

add_library(session-ui-dbus-shared OBJECT
    ${SESSION_UI_DBUS_INTERFACE_SRCS}
)

target_include_directories(session-ui-dbus-shared
PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(session-ui-dbus-shared
PRIVATE
    Qt${QT_VERSION_MAJOR}::DBus
    Dtk${DTK_VERSION_MAJOR}::Core
)
