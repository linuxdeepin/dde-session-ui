# SPDX-FileCopyrightText: 2023 - 2026 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(Common_SRCS
    accessibilitycheckerex.cpp
    accessibilitycheckerex.h
)

add_library(session-ui-common-shared OBJECT
    ${Common_SRCS}
)

target_include_directories(session-ui-common-shared
PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(session-ui-common-shared
PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Dtk${DTK_VERSION_MAJOR}::Widget
)
