cmake_minimum_required(VERSION 3.20)
project(renderer VERSION 0.0.1 LANGUAGES CXX)

set(PROJECT_MAIN_DIR Renderer)
set(PROJECT_INCLUDE_DIRS ${PROJECT_MAIN_DIR})
set(PROJECT_VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Vendor)

add_executable(${PROJECT_NAME} ${PROJECT_MAIN_DIR}/main.cpp)

find_package(GLEW REQUIRED)

set(ASSIMP_BUILD_TESTS OFF)

set(GLFW_INCLUDE_DIRS ${PROJECT_VENDOR_DIR}/glfw/include)
set(IMGUI_INCLUDE_DIRS ${PROJECT_VENDOR_DIR}/imgui)
set(ASSIMP_INCLUDE_DIRS ${PROJECT_VENDOR_DIR}/assimp/include)

add_subdirectory(${PROJECT_VENDOR_DIR}/glfw)
add_subdirectory(${PROJECT_VENDOR_DIR}/glm)
add_subdirectory(${PROJECT_VENDOR_DIR}/assimp)

add_library(IMGUI STATIC)

target_sources(IMGUI
    PRIVATE
        ${IMGUI_INCLUDE_DIRS}/imgui_demo.cpp
        ${IMGUI_INCLUDE_DIRS}/imgui_draw.cpp
        ${IMGUI_INCLUDE_DIRS}/imgui_tables.cpp
        ${IMGUI_INCLUDE_DIRS}/imgui_widgets.cpp
        ${IMGUI_INCLUDE_DIRS}/imgui.cpp
        ${IMGUI_INCLUDE_DIRS}/misc/cpp/imgui_stdlib.cpp

    PRIVATE
        ${IMGUI_INCLUDE_DIRS}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_INCLUDE_DIRS}/backends/imgui_impl_glfw.cpp
    )
target_include_directories(IMGUI
    PUBLIC 
        ${IMGUI_INCLUDE_DIRS}
        ${IMGUI_INCLUDE_DIRS}/backends
        ${GLFW_INCLUDE_DIRS}
    )

file(GLOB_RECURSE PROJECT_SOURCES ${PROJECT_MAIN_DIR}/*.cpp)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCES}
    )

target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${PROJECT_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
        ${IMGUI_INCLUDE_DIRS}
        ${ASSIMP_INCLUDE_DIRS}
    )

target_link_libraries(${PROJECT_NAME}
    assimp
    IMGUI
    glfw
    glm::glm
    GL
    GLEW::GLEW
    )

target_precompile_headers(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_INCLUDE_DIRS}/pch.h
    )

set_property(TARGET renderer PROPERTY CXX_STANDARD 20) # should be fixed and checked on other platforms