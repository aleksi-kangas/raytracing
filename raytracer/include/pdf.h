#pragma once

#include <variant>

#include "glm/glm.hpp"

#include "collidables.h"
#include "crtp.h"
#include "onb.h"
#include "random.h"

namespace rt {
template<class T>
class PDF : public CRTP<PDF<T>> {
 public:
  [[nodiscard]] float Value(const glm::vec3& direction) const {
    return this->Actual().Value(direction);
  }

  [[nodiscard]] glm::vec3 Generate() const {
    return this->Actual().Generate();
  }

 private:
  PDF() = default;
  friend T;
};

class CosinePDF : public PDF<CosinePDF> {
 public:
  explicit CosinePDF(const glm::vec3& w);

  [[nodiscard]] float Value(const glm::vec3& direction) const;

  [[nodiscard]] glm::vec3 Generate() const;

 private:
  ONB onb_;
};

class CollidablePDF : public PDF<CollidablePDF> {
 public:
  CollidablePDF(collidable_t  collidable, const glm::vec3& origin);

  [[nodiscard]] float Value(const glm::vec3& direction) const;

  [[nodiscard]] glm::vec3 Generate() const;

 private:
  const collidable_t collidable_;
  glm::vec3 origin_;
};

template<class T1, class T2>
class MixturePDF : public PDF<MixturePDF<T1, T2>> {
 public:
  MixturePDF(T1 pdf1, T2 pdf2) : pdf1_{pdf1}, pdf2_{pdf2} {}

  [[nodiscard]] float Value(const glm::vec3& direction) const {
    return 0.5f * pdf1_.Value(direction) + 0.5f * pdf2_.Value(direction);
  }

  [[nodiscard]] glm::vec3 Generate() const {
    return random::Float() < 0.5f ? pdf1_.Generate() : pdf2_.Generate();
  }

 private:
  T1 pdf1_;
  T2 pdf2_;
};

using pdf_t = std::variant<CosinePDF, CollidablePDF, MixturePDF<CosinePDF, CollidablePDF>>;
}  // namespace rt
