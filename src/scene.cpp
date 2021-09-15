#include "scene.h"

#include <memory>

#include "sphere.h"

Scene::Scene(int image_width, int image_height, int samples_per_pixel)
    : image_width(image_width), image_height(image_height), samples_per_pixel(samples_per_pixel) {}

void Scene::InitializeWorld() {
  world.AddCollidable(std::make_shared<Sphere>(Point3D(0, 0, -1), 0.5));
  world.AddCollidable(std::make_shared<Sphere>(Point3D(0, -100.5, -1), 100));
}
