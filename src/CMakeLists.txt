
if (DSTORE_RELEASE)
  add_definitions(-DDSTORE_WEB_DIR="${CMAKE_INSTALL_PREFIX}/share/deepin-appstore/web")
else()
  add_definitions(-DDSTORE_WEB_DIR="${CMAKE_CURRENT_BINARY_DIR}/web")
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
find_package(Qt5DeepinWebEngineWidgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5LinguistTools)
find_package(Qt5Sql REQUIRED)
find_package(Qt5WebChannel REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

pkg_search_module(DtkWidget REQUIRED dtkwidget)

include_directories(${DtkWidget_INCLUDE_DIRS})

set(LINK_LIBS
    Qt5::Core
    Qt5::DBus
    Qt5::DeepinWebEngineWidgets
    Qt5::Sql
    Qt5::Widgets
    Qt5::WebChannel

    ${DtkWidget_LDFLAGS}
    )

set(BASE_FILES
    base/command.cpp
    base/command.h
    base/consts.cpp
    base/consts.h
    base/file_util.cpp
    base/file_util.h
    )

set(RESOURCES_FILES
    resources/images.cpp
    resources/images.h
    )

set(UI_FILES
    ui/web_window.cpp
    ui/web_window.h
    )


## Executable files
# Generate .qm files from .ts files.
file(GLOB DMAN_TRANSLATION_TS
     ${CMAKE_SOURCE_DIR}/translations/deepin-appstore*.ts)
set_source_files_properties(${DMAN_TRANSLATION_TS}
                            PROPERTIES OUTPUT_LOCATION ${CMAKE_BINARY_DIR}/translations)
qt5_add_translation(DMAN_TRANSLATION_QM ${DMAN_TRANSLATION_TS})
add_custom_target(update-qm DEPENDS ${DMAN_TRANSLATION_QM})

# Build web project and generate js files.
add_custom_command(OUTPUT rebuild-web
                   COMMAND ./node_modules/.bin/ng build --output-path=${CMAKE_CURRENT_BINARY_DIR}/web
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/web/
                   )
add_custom_target(update-web DEPENDS rebuild-web)

add_executable(deepin-appstore
               app/deepin_appstore.cpp
               ${BASE_FILES}
               ${RESOURCES_FILES}
               ${UI_FILES}
               )
target_link_libraries(deepin-appstore ${LINK_LIBS})
add_dependencies(deepin-appstore
                 update-qm
                 update-web)

install(TARGETS deepin-appstore
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)