#include "scene.h"

#include <stdexcept>

#include "dielectric.h"
#include "metal.h"
#include "lambertian.h"

namespace rt {
Scene::Scene(SceneType scene_type) {
  switch (scene_type) {
    case SceneType::Part1Section10:
      InitializePart1Section10();
      break;
    default:
      throw std::runtime_error{"Unknown scene."};
  }
}

bool Scene::Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const {
  bool collided = false;
  float closest = t_max;
  for (const auto& sphere : spheres_) {
    if (sphere.Collide(ray, t_min, closest, collision)) {
      closest = collision.t;
      collided = true;
    }
  }
  return collided;
}

void Scene::InitializePart1Section10() {
  auto ground_material = materials_.emplace_back(new Lambertian({0.8f, 0.8f, 0.0f})).get();
  spheres_.push_back(Sphere{{0, -100.5f, -1}, 100, ground_material});
  auto center_material = materials_.emplace_back(new Lambertian({0.1f, 0.2f, 0.5f})).get();
  spheres_.push_back(Sphere{{0, 0, -1}, 0.5f, center_material});
  auto left_material = materials_.emplace_back(new Dielectric(1.5f)).get();
  spheres_.push_back(Sphere{{-1, 0, -1}, 0.5f, left_material});
  spheres_.push_back(Sphere{{-1, 0, -1}, -0.4f, left_material});
  auto right_material = materials_.emplace_back(new Metal({0.8f, 0.6f, 0.2f}, 0.0f)).get();
  spheres_.push_back(Sphere{{1, 0, -1}, 0.5f, right_material});
}

}  // namespace rt
