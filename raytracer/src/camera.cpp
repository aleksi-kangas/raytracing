#include "camera.h"

namespace rt {
Camera::Camera(glm::vec3 origin) : origin_{origin} {}

Ray Camera::ShootRay(const glm::vec2& coordinate) const {
  constexpr float kFocalLength = 1.0f;
  return {origin_, {coordinate, -kFocalLength}};
}

}  // namespace rt
