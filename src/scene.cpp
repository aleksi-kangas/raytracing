#include "scene.h"

#include <memory>

#include "material.h"
#include "sphere.h"
#include "utils.h"

Scene::Scene(int image_width, int image_height, int samples_per_pixel)
    : image_width(image_width), image_height(image_height), samples_per_pixel(samples_per_pixel) {}

void Scene::InitializeCamera() {
  const Point3D kPosition{3, 3, 2};
  const Point3D kTarget{0, 0, -1};
  const double kAspectRatio = static_cast<double>(image_width) / static_cast<double>(image_height);
  constexpr double kVerticalFOVDegrees = 20.0f;
  constexpr double kAperture = 2.0f;
  const double kFocusDistance = (kTarget - kPosition).Length();
  camera = std::make_unique<Camera>(kPosition, kTarget, kAspectRatio, kVerticalFOVDegrees, kAperture, kFocusDistance);
}

void Scene::InitializeWorld() {
  std::shared_ptr<Material> ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
  std::shared_ptr<Material> center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
  std::shared_ptr<Material> left = std::make_shared<Dielectric>(1.5);
  std::shared_ptr<Material> right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

  world.AddCollidable(std::make_shared<Sphere>(Point3D(0, -100.5, -1), 100, ground));
  world.AddCollidable(std::make_shared<Sphere>(Point3D(0, 0, -1), 0.5, center));
  world.AddCollidable(std::make_shared<Sphere>(Point3D(-1, 0, -1), 0.5, left));
  world.AddCollidable(std::make_shared<Sphere>(Point3D(-1, 0, -1), -0.45, left));
  world.AddCollidable(std::make_shared<Sphere>(Point3D(1, 0, -1), 0.5, right));
}
