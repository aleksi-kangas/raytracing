cmake_minimum_required(VERSION 3.24)
project(raytracer)

add_library(${PROJECT_NAME}
        include/camera.h        src/camera.cpp
        include/collidable.h
        include/collision.h
        include/image.h src/image.cpp
        include/random.h        src/random.cpp
        include/ray.h           src/ray.cpp
        include/raytracer.h     src/raytracer.cpp
        include/renderer.h      src/renderer.cpp
        include/scene.h         src/scene.cpp
        include/sphere.h        src/sphere.cpp
        include/utils.h         src/utils.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw glad imgui glm thread_pool)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
