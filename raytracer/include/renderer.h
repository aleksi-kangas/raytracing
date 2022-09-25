#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "glm/glm.hpp"

#include "image.h"
#include "ray.h"

namespace rt {
class Renderer {
 public:
  Renderer() = default;

  void OnResize(int32_t width, int32_t height);

  void Render();

  [[nodiscard]] std::shared_ptr<Image> GetResult() const;

 private:
  std::shared_ptr<Image> image_;
  std::vector<uint32_t> image_data_;

  void RenderChuck(glm::i32vec2 rows, glm::i32vec2 columns);

  static uint32_t RenderPixel(const Ray& ray);
};

}  // namespace rt
