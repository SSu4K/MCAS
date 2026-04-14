file(GLOB_RECURSE TEST_SOURCES *.cpp *.h)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(tests ${TEST_SOURCES})

target_include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(tests PRIVATE
    project_pch
    #CoreLib
    SimulationLib
    Qt6::Core
    Qt6::Test
)
target_link_libraries(tests PRIVATE Qt6::Core)
