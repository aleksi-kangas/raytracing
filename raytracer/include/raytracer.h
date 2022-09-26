#pragma once

#include <memory>

#include "image.h"
#include "renderer.h"
#include "scene.h"

namespace rt {
class Raytracer {
 public:
  void Run();

 private:
  Renderer renderer_;
  RendererSettings renderer_settings_;

  int32_t viewport_width_ = 0, viewport_height_ = 0;

  void RenderUI();
  void RenderUISettings();
  void RenderUIStatistics();
  void RenderPreview();

  void OnRender();
};
}  // namespace rt
