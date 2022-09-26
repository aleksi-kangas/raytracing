#include "scene.h"

#include <stdexcept>

#include "dielectric.h"
#include "metal.h"
#include "lambertian.h"

namespace rt {
Scene::Scene(SceneType scene_type, float aspect_ratio) {
  switch (scene_type) {
    case SceneType::Part1Section11Subsection1:
      InitializePart1Section11Subsection1(aspect_ratio);
      break;
    case SceneType::Part1Section11Subsection2Far:
      InitializePart1Section11Subsection2Far(aspect_ratio);
      break;
    case SceneType::Part1Section11Subsection2Close:
      InitializePart1Section11Subsection2Close(aspect_ratio);
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

void Scene::InitializePart1Section11Subsection1(float aspect_ratio) {
  camera_ = std::make_unique<Camera>(glm::vec3{0, 0, 0}, glm::vec3{0, 0, -1}, glm::vec3{0, 1, 0}, 90.0f, aspect_ratio);

  const float kRadius = std::cos(3.14159265358979323846f / 4.0f);
  auto* left_material = materials_.emplace_back(new Lambertian({0, 0, 1})).get();
  spheres_.emplace_back(glm::vec3{-kRadius, 0, -1}, kRadius, left_material);
  auto* right_material = materials_.emplace_back(new Lambertian({1, 0, 0})).get();
  spheres_.emplace_back(glm::vec3{kRadius, 0, -1}, kRadius, right_material);
}

void Scene::InitializePart1Section11Subsection2Far(float aspect_ratio) {
  camera_ = std::make_unique<Camera>(glm::vec3{-2, 2, 1}, glm::vec3{0, 0, -1}, glm::vec3{0, 1, 0}, 90.0f, aspect_ratio);

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

void Scene::InitializePart1Section11Subsection2Close(float aspect_ratio) {
  camera_ = std::make_unique<Camera>(glm::vec3{-2, 2, 1}, glm::vec3{0, 0, -1}, glm::vec3{0, 1, 0}, 20.0f, aspect_ratio);

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
