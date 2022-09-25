cmake_minimum_required(VERSION 3.24)
project(raytracer)

add_library(${PROJECT_NAME}
        include/image.h         src/image.cpp
        include/random.h        src/random.cpp
        include/raytracer.h     src/raytracer.cpp
        include/renderer.h      src/renderer.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw glad imgui glm)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
