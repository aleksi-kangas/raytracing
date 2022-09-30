#include "texture.h"

namespace rt {

SolidColor::SolidColor(glm::vec3 color) : color_{color} {}

SolidColor::SolidColor(float red, float green, float blue) : color_{red, green, blue} {}

glm::vec3 SolidColor::Sample(float u, float v, const glm::vec3& point) const {
  return color_;
}

}  // namespace rt
