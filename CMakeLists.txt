cmake_minimum_required(VERSION 3.16)

project(MCAS VERSION 0.1 LANGUAGES CXX)

# Qt Setup
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets LinguistTools Test)

# Build info macros
set(APP_VERSION "v0.1.0-prealpha")

add_compile_definitions(
    APP_VERSION="${APP_VERSION}"
    BUILD_TYPE="$<CONFIG>"
    BUILD_PLATFORM="${CMAKE_SYSTEM_NAME}"
)

if (MSVC)
    add_compile_definitions(COMPILER_NAME="MSVC")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_definitions(COMPILER_NAME="Clang")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_definitions(COMPILER_NAME="GCC")
endif()

add_library(pch INTERFACE)
target_precompile_headers(pch INTERFACE source/pch.h)

# Project sources
file(GLOB_RECURSE CPP_FILES   source/*.cpp)
file(GLOB_RECURSE HEADER_FILES source/*.h)
file(GLOB_RECURSE UI_FILES    source/*.ui)
file(GLOB_RECURSE QRC_FILES   source/*.qrc)

# Main Application
qt_add_executable(MCAS
    MANUAL_FINALIZATION
    ${CPP_FILES}
    ${HEADER_FILES}
    ${UI_FILES}
    ${QRC_FILES}
)

target_link_libraries(MCAS PRIVATE
    Qt6::Core
    Qt6::Widgets
    pch
)

target_include_directories(MCAS PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

# Tests
add_executable(tests
    source/Common/hexint.cpp
    source/Common/twoscomplement.cpp
    source/Assembler/tokenizer.cpp
    source/Assembler/instructiondefinition.cpp
    source/Assembler/labeldata.cpp
    source/Assembler/assemblystatus.cpp
    source/Assembler/instruction.cpp
    source/Assembler/assembler.cpp
    source/Assembler/disassembler.cpp

    tests/testinstructions.cpp tests/testinstructions.h
    tests/testassembler.cpp tests/testassembler.h
    tests/testdisassembler.cpp tests/testdisassembler.h
    tests/testrunner.cpp
)

target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source ${CMAKE_CURRENT_SOURCE_DIR}/tests)

target_link_libraries(tests PRIVATE
    Qt6::Core
    Qt6::Test
    pch
)

# Translations
file(GLOB TS_FILES translations/*.ts)
if(TS_FILES)
    qt_add_translations(MCAS
        TS_FILES ${TS_FILES}
        LUPDATE_OPTIONS -recursive ${CMAKE_SOURCE_DIR}
    )
endif()

# Install
include(GNUInstallDirs)
install(TARGETS MCAS
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(MCAS)
