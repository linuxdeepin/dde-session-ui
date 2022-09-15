set(Suspend_Dialog_Name dde-suspend-dialog)

add_executable(${Suspend_Dialog_Name}
    ${Suspend_Dialog_SRCS}
    ${DBUS_INTERFACE_SRCS}
    main.cpp
    )

target_include_directories(${Suspend_Dialog_Name} PUBLIC
    ${Suspend_Dialog_Includes}
    )

target_link_libraries(${Suspend_Dialog_Name} PRIVATE
    ${Suspend_Dialog_Libraries}
    )

## bin
install(TARGETS ${Suspend_Dialog_Name} DESTINATION lib/deepin-daemon)

## icons
install(FILES data/computer.svg DESTINATION share/icons/hicolor/scalable/devices)
