#include "renderer.h"

#include <chrono>
#include <limits>
#include <random>
#include <stdexcept>

#include "random.h"
#include "utils.h"

namespace rt {
void Renderer::OnResize(int32_t width, int32_t height) {
  if (state_ == RenderState::Running) return;
  image_data_.clear();
  image_data_.resize(width * height);
  if (preview_) {
    preview_->Resize(width, height, image_data_.data());
  } else {
    preview_ = std::make_shared<Image>(width, height, image_data_.data());
  }
}

void Renderer::Render(const RendererSettings& settings) {
  settings_ = settings;

  auto render_task = [&]() {
    using namespace std::chrono;
    Camera camera{{0, 0, 0}};
    Scene scene{static_cast<SceneType>(settings_.scene_type)};

    state_ = RenderState::Running;
    statistics_.render_time_ms = 0.0f;
    statistics_.width = preview_->Width();
    statistics_.height = preview_->Height();
    auto start_time = high_resolution_clock::now();

    switch (settings_.mode) {
      case RenderMode::RowByRow: {
        for (int32_t row = 0; row < preview_->Height(); ++row) {
          pool_.push_task(&Renderer::RenderRow, this, row, camera, scene);
        }
      }
        break;
      case RenderMode::ChunkByChunk: {
        const auto kHorizontalChunks =
            static_cast<int32_t>(std::ceil(
                static_cast<float>(preview_->Width()) / static_cast<float>(settings_.chunk_size)));
        const auto kVerticalChunks =
            static_cast<int32_t>(std::ceil(
                static_cast<float>(preview_->Height()) / static_cast<float>(settings_.chunk_size)));
        for (int32_t v_chunk = 1; v_chunk <= kVerticalChunks; ++v_chunk) {
          for (int32_t h_chunk = 1; h_chunk <= kHorizontalChunks; ++h_chunk) {
            glm::i32vec2 rows{(v_chunk - 1) * settings_.chunk_size,
                              std::min(v_chunk * settings_.chunk_size, preview_->Height())};
            glm::i32vec2 columns{(h_chunk - 1) * settings_.chunk_size,
                                 std::min(h_chunk * settings_.chunk_size, preview_->Width())};
            pool_.push_task(&Renderer::RenderChuck, this, rows, columns, camera, scene);
          }
        }
      }
        break;
      default:
        throw std::runtime_error{"Unknown rendering mode"};
    }

    pool_.wait_for_tasks();
    auto end_time = high_resolution_clock::now();
    state_ = RenderState::Stopped;
    statistics_.render_time_ms = static_cast<float>(duration_cast<milliseconds>(end_time - start_time).count());
  };

  main_render_thread_ = std::thread{render_task};
  main_render_thread_.detach();
}

std::shared_ptr<Image> Renderer::Result() const {
  return preview_;
}

Renderer::RenderState Renderer::State() const {
  return state_;
}

RendererStatistics Renderer::Statistics() const {
  return statistics_;
}

void Renderer::RenderRow(int32_t row, const Camera& camera, const Scene& scene) {
  for (int32_t column = 0; column < preview_->Width(); ++column) {
    glm::vec4 color{};
    for (int32_t sample = 1; sample <= settings_.samples_per_pixel; ++sample) {
      glm::vec2 coordinate{
          (static_cast<float>(column) + random::Float()) / static_cast<float>(preview_->Width()),
          (static_cast<float>(row) + random::Float()) / static_cast<float>(preview_->Height())
      };
      coordinate = coordinate * 2.0f - 1.0f;  // Map [0, 1] -> [-1, 1]
      coordinate.x *= preview_->AspectRatio();
      const Ray ray = camera.ShootRay(coordinate);
      color += RenderPixel(ray, scene);
    }
    color /= settings_.samples_per_pixel;
    image_data_[row * preview_->Width() + column] = utils::ColorToRGBA(color);
  }
}

void Renderer::RenderChuck(glm::i32vec2 rows, glm::i32vec2 columns, const Camera& camera, const Scene& scene) {
  for (int32_t row = rows[0]; row < rows[1]; ++row) {
    for (int32_t column = columns[0]; column < columns[1]; ++column) {
      glm::vec4 color{};
      for (int32_t sample = 1; sample <= settings_.samples_per_pixel; ++sample) {
        glm::vec2 coordinate{
            (static_cast<float>(column) + random::Float()) / static_cast<float>(preview_->Width()),
            (static_cast<float>(row) + random::Float()) / static_cast<float>(preview_->Height())
        };
        coordinate = coordinate * 2.0f - 1.0f;  // Map [0, 1] -> [-1, 1]
        coordinate.x *= preview_->AspectRatio();
        const Ray ray = camera.ShootRay(coordinate);
        color += RenderPixel(ray, scene);
      }
      color /= settings_.samples_per_pixel;
      image_data_[row * preview_->Width() + column] = utils::ColorToRGBA(color);
    }
  }
}

glm::vec4 Renderer::RenderPixel(const Ray& ray, const Scene& scene) {
  glm::vec3 color{};
  Collision collision{};
  if (scene.Collide(ray, 0.0f, std::numeric_limits<float>::max(), collision)) {
    color = 0.5f * (collision.normal + 1.0f);
  } else {
    const glm::vec3 unit_direction = glm::normalize(ray.Direction());
    const float t = 0.5f * (unit_direction.y + 1.0f);
    color = (1.0f - t) * glm::vec3{1, 1, 1} + t * glm::vec3{0.5, 0.7, 1.0};
  }
  return {color, 1.0f};
}

}  // namespace rt
