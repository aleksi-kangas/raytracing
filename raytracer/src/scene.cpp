#include "scene.h"

#include <stdexcept>

#include "dielectric.h"
#include "metal.h"
#include "lambertian.h"
#include "random.h"

namespace rt {
Scene::Scene(SceneType scene_type, float aspect_ratio) {
  switch (scene_type) {
    case SceneType::Part1Section13:
      InitializePart1Section13(aspect_ratio);
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

void Scene::InitializePart1Section13(float aspect_ratio) {
  constexpr glm::vec3 camera_origin{13, 2, 3};
  constexpr glm::vec3 camera_target{0, 0, 0};
  constexpr glm::vec3 camera_vup{0, 1, 0};
  constexpr float camera_fov = 20.0f;
  constexpr float camera_aperture = 0.1f;
  constexpr float camera_focus_distance = 10.0f;

  camera_ = std::make_unique<Camera>(camera_origin,
                                     camera_target,
                                     camera_vup,
                                     camera_fov,
                                     aspect_ratio,
                                     camera_aperture,
                                     camera_focus_distance);

  auto* ground_material = materials_.emplace_back(new Lambertian({0.5f, 0.5f, 0.5f})).get();
  spheres_.emplace_back(glm::vec3{0, -1000, 0}, 1000, ground_material);

  for (int32_t i = -11; i < 11; ++i) {
    for (int32_t j = -11; j < 11; ++j) {
      const glm::vec3 center{
          static_cast<float>(i) + 0.9f * random::Float(),
          0.2f,
          static_cast<float>(j) + 0.9f * random::Float()};

      if (glm::length(center - glm::vec3{4.0f, 2.0f, 0.0f}) > 0.9f) {
        const float random_float = random::Float();
        if (random_float < 0.8f) {  // Lambertian
          const glm::vec3 albedo = random::Vec3() * random::Vec3();
          auto* material = materials_.emplace_back(new Lambertian(albedo)).get();
          spheres_.emplace_back(center, 0.2f, material);
        } else if (random_float < 0.95f) {  // Metal
          const glm::vec3 albedo = random::Vec3(0.5f, 1.0f);
          const float fuzziness = random::Float(0.0f, 0.5f);
          auto* material = materials_.emplace_back(new Metal(albedo, fuzziness)).get();
          spheres_.emplace_back(center, 0.2f, material);
        } else {  // Dielectric
          auto* material = materials_.emplace_back(new Dielectric(1.5f)).get();
          spheres_.emplace_back(center, 0.2f, material);
        }
      }
    }
  }

  {
    auto* material = materials_.emplace_back(new Dielectric(1.5f)).get();
    spheres_.emplace_back(glm::vec3{0.0f, 1.0f, 0.0f}, 1.0f, material);
  }
  {
    auto* material = materials_.emplace_back(new Lambertian({0.4f, 0.2f, 0.1f})).get();
    spheres_.emplace_back(glm::vec3{-4.0f, 1.0f, 0.0f}, 1.0f, material);
  }
  {
    auto* material = materials_.emplace_back(new Metal({0.7f, 0.6f, 0.5f}, 0.0f)).get();
    spheres_.emplace_back(glm::vec3{4.0f, 1.0f, 0.0f}, 1.0f, material);
  }
}

}  // namespace rt
