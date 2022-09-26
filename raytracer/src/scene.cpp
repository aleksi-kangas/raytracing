#include "scene.h"

#include <stdexcept>

#include "metal.h"
#include "lambertian.h"

namespace rt {
Scene::Scene(SceneType scene_type) {
  switch (scene_type) {
    case SceneType::Part1Section9:
      InitializePart1Section9();
      break;
    case SceneType::Part1Section9Fuzzy:
      InitializePart1Section9Fuzzy();
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

void Scene::InitializePart1Section9() {
  auto ground_material = materials_.emplace_back(new Lambertian({0.8f, 0.8f, 0.0f})).get();
  Sphere ground{{0, -100.5f, -1}, 100, ground_material};
  spheres_.push_back(ground);
  auto center_material = materials_.emplace_back(new Lambertian({0.7f, 0.3f, 0.3f})).get();
  Sphere center{{0, 0, -1}, 0.5f, center_material};
  spheres_.push_back(center);
  auto left_material = materials_.emplace_back(new Metal({0.8f, 0.8f, 0.8f}, 0.0f)).get();
  Sphere left{{-1, 0, -1}, 0.5f, left_material};
  spheres_.push_back(left);
  auto right_material = materials_.emplace_back(new Metal({0.8f, 0.6f, 0.2f}, 0.0f)).get();
  Sphere right{{1, 0, -1}, 0.5f, right_material};
  spheres_.push_back(right);
}

void Scene::InitializePart1Section9Fuzzy() {
  auto ground_material = materials_.emplace_back(new Lambertian({0.8f, 0.8f, 0.0f})).get();
  Sphere ground{{0, -100.5f, -1}, 100, ground_material};
  spheres_.push_back(ground);
  auto center_material = materials_.emplace_back(new Lambertian({0.7f, 0.3f, 0.3f})).get();
  Sphere center{{0, 0, -1}, 0.5f, center_material};
  spheres_.push_back(center);
  auto left_material = materials_.emplace_back(new Metal({0.8f, 0.8f, 0.8f}, 0.3f)).get();
  Sphere left{{-1, 0, -1}, 0.5f, left_material};
  spheres_.push_back(left);
  auto right_material = materials_.emplace_back(new Metal({0.8f, 0.6f, 0.2f}, 1.0f)).get();
  Sphere right{{1, 0, -1}, 0.5f, right_material};
  spheres_.push_back(right);
}

}  // namespace rt
