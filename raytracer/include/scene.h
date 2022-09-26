#pragma once

#include <memory>
#include <vector>

#include "material.h"
#include "sphere.h"

namespace rt {

enum SceneType {
  Part1Section9 = 0,
  Part1Section9Fuzzy = 1,
};

static const char* kSceneNames[] = {"Part 1-9 | Metal",
                                    "Part 1-9 | Metal Fuzziness"};

class Scene {
 public:
  explicit Scene(SceneType scene_type);

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

 private:
  std::vector<std::shared_ptr<Material>> materials_;
  std::vector<Sphere> spheres_;

  void InitializePart1Section9();
  void InitializePart1Section9Fuzzy();
};
}  // namespace rt
