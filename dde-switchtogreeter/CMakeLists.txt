file(GLOB_RECURSE Switchtogreeter_SRCS
    "*.h"
    "*.c"
    )

set(Switchtogreeter_Includes
    ${SYSTEMD_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${GIO_INCLUDE_DIRS}
    )
set(Switchtogreeter_Libraries
    ${SYSTEMD_LIBRARIES}
    ${GLIB_LIBRARIES}
    ${GIO_LIBRARIES}
    )

add_subdirectory("src")
#add_subdirectory("tests")
