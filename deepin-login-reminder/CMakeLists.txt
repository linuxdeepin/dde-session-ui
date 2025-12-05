# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(BIN_NAME deepin-login-reminder)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network DBus REQUIRED)
find_package(Dtk${DTK_VERSION_MAJOR} COMPONENTS Core REQUIRED)

find_package(PkgConfig REQUIRED)

message(${PROJECT_SOURCE_DIR})

add_executable(${BIN_NAME}
    main.cpp
    loginreminderinfo.h
    loginreminderinfo.cpp
)

target_link_libraries(${BIN_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::DBus
    Qt${QT_VERSION_MAJOR}::Network
    Dtk${DTK_VERSION_MAJOR}::Core
)

target_compile_definitions(${BIN_NAME} PRIVATE
    CMAKE_INSTALL_FULL_DATADIR="${CMAKE_INSTALL_FULL_DATADIR}"
)

install(TARGETS ${BIN_NAME} DESTINATION bin)

# Install desktop file to autostart directory
install(FILES deepin-login-reminder.desktop DESTINATION /etc/xdg/autostart/)

# Install dconf configuration files
dtk_add_config_meta_files(
    APPID org.deepin.login-reminder
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/dconf/org.deepin.login-reminder.json
)
