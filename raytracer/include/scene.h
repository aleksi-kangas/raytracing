#pragma once

#include <memory>
#include <vector>

#include "bvh.h"
#include "camera.h"
#include "material.h"
#include "sphere.h"

namespace rt {

enum SceneType {
  Part1Section13 = 0,
  Part1Section13BVH = 1,
  Part2Section3 = 2,
};

static const char* kSceneNames[] = {"Part 1-13 | Where Next? - Final Render",
                                    "Part 1-13 | Where Next? - Final Render (BVH)",
                                    "Part 2-3 | Bounding Volume Hierarchies"};

class Scene {
 public:
  Scene(SceneType scene_type,
        float aspect_ratio,
        BVHSplitStrategy bvh_split_strategy,
        BVHTraversalStrategy bvh_traversal_strategy);
  Scene(const Scene& scene) = delete;
  Scene& operator=(const Scene& scene) = delete;

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

  [[nodiscard]] Camera* GetCamera() const;

 private:
  float aspect_ratio_ = 1.0f;
  BVHSplitStrategy bvh_split_strategy_ = BVHSplitStrategy::SurfaceAreaHeuristic;
  BVHTraversalStrategy bvh_traversal_strategy_ = BVHTraversalStrategy::Iterative;

  std::unique_ptr<Camera> camera_;
  std::vector<std::shared_ptr<Material>> materials_;
  std::vector<Sphere> spheres_;
  std::unique_ptr<BVH<Sphere>> bvh_spheres_;
  std::vector<MovingSphere> moving_spheres_;
  std::unique_ptr<BVH<MovingSphere>> bvh_moving_spheres_;

  void InitializePart1Section13();
  void InitializePart1Section13BVH();
  void InitializePart2Section3();
};
}  // namespace rt
