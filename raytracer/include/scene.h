#pragma once

#include <map>
#include <vector>

#include "sphere.h"

namespace rt {

enum SceneType {
  Part1Section6 = 0,
  Part1Section7 = 1,
  Part1Section8 = 2,
};

static const char* kSceneNames[] = {"Part 1-6 | Surface Normals and Multiple Objects",
                                    "Part 1-7 | Antialiasing",
                                    "Part 1-8 | Diffuse Materials"};

class Scene {
 public:
  explicit Scene(SceneType scene_type);

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

 private:
  std::vector<Sphere> spheres_;

  void InitializePart1Section6();
};
}  // namespace rt
