file(GLOB_RECURSE SIMULATION_SOURCES   *.cpp)
file(GLOB_RECURSE SIMULATION_HEADERS   *.h)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(SimulationLib SHARED
    ${SIMULATION_SOURCES}
    ${SIMULATION_HEADERS}
)

target_link_libraries(SimulationLib PUBLIC
    project_pch
    CoreLib
    Qt6::Core
    Qt6::Gui
)
target_link_libraries(SimulationLib PRIVATE Qt6::Core)

target_include_directories(SimulationLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
