cmake_minimum_required(VERSION 3.2)
project(Shell)
#set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5X11Extras REQUIRED)
find_package(Qt5Gui REQUIRED)
#find_package(Qt5WebChannel REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_search_module(X11 REQUIRED x11)
pkg_search_module(DUI REQUIRED dui)
pkg_search_module(GIO REQUIRED gio-unix-2.0)

include_directories(AFTER ${GLIB_INCLUDE_DIRS})
include_directories(AFTER ${X11_INCLUDE_DIRS})
include_directories(AFTER ${GIO_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp
                 MainWindow.cpp MainWindow.h
                 Shell.cpp Shell.h
                 Bridge.cpp Bridge.h
                 ProgressButton.cpp ProgressButton.h
                 FilterMouseMove.cpp FilterMouseMove.h
                 AboutWindow.cpp AboutWindow.h)

set(DBUSMENU_FILES dbusmenu.cpp dbusmenu.h dbusmenumanager.cpp dbusmenumanager.h)
set(DBUS_BACKEND_FILES LAStoreBridge.cpp LAStoreBridge.h)

set(LINK_LIBS
        Qt5::Widgets
        Qt5::X11Extras
        Qt5::DBus
        Qt5::Gui
#        Qt5::WebChannel
        ${GLIB_LIBRARIES}
        ${X11_LIBRARIES}
        ${GIO_LIBRARIES}
        dui)

# Link against either QtWebKit or QtWebEngine
#find_package(Qt5WebEngineWidgets)
if(Qt5WebEngineWidgets_FOUND)
    set(WEB_WIDGET_FILES WebEngine/WebView.cpp WebEngine/WebView.h WebEngine/WebPage.cpp WebEngine/WebPage.h)
    add_definitions(-DBUILD_WITH_WEBENGINE=1)
    add_executable(dstore-client ${SOURCE_FILES} ${TOOLTIP_FILES} ${DBUSMENU_FILES} ${DBUS_BACKEND_FILES} ${WEB_WIDGET_FILES}
                   res.qrc)
    target_link_libraries(dstore-client ${LINK_LIBS} Qt5::WebEngineWidgets)
else()
    set(WEB_WIDGET_FILES WebKit/WebView.cpp WebKit/WebView.h WebKit/WebPage.cpp WebKit/WebPage.h)
    find_package(Qt5WebKit REQUIRED)
    find_package(Qt5WebKitWidgets REQUIRED)
    add_definitions(-DBUILD_WITH_WEBKIT=1)
    add_executable(dstore-client ${SOURCE_FILES} ${TOOLTIP_FILES} ${DBUSMENU_FILES} ${DBUS_BACKEND_FILES} ${WEB_WIDGET_FILES}
                   res.qrc
                   "/usr/include/lastore-daemon.h")

    target_link_libraries(dstore-client ${LINK_LIBS} Qt5::WebKitWidgets)
endif()

install(TARGETS dstore-client
        DESTINATION "/usr/bin")
install(FILES support/dstore-client.desktop
        DESTINATION "/usr/share/applications")
