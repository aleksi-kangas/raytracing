#pragma once

#include <memory>
#include <vector>

#include "camera.h"
#include "material.h"
#include "sphere.h"

namespace rt {

enum SceneType {
  Part1Section11Subsection1 = 0,
  Part1Section11Subsection2Far = 1,
  Part1Section11Subsection2Close = 2,
};

static const char* kSceneNames[] = {"Part 1-11-1 | Positionable Camera",
                                    "Part 1-11-2 | Positionable Camera (Far)",
                                    "Part 1-11-2 | Positionable Camera (Close)"};

class Scene {
 public:
  Scene(SceneType scene_type, float aspect_ratio);
  Scene(const Scene& scene) = delete;
  Scene& operator=(const Scene& scene) = delete;

  bool Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const;

  [[nodiscard]] Camera* GetCamera() const;

 private:
  std::unique_ptr<Camera> camera_;
  std::vector<std::shared_ptr<Material>> materials_;
  std::vector<Sphere> spheres_;

  void InitializePart1Section11Subsection1(float aspect_ratio);
  void InitializePart1Section11Subsection2Far(float aspect_ratio);
  void InitializePart1Section11Subsection2Close(float aspect_ratio);
};
}  // namespace rt
