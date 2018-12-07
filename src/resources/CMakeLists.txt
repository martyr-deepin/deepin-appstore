install(FILES deepin-appstore.desktop
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/)

install(FILES themes/common/images/deepin-appstore.svg
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable/apps/)

install(FILES settings.ini.default
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-appstore/)
