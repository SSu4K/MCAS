cmake_minimum_required(VERSION 3.16)
project(MCAS VERSION 1.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Global Qt setup
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets LinguistTools Test)

add_library(project_pch INTERFACE)
target_precompile_headers(project_pch INTERFACE src/pch.h)

set(APP_VERSION "v1.0.1-beta")
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

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Enabling AddressSanitizer for Debug build")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
endif()

add_subdirectory(resources)
add_subdirectory(src)
add_subdirectory(tests)
