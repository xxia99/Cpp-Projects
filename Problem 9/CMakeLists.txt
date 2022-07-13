cmake_minimum_required(VERSION 3.0)
project(FinalProject)
set(CMAKE_CXX_STANDARD 11)
find_package(OpenGL REQUIRED)
add_subdirectory(glfw)
add_subdirectory(glew)
set(ALL_LIBS
    glfw
    glew
    ${OPENGL_LIBRARY})
include_directories(
        common/
        glfw/include/GLFW/
        glew/include/)
add_executable(GaTechBuzzyBowl
        main.cpp
        ECE_UAV.cpp
        common/controls.cpp
        common/objloader.cpp
        common/shader.cpp
        common/texture.cpp
        common/vboindexer.cpp)
target_link_libraries(FinalProject
        ${ALL_LIBS})