add_subdirectory(Core)
add_subdirectory(App)
add_subdirectory(Model)
add_subdirectory(Ui)

add_executable(MCAS main.cpp)

target_link_libraries(MCAS PRIVATE
    UiLib
    AppLib
    ModelLib
    CoreLib
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

qt_finalize_executable(MCAS)

include(GNUInstallDirs)
install(TARGETS MCAS
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
