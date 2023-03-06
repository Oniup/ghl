cmake_minimum_required(VERSION 3.2)

add_subdirectory(thirdparty/glfw)

file(GLOB_RECURSE GHL_HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.hpp")
file(GLOB_RECURSE GHL_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

add_library(
    ghl STATIC

    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/include/glad/glad.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/include/KHR/khrplatform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/src/glad.c

    ${GHL_HEADER_FILES}
    ${GHL_SOURCE_FILES}
)

target_include_directories(
    ghl

    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/entt/single_include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/include
    PUBLUC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm
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