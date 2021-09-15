cmake_minimum_required(VERSION 3.21)
project(raytracing)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        src/main.cpp
        src/utils.cpp src/utils.h
        src/vector3d.cpp src/vector3d.h)

add_subdirectory(libs/lodepng)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} lodepng)
