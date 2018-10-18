cmake_minimum_required(VERSION 3.0)
project(deepin-appstore)

add_subdirectory(src)

add_custom_command(OUTPUT update-qt-i18n
    COMMAND lupdate -recursive src/app src/ui -ts translations/deepin-appstore.ts
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
add_custom_command(OUTPUT update-web-i18n
    COMMAND npm run i18n
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/web
    )
add_custom_command(OUTPUT upload-to-tx
    COMMAND tx push -s
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
add_custom_command(OUTPUT download-from-tx
    COMMAND tx pull -f --minimum-perc=1
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

add_custom_target(upload-i18n DEPENDS
                  update-qt-i18n
                  update-web-i18n
                  upload-to-tx
                  )
add_custom_target(download-i18n DEPENDS download-from-tx)
