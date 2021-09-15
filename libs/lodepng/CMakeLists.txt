cmake_minimum_required(VERSION 3.21)
project(lodepng)

add_library(${PROJECT_NAME}
        src/lodepng.cpp
        )

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE src)