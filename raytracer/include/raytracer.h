#pragma once

#include <memory>

#include "image.h"
#include "renderer.h"

namespace rt {
class Raytracer {
 public:
  void Run();

 private:
  Renderer renderer_;
  int32_t viewport_width_ = 0, viewport_height_ = 0;

  float render_time_ms_ = 0.0f;

  void RenderUI();
  void Render();
};
}  // namespace rt
