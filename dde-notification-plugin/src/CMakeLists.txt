set(Notification_Plugin_Name notifications)

add_library(${Notification_Plugin_Name} MODULE
    ${Notification_Plugin_SRCS}
    )

target_include_directories(${Notification_Plugin_Name} PUBLIC
    ${Notification_Plugin_Includes}
    )

target_link_libraries(${Notification_Plugin_Name} PRIVATE
    ${Notification_Plugin_Libraries}
    )

## bin
install(TARGETS ${Notification_Plugin_Name} LIBRARY DESTINATION lib/dde-dock/plugins)

## schemas
install(FILES gschema/com.deepin.dde.dock.module.notifications.gschema.xml DESTINATION share/glib-2.0/schemas)
