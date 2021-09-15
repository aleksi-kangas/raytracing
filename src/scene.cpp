#include "scene.h"

#include <memory>

#include "bvh.h"
#include "material.h"
#include "moving_sphere.h"
#include "random.h"
#include "sphere.h"
#include "texture.h"
#include "utils.h"

Scene::Scene(int image_width, int image_height, int samples_per_pixel)
    : image_width(image_width), image_height(image_height), samples_per_pixel(samples_per_pixel) {}

void Scene::InitializeCamera() {
  const Point3D kPosition{13, 2, 2};
  const Point3D kTarget{0, 0, 0};
  const double kAspectRatio = static_cast<double>(image_width) / static_cast<double>(image_height);
  constexpr double kVerticalFOVDegrees = 20.0f;
  constexpr double kAperture = 0.0f;
  constexpr double kFocusDistance = 10.0f;
  camera = std::make_unique<Camera>(
      kPosition, kTarget, kAspectRatio, kVerticalFOVDegrees, kAperture, kFocusDistance, 0.0, 1.0);
}

void Scene::InitializeWorld() {
  Collidables collidables;

  std::shared_ptr<Texture>
      checker_texture = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
  collidables.AddCollidable(std::make_shared<Sphere>(Point3D(0, -10, 0),
                                                     10,
                                                     std::make_shared<Lambertian>(checker_texture)));
  collidables.AddCollidable(std::make_shared<Sphere>(Point3D(0, 10, 0),
                                                     10,
                                                     std::make_shared<Lambertian>(checker_texture)));

  world.AddCollidable(std::make_shared<BoundingVolumeHierarchyNode>(collidables, 0.0, 1.0));
}
