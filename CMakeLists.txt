cmake_minimum_required(VERSION 3.26)
project(sw3d)

set(CMAKE_CXX_STANDARD 20)

add_executable(sw3d WIN32 main.cpp
        3dmath.cpp
        pipeline.cpp
        utils.cpp
        window.cpp
        shaders.cpp "texture.h" "texture.cpp")
