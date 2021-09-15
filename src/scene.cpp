#include "scene.h"

#include <filesystem>
#include <memory>

#include "box.h"
#include "bvh.h"
#include "constant_medium.h"
#include "flip_face.h"
#include "material.h"
#include "moving_sphere.h"
#include "random.h"
#include "rectangle.h"
#include "rotate.h"
#include "sphere.h"
#include "texture.h"
#include "translate.h"
#include "utils.h"

Scene::Scene(int image_width, int image_height, int samples_per_pixel)
    : image_width(image_width), image_height(image_height), samples_per_pixel(samples_per_pixel) {}

void Scene::InitializeCamera() {
  const Point3D kPosition{278, 278, -800};
  const Point3D kTarget{278, 278, 0};
  const double kAspectRatio = static_cast<double>(image_width) / static_cast<double>(image_height);
  constexpr double kVerticalFOVDegrees = 40.0f;
  constexpr double kAperture = 0.0f;
  constexpr double kFocusDistance = 10.0f;
  camera = std::make_unique<Camera>(
      kPosition, kTarget, kAspectRatio, kVerticalFOVDegrees, kAperture, kFocusDistance, 0.0, 1.0);
}

void Scene::InitializeWorld() {
  std::shared_ptr<Material> red = std::make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
  std::shared_ptr<Material> green = std::make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
  std::shared_ptr<Material> white = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
  std::shared_ptr<Material> light_material = std::make_shared<DiffuseLight>(Color(15, 15, 15));

  world.AddCollidable(std::make_shared<RectangleYZ>(0, 555, 0, 555, 555, green));
  world.AddCollidable(std::make_shared<RectangleYZ>(0, 555, 0, 555, 0, red));
  world.AddCollidable(std::make_shared<RectangleXZ>(0, 555, 0, 555, 555, white));
  world.AddCollidable(std::make_shared<RectangleXZ>(0, 555, 0, 555, 0, white));
  world.AddCollidable(std::make_shared<RectangleXY>(0, 555, 0, 555, 555, white));
  world.AddCollidable(std::make_shared<FlipFace>(
      std::make_shared<RectangleXZ>(213, 343, 227, 332, 554, light_material)));

  {
    std::shared_ptr<Collidable> box = std::make_shared<Box>(Point3D(0, 0, 0), Point3D(165, 330, 165), white);
    box = std::make_shared<RotateY>(box, 15.0);
    box = std::make_shared<Translate>(box, Vector3D(265, 0, 295));
    world.AddCollidable(box);
  }
  {
    std::shared_ptr<Collidable> box = std::make_shared<Box>(Point3D(0, 0, 0), Point3D(165, 165, 165), white);
    box = std::make_shared<RotateY>(box, -18.0);
    box = std::make_shared<Translate>(box, Vector3D(130, 0, 65));
    world.AddCollidable(box);
  }

  light = std::make_shared<RectangleXZ>(213, 343, 227, 332, 554, std::make_shared<Material>());
}
