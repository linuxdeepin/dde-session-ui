# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(OSD_Name dde-osd)

add_executable(${OSD_Name}
    ${OSD_SRCS}
    main.cpp
    )

target_include_directories(${OSD_Name} PUBLIC
    ${OSD_Includes}
    )

target_link_libraries(${OSD_Name} PRIVATE
    session-ui-dbus-shared
    ${OSD_Libraries}
    ${Test_Libraries}
    )

## bin
install(TARGETS ${OSD_Name} DESTINATION lib/deepin-daemon)

## service
file(GLOB OSD_SERVICE_FILES "files/*.service")
install(FILES ${OSD_SERVICE_FILES} DESTINATION share/dbus-1/services)
