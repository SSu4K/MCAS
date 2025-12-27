add_subdirectory(Core)
add_subdirectory(Simulation)
add_subdirectory(Model)
add_subdirectory(Ui)
add_subdirectory(App)

# Create the executable
add_executable(MCAS main.cpp)

if (DEFINED MCAS_QRC_SOURCES)
    target_sources(MCAS PRIVATE ${MCAS_QRC_SOURCES})
endif()

# Link all sublibraries
target_link_libraries(MCAS PRIVATE
    UiLib
    AppLib
    ModelLib
    CoreLib
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

set(MCAS_TS_FILES
    "${CMAKE_SOURCE_DIR}/resources/i18n/MCAS_en.ts"
    "${CMAKE_SOURCE_DIR}/resources/i18n/MCAS_pl.ts"
)

qt_add_translations(MCAS
    TS_FILES ${MCAS_TS_FILES}
    RESOURCE_PREFIX "/i18n"
)

qt_finalize_executable(MCAS)

# Optional install
include(GNUInstallDirs)
install(TARGETS MCAS
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
