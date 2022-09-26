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
    scene_ = std::make_shared<Scene>(static_cast<SceneType>(settings_.scene_type), preview_->AspectRatio());

    state_ = RenderState::Running;
    statistics_.render_time_ms = 0.0f;
    statistics_.width = preview_->Width();
    statistics_.height = preview_->Height();
    auto start_time = high_resolution_clock::now();

    switch (settings_.mode) {
      case RenderMode::RowByRow: {
        for (int32_t row = 0; row < preview_->Height(); ++row) {
          pool_.push_task(&Renderer::RenderRow, this, row);
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
            pool_.push_task(&Renderer::RenderChuck, this, rows, columns);
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

void Renderer::RenderRow(int32_t row) {
  for (int32_t column = 0; column < preview_->Width(); ++column) {
    glm::vec4 color{};
    for (int32_t sample = 1; sample <= settings_.samples_per_pixel; ++sample) {
      glm::vec2 coordinate{
          (static_cast<float>(column) + random::Float()) / static_cast<float>(preview_->Width()),
          (static_cast<float>(row) + random::Float()) / static_cast<float>(preview_->Height())
      };
      const Ray ray = scene_->GetCamera()->ShootRay(coordinate);
      color += RenderPixel(ray, settings_.child_rays);
    }
    color = ColorCorrection(settings_.samples_per_pixel, color);
    image_data_[row * preview_->Width() + column] = utils::ColorToRGBA(color);
  }
}

void Renderer::RenderChuck(glm::i32vec2 rows, glm::i32vec2 columns) {
  for (int32_t row = rows[0]; row < rows[1]; ++row) {
    for (int32_t column = columns[0]; column < columns[1]; ++column) {
      glm::vec4 color{};
      for (int32_t sample = 1; sample <= settings_.samples_per_pixel; ++sample) {
        glm::vec2 coordinate{
            (static_cast<float>(column) + random::Float()) / static_cast<float>(preview_->Width()),
            (static_cast<float>(row) + random::Float()) / static_cast<float>(preview_->Height())
        };
        const Ray ray = scene_->GetCamera()->ShootRay(coordinate);
        color += RenderPixel(ray, settings_.child_rays);
      }
      color = ColorCorrection(settings_.samples_per_pixel, color);
      image_data_[row * preview_->Width() + column] = utils::ColorToRGBA(color);
    }
  }
}

glm::vec4 Renderer::RenderPixel(const Ray& ray, int32_t child_rays) {
  glm::vec3 color{1, 1, 1};
  Ray current_ray = ray;
  while (child_rays--) {
    Collision collision{};
    const bool collided = scene_->Collide(current_ray, 0.001f, std::numeric_limits<float>::max(), collision);
    if (!collided) {
      const glm::vec3 unit_direction = glm::normalize(ray.Direction());
      const float t = 0.5f * (unit_direction.y + 1.0f);
      color *= (1.0f - t) * glm::vec3{1, 1, 1} + t * glm::vec3{0.5, 0.7, 1.0};
      return {color, 1.0f};
    }
    Ray scattered_ray{};
    glm::vec3 attenuation{0, 0, 0};
    const bool scattered = collision.material->Scatter(current_ray, collision, attenuation, scattered_ray);
    if (scattered) {
      color *= attenuation;
      current_ray = scattered_ray;
    } else {
      break;
    }
  }
  return {0.0f, 0.0f, 0.0f, 1.0f};
}

glm::vec4 Renderer::ColorCorrection(int32_t samples_per_pixel, const glm::vec4& color) {
  glm::vec3 corrected = color;
  corrected *= 1.0f / static_cast<float>(samples_per_pixel);
  corrected = glm::sqrt(corrected);
  return {corrected, 1.0f};
}

}  // namespace rt
