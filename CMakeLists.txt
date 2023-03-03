cmake_minimum_required(VERSION 3.2)

add_subdirectory(thirdparty/glfw)

add_library(
    ghl SHARED

    include/ghl/core/application.hpp
    include/ghl/renderer/window.hpp
    include/ghl/utils/shared.hpp

    src/core/application.cpp
    src/renderer/window.cpp
)

target_include_directories(
    ghl

    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/include
)

target_link_libraries(
    ghl

    PUBLIC glfw
)

target_compile_definitions(
    ghl

    PRIVATE GHL_BUILD_DLL
)

set_target_properties(
    ghl

    PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED OFF
    CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)