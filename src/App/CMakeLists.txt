file(GLOB_RECURSE APP_SOURCES
    *.cpp
    *.h
)

add_library(AppLib SHARED ${APP_SOURCES})

target_include_directories(AppLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppLib PUBLIC
    CoreLib
    ModelLib
    UiLib
)
