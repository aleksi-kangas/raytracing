cmake_minimum_required(VERSION 3.24)
project(raytracing
        LANGUAGES C CXX
        VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(libs)
add_subdirectory(raytracer)

add_executable(raytracing main.cpp)
target_link_libraries(raytracing PUBLIC raytracer)
