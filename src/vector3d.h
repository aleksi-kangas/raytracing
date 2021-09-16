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
  Vector3D &operator*=(const Vector3D &vector);
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

  /**
   * Compute dot product of the given vectors.
   * @param u the first vector
   * @param v the second vector
   * @return value of dot product
   */
  static double DotProduct(const Vector3D &u, const Vector3D &v);

  /**
   * Compute cross product of the given vectors.
   * @param u first vector
   * @param v second vector
   * @return cross product vector
   */
  static Vector3D CrossProduct(const Vector3D &u, const Vector3D &v);

  /**
   * Create an unit vector that has a length of 1.0 and the direction of the input vector.
   * @param vector from which to create the unit vector
   * @return unit vector
   */
  static Vector3D UnitVector(const Vector3D &vector);

  /**
   * Generate a random 3D-vector with coordinates in range [0, 1].
   * @return generated vector
   */
  static Vector3D Random();

  /**
   * Generate a random 3D-vector with coordinates in range [min, max].
   * @param min lower bound for coordinates
   * @param max upper bound for coordinates
   * @return generated vector
   */
  static Vector3D Random(double min, double max);

  /**
   * Generate a random 3D-vector inside unit sphere.
   * @return generated vector
   */
  static Vector3D RandomInUnitSphere();

  /**
   * Generate a random unit vector.
   * @return generated vector
   */
  static Vector3D RandomUnitVector();

  /**
   * Generate a random 3D-vector in unit disk (2D-space).
   * @return generated vector
   */
  static Vector3D RandomInUnitDisk();

  /**
   * Generate a random 3D-vector in the given normal's hemisphere.
   * @param normal surface normal
   * @return generated vector
   */
  static Vector3D RandomInHemisphere(const Vector3D &normal);

  /**
   * Generate a random cosine direction.
   * @return generated vector
   */
  static Vector3D RandomCosineDirection();

  static Vector3D RandomToSphere(double radius, double distance_squared);

  /**
   * Compute a reflected vector from an inbound vector and surface normal vector.
   * @param vector inbound vector
   * @param normal surface normal vector
   * @return reflected vector
   */
  static Vector3D Reflect(const Vector3D &vector, const Vector3D &normal);

  /**
   * Compute a refracted vector from an inbound vector, surface normal vector and refraction ratio.
   * @param vector inbound vector
   * @param normal surface normal vector
   * @param refraction_ratio ratio of refractive indices https://en.wikipedia.org/wiki/Refractive_index
   * @return refracted vector
   */
  static Vector3D Refract(const Vector3D &vector, const Vector3D &normal, double refraction_ratio);

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
