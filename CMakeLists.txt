cmake_minimum_required(VERSION 3.2)

add_subdirectory(thirdparty/glfw)

add_library(
    ghl STATIC

    src/ghl/core/application.hpp
    src/ghl/core/application.cpp
    src/ghl/core/application_layer.hpp
    src/ghl/core/debug.hpp
    src/ghl/core/debug.cpp
    src/ghl/renderer/window.hpp
    src/ghl/renderer/window.cpp
    src/ghl/utils/utils.hpp
)

target_include_directories(
    ghl

    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/include
)

target_link_libraries(
    ghl

    PUBLIC glfw
)

target_compile_definitions(
    ghl PUBLIC 

    _CRT_NONSTDC_NO_WARNINGS
)

set_target_properties(
    ghl

    PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED OFF
    CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)