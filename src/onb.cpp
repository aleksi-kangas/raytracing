#include "onb.h"

OrthonormalBasis::OrthonormalBasis(const Vector3D &w) {
  axes_[2] = w.UnitVector();
  Vector3D a = std::abs(W().X()) > 0.9 ? Vector3D(0, 1, 0) : Vector3D(1, 0, 0);
  axes_[1] = Vector3D::CrossProduct(W(), a).UnitVector();
  axes_[0] = Vector3D::CrossProduct(W(), V());
}

Vector3D OrthonormalBasis::operator[](int i) const {
  return axes_[i];
}

Vector3D OrthonormalBasis::U() const {
  return axes_[0];
}

Vector3D OrthonormalBasis::V() const {
  return axes_[1];
}

Vector3D OrthonormalBasis::W() const {
  return axes_[2];
}

Vector3D OrthonormalBasis::Local(double a, double b, double c) const {
  return a * U() + b * V() + c * W();
}

Vector3D OrthonormalBasis::Local(const Vector3D &a) const {
  return a.X() * U() + a.Y() * V() + a.Z() * W();
}
