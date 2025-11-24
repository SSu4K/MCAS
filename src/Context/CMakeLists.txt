add_library(ContextLib SHARED appcontext.cpp appcontext.h)

target_include_directories(ContextLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(ContextLib PUBLIC
    CoreLib
    ModelLib
)
