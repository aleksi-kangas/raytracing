#pragma once

#include <memory>
#include <vector>

#include "glm/glm.hpp"

#include "box.h"
#include "bvh.h"
#include "camera.h"
#include "constant_medium.h"
#include "flip.h"
#include "rectangle.h"
#include "sphere.h"
#include "transform.h"

namespace rt {

enum SceneType {
  Part3Section10 = 0,
};

static const char* kSceneNames[] = {"Part 3-10 | Mixture Densities"};

class Scene {
 public:
  Scene(SceneType scene_type, float aspect_ratio, BVHSplitStrategy bvh_split_strategy);
  Scene(const Scene& scene) = delete;
  Scene& operator=(const Scene& scene) = delete;

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

  [[nodiscard]] Camera* GetCamera() const;
  [[nodiscard]] glm::vec3 BackgroundColor() const;
  [[nodiscard]] collidable_t* Light() const;

 private:
  float aspect_ratio_ = 1.0f;
  glm::vec3 background_color_{0, 0, 0};
  BVHSplitStrategy bvh_split_strategy_ = BVHSplitStrategy::SurfaceAreaHeuristic;

  std::unique_ptr<Camera> camera_;
  collidable_container_t collidables_;
  std::unique_ptr<BVH> bvh_;
  std::unique_ptr<collidable_t> light_;

  void InitializePart3Section10();
};
}  // namespace rt
