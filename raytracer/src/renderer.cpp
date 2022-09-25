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
  for (int32_t row = rows[1] - 1; row >= rows[0]; --row) {
    for (int32_t column = columns[0]; column < columns[1]; ++column) {
      glm::vec2 coordinate{static_cast<float>(column) / static_cast<float>(image_->Width()),
                           static_cast<float>(row) / static_cast<float>(image_->Height())};
      image_data_[row * image_->Width() + column] = RenderPixel(coordinate);
    }
  }
}
uint32_t Renderer::RenderPixel(const glm::vec2& coordinate) {
  const float r = coordinate.x;
  const float g = coordinate.y;
  const float b = 0.25f;

  const auto uir = static_cast<uint8_t>(255.999f * r);
  const auto uig = static_cast<uint8_t>(255.999f * g);
  const auto uib = static_cast<uint8_t>(255.999f * b);
  constexpr uint8_t kAlpha = 0xff;

  return (kAlpha << 24) | (uib << 16) | (uig << 8) | (uir << 0);
}

}  // namespace rt
