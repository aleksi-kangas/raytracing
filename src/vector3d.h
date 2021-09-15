#pragma once

#include <array>
#include <iostream>

/**
 * Class representing a vector from origin in a three dimensional space.
 * Aliases are Color and Point3D.
 */
class Vector3D {
 public:
  Vector3D();
  Vector3D(double x, double y, double z);

  Vector3D(Vector3D &&) = default;
  Vector3D &operator=(Vector3D &&) = default;
  Vector3D(const Vector3D &) = default;
  Vector3D &operator=(const Vector3D &) = default;

  /**
   * Retrieve X-coordinate of the vector.
   * @return X-coordinate
   */
  [[nodiscard]] double X() const;

  /**
   * Retrieve Y-coordinate of the vector.
   * @return Y-coordinate
   */
  [[nodiscard]] double Y() const;

  /**
   * Retrieve Z-coordinate of the vector.
   * @return Z-coordinate
   */
  [[nodiscard]] double Z() const;

  /**
   * Retrieve coordinate at the given index.
   * Throws out_of_range if index is out of bounds.
   * @param index 0, 1 or 2
   * @return coordinate at the given index
   */
  [[nodiscard]] double Get(int index) const;
  double operator[](int index) const;
  double &operator[](int index);

  Vector3D operator-() const;
  Vector3D &operator+=(const Vector3D &vector);
  Vector3D &operator*=(double c);
  Vector3D &operator/=(double c);

  /**
   * Retrieve length of the vector.
   * Equals to the Euclidean norm of the vector.
   * @return length of the vector
   */
  [[nodiscard]] double Length() const;

  /**
   * Retrieve squared length of the vector.
   * @return squared length of the vector
   */
  [[nodiscard]] double LengthSquared() const;

  [[nodiscard]] bool IsNearZero() const;

  /**
   * Create an unit vector from this vector.
   * @return unit vector
   */
  [[nodiscard]] Vector3D UnitVector() const;

 private:
  std::array<double, 3> elements_;
};

/*
 * Aliases
 */
using Color = Vector3D;
using Point3D = Vector3D;

std::ostream &operator<<(std::ostream &out, const Vector3D &vector);
Vector3D operator+(const Vector3D &u, const Vector3D &v);
Vector3D operator-(const Vector3D &u, const Vector3D &v);
Vector3D operator*(const Vector3D &u, const Vector3D &v);
Vector3D operator*(double c, const Vector3D &vector);
Vector3D operator*(const Vector3D &vector, double c);
Vector3D operator/(const Vector3D &vector, double c);
