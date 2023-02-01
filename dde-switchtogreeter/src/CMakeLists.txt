set(Switchtogreeter_Name dde-switchtogreeter)

add_executable(${Switchtogreeter_Name}
    ${Switchtogreeter_SRCS}
    )

target_include_directories(${Switchtogreeter_Name} PUBLIC
    ${Switchtogreeter_Includes}
    )

target_link_libraries(${Switchtogreeter_Name} PRIVATE
    ${Switchtogreeter_Libraries}
    )

## bin
install(TARGETS ${Switchtogreeter_Name} DESTINATION bin)
