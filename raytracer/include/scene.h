#pragma once

#include <map>
#include <vector>

#include "sphere.h"

namespace rt {

enum class SceneType {
  Part1Section6 = 0,
};

static const char* kSceneNames[] = {"Part 1-6"};

class Scene {
 public:
  explicit Scene(SceneType scene_type);

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

 private:
  std::vector<Sphere> spheres_;

  void InitializePart1Section6();
};
}  // namespace rt
