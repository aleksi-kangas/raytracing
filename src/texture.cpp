#include "texture.h"

#include <cmath>

SolidColorTexture::SolidColorTexture(Color color) : color_(color) {}

SolidColorTexture::SolidColorTexture(double red, double green, double blue) : color_{red, green, blue} {}

Color SolidColorTexture::SampleColor(double u, double v, const Point3D &point) const {
  return color_;
}

CheckerTexture::CheckerTexture(std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd)
    : even_(std::move(even)), odd_(std::move(odd)) {}

CheckerTexture::CheckerTexture(const Color &even, const Color &odd)
    : even_(std::make_shared<SolidColorTexture>(even)), odd_(std::make_shared<SolidColorTexture>(odd)) {}

Color CheckerTexture::SampleColor(double u, double v, const Point3D &point) const {
  double sines = sin(10.0 * point.X()) * sin(10.0 * point.Y()) * sin(10.0 * point.Z());
  return sines < 0 ? odd_->SampleColor(u, v, point) : even_->SampleColor(u, v, point);
}
