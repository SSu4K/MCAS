file(GLOB TEST_SOURCES *.cpp *.h)

add_executable(tests ${TEST_SOURCES})

target_include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}/source
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(tests PRIVATE
    Qt6::Core
    Qt6::Test
    core
    uiLib
    project_pch
)
