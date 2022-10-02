#include "scene.h"

#include <stdexcept>
#include <variant>

#include "dielectric.h"
#include "metal.h"
#include "lambertian.h"
#include "random.h"

namespace rt {
Scene::Scene(SceneType scene_type,
             float aspect_ratio,
             BVHSplitStrategy bvh_split_strategy,
             BVHTraversalStrategy bvh_traversal_strategy)
    : aspect_ratio_{aspect_ratio},
      bvh_split_strategy_{bvh_split_strategy},
      bvh_traversal_strategy_{bvh_traversal_strategy} {
  switch (scene_type) {
    case SceneType::Part1Section13:
      InitializePart1Section13();
      break;
    case SceneType::Part1Section13BVH:
      InitializePart1Section13BVH();
      break;
    case SceneType::Part2Section4Subsection3:
      InitializePart2Section4Subsection3();
      break;
    case SceneType::Part2Section4Subsection4:
      InitializePart2Section4Subsection4();
      break;
    case SceneType::Part2Section5:
      InitializePart2Section5();
      break;
    case SceneType::Part2Section6:
      InitializePart2Section6();
      break;
    default:
      throw std::runtime_error{"Unknown scene."};
  }
}

bool Scene::Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const {
  if (bvh_) {
    return bvh_->Collide(ray, t_min, t_max, collision);
  } else {
    bool collided = false;
    float closest = t_max;
    for (const auto& collidable : collidables_) {
      if (std::visit([&](const auto& object) { return object.Collide(ray, t_min, closest, collision); }, collidable)) {
        collided = true;
        closest = collision.t;
      }
    }
    return collided;
  }
}

Camera* Scene::GetCamera() const {
  return camera_.get();
}

void Scene::InitializePart1Section13() {
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
                                     aspect_ratio_,
                                     camera_aperture,
                                     camera_focus_distance);

  auto* ground_texture = textures_.emplace_back(new SolidColor{0.5f, 0.5f, 0.5f}).get();
  auto* ground_material = materials_.emplace_back(new Lambertian(ground_texture)).get();
  collidables_.emplace_back(Sphere{glm::vec3{0.0f, -1000.0f, 0.0f}, 1000.0f, ground_material});

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
          auto* solid_color_texture = textures_.emplace_back(new SolidColor{albedo}).get();
          auto* material = materials_.emplace_back(new Lambertian(solid_color_texture)).get();
          collidables_.emplace_back(Sphere{center, 0.2f, material});
        } else if (random_float < 0.95f) {  // Metal
          const glm::vec3 albedo = random::Vec3(0.5f, 1.0f);
          const float fuzziness = random::Float(0.0f, 0.5f);
          auto* material = materials_.emplace_back(new Metal(albedo, fuzziness)).get();
          collidables_.emplace_back(Sphere{center, 0.2f, material});
        } else {  // Dielectric
          auto* material = materials_.emplace_back(new Dielectric(1.5f)).get();
          collidables_.emplace_back(Sphere{center, 0.2f, material});
        }
      }
    }
  }

  {
    auto* material = materials_.emplace_back(new Dielectric(1.5f)).get();
    collidables_.emplace_back(Sphere{glm::vec3{0.0f, 1.0f, 0.0f}, 1.0f, material});
  }
  {
    auto* solid_color_texture = textures_.emplace_back(new SolidColor{0.4f, 0.2f, 0.1f}).get();
    auto* material = materials_.emplace_back(new Lambertian(solid_color_texture)).get();
    collidables_.emplace_back(Sphere{glm::vec3{-4.0f, 1.0f, 0.0f}, 1.0f, material});
  }
  {
    auto* material = materials_.emplace_back(new Metal({0.7f, 0.6f, 0.5f}, 0.0f)).get();
    collidables_.emplace_back(Sphere{glm::vec3{4.0f, 1.0f, 0.0f}, 1.0f, material});
  }
}

void Scene::InitializePart1Section13BVH() {
  InitializePart1Section13();
  bvh_ = std::make_unique<BVH<collidable_t>>(bvh_split_strategy_, bvh_traversal_strategy_, collidables_, 0.0f, 1.0f);
}

void Scene::InitializePart2Section4Subsection3() {
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
                                     aspect_ratio_,
                                     camera_aperture,
                                     camera_focus_distance,
                                     0.0f,
                                     1.0f);

  auto* checker_even = textures_.emplace_back(new SolidColor{0.2f, 0.3f, 0.1f}).get();
  auto* checker_odd = textures_.emplace_back(new SolidColor{0.9f, 0.9f, 0.9f}).get();
  auto* ground_texture = textures_.emplace_back(new Checker{checker_even, checker_odd}).get();
  auto* ground_material = materials_.emplace_back(new Lambertian(ground_texture)).get();
  collidables_.emplace_back(Sphere{glm::vec3{0.0f, -1000.0f, 0.0f}, 1000.0f, ground_material});

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
          const glm::vec3 center2 = center + glm::vec3{0.0f, random::Float(0.0f, 0.5f), 0.0f};
          auto* solid_color_texture = textures_.emplace_back(new SolidColor{albedo}).get();
          auto* material = materials_.emplace_back(new Lambertian(solid_color_texture)).get();
          collidables_.emplace_back(MovingSphere{center, center2, 0.0f, 1.0f, 0.2f, material});
        } else if (random_float < 0.95f) {  // Metal
          const glm::vec3 albedo = random::Vec3(0.5f, 1.0f);
          const float fuzziness = random::Float(0.0f, 0.5f);
          auto* material = materials_.emplace_back(new Metal(albedo, fuzziness)).get();
          collidables_.emplace_back(Sphere{center, 0.2f, material});
        } else {  // Dielectric
          auto* material = materials_.emplace_back(new Dielectric(1.5f)).get();
          collidables_.emplace_back(Sphere{center, 0.2f, material});
        }
      }
    }
  }

  {
    auto* material = materials_.emplace_back(new Dielectric(1.5f)).get();
    collidables_.emplace_back(Sphere{glm::vec3{0.0f, 1.0f, 0.0f}, 1.0f, material});
  }
  {
    auto* solid_color_texture = textures_.emplace_back(new SolidColor{0.4f, 0.2f, 0.1f}).get();
    auto* material = materials_.emplace_back(new Lambertian(solid_color_texture)).get();
    collidables_.emplace_back(Sphere{glm::vec3{-4.0f, 1.0f, 0.0f}, 1.0f, material});
  }
  {
    auto* material = materials_.emplace_back(new Metal({0.7f, 0.6f, 0.5f}, 0.0f)).get();
    collidables_.emplace_back(Sphere{glm::vec3{4.0f, 1.0f, 0.0f}, 1.0f, material});
  }

  bvh_ = std::make_unique<BVH<collidable_t>>(bvh_split_strategy_, bvh_traversal_strategy_, collidables_, 0.0f, 1.0f);
}

void Scene::InitializePart2Section4Subsection4() {
  constexpr glm::vec3 camera_origin{13, 2, 3};
  constexpr glm::vec3 camera_target{0, 0, 0};
  constexpr glm::vec3 camera_vup{0, 1, 0};
  constexpr float camera_fov = 20.0f;
  constexpr float camera_aperture = 0.0f;
  constexpr float camera_focus_distance = 10.0f;

  camera_ = std::make_unique<Camera>(camera_origin,
                                     camera_target,
                                     camera_vup,
                                     camera_fov,
                                     aspect_ratio_,
                                     camera_aperture,
                                     camera_focus_distance,
                                     0.0f,
                                     1.0f);

  auto* checker_even = textures_.emplace_back(new SolidColor{0.2f, 0.3f, 0.1f}).get();
  auto* checker_odd = textures_.emplace_back(new SolidColor{0.9f, 0.9f, 0.9f}).get();
  auto* checker_texture = textures_.emplace_back(new Checker{checker_even, checker_odd}).get();
  auto* lambertian = materials_.emplace_back(new Lambertian(checker_texture)).get();
  collidables_.emplace_back(Sphere{glm::vec3{0.0f, -10.0f, 0.0f}, 10.0f, lambertian});
  collidables_.emplace_back(Sphere{glm::vec3{0.0f, 10.0f, 0.0f}, 10.0f, lambertian});

  bvh_ = std::make_unique<BVH<collidable_t>>(bvh_split_strategy_, bvh_traversal_strategy_, collidables_, 0.0f, 1.0f);
}

void Scene::InitializePart2Section5() {
  constexpr glm::vec3 camera_origin{13, 2, 3};
  constexpr glm::vec3 camera_target{0, 0, 0};
  constexpr glm::vec3 camera_vup{0, 1, 0};
  constexpr float camera_fov = 20.0f;
  constexpr float camera_aperture = 0.0f;
  constexpr float camera_focus_distance = 10.0f;

  camera_ = std::make_unique<Camera>(camera_origin,
                                     camera_target,
                                     camera_vup,
                                     camera_fov,
                                     aspect_ratio_,
                                     camera_aperture,
                                     camera_focus_distance,
                                     0.0f,
                                     1.0f);

  auto* noise_texture = textures_.emplace_back(new Noise{4.0f}).get();
  auto* lambertian = materials_.emplace_back(new Lambertian{noise_texture}).get();
  collidables_.emplace_back(Sphere{glm::vec3{0.0f, -1000.0f, 0.0f}, 1000.0f, lambertian});
  collidables_.emplace_back(Sphere{glm::vec3{0.0f, 2.0f, 0.0f}, 2.0f, lambertian});

  bvh_ = std::make_unique<BVH<collidable_t>>(bvh_split_strategy_, bvh_traversal_strategy_, collidables_, 0.0f, 1.0f);
}

void Scene::InitializePart2Section6() {
  constexpr glm::vec3 camera_origin{13, 2, 3};
  constexpr glm::vec3 camera_target{0, 0, 0};
  constexpr glm::vec3 camera_vup{0, 1, 0};
  constexpr float camera_fov = 20.0f;
  constexpr float camera_aperture = 0.0f;
  constexpr float camera_focus_distance = 10.0f;

  camera_ = std::make_unique<Camera>(camera_origin,
                                     camera_target,
                                     camera_vup,
                                     camera_fov,
                                     aspect_ratio_,
                                     camera_aperture,
                                     camera_focus_distance,
                                     0.0f,
                                     1.0f);

  auto* earth_texture = textures_.emplace_back(new ImageTexture{"resources/textures/earthmap.png"}).get();
  auto* earth_surface = materials_.emplace_back(new Lambertian{earth_texture}).get();
  collidables_.emplace_back(Sphere{glm::vec3{0.0f, 0.0f, 0.0f}, 2.0f, earth_surface});
  bvh_ = std::make_unique<BVH<collidable_t>>(bvh_split_strategy_, bvh_traversal_strategy_, collidables_, 0.0f, 1.0f);
}

}  // namespace rt
