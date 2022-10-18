#include "rotate_translate.h"

#include <array>
#include <utility>
#include <variant>

#include "glm/gtc/matrix_transform.hpp"

namespace rt {
RotateTranslate::RotateTranslate(transformable_t collidable, float rotate_y, glm::vec3 translate)
    : collidable_{std::move(collidable)},
      rotate_y_{rotate_y},
      translate_{translate} {}

bool RotateTranslate::Collide(const Ray& ray, float t_min, float t_max, Collision& collision) const {
  const glm::mat4 inverse_transform = InverseTransformationMatrix();
  Ray transformed_ray{glm::vec3{inverse_transform * glm::vec4{ray.Origin(), 1.0f}},
                      glm::vec3{inverse_transform * glm::vec4{ray.Direction(), 0.0f}}};
  if (!std::visit([&](const auto& collidable) { return collidable.Collide(transformed_ray, t_min, t_max, collision); },
                  collidable_)) {
    return false;
  }
  const glm::mat4 transform = TransformationMatrix();
  collision.point = transform * glm::vec4{collision.point, 1.0f};
  collision.SetNormal(transformed_ray, transform * glm::vec4{collision.normal, 0.0f});
  return true;
}

bool RotateTranslate::BoundingBox(float time0, float time1, AABB& bounding_box) const {
  if (!std::visit([&](const auto& collidable) { return collidable.BoundingBox(time0, time1, bounding_box); },
                  collidable_)) {
    return false;
  }
  std::array<glm::vec3, 8> aabb_vertices{
      {
          {bounding_box.MinPoint().x, bounding_box.MinPoint().y, bounding_box.MinPoint().z},
          {bounding_box.MaxPoint().x, bounding_box.MinPoint().y, bounding_box.MinPoint().z},
          {bounding_box.MaxPoint().x, bounding_box.MaxPoint().y, bounding_box.MinPoint().z},
          {bounding_box.MinPoint().x, bounding_box.MaxPoint().y, bounding_box.MinPoint().z},
          {bounding_box.MinPoint().x, bounding_box.MinPoint().y, bounding_box.MaxPoint().z},
          {bounding_box.MaxPoint().x, bounding_box.MinPoint().y, bounding_box.MaxPoint().z},
          {bounding_box.MaxPoint().x, bounding_box.MaxPoint().y, bounding_box.MaxPoint().z},
          {bounding_box.MinPoint().x, bounding_box.MaxPoint().y, bounding_box.MaxPoint().z}
      }};
  const glm::mat4 transform = TransformationMatrix();
  for (auto& vertex : aabb_vertices) {
    vertex = transform * glm::vec4{vertex, 1.0f};
  }
  glm::vec3 min_point{std::numeric_limits<float>::max()};
  glm::vec3 max_point{std::numeric_limits<float>::lowest()};
  for (const auto& vertex : aabb_vertices) {
    min_point = glm::min(min_point, vertex);
    max_point = glm::max(max_point, vertex);
  }
  bounding_box = AABB{min_point, max_point};
  return true;
}

glm::vec3 RotateTranslate::Centroid() const {
  const glm::vec3 primitive_centroid = std::visit([](const auto& collidable) { return collidable.Centroid(); },
                                                  collidable_);
  const glm::mat4 transform = TransformationMatrix();
  return transform * glm::vec4{primitive_centroid, 1.0f};
}

glm::mat4 RotateTranslate::TransformationMatrix() const {
  glm::mat4 rotation = glm::rotate(glm::mat4{1.0f}, glm::radians(rotate_y_), glm::vec3{0.0f, 1.0f, 0.0f});
  glm::mat4 translation = glm::translate(glm::mat4{1.0f}, translate_);
  return translation * rotation;
}

glm::mat4 RotateTranslate::InverseTransformationMatrix() const {
  glm::mat4 inverse_rotation = glm::rotate(glm::mat4{1.0f}, glm::radians(-rotate_y_), glm::vec3{0.0f, 1.0f, 0.0f});
  glm::mat4 inverse_translation = glm::translate(glm::mat4{1.0f}, -translate_);
  return inverse_rotation * inverse_translation;
}

float RotateTranslate::PDFValue(const glm::vec3& origin, const glm::vec3& direction) const {
  // TODO
  return 0.0f;
}

glm::vec3 RotateTranslate::RandomTowards(const glm::vec3& origin) const {
  // TODO
  return {0.0f, 0.0f, 0.0f};
}

}  // namespace rt