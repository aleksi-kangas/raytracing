#include "scene.h"

#include <memory>

#include "material.h"
#include "random.h"
#include "sphere.h"
#include "utils.h"

Scene::Scene(int image_width, int image_height, int samples_per_pixel)
    : image_width(image_width), image_height(image_height), samples_per_pixel(samples_per_pixel) {}

void Scene::InitializeCamera() {
  const Point3D kPosition{13, 2, 2};
  const Point3D kTarget{0, 0, 0};
  const double kAspectRatio = static_cast<double>(image_width) / static_cast<double>(image_height);
  constexpr double kVerticalFOVDegrees = 20.0f;
  constexpr double kAperture = 0.1f;
  constexpr double kFocusDistance = 10.0f;
  camera = std::make_unique<Camera>(kPosition, kTarget, kAspectRatio, kVerticalFOVDegrees, kAperture, kFocusDistance);
}

void Scene::InitializeWorld() {
  std::shared_ptr<Material> ground = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
  world.AddCollidable(std::make_shared<Sphere>(Point3D(0, -1000, 0), 1000, ground));

  for (int i = -11; i < 11; ++i) {
    for (int j = -11; j < 11; ++j) {
      double random_double = RandomDouble();
      Point3D center(i + 0.9 * RandomDouble(), 0.2, j + 0.9 * RandomDouble());

      if ((center - Point3D(4.0, 0.2, 0.0)).Length() > 0.9) {
        std::shared_ptr<Material> material = nullptr;
        if (random_double < 0.8) {
          // Lambertian
          Color albedo = Color::Random() * Color::Random();
          material = std::make_shared<Lambertian>(albedo);
        } else if (random_double < 0.95) {
          // Metal
          Color albedo = Color::Random(0.5, 1.0);
          double fuzziness = RandomDouble(0.0, 0.5);
          material = std::make_shared<Metal>(albedo, fuzziness);
        } else {
          // Dielectric
          material = std::make_shared<Dielectric>(1.5);
        }
        world.AddCollidable(std::make_shared<Sphere>(center, 0.2, material));
      }
    }
  }

  std::shared_ptr<Material> glass = std::make_shared<Dielectric>(1.5);
  world.AddCollidable(std::make_shared<Sphere>(Point3D(0.0, 1.0, 0.0), 1.0, glass));

  std::shared_ptr<Material> lambertian = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
  world.AddCollidable(std::make_shared<Sphere>(Point3D(-4.0, 1.0, 0.0), 1.0, lambertian));

  std::shared_ptr<Material> metal = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
  world.AddCollidable(std::make_shared<Sphere>(Point3D(4.0, 1.0, 0.0), 1.0, metal));
}
