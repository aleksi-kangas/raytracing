#pragma once

#include <cstdint>
#include <vector>

#include "glad/glad.h"

namespace rt {
class Image {
 public:
  Image(int32_t width, int32_t height, uint32_t* data);

  Image(const Image& preview) = delete;
  Image& operator=(const Image& preview) = delete;

  void Update() const;

  [[nodiscard]] inline intptr_t Texture() const { return texture_; };

  [[nodiscard]] inline uint32_t* Buffer() const { return data_; }
  [[nodiscard]] inline int32_t Width() const { return width_; };
  [[nodiscard]] inline int32_t Height() const { return height_; };
  [[nodiscard]] inline float AspectRatio() const { return static_cast<float>(width_) / static_cast<float>(height_); };

  void Resize(int32_t width, int32_t height, uint32_t* data);

 private:
  int32_t width_ = 0, height_ = 0;
  GLuint buffer_ = 0, texture_ = 0;
  uint32_t* data_ = nullptr;
};
}  // namespace rt
