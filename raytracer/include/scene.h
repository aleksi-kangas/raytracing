#pragma once

#include <memory>
#include <vector>

#include "material.h"
#include "sphere.h"

namespace rt {

enum SceneType {
  Part1Section10 = 0,
};

static const char* kSceneNames[] = {"Part 1-10 | Dielectrics"};

class Scene {
 public:
  explicit Scene(SceneType scene_type);

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

 private:
  std::vector<std::shared_ptr<Material>> materials_;
  std::vector<Sphere> spheres_;

  void InitializePart1Section10();
};
}  // namespace rt
