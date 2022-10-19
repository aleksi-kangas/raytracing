#include "scene.h"

#include <stdexcept>
#include <variant>

#include "material.h"
#include "random.h"
#include "texture.h"

namespace rt {
Scene::Scene(SceneType scene_type, float aspect_ratio, BVHSplitStrategy bvh_split_strategy)
    : aspect_ratio_{aspect_ratio}, bvh_split_strategy_{bvh_split_strategy} {
  switch (scene_type) {
    case SceneType::Part3Section10:
      InitializePart3Section10();
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

collidable_t* Scene::Light() const {
  return light_.get();
}

void Scene::InitializePart3Section10() {
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

  collidables_.emplace_back(Flip{RectangleXZ{glm::vec2{213.0f, 343.0f}, glm::vec2{227.0f, 332.0f}, 554.0f,
                                             DiffuseLight{glm::vec3{15.0f, 15.0f, 15.0f}}}});

  collidables_.emplace_back(Transform{Box{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{165.0f, 330.0f, 165.0f},
                                          Lambertian{SolidColorTexture{0.73f, 0.73f, 0.73f}}},
                                      15.0f, glm::vec3{265.0f, 0.0f, 295.0f}});

  collidables_.emplace_back(Transform{Box{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{165.0f, 165.0f, 165.0f},
                                          Lambertian{SolidColorTexture{0.73f, 0.73f, 0.73f}}},
                                      -18.0f, glm::vec3{130.0f, 0.0f, 65.0f}});

  bvh_ = std::make_unique<BVH>(bvh_split_strategy_, collidables_, 0.0f, 1.0f);
  light_ = std::make_unique<collidable_t>(RectangleXZ{glm::vec2{213.0f, 343.0f}, glm::vec2{227.0f, 332.0f}, 554.0f,
                                                      DiffuseLight{glm::vec3{15.0f, 15.0f, 15.0f}}});
}

}  // namespace rt
