# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(Welcome_Name dde-welcome)

add_executable(${Welcome_Name}
    ${Widgets_SRCS}
    ${Global_Util_SRCS}
    main.cpp
)

target_link_libraries(${Welcome_Name}
PRIVATE
    dde-welcome-shared
)

## bin
install(TARGETS ${Welcome_Name} DESTINATION lib/deepin-daemon)

## service
install(FILES org.deepin.dde.Welcome1.service DESTINATION share/dbus-1/services)
