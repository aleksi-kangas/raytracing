#include "texture.h"

#include <algorithm>
#include <cmath>

#include "utils.h"

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

NoiseTexture::NoiseTexture(double scale) : scale_(scale) {}

Color NoiseTexture::SampleColor(double u, double v, const Point3D &point) const {
  // Scaling fix: https://github.com/RayTracing/raytracing.github.io/issues/896
  return Color(1, 1, 1) * 0.5 * (1.0 + sin(scale_ * point.Z() + 10.0 * perlin_.Turbulence(scale_ * point)));
}

ImageTexture::ImageTexture(const std::filesystem::path &path) {
  texture_ = utils::ReadPNG(path, width_, height_);
}

Color ImageTexture::SampleColor(double u, double v, const Point3D &point) const {
  if (texture_.empty()) {
    return {1, 0, 0};  // Debug.
  }

  u = std::clamp(u, 0.0, 1.0);
  v = 1.0 - std::clamp(v, 0.0, 1.0);

  int row = static_cast<int>(v * height_);
  if (row >= height_) {
    row = static_cast<int>(height_) - 1;
  }
  int column = static_cast<int>(u * width_);
  if (column >= width_) {
    column = static_cast<int>(width_) - 1;
  }

  const double kColorScale = 1.0 / 255.0;
  return texture_[row][column] * kColorScale;
}
