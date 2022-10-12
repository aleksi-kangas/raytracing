#include "pdf.h"

#include <numbers>
#include <utility>
#include <variant>

#include "random.h"

namespace rt {
CosinePDF::CosinePDF(const glm::vec3& w) : onb_{w} {}

float CosinePDF::Value(const glm::vec3& direction) const {
  const float cosine = glm::dot(glm::normalize(direction), onb_.w);
  return cosine <= 0.0f ? 0.0f : cosine / std::numbers::pi_v<float>;
}

glm::vec3 CosinePDF::Generate() const {
  return onb_.Local(random::CosineDirection());
}

CollidablePDF::CollidablePDF(collidable_t  collidable, const glm::vec3& origin)
    : collidable_{std::move(collidable)}, origin_{origin} {}

float CollidablePDF::Value(const glm::vec3& direction) const {
  return std::visit([&](const auto& collidable) { return collidable.PDFValue(origin_, direction); }, collidable_);
}

glm::vec3 CollidablePDF::Generate() const {
  return std::visit([&](const auto& collidable) { return collidable.RandomTowards(origin_); }, collidable_);
}

}  // namespace rt
