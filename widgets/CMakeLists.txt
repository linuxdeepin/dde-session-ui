file(GLOB_RECURSE Widgets_SRCS
    "*.h"
    "*.cpp"
)

add_library(session-ui-widgets-shared OBJECT
    ${Widgets_SRCS}
)

target_include_directories(session-ui-widgets-shared
PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(session-ui-widgets-shared
PRIVATE
    Qt5::Widgets
)
