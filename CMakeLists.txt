cmake_minimum_required(VERSION 3.2)

add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/assimp)

file(GLOB_RECURSE OGL_HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.hpp")
file(GLOB_RECURSE OGL_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

add_library(
    ogl STATIC

    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/include/glad/glad.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/include/KHR/khrplatform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/src/glad.c

    ${OGL_HEADER_FILES}
    ${OGL_SOURCE_FILES}
)

target_include_directories(
    ogl

    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/entt/single_include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/include
    PUBLUC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/assimp/include
)

target_link_libraries(
    ogl

    PUBLIC glfw
    PUBLIC assimp
)

target_compile_definitions(
    ogl PUBLIC 

    _CRT_NONSTDC_NO_WARNINGS
)

set_target_properties(
    ogl

    PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED OFF
    CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
