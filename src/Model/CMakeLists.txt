file(GLOB_RECURSE MODEL_SOURCES
    Common/texttablemodel.cpp
    Instruction/instructionmodel.cpp
    Instruction/breakpointmodel.cpp
    Instruction/instructionsetmodel.cpp
    Memory/memorymodel.cpp
    Microcode/jumptablemodel.cpp
    Microcode/microcodemodel.cpp
)
file(GLOB_RECURSE MODEL_HEADERS
    Common/texttablemodel.h
    Instruction/instructionmodel.h
    Instruction/breakpointmodel.h
    Instruction/instructionsetmodel.h
    Memory/memorymodel.h
    Microcode/jumptablemodel.h
    Microcode/microcodemodel.h
)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(ModelLib SHARED
    ${MODEL_SOURCES}
    ${MODEL_HEADERS}
)

set_target_properties(ModelLib PROPERTIES
    AUTOMOC ON
)

target_link_libraries(ModelLib PUBLIC
    project_pch
    CoreLib
    Qt6::Core
    Qt6::Gui
)

target_include_directories(ModelLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(ModelLib PRIVATE MODELLIB_LIBRARY)
