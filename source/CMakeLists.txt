file(GLOB_RECURSE UI_SOURCES   *.cpp)
file(GLOB_RECURSE UI_HEADERS   *.h)
file(GLOB_RECURSE UI_FORMS     *.ui)
file(GLOB_RECURSE UI_RESOURCES *.qrc)

add_library(uiLib SHARED
    ${UI_SOURCES}
    ${UI_HEADERS}
    ${UI_FORMS}
    ${UI_RESOURCES}
)

target_link_libraries(uiLib PUBLIC
    Qt6::Core
    Qt6::Widgets
    core
    project_pch
)

target_include_directories(uiLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB TS_FILES ${CMAKE_SOURCE_DIR}/translations/*.ts)
if(TS_FILES)
    qt_add_translations(uiLib
        TS_FILES ${TS_FILES}
        LUPDATE_OPTIONS -recursive ${CMAKE_SOURCE_DIR}
    )
endif()
