#include "scene.h"

#include <stdexcept>

#include "dielectric.h"
#include "metal.h"
#include "lambertian.h"

namespace rt {
Scene::Scene(SceneType scene_type, float aspect_ratio) {
  switch (scene_type) {
    case SceneType::Part1Section12:
      InitializePart1Section12(aspect_ratio);
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

Camera* Scene::GetCamera() const {
  return camera_.get();
}

void Scene::InitializePart1Section12(float aspect_ratio) {
  constexpr glm::vec3 camera_origin{3, 3, 2};
  constexpr glm::vec3 camera_target{0, 0, -1};
  constexpr glm::vec3 camera_vup{0, 1, 0};
  constexpr float camera_fov = 20.0f;
  constexpr float camera_aperture = 2.0f;
  const float camera_focus_distance = glm::length(camera_origin - camera_target);

  camera_ = std::make_unique<Camera>(camera_origin,
                                     camera_target,
                                     camera_vup,
                                     camera_fov,
                                     aspect_ratio,
                                     camera_aperture,
                                     camera_focus_distance);

  auto* ground_material = materials_.emplace_back(new Lambertian({0.8f, 0.8f, 0.0f})).get();
  spheres_.emplace_back(glm::vec3{0, -100.5f, -1}, 100, ground_material);
  auto* center_material = materials_.emplace_back(new Lambertian({0.1f, 0.2f, 0.5f})).get();
  spheres_.emplace_back(glm::vec3{0, 0, -1}, 0.5f, center_material);
  auto* left_material = materials_.emplace_back(new Dielectric(1.5f)).get();
  spheres_.emplace_back(glm::vec3{-1, 0, -1}, 0.5f, left_material);
  spheres_.emplace_back(glm::vec3{-1, 0, -1}, -0.4f, left_material);
  auto* right_material = materials_.emplace_back(new Metal({0.8f, 0.6f, 0.2f}, 0.0f)).get();
  spheres_.emplace_back(glm::vec3{1, 0, -1}, 0.5f, right_material);
}

}  // namespace rt
