#include "scene.h"

#include <filesystem>
#include <memory>

#include "box.h"
#include "bvh.h"
#include "constant_medium.h"
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
  const Point3D kPosition{478, 278, -600};
  const Point3D kTarget{278, 278, 0};
  const double kAspectRatio = static_cast<double>(image_width) / static_cast<double>(image_height);
  constexpr double kVerticalFOVDegrees = 40.0f;
  constexpr double kAperture = 0.0f;
  constexpr double kFocusDistance = 10.0f;
  camera = std::make_unique<Camera>(
      kPosition, kTarget, kAspectRatio, kVerticalFOVDegrees, kAperture, kFocusDistance, 0.0, 1.0);
}

void Scene::InitializeWorld() {
  {
    std::shared_ptr<Material> diffuse_light = std::make_shared<DiffuseLight>(Color(7, 7, 7));
    world.AddCollidable(std::make_shared<RectangleXZ>(123, 423, 147, 412, 554, diffuse_light));
  }
  {
    Point3D center0 = Point3D(400, 400, 200);
    Point3D center1 = center0 + Vector3D(30, 0, 0);
    std::shared_ptr<Material> material = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    world.AddCollidable(std::make_shared<MovingSphere>(center0, center1, 0, 1, 50, material));
  }
  {
    std::shared_ptr<Material> dielectric = std::make_shared<Dielectric>(1.5);
    world.AddCollidable(std::make_shared<Sphere>(Point3D(260, 150, 45), 50, dielectric));
  }
  {
    std::shared_ptr<Material> metal = std::make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0);
    world.AddCollidable(std::make_shared<Sphere>(Point3D(0, 150, 145), 50, metal));
  }
  {
    std::shared_ptr<Material> dielectric = std::make_shared<Dielectric>(1.5);
    std::shared_ptr<Collidable> boundary = std::make_shared<Sphere>(Point3D(360, 150, 145), 70, dielectric);
    world.AddCollidable(boundary);
    world.AddCollidable(std::make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary = std::make_shared<Sphere>(Point3D(0, 0, 0), 5000, dielectric);
    world.AddCollidable(std::make_shared<ConstantMedium>(boundary, 0.0001, Color(1, 1, 1)));
  }
  {
    std::shared_ptr<Texture> earth_texture = std::make_shared<ImageTexture>("resources/textures/earth_map.png");
    std::shared_ptr<Material> material = std::make_shared<Lambertian>(earth_texture);
    world.AddCollidable(std::make_shared<Sphere>(Point3D(400, 200, 400), 100, material));
  }
  {
    std::shared_ptr<Texture> noise_texture = std::make_shared<NoiseTexture>(0.1);
    std::shared_ptr<Material> material = std::make_shared<Lambertian>(noise_texture);
    world.AddCollidable(std::make_shared<Sphere>(Point3D(220, 280, 300), 80, material));
  }
  {
    Collidables boxes;
    std::shared_ptr<Material> ground = std::make_shared<Lambertian>(Color(0.48, 0.83, 0.53));
    constexpr int kBoxesPerSide = 20;
    for (int i = 0; i < kBoxesPerSide; ++i) {
      for (int j = 0; j < kBoxesPerSide; ++j) {
        double width = 100.0;
        double x0 = -1000.0 + i * width;
        double x1 = x0 + width;
        double z0 = -1000.0 + j * width;
        double z1 = z0 + width;
        double y0 = 0.0;
        double y1 = RandomDouble(1, 101);
        boxes.AddCollidable(std::make_shared<Box>(Point3D(x0, y0, z0), Point3D(x1, y1, z1), ground));
      }
    }
    world.AddCollidable(std::make_shared<BoundingVolumeHierarchyNode>(boxes, 0.0, 1.0));
  }
  {
    Collidables spheres;
    std::shared_ptr<Material> white = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    constexpr int kNumberOfSpheres = 1000;
    for (int i = 0; i < kNumberOfSpheres; ++i) {
      spheres.AddCollidable(std::make_shared<Sphere>(Point3D::Random(0, 165), 10, white));
    }
    std::shared_ptr<Collidable> bvh = std::make_shared<BoundingVolumeHierarchyNode>(spheres, 0.0, 1.0);
    bvh = std::make_shared<RotateY>(bvh, 15.0);
    bvh = std::make_shared<Translate>(bvh, Vector3D(-100, 270, 395));
    world.AddCollidable(bvh);
  }
}
