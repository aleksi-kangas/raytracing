#pragma once

#include "glm/glm.hpp"

#include "perlin.h"

namespace rt {
class Texture {
 public:
  virtual ~Texture() = default;
  [[nodiscard]] virtual glm::vec3 Sample(float u, float v, const glm::vec3& point) const = 0;
};

class SolidColor : public Texture {
 public:
  SolidColor() = default;
  explicit SolidColor(glm::vec3 color);
  SolidColor(float red, float green, float blue);

  [[nodiscard]] glm::vec3 Sample(float u, float v, const glm::vec3& point) const override;

 private:
  glm::vec3 color_{0, 0, 0};
};

class Checker : public Texture {
 public:
  Checker(const Texture* even, const Texture* odd);

  [[nodiscard]] glm::vec3 Sample(float u, float v, const glm::vec3& point) const override;

 private:
  const Texture* even_;
  const Texture* odd_;
};

class Noise : public Texture {
 public:
  Noise() = default;
  explicit Noise(float scale);

  [[nodiscard]] glm::vec3 Sample(float u, float v, const glm::vec3& point) const override;

 private:
  Perlin perlin_;
  float scale_ = 1.0f;
};
}  // namespace rt
