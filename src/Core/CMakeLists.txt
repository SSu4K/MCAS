file(GLOB_RECURSE CORE_SOURCES
    *.cpp
    *.h
)

add_library(CoreLib STATIC ${CORE_SOURCES})

target_include_directories(CoreLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(CoreLib PUBLIC
    project_pch
    Qt6::Core
)

set_target_properties(CoreLib PROPERTIES POSITION_INDEPENDENT_CODE ON)
