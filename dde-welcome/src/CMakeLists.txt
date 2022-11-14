set(Welcome_Name dde-welcome)

add_executable(${Welcome_Name}
    ${Welcome_SRCS}
    ${Widgets_SRCS}
    ${Global_Util_SRCS}
    ${DBUS_INTERFACE_SRCS}
    main.cpp
    )

target_include_directories(${Welcome_Name} PUBLIC
    ${Welcome_Includes}
    )

target_link_libraries(${Welcome_Name} PRIVATE
    ${Welcome_Libraries}
    )

## bin
install(TARGETS ${Welcome_Name} DESTINATION lib/deepin-daemon)

## service
install(FILES org.deepin.dde.Welcome1.service DESTINATION share/dbus-1/services)
