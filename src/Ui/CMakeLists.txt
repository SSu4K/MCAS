file(GLOB_RECURSE UI_SOURCES
    *.cpp
    *.h
)

add_library(UiLib SHARED ${UI_SOURCES})

target_include_directories(UiLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(UiLib PUBLIC
    project_pch
    ModelLib
    SimulationLib
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

target_compile_definitions(UiLib PRIVATE UILIB_LIBRARY)

