#pragma once

#include <cstdint>
#include <memory>
#include <thread>
#include <vector>

#include "BS_thread_pool.hpp"
#include "glm/glm.hpp"

#include "image.h"
#include "ray.h"
#include "scene.h"

namespace rt {
enum RenderMode { ChunkByChunk = 0, RowByRow = 1 };

struct RendererSettings {
  int32_t scene_type = SceneType::Part1Section12;
  int32_t mode = RenderMode::ChunkByChunk;
  int32_t chunk_size = 64;
  int32_t samples_per_pixel = 100;
  int32_t child_rays = 50;
};

struct RendererStatistics {
  int32_t width = 0, height = 0;
  float render_time_ms = 0.0f;
};

class Renderer {
 public:
  enum class RenderState { Stopped, Running };

  Renderer() = default;

  void OnResize(int32_t width, int32_t height);

  void Render(const RendererSettings& settings);

  [[nodiscard]] std::shared_ptr<Image> Result() const;

  RenderState State() const;

  RendererStatistics Statistics() const;

 private:
  RenderState state_{RenderState::Stopped};
  std::thread main_render_thread_;
  BS::thread_pool pool_{std::max(1U, std::thread::hardware_concurrency() - 2)};

  std::shared_ptr<Scene> scene_;
  std::shared_ptr<Image> preview_;
  std::vector<uint32_t> image_data_;

  RendererSettings settings_;
  RendererStatistics statistics_;

  void RenderRow(int32_t row);

  void RenderChuck(glm::i32vec2 rows, glm::i32vec2 columns);

  glm::vec4 RenderPixel(const Ray& ray, int32_t child_rays);

  static glm::vec4 ColorCorrection(int32_t samples_per_pixel, const glm::vec4& color);
};

}  // namespace rt
