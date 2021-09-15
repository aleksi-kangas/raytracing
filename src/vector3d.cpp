#include "vector3d.h"

#include <cmath>

#include "utils.h"

Vector3D::Vector3D() : elements_{0, 0, 0} {}

Vector3D::Vector3D(double x, double y, double z) : elements_{x, y, z} {}

double Vector3D::X() const {
  return elements_[0];
}

double Vector3D::Y() const {
  return elements_[1];
}

double Vector3D::Z() const {
  return elements_[2];
}

double Vector3D::Get(int index) const {
  if (index < 0 || index > 2) {
    throw std::out_of_range("Index out of bounds [0, 2].");
  }
  return elements_[index];
}

double Vector3D::operator[](int index) const {
  if (index < 0 || index > 2) {
    throw std::out_of_range("Index out of bounds [0, 2].");
  }
  return elements_[index];
}

double &Vector3D::operator[](int index) {
  if (index < 0 || index > 2) {
    throw std::out_of_range("Index out of bounds [0, 2].");
  }
  return elements_[index];
}

Vector3D Vector3D::operator-() const {
  return {-X(), -Y(), -Z()};
}

Vector3D &Vector3D::operator+=(const Vector3D &vector) {
  elements_[0] += vector.X();
  elements_[1] += vector.Y();
  elements_[2] += vector.Z();
  return *this;
}

Vector3D &Vector3D::operator*=(double c) {
  elements_[0] *= c;
  elements_[1] *= c;
  elements_[2] *= c;
  return *this;
}

Vector3D &Vector3D::operator/=(double c) {
  return *this *= 1 / c;
}

double Vector3D::Length() const {
  return sqrt(LengthSquared());
}

double Vector3D::LengthSquared() const {
  return X() * X() + Y() * Y() + Z() * Z();
}

bool Vector3D::IsNearZero() const {
  double threshold = 1e-8;
  return (std::abs(X()) < threshold) && (std::abs(Y()) < threshold) && (std::abs(Z()) < threshold);
}

Vector3D Vector3D::UnitVector() const {
  return *this / Length();
}

double Vector3D::DotProduct(const Vector3D &u, const Vector3D &v) {
  return u.X() * v.X() + u.Y() * v.Y() + u.Z() * v.Z();
}

Vector3D Vector3D::CrossProduct(const Vector3D &u, const Vector3D &v) {
  return {
      u.Y() * v.Z() - u.Z() * v.Y(),
      u.Z() * v.X() - u.X() * v.Z(),
      u.X() * v.Y() - u.Y() * v.X()
  };
}

Vector3D Vector3D::UnitVector(const Vector3D &vector) {
  return vector / vector.Length();
}

Vector3D operator+(const Vector3D &u, const Vector3D &v) {
  return {u.X() + v.X(), u.Y() + v.Y(), u.Z() + v.Z()};
}

Vector3D operator-(const Vector3D &u, const Vector3D &v) {
  return {u.X() - v.X(), u.Y() - v.Y(), u.Z() - v.Z()};
}

Vector3D operator*(const Vector3D &u, const Vector3D &v) {
  return {u.X() * v.X(), u.Y() * v.Y(), u.Z() * v.Z()};
}

Vector3D operator*(double c, const Vector3D &vector) {
  return {c * vector.X(), c * vector.Y(), c * vector.Z()};
}

Vector3D operator*(const Vector3D &vector, double c) {
  return c * vector;
}

Vector3D operator/(const Vector3D &vector, double c) {
  return (1 / c) * vector;
}
