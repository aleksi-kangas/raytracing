#pragma once

#include <cstdint>
#include <vector>

#include "glad/glad.h"

namespace rt {
class Image {
 public:
  Image(int32_t width, int32_t height);

  void Update(const std::vector<uint32_t>& buffer) const;

  [[nodiscard]] intptr_t Texture() const;

  [[nodiscard]] int32_t Width() const;
  [[nodiscard]] int32_t Height() const;

  void Resize(int32_t width, int32_t height);

 private:
  int32_t width_ = 0, height_ = 0;
  GLuint texture_ = 0;
};
}  // namespace rt
