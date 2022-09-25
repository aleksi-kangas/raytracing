#include "renderer.h"

namespace rt {
void Renderer::OnResize(int32_t width, int32_t height) {
  if (image_) {
    if (image_->Width() == width && image_->Height() == height) return;
    image_->Resize(width, height);
  } else {
    image_ = std::make_shared<Image>(width, height);
  }
  image_data_.clear();
  image_data_.resize(width * height);
}

void Renderer::Render() {
  RenderChuck({0, image_->Height()}, {0, image_->Width()});
  image_->Update(image_data_);
}

std::shared_ptr<Image> Renderer::GetResult() const {
  return image_;
}

void Renderer::RenderChuck(glm::i32vec2 rows, glm::i32vec2 columns) {
  // TODO Need a proper camera system
  constexpr glm::vec3 kOrigin{0, 0, 0};
  constexpr float kFocalLength = 1.0f;
  const glm::vec3 kHorizontal{image_->AspectRatio() * 2.0f, 0, 0};
  const glm::vec3 kVertical{0, 2.0f, 0};
  const glm::vec3 kLowerLeftCorner = kOrigin - kHorizontal / 2.0f - kVertical / 2.0f - glm::vec3{0, 0, kFocalLength};

  for (int32_t row = rows[0]; row < rows[1]; ++row) {
    for (int32_t column = columns[0]; column < columns[1]; ++column) {
      glm::vec2 coordinate{static_cast<float>(column) / static_cast<float>(image_->Width()),
                           static_cast<float>(row) / static_cast<float>(image_->Height())};
      coordinate = coordinate * 2.0f - 1.0f;  // Map [0, 1] -> [-1, 1]
      coordinate.x *= image_->AspectRatio();
      const Ray ray{kOrigin, {coordinate.x, coordinate.y, -kFocalLength}};
      image_data_[row * image_->Width() + column] = RenderPixel(ray);
    }
  }
}
uint32_t Renderer::RenderPixel(const Ray& ray) {
  const glm::vec3 unit_direction = glm::normalize(ray.Direction());
  const float t = 0.5f * (unit_direction.y + 1.0f);
  const glm::vec3 color = (1.0f - t) * glm::vec3{1, 1, 1} + t * glm::vec3{0.5, 0.7, 1.0};

  const auto r = static_cast<uint8_t>(255.999f * color.r);
  const auto g = static_cast<uint8_t>(255.999f * color.g);
  const auto b = static_cast<uint8_t>(255.999f * color.b);
  constexpr uint8_t kAlpha = 0xff;

  return (kAlpha << 24) | (b << 16) | (g << 8) | (r << 0);
}

}  // namespace rt
