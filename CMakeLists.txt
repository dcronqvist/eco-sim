cmake_minimum_required(VERSION 3.12.0)
project(opengl-imgui-cmake-template VERSION 0.1.0)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_PARALLEL_LEVEL 8)

set(CMAKE_CXX_EXTENSIONS OFF)

link_directories(libs/glfw)

include_directories(libs/glad)
include_directories(libs/KHR)
include_directories(libs/imgui)
include_directories(libs/glfw)
include_directories(include)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(opengl-imgui-cmake-template ${SOURCES})

find_package(OpenGL REQUIRED)
target_link_libraries(opengl-imgui-cmake-template PRIVATE ${OPENGL_gl_LIBRARY})
target_link_libraries(opengl-imgui-cmake-template PRIVATE glfw3)

file(COPY resources DESTINATION /)
