#include "onb.h"

namespace rt {
ONB::ONB(const glm::vec3& w) : w{glm::normalize(w)} {
  const glm::vec3 a = glm::abs(w.x) > 0.9f ? glm::vec3{0.0f, 1.0f, 0.0f} : glm::vec3{1.0f, 0.0f, 0.0f};
  v = glm::normalize(glm::cross(w, a));
  u = glm::cross(w, v);
}

glm::vec3 ONB::Local(float a, float b, float c) const {
  return a * u + b * v + c * w;
}

glm::vec3 ONB::Local(const glm::vec3& a) const {
  return a.x * u + a.y * v + a.z * w;
}

}  // namespace rt
