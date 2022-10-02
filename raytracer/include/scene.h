#pragma once

#include <memory>
#include <vector>

#include "bvh.h"
#include "camera.h"
#include "material.h"
#include "sphere.h"
#include "texture.h"

namespace rt {

enum SceneType {
  Part1Section13 = 0,
  Part1Section13BVH = 1,
  Part2Section4Subsection3 = 2,
  Part2Section4Subsection4 = 3,
  Part2Section5 = 4,
  Part2Section6 = 5,
};

static const char* kSceneNames[] = {"Part 1-13 | Where Next? - Final Render",
                                    "Part 1-13 | Where Next? - Final Render (BVH)",
                                    "Part 2-4-3 | Checker Texture",
                                    "Part 2-4-4 | Rendering a Scene with a Checkered Texture",
                                    "Part 2-5 | Perlin Noise",
                                    "Part 2-6 | Image Texture Mapping"};

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
  std::vector<std::shared_ptr<Texture>> textures_;
  std::vector<std::shared_ptr<Material>> materials_;
  collidable_container_t collidables_;
  std::unique_ptr<BVH<collidable_t>> bvh_;

  void InitializePart1Section13();
  void InitializePart1Section13BVH();
  void InitializePart2Section4Subsection3();
  void InitializePart2Section4Subsection4();
  void InitializePart2Section5();
  void InitializePart2Section6();
};
}  // namespace rt
