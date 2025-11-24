file(GLOB_RECURSE MODEL_SOURCES   *.cpp)
file(GLOB_RECURSE MODEL_HEADERS   *.h)

add_library(ModelLib SHARED
    ${MODEL_SOURCES}
    ${MODEL_HEADERS}
)

target_link_libraries(ModelLib PUBLIC
    project_pch
    CoreLib
    Qt6::Core
    Qt6::Gui
)

target_include_directories(ModelLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
