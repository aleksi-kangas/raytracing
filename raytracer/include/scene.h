#pragma once

#include <memory>
#include <vector>

#include "camera.h"
#include "material.h"
#include "sphere.h"

namespace rt {

enum SceneType {
  Part1Section12 = 0,
};

static const char* kSceneNames[] = {"Part 1-12 | Defocus Blur"};

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

  void InitializePart1Section12(float aspect_ratio);
};
}  // namespace rt
