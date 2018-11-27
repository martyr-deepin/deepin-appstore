option(USE_SYSTEM_RESOURCES "Use web resources in source tree or in system" OFF)
option(BUILD_WEB_RESOURCES "Build web resources with npm" ON)

if(USE_SYSTEM_RESOURCES)
  add_definitions(
    -DDSTORE_WEB_DIR="${CMAKE_INSTALL_PREFIX}/share/deepin-appstore/web_dist")
  add_definitions(
    -DSETTINGS_FILE="${CMAKE_INSTALL_PREFIX}/share/deepin-appstore/settings.ini"
    )
else()
  add_definitions(-DDSTORE_WEB_DIR="${CMAKE_CURRENT_SOURCE_DIR}/web_dist")
  add_definitions(
    -DSETTINGS_FILE="${CMAKE_CURRENT_SOURCE_DIR}/resources/settings.ini")
endif()

if (DISABLE_MULTI_THREAD)
  add_definitions(-DDSTORE_DISABLE_MULTI_THREAD="YES")
endif()

if(CONFIG_VERSION)
  add_definitions(-DVERSION=${CONFIG_VERSION})
endif()

add_subdirectory(dbus)
add_subdirectory(resources)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_definitions("-DQT_MESSAGELOGCONTEXT")

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Werror -Wextra")
  # -fno-rtti, Disable real-time type information -fno-exceptions Disable
  # exceptions -fsanitize=address Enable memory sanitize plugin
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -g -Wall -Werror -Wextra -fno-exceptions")
  # -fsanitize=address, Enable memory address sanitizer. set(CMAKE_CXX_FLAGS
  # "${CMAKE_CXX_FLAGS} -fsanitize=address") -fsanitize=leak, Enable memory leak
  # sanitizer. set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
  # -fsanitize=memory, Enable detecting uninitited memory sanitizer.
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory") "-fsanitize-
  # memory-track-origins -fPIC -pie -fno-omit-frame-pointer")

  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wimplicit-fallthrough=1")

  # Enable Qt builtin debug mode
else()
  # -Wl, -O2 Enable linker optimizations -Wl, --gc-sections Remove unused code
  # resulting from -fdsta-sections and -ffunction-sections
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

set(Qt_LIBS Qt5::Core Qt5::DBus Qt5::Sql Qt5::Widgets Qt5::WebChannel)

set(LINK_LIBS ${Qt_LIBS} ${DtkWidget_LDFLAGS} ${LibQCef_LDFLAGS})

set(BASE_FILES
    base/command.cpp
    base/command.h
    base/consts.cpp
    base/consts.h
    base/file_util.cpp
    base/file_util.h
    base/launcher.cpp
    base/launcher.h)

set(DBUS_FILES
    dbus/dbus_variant/app_metadata.cpp
    dbus/dbus_variant/app_metadata.h
    dbus/dbus_variant/app_version.cpp
    dbus/dbus_variant/app_version.h
    dbus/dbus_variant/app_version.h
    dbus/dbus_variant/installed_app_info.cpp
    dbus/dbus_variant/installed_app_info.h
    dbus/dbus_variant/installed_app_timestamp.cpp
    dbus/dbus_variant/installed_app_timestamp.h
    dbus/app_store_dbus_adapter.cpp
    dbus/app_store_dbus_adapter.h
    dbus/app_store_dbus_interface.cpp
    dbus/app_store_dbus_interface.h
    dbus/app_store_metadata_dbus_adapter.cpp
    dbus/app_store_metadata_dbus_adapter.h
    dbus/app_store_metadata_dbus_proxy.cpp
    dbus/app_store_metadata_dbus_proxy.h
    dbus/dbus_consts.cpp
    dbus/dbus_consts.h
    dbus/dbus_extended_abstract_interface.cpp
    dbus/dbus_extended_abstract_interface.h
    dbus/lastore_deb_interface.cpp
    dbus/lastore_deb_interface.h
    dbus/lastore_job_interface.cpp
    dbus/lastore_job_interface.h)

set(RESOURCES_FILES
    resources/images.cpp
    resources/images.h
    resources/theme.cpp
    resources/theme.h
    resources/themes/themes.qrc)

set(SERVICES_FILES
    services/apt_util_worker.cpp
    services/apt_util_worker.h
    services/dbus_manager.cpp
    services/dbus_manager.h
    services/metadata_manager.cpp
    services/metadata_manager.h
    services/rcc_scheme_handler.cpp
    services/rcc_scheme_handler.h
    services/search_manager.cpp
    services/search_manager.h
    services/search_result.cpp
    services/search_result.h
    services/session_manager.cpp
    services/session_manager.h
    services/settings_manager.cpp
    services/settings_manager.h
    services/store_daemon_manager.cpp
    services/store_daemon_manager.h
    services/backend/chinese2pinyin.cpp
    services/backend/chinese2pinyin.h
    services/backend/metadata_cache_worker.cpp
    services/backend/metadata_cache_worker.h
    services/backend/backend.qrc
    services/package/package_manager_interface.h
    services/package/package_manager_interface.cpp
    services/package/package_manager.h
    services/package/package_manager.cpp
    services/package/apt_package_manager.h
    services/package/apt_package_manager.cpp
    services/package/dpk_url.h
    services/package/dpk_url.cpp)

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
    ui/channel/channel_proxy.h
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
    ui/widgets/user_menu.h
    ui/widgets/user_menu.cpp
    ui/widgets/title_bar_menu.cpp
    ui/widgets/title_bar_menu.h)

# Executable files Generate .qm files from .ts files.
file(GLOB DMAN_TRANSLATION_TS
          ${CMAKE_SOURCE_DIR}/translations/deepin-appstore*.ts)
set_source_files_properties(${DMAN_TRANSLATION_TS}
                            PROPERTIES
			    OUTPUT_LOCATION
			    ${CMAKE_BINARY_DIR}/translations)
qt5_add_translation(DMAN_TRANSLATION_QM ${DMAN_TRANSLATION_TS})
add_custom_target(update-qm DEPENDS ${DMAN_TRANSLATION_QM})

# Update angular dist/. Works on x86 based platforms.
if(BUILD_WEB_RESOURCES)
  add_custom_command(OUTPUT npm-update-web-dist
                     COMMAND sh -c
		             "python3 fix-i18n.py; PATH=./node_modules/.bin:$PATH npm run build; PATH=./node_modules/.bin:$PATH npm run build-aot; "
			WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/web"
		     VERBATIM)
else()
  add_custom_command(OUTPUT npm-update-web-dist
                     COMMAND sh -c "mkdir -p web_dist web_dist_aot/appstore || true"
		     WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
		     VERBATIM)
endif()

add_custom_command(OUTPUT deepin-update-desktop
COMMAND sh -c "deepin-desktop-ts-convert ts2desktop resources/deepin-appstore.desktop ../translations/desktop resources/deepin-appstore.desktop")

add_custom_target(update-desktop DEPENDS deepin-update-desktop)
add_custom_target(update-web-dist DEPENDS npm-update-web-dist)

add_executable(deepin-appstore
               app/deepin_appstore.cpp
	       ${BASE_FILES}
	       ${DBUS_FILES}
	       ${RESOURCES_FILES}
	       ${SERVICES_FILES}
	       ${UI_FILES})

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${LibQCef_LIBDIR}/qcef")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set_target_properties(deepin-appstore
                      PROPERTIES INSTALL_RPATH "${LibQCef_LIBDIR}/qcef")
target_link_libraries(deepin-appstore ${LibQCef_LIBDIR}/qcef/libcef.so
                      ${LINK_LIBS})

if(CMAKE_BUILD_TYPE MATCHES Release)
  add_dependencies(deepin-appstore update-qm update-web-dist update-desktop)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
  add_executable(test-launcher
                 app/test_launcher.cpp
		 base/launcher.cpp
		 base/launcher.h)
  target_link_libraries(test-launcher ${LINK_LIBS})
  add_executable(test-base64-image
                 app/base64_image.cpp
		 base/file_util.cpp
		 base/file_util.h)
  target_link_libraries(test-base64-image ${LINK_LIBS})

  add_executable(qurl-demo app/qurl_demo.cpp)
  target_link_libraries(qurl-demo ${LINK_LIBS})

  add_executable(test-image-viewer
                 app/test_image_viewer.cpp
		 base/file_util.cpp
		 base/file_util.h
		 ui/widgets/image_viewer.cpp
		 ui/widgets/image_viewer.h
		 resources/themes/themes.qrc)
  target_link_libraries(test-image-viewer ${LINK_LIBS})
endif()

add_executable(deepin-appstore-metadata
               app/deepin_appstore_metadata.cpp
	       ${BASE_FILES}
	       ${DBUS_FILES}
	       ${SERVICES_FILES})
target_link_libraries(deepin-appstore-metadata ${Qt_LIBS})

install(TARGETS deepin-appstore deepin-appstore-metadata
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/web_dist
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-appstore/)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/web_dist_aot
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-appstore/)

install(DIRECTORY ${CMAKE_BINARY_DIR}/translations
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-appstore/
	FILES_MATCHING
	PATTERN "*")
