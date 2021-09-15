cmake_minimum_required(VERSION 3.21)
project(raytracing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(SOURCE_FILES
        src/aabb.cpp src/aabb.h
        src/box.cpp src/box.h
        src/bvh.cpp src/bvh.h
        src/camera.cpp src/camera.h
        src/collidable.h
        src/collidables.cpp src/collidables.h
        src/main.cpp
        src/material.cpp src/material.h
        src/moving_sphere.cpp src/moving_sphere.h
        src/perlin.cpp src/perlin.h
        src/random.cpp src/random.h
        src/ray.cpp src/ray.h
        src/rectangle.cpp src/rectangle.h
        src/renderer.cpp src/renderer.h
        src/rotate.cpp src/rotate.h
        src/scene.cpp src/scene.h
        src/sphere.cpp src/sphere.h
        src/texture.cpp src/texture.h
        src/translate.cpp src/translate.h
        src/utils.cpp src/utils.h
        src/vector3d.cpp src/vector3d.h)

add_subdirectory(libs/lodepng)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} lodepng)
