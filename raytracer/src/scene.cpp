#include "scene.h"

#include <stdexcept>

namespace rt {
Scene::Scene(SceneType scene_type) {
  switch (scene_type) {
    case SceneType::Part1Section6:
    case SceneType::Part1Section7:  // No changes from the previous section.
      InitializePart1Section6();
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

void Scene::InitializePart1Section6() {
  Sphere ground{{0, -100.5, -1}, 100};
  spheres_.push_back(ground);
  Sphere sphere{{0, 0, -1}, 0.5};
  spheres_.push_back(sphere);
}

}  // namespace rt
