#pragma once

#include "glm/glm.hpp"

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

  [[nodiscard]] glm::vec3 Sample(float u, float v, const glm::vec3& point) const;

 private:
  glm::vec3 color_{0, 0, 0};
};

class Checker : public Texture {
 public:
  Checker(const Texture* even, const Texture* odd) : even_{even}, odd_{odd} {}

  [[nodiscard]] glm::vec3 Sample(float u, float v, const glm::vec3& point) const {
    const float sines = sinf(10.0f * point.x) * sinf(10.0f * point.y) * sinf(10.0f * point.z);
    if (sines >= 0.0f) {
      return even_->Sample(u, v, point);
    } else {
      return odd_->Sample(u, v, point);
    }
  }

 private:
  const Texture* even_;
  const Texture* odd_;
};

}  // namespace rt
