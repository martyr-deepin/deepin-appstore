
if (CMAKE_BUILD_TYPE MATCHES Release)
  add_definitions(-DDSTORE_WEB_DIR="${CMAKE_INSTALL_PREFIX}/share/deepin-appstore/web_dist")
  add_definitions(-DSETTINGS_FILE="${CMAKE_INSTALL_PREFIX}/share/deepin-appstore/settings.ini")
else()
  add_definitions(-DDSTORE_WEB_DIR="${CMAKE_CURRENT_SOURCE_DIR}/web_dist")
  add_definitions(-DSETTINGS_FILE="${CMAKE_CURRENT_SOURCE_DIR}/resources/settings.ini")
endif ()

add_subdirectory(dbus)
add_subdirectory(resources)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if (CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Werror -Wextra")
  # -fno-rtti, Disable real-time type information
  # -fno-exceptions Disable exceptions
  # -fsanitize=address Enable memory sanitize plugin
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror -Wextra -fno-exceptions")
  # -fsanitize=address, Enable memory address sanitizer.
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  # -fsanitize=leak, Enable memory leak sanitizer.
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
  # -fsanitize=memory, Enable detecting uninitited memory sanitizer.
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory")
  #    "-fsanitize-memory-track-origins -fPIC -pie -fno-omit-frame-pointer")

  #  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wimplicit-fallthrough=1")

  # Enable Qt builtin debug mode
  add_definitions("-DQT_MESSAGELOGCONTEXT")
else()
  # -Wl, -O2 Enable linker optimizations
  # -Wl, --gc-sections Remove unused code resulting from -fdsta-sections and
  # -ffunction-sections
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2 -Wl,-O1 -Wl,--gc-sections")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -Wl,-O1 -Wl,--gc-sections")
endif()

find_package(PkgConfig REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5LinguistTools)
find_package(Qt5Sql REQUIRED)
find_package(Qt5WebChannel REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

pkg_search_module(DtkWidget REQUIRED dtkwidget)
pkg_search_module(LibQCef REQUIRED libqcef)

include_directories(${DtkWidget_INCLUDE_DIRS})
include_directories(${LibQCef_INCLUDE_DIRS})

set(LINK_LIBS
    Qt5::Core
    Qt5::DBus
    Qt5::Sql
    Qt5::Widgets
    Qt5::WebChannel

    ${DtkWidget_LDFLAGS}
    ${LibQCef_LDFLAGS}
    )

set(BASE_FILES
    base/command.cpp
    base/command.h
    base/consts.cpp
    base/consts.h
    base/file_util.cpp
    base/file_util.h
    base/launcher.cpp
    base/launcher.h
    )

set(DBUS_FILES
    dbus/dbusvariant/app_update_info.cpp
    dbus/dbusvariant/app_update_info.h
    dbus/dbusvariant/locale_mirror_source.cpp
    dbus/dbusvariant/locale_mirror_source.h

    dbus/app_store_dbus_adapter.cpp
    dbus/app_store_dbus_adapter.h
    dbus/app_store_dbus_interface.cpp
    dbus/app_store_dbus_interface.h
    dbus/app_store_dbus_proxy.cpp
    dbus/app_store_dbus_proxy.h
    dbus/dbus_consts.cpp
    dbus/dbus_consts.h
    dbus/lastore_job_interface.cpp
    dbus/lastore_job_interface.h
    dbus/lastore_manager_interface.cpp
    dbus/lastore_manager_interface.h
    dbus/lastore_updater_interface.cpp
    dbus/lastore_updater_interface.h
    )

set(RESOURCES_FILES
    resources/images.cpp
    resources/images.h
    resources/themes/themes.qrc
    )

set(SERVICES_FILES
    services/args_parser.cpp
    services/args_parser.h
    services/dpk_link_validation.cpp
    services/dpk_link_validation.h
    services/rcc_scheme_handler.cpp
    services/rcc_scheme_handler.h
    services/search_manager.cpp
    services/search_manager.h
    services/search_result.cpp
    services/search_result.h
    services/settings_manager.cpp
    services/settings_manager.h
    services/store_daemon_worker.cpp
    services/store_daemon_worker.h
    )

set(UI_FILES
    ui/web_event_delegate.cpp
    ui/web_event_delegate.h
    ui/web_window.cpp
    ui/web_window.h

    ui/channel/image_viewer_proxy.cpp
    ui/channel/image_viewer_proxy.h
    ui/channel/log_proxy.cpp
    ui/channel/log_proxy.h
    ui/channel/menu_proxy.cpp
    ui/channel/menu_proxy.h
    ui/channel/search_proxy.cpp
    ui/channel/search_proxy.h
    ui/channel/settings_proxy.cpp
    ui/channel/settings_proxy.h
    ui/channel/store_daemon_proxy.cpp
    ui/channel/store_daemon_proxy.h

    ui/utils/theme_manager.cpp
    ui/utils/theme_manager.h

    ui/widgets/image_viewer.cpp
    ui/widgets/image_viewer.h
    ui/widgets/search_button.cpp
    ui/widgets/search_button.h
    ui/widgets/search_completion_window.cpp
    ui/widgets/search_completion_window.h
    ui/widgets/search_edit.cpp
    ui/widgets/search_edit.h
    ui/widgets/title_bar.cpp
    ui/widgets/title_bar.h
    ui/widgets/title_bar_menu.cpp
    ui/widgets/title_bar_menu.h
    )

## Executable files
# Generate .qm files from .ts files.
file(GLOB DMAN_TRANSLATION_TS
     ${CMAKE_SOURCE_DIR}/translations/deepin-appstore*.ts)
set_source_files_properties(${DMAN_TRANSLATION_TS}
                            PROPERTIES OUTPUT_LOCATION ${CMAKE_BINARY_DIR}/translations)
qt5_add_translation(DMAN_TRANSLATION_QM ${DMAN_TRANSLATION_TS})
add_custom_target(update-qm DEPENDS ${DMAN_TRANSLATION_QM})

add_executable(deepin-appstore
               app/deepin_appstore.cpp
               ${BASE_FILES}
               ${DBUS_FILES}
               ${RESOURCES_FILES}
               ${SERVICES_FILES}
               ${UI_FILES}
               )
target_link_libraries(deepin-appstore ${LINK_LIBS})
if (CMAKE_BUILD_TYPE MATCHES Debug)
  add_executable(test-launcher
                 app/test_launcher.cpp
                 base/launcher.cpp
                 base/launcher.h
                 )
  target_link_libraries(test-launcher ${LINK_LIBS})
  add_executable(test-base64-image
                 app/base64_image.cpp
                 base/file_util.cpp
                 base/file_util.h
                 )
  target_link_libraries(test-base64-image ${LINK_LIBS})
endif()

if (CMAKE_BUILD_TYPE MATCHES Release)
  add_dependencies(deepin-appstore update-qm)
endif ()

install(TARGETS deepin-appstore
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/web_dist
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-appstore/)
