#include "scene.h"

#include <stdexcept>
#include <variant>

#include "material.h"
#include "random.h"
#include "texture.h"

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
    case SceneType::Part2Section7Subsection4:
      InitializePart2Section7Subsection4();
      break;
    case SceneType::Part2Section7Subsection6:
      InitializePart2Section7Subsection6();
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

glm::vec3 Scene::BackgroundColor() const {
  return background_color_;
}

void Scene::InitializePart1Section13() {
  background_color_ = {0.7f, 0.8f, 1.0f};

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

  collidables_.emplace_back(Sphere{glm::vec3{0.0f, -1000.0f, 0.0f}, 1000.0f,
                                   Lambertian{SolidColorTexture{0.5f, 0.5f, 0.5f}}});

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
          collidables_.emplace_back(Sphere{center, 0.2f, Lambertian{SolidColorTexture{albedo}}});
        } else if (random_float < 0.95f) {  // Metal
          const glm::vec3 albedo = random::Vec3(0.5f, 1.0f);
          const float fuzziness = random::Float(0.0f, 0.5f);
          collidables_.emplace_back(Sphere{center, 0.2f, Metal{albedo, fuzziness}});
        } else {  // Dielectric
          collidables_.emplace_back(Sphere{center, 0.2f, Dielectric{1.5f}});
        }
      }
    }
  }

  collidables_.emplace_back(Sphere{glm::vec3{0.0f, 1.0f, 0.0f}, 1.0f, Dielectric{1.5f}});
  collidables_.emplace_back(Sphere{glm::vec3{-4.0f, 1.0f, 0.0f}, 1.0f,
                                   Lambertian{SolidColorTexture{0.4f, 0.2f, 0.1f}}});
  collidables_.emplace_back(Sphere{glm::vec3{4.0f, 1.0f, 0.0f}, 1.0f, Metal{{0.7f, 0.6f, 0.5f}, 0.0f}});
}

void Scene::InitializePart1Section13BVH() {
  InitializePart1Section13();
  bvh_ = std::make_unique<BVH>(bvh_split_strategy_, bvh_traversal_strategy_, collidables_, 0.0f, 1.0f);
}

void Scene::InitializePart2Section4Subsection3() {
  background_color_ = {0.7f, 0.8f, 1.0f};

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

  collidables_.emplace_back(Sphere{glm::vec3{0.0f, -1000.0f, 0.0f}, 1000.0f, Lambertian{
      CheckerTexture{SolidColorTexture{0.2f, 0.3f, 0.1f}, SolidColorTexture{0.9f, 0.9f, 0.9f}}}});

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
          collidables_.emplace_back(MovingSphere{center, center2, 0.0f, 1.0f, 0.2f,
                                                 Lambertian{SolidColorTexture{albedo}}});
        } else if (random_float < 0.95f) {  // Metal
          const glm::vec3 albedo = random::Vec3(0.5f, 1.0f);
          const float fuzziness = random::Float(0.0f, 0.5f);
          collidables_.emplace_back(Sphere{center, 0.2f, Metal{albedo, fuzziness}});
        } else {  // Dielectric
          collidables_.emplace_back(Sphere{center, 0.2f, Dielectric{1.5f}});
        }
      }
    }
  }

  collidables_.emplace_back(Sphere{glm::vec3{0.0f, 1.0f, 0.0f}, 1.0f, Dielectric{0.5f}});
  collidables_.emplace_back(Sphere{glm::vec3{-4.0f, 1.0f, 0.0f}, 1.0f,
                                   Lambertian{SolidColorTexture{0.4f, 0.2f, 0.1f}}});
  collidables_.emplace_back(Sphere{glm::vec3{4.0f, 1.0f, 0.0f}, 1.0f, Metal{{0.7f, 0.6f, 0.5f}, 0.0f}});

  bvh_ = std::make_unique<BVH>(bvh_split_strategy_, bvh_traversal_strategy_, collidables_, 0.0f, 1.0f);
}

void Scene::InitializePart2Section4Subsection4() {
  background_color_ = {0.7f, 0.8f, 1.0f};

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

  collidables_.emplace_back(Sphere{glm::vec3{0.0f, -10.0f, 0.0f}, 10.0f, Lambertian{
      CheckerTexture{SolidColorTexture{0.2f, 0.3f, 0.1f}, SolidColorTexture{0.9f, 0.9f, 0.9f}}}});
  collidables_.emplace_back(Sphere{glm::vec3{0.0f, 10.0f, 0.0f}, 10.0f, Lambertian{
      CheckerTexture{SolidColorTexture{0.2f, 0.3f, 0.1f}, SolidColorTexture{0.9f, 0.9f, 0.9f}}}});

  bvh_ = std::make_unique<BVH>(bvh_split_strategy_, bvh_traversal_strategy_, collidables_, 0.0f, 1.0f);
}

void Scene::InitializePart2Section5() {
  background_color_ = {0.7f, 0.8f, 1.0f};

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

  collidables_.emplace_back(Sphere{glm::vec3{0.0f, -1000.0f, 0.0f}, 1000.0f, Lambertian{NoiseTexture{4.0f}}});
  collidables_.emplace_back(Sphere{glm::vec3{0.0f, 2.0f, 0.0f}, 2.0f, Lambertian{NoiseTexture{4.0f}}});

  bvh_ = std::make_unique<BVH>(bvh_split_strategy_, bvh_traversal_strategy_, collidables_, 0.0f, 1.0f);
}

void Scene::InitializePart2Section6() {
  background_color_ = {0.7f, 0.8f, 1.0f};

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

  collidables_.emplace_back(Sphere{glm::vec3{0.0f, 0.0f, 0.0f}, 2.0f,
                                   Lambertian{ImageTexture{"resources/textures/earthmap.png"}}});
  bvh_ = std::make_unique<BVH>(bvh_split_strategy_, bvh_traversal_strategy_, collidables_, 0.0f, 1.0f);
}

void Scene::InitializePart2Section7Subsection4() {
  background_color_ = {0.0f, 0.0f, 0.0f};

  constexpr glm::vec3 camera_origin{26, 3, 6};
  constexpr glm::vec3 camera_target{0, 2, 0};
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

  collidables_.emplace_back(Sphere{glm::vec3{0.0f, -1000.0f, 0.0f}, 1000.0f, Lambertian{NoiseTexture{4.0f}}});
  collidables_.emplace_back(Sphere{glm::vec3{0.0f, 2.0f, 0.0f}, 2.0f, Lambertian{NoiseTexture{4.0f}}});

  collidables_.emplace_back(RectangleXY{glm::vec2{3.0f, 5.0f}, glm::vec2{1.0f, 3.0f}, -2.0f,
                                        DiffuseLight{glm::vec3{4.0f, 4.0f, 4.0f}}});

  bvh_ = std::make_unique<BVH>(bvh_split_strategy_, bvh_traversal_strategy_, collidables_, 0.0f, 1.0f);
}

void Scene::InitializePart2Section7Subsection6() {
  background_color_ = {0.0f, 0.0f, 0.0f};

  constexpr glm::vec3 camera_origin{278, 278, -800};
  constexpr glm::vec3 camera_target{278, 278, 0};
  constexpr glm::vec3 camera_vup{0, 1, 0};
  constexpr float camera_fov = 40.0f;
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

  collidables_.emplace_back(RectangleYZ{glm::vec2{0.0f, 555.0f}, glm::vec2{0.0f, 555.0f}, 555.0f,
                                        Lambertian{SolidColorTexture{0.12f, 0.45f, 0.15f}}});
  collidables_.emplace_back(RectangleYZ{glm::vec2{0.0f, 555.0f}, glm::vec2{0.0f, 555.0f}, 0.0f,
                                        Lambertian{SolidColorTexture{0.65f, 0.05f, 0.05f}}});
  collidables_.emplace_back(RectangleXZ{glm::vec2{0.0f, 555.0f}, glm::vec2{0.0f, 555.0f}, 0.0f,
                                        Lambertian{SolidColorTexture{0.73f, 0.73f, 0.73f}}});
  collidables_.emplace_back(RectangleXZ{glm::vec2{0.0f, 555.0f}, glm::vec2{0.0f, 555.0f}, 555.0f,
                                        Lambertian{SolidColorTexture{0.73f, 0.73f, 0.73f}}});
  collidables_.emplace_back(RectangleXY{glm::vec2{0.0f, 555.0f}, glm::vec2{0.0f, 555.0f}, 555.0f,
                                        Lambertian{SolidColorTexture{0.73f, 0.73f, 0.73f}}});

  collidables_.emplace_back(RectangleXZ{glm::vec2{213.0f, 343.0f}, glm::vec2{227.0f, 332.0f}, 554.0f,
                                        DiffuseLight{glm::vec3{15.0f, 15.0f, 15.0f}}});

  bvh_ = std::make_unique<BVH>(bvh_split_strategy_, bvh_traversal_strategy_, collidables_, 0.0f, 1.0f);
}

}  // namespace rt
