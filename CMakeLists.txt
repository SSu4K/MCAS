cmake_minimum_required(VERSION 3.16)

project(MCAS VERSION 0.1 LANGUAGES CXX)

# Enable automatic Qt processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt 6 modules
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools)

# Define version and build info
set(APP_VERSION "v0.1.0-prealpha")
set(BUILD_TYPE ${CMAKE_BUILD_TYPE})
set(BUILD_PLATFORM ${CMAKE_SYSTEM_NAME})

add_compile_definitions(
    APP_VERSION="${APP_VERSION}"
    BUILD_TYPE="${BUILD_TYPE}"
    BUILD_PLATFORM="${BUILD_PLATFORM}"
)

if (MSVC)
    add_compile_definitions(COMPILER_NAME="MSVC")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_definitions(COMPILER_NAME="Clang")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_definitions(COMPILER_NAME="GCC")
endif()

# Include the source directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)

# Translation files (optional)
file(GLOB TS_FILES translations/*.ts)

# Source files
file(GLOB_RECURSE CPP_FILES source/*.cpp)
file(GLOB_RECURSE HEADER_FILES source/*.h)
file(GLOB_RECURSE UI_FILES source/*.ui)
file(GLOB_RECURSE QRC_FILES source/*.qrc)

set(PROJECT_SOURCES
    ${CPP_FILES}
    ${HEADER_FILES}
    ${UI_FILES}
    ${QRC_FILES}
)

# Add executable
qt_add_executable(MCAS
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    source/MicrocodeEditor/microcodeeditordelegate.cpp source/MicrocodeEditor/microcodeeditordelegate.h
    source/zoomwidget.h source/zoomwidget.cpp
    source/mainwindow.h source/mainwindow.cpp
    source/MicrocodeEditor/jumptable.h source/MicrocodeEditor/jumptable.cpp
    source/MicrocodeEditor/jumptable.h source/MicrocodeEditor/jumptable.cpp
    source/MicrocodeEditor/jumptablemodel.h source/MicrocodeEditor/jumptablemodel.cpp
    source/MicrocodeEditor/jumptableeditorwidget.h source/MicrocodeEditor/jumptableeditorwidget.cpp
    source/texttablemodel.h source/texttablemodel.cpp
    source/MicrocodeEditor/jumptableeditordelegate.h source/MicrocodeEditor/jumptableeditordelegate.cpp
    source/appcontext.h source/appcontext.cpp
    source/mcasapp.h source/mcasapp.cpp


)

# Link Qt libraries
target_link_libraries(MCAS PRIVATE Qt6::Widgets)

# Create translations if they exist
if(TS_FILES)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# Install target
include(GNUInstallDirs)
install(TARGETS MCAS
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize executable
qt_finalize_executable(MCAS)
