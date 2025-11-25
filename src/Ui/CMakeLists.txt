file(GLOB_RECURSE UI_SOURCES   *.cpp)
file(GLOB_RECURSE UI_HEADERS   *.h)
file(GLOB_RECURSE UI_FORMS     *.ui)
file(GLOB_RECURSE UI_RESOURCES *.qrc)

add_library(UiLib SHARED
    ${UI_SOURCES}
    ${UI_HEADERS}
    ${UI_FORMS}
    ${UI_RESOURCES}
)

target_link_libraries(UiLib PUBLIC
    project_pch
    CoreLib
    ModelLib
    ContextLib
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

target_include_directories(UiLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
