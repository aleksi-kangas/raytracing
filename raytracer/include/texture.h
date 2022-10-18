#pragma once

#include <filesystem>
#include <variant>

#include "glm/glm.hpp"

#include "crtp.h"
#include "perlin.h"

namespace rt {

template<class T>
class Texture : public CRTP<Texture<T>> {
 public:
  [[nodiscard]] glm::vec3 Sample(float u, float v, const glm::vec3& point) const {
    return this->Actual().Sample(u, v, point);
  }

 private:
  Texture() = default;
  friend T;
};

class SolidColorTexture : public Texture<SolidColorTexture> {
 public:
  SolidColorTexture() = default;
  explicit SolidColorTexture(glm::vec3 color);
  SolidColorTexture(float red, float green, float blue);

  [[nodiscard]] glm::vec3 Sample(float u, float v, const glm::vec3& point) const;

 private:
  glm::vec3 color_{0, 0, 0};
};

class CheckerTexture : public Texture<CheckerTexture> {
 public:
  CheckerTexture(SolidColorTexture even, SolidColorTexture odd) : even_{even}, odd_{odd} {}

  [[nodiscard]] glm::vec3 Sample(float u, float v, const glm::vec3& point) const {
    const float sines = sinf(10.0f * point.x) * sinf(10.0f * point.y) * sinf(10.0f * point.z);
    if (sines >= 0.0f) {
      return even_.Sample(u, v, point);
    } else {
      return odd_.Sample(u, v, point);
    }
  }

 private:
  SolidColorTexture even_;
  SolidColorTexture odd_;
};

class NoiseTexture : public Texture<NoiseTexture> {
 public:
  NoiseTexture() = default;
  explicit NoiseTexture(float scale);

  [[nodiscard]] glm::vec3 Sample(float u, float v, const glm::vec3& point) const;

 private:
  Perlin perlin_;
  float scale_ = 1.0f;
};

class ImageTexture : public Texture<ImageTexture> {
 public:
  explicit ImageTexture(const std::filesystem::path& path);

  [[nodiscard]] glm::vec3 Sample(float u, float v, const glm::vec3& point) const;

 private:
  int32_t width_ = 0, height_ = 0;
  std::vector<uint8_t> buffer_;
};
}  // namespace rt
