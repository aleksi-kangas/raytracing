#pragma once

#include <memory>
#include <vector>

#include "glm/glm.hpp"

#include "box.h"
#include "bvh.h"
#include "camera.h"
#include "constant_medium.h"
#include "rectangle.h"
#include "sphere.h"
#include "rotate_translate.h"

namespace rt {

enum SceneType {
  Part1Section13 = 0,
  Part1Section13BVH = 1,
  Part2Section4Subsection3 = 2,
  Part2Section4Subsection4 = 3,
  Part2Section5 = 4,
  Part2Section6 = 5,
  Part2Section7Subsection4 = 6,
  Part2Section7Subsection6 = 7,
  Part2Section8Subsection0 = 8,
  Part2Section8Subsection2 = 9,
  Part2Section9 = 10,
  Part2Section10 = 11,
  Part3Section6 = 12,
};

static const char* kSceneNames[] = {"Part 1-13 | Where Next? - Final Render",
                                    "Part 1-13 | Where Next? - Final Render (BVH)",
                                    "Part 2-4-3 | Checker Texture",
                                    "Part 2-4-4 | Rendering a Scene with a Checkered Texture",
                                    "Part 2-5 | Perlin Noise",
                                    "Part 2-6 | Image Texture Mapping",
                                    "Part 2-7-4 | Turning Objects into Lights",
                                    "Part 2-7-6 | Creating an Empty 'Cornell Box",
                                    "Part 2-8-0 | Instances",
                                    "Part 2-8-2 | Translation and Rotation",
                                    "Part 2-9 | Volumes",
                                    "Part 2-10 | A Scene Testing All New Features",
                                    "Part 3-6 | Importance Sampling Materials"};

class Scene {
 public:
  Scene(SceneType scene_type, float aspect_ratio, BVHSplitStrategy bvh_split_strategy);
  Scene(const Scene& scene) = delete;
  Scene& operator=(const Scene& scene) = delete;

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

  [[nodiscard]] Camera* GetCamera() const;
  [[nodiscard]] glm::vec3 BackgroundColor() const;

 private:
  float aspect_ratio_ = 1.0f;
  glm::vec3 background_color_{0, 0, 0};
  BVHSplitStrategy bvh_split_strategy_ = BVHSplitStrategy::SurfaceAreaHeuristic;

  std::unique_ptr<Camera> camera_;
  collidable_container_t collidables_;
  std::unique_ptr<BVH> bvh_;

  void InitializePart1Section13();
  void InitializePart1Section13BVH();
  void InitializePart2Section4Subsection3();
  void InitializePart2Section4Subsection4();
  void InitializePart2Section5();
  void InitializePart2Section6();
  void InitializePart2Section7Subsection4();
  void InitializePart2Section7Subsection6();
  void InitializePart2Section8Subsection0();
  void InitializePart2Section8Subsection2();
  void InitializePart2Section9();
  void InitializePart2Section10();
  void InitializePart3Section6();
};
}  // namespace rt
