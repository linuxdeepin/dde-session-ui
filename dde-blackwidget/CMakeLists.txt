# SPDX-FileCopyrightText: 2025 - 2026 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(Blackwidget_Name dde-blackwidget)
file(GLOB_RECURSE Blackwidget_SRCS
    "src/*.h"
    "src/*.cpp"
)

pkg_check_modules(X11 REQUIRED x11)
find_package(Dtk${DTK_VERSION_MAJOR}DConfig REQUIRED)

add_executable(${Blackwidget_Name}
    ${Blackwidget_SRCS}
)
set(Blackwidget_Includes
    ${GLIB_INCLUDE_DIRS}
    ${GIO_INCLUDE_DIRS}
)
set(Blackwidget_Libraries
    ${GLIB_LIBRARIES}
    ${GIO_LIBRARIES}
    ${X11_LIBRARIES}
    Dtk${DTK_VERSION_MAJOR}::Widget
    Dtk${DTK_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::DBus
)

target_include_directories(${Blackwidget_Name} PUBLIC
    ${Blackwidget_Includes}
)

target_link_libraries(${Blackwidget_Name} PRIVATE
    ${Blackwidget_Libraries}
)

## bin
install(TARGETS ${Blackwidget_Name} DESTINATION lib/deepin-daemon)
## service
install(FILES misc/dbus-services/org.deepin.dde.BlackScreen1.service DESTINATION share/dbus-1/services)
install(FILES misc/systemd/user/dde-blackwidget.service DESTINATION ${SYSTEMD_USER_UNIT_DIR})
install(FILES misc/systemd/user/dde-blackwidget-nodbus.service DESTINATION ${SYSTEMD_USER_UNIT_DIR})
## dconfig
dtk_add_config_meta_files(APPID org.deepin.dde.session.ui FILES ${CMAKE_SOURCE_DIR}/configs/org.deepin.dde.session.ui.json)
