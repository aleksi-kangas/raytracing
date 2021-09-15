#include "scene.h"

#include <memory>

#include "material.h"
#include "sphere.h"

Scene::Scene(int image_width, int image_height, int samples_per_pixel)
    : image_width(image_width), image_height(image_height), samples_per_pixel(samples_per_pixel) {}

void Scene::InitializeCamera() {
  const double kAspectRatio = static_cast<double>(image_width) / static_cast<double>(image_height);
  const Point3D kOrigin{0, 0, 0};
  constexpr double kViewportHeight = 2.0;
  constexpr double kFocalLength = 1.0;
  camera = std::make_unique<Camera>(kOrigin, kAspectRatio, kViewportHeight, kFocalLength);
}

void Scene::InitializeWorld() {

  std::shared_ptr<Material> ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
  std::shared_ptr<Material> center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
  std::shared_ptr<Material> left = std::make_shared<Dielectric>(1.5);
  std::shared_ptr<Material> right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

  world.AddCollidable(std::make_shared<Sphere>(Point3D(0, -100.5, -1), 100, ground));
  world.AddCollidable(std::make_shared<Sphere>(Point3D(0, 0, -1), 0.5, center));
  world.AddCollidable(std::make_shared<Sphere>(Point3D(-1, 0, -1), 0.5, left));
  world.AddCollidable(std::make_shared<Sphere>(Point3D(-1, 0, -1), -0.4, left));
  world.AddCollidable(std::make_shared<Sphere>(Point3D(1, 0, -1), 0.5, right));
}
