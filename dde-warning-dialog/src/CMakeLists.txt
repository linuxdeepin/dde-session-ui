set(Warning_Dialog_Name dde-warning-dialog)

add_executable(${Warning_Dialog_Name}
    ${Warning_Dialog_SRCS}
    main.cpp
    )

target_include_directories(${Warning_Dialog_Name} PUBLIC
    ${Warning_Dialog_Includes}
    )

target_link_libraries(${Warning_Dialog_Name} PRIVATE
    ${Warning_Dialog_Libraries}
    )

## bin
install(TARGETS ${Warning_Dialog_Name} DESTINATION lib/deepin-daemon)

## service
install(FILES org.deepin.dde.WarningDialog1.service DESTINATION share/dbus-1/services)
