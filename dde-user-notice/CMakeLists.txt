# SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(UserNotice_Name dde-user-notice)

file(GLOB_RECURSE UserNotice_SRCS
    "src/*.h"
    "src/*.cpp"
)

find_package(Dtk${DTK_VERSION_MAJOR}DConfig REQUIRED)

add_executable(${UserNotice_Name} ${UserNotice_SRCS})

target_include_directories(${UserNotice_Name} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${UserNotice_Name} PRIVATE
    Dtk${DTK_VERSION_MAJOR}::Widget
    Dtk${DTK_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::DBus
    Qt${QT_VERSION_MAJOR}::Concurrent
)

## bin
install(TARGETS ${UserNotice_Name} DESTINATION ${CMAKE_INSTALL_BINDIR})

## 需要在 greeter 之前启动的配置和脚本
install(FILES misc/109_user_notice_conf.json
    DESTINATION ${CMAKE_INSTALL_DATADIR}/dde-session-shell/greeters.d/launch.conf.d)
install(PROGRAMS
    misc/user_notice_check.sh
    misc/dde_user_notice_wrapper
    DESTINATION ${CMAKE_INSTALL_BINDIR})
