#include "pdf.h"

#include <utility>

#include "random.h"
#include "utils.h"

CosinePDF::CosinePDF(const Vector3D &w) : onb_(w) {}

double CosinePDF::Value(const Vector3D &direction) const {
  double cosine = Vector3D::DotProduct(direction.UnitVector(), onb_.W());
  return std::max(cosine, 0.0) / utils::kPI;
}

Vector3D CosinePDF::Generate() const {
  return onb_.Local(Vector3D::RandomCosineDirection());
}

CollidablePDF::CollidablePDF(std::shared_ptr<Collidable> object, const Point3D &origin)
    : object_(std::move(object)), origin_(origin) {}

double CollidablePDF::Value(const Vector3D &direction) const {
  return object_->PDFValue(origin_, direction);
}

Vector3D CollidablePDF::Generate() const {
  return object_->Random(origin_);
}

MixturePDF::MixturePDF(std::shared_ptr<ProbabilityDensityFunction> pdf0,
                       std::shared_ptr<ProbabilityDensityFunction> pdf1)
    : pdfs_{std::move(pdf0), std::move(pdf1)} {}

double MixturePDF::Value(const Vector3D &direction) const {
  return 0.5 * pdfs_[0]->Value(direction) + 0.5 * pdfs_[1]->Value(direction);
}

Vector3D MixturePDF::Generate() const {
  return RandomDouble() < 0.5 ? pdfs_[0]->Generate() : pdfs_[1]->Generate();
}
