cmake_minimum_required(VERSION 3.24)
project(raytracer)

add_library(${PROJECT_NAME}
        include/aabb.h              src/aabb.cpp
        include/box.h               src/box.cpp
        include/bvh.h               src/bvh.cpp
        include/camera.h            src/camera.cpp
        include/collidable.h
        include/collidables.h
        include/collision.h
        include/constant_medium.h   src/constant_medium.cpp
        include/crtp.h
        include/flip.h              src/flip.cpp
        include/material.h          src/material.cpp
        include/image.h             src/image.cpp
        include/onb.h               src/onb.cpp
        include/pdf.h               src/pdf.cpp
        include/perlin.h            src/perlin.cpp
        include/primitives.h
        include/random.h            src/random.cpp
        include/ray.h               src/ray.cpp
        include/raytracer.h         src/raytracer.cpp
        include/rectangle.h         src/rectangle.cpp
        include/renderer.h          src/renderer.cpp
        include/scene.h             src/scene.cpp
        include/sphere.h            src/sphere.cpp
        include/texture.h           src/texture.cpp
        include/transformables.h
        include/transform.h src/transform.cpp
        include/utils.h             src/utils.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw glad imgui glm thread_pool stbi)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_compile_options(${PROJECT_NAME} PRIVATE /W3)
