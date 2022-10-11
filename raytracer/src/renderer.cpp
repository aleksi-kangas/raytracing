#include "renderer.h"

#include <limits>
#include <random>
#include <stdexcept>
#include <variant>

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
    scene_ = std::make_shared<Scene>(static_cast<SceneType>(settings_.scene_type),
                                     preview_->AspectRatio(),
                                     static_cast<BVHSplitStrategy>(settings_.bvh_split_strategy));

    state_ = RenderState::Running;
    statistics_.render_time_ms = std::chrono::milliseconds::zero();
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
    statistics_.render_time_ms = duration_cast<milliseconds>(end_time - start_time);
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
      color += RenderPixel(ray, settings_.max_child_rays);
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
        color += RenderPixel(ray, settings_.max_child_rays);
      }
      color = ColorCorrection(settings_.samples_per_pixel, color);
      image_data_[row * preview_->Width() + column] = utils::ColorToRGBA(color);
    }
  }
}

glm::vec4 Renderer::RenderPixel(const Ray& ray, int32_t child_rays) {
  glm::vec3 color{0, 0, 0};

  Ray current_ray = ray;
  glm::vec3 current_attenuation{1, 1, 1};
  while (child_rays--) {
    Collision collision{};
    const bool collided = scene_->Collide(current_ray, 0.001f, std::numeric_limits<float>::max(), collision);
    if (!collided) {
      color += scene_->BackgroundColor() * current_attenuation;
      break;
    }
    const glm::vec3 emitted =
        std::visit([&](const auto& material) {
          return material.Emit(collision.u, collision.v, collision.point);
        }, *collision.material);
    color += emitted * current_attenuation;

    Ray scattered_ray{};
    glm::vec3 attenuation{0, 0, 0};
    float pdf = 0.0f;
    const bool scattered = std::visit([&](const auto& material) {
      return material.Scatter(current_ray, collision, attenuation, scattered_ray, pdf);
    }, *collision.material);
    if (!scattered) {
      break;
    }
    current_ray = scattered_ray;
    current_attenuation *= attenuation * std::visit([&](const auto& material) {
      return material.ScatteringPDF(current_ray,
                                    collision,
                                    scattered_ray);
    }, *collision.material) / pdf;
  }
  return {color, 1.0f};
}

glm::vec4 Renderer::ColorCorrection(int32_t samples_per_pixel, const glm::vec4& color) {
  glm::vec3 corrected = color;
  corrected *= 1.0f / static_cast<float>(samples_per_pixel);
  corrected = glm::sqrt(corrected);
  return {corrected, 1.0f};
}

}  // namespace rt
