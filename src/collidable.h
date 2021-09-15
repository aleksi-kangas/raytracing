#pragma once

#include "aabb.h"
#include "ray.h"
#include "vector3d.h"

// Forward declares.
class Material;

/**
 * Contains collision information.
 */
struct Collision {
  Point3D point;
  Vector3D normal;
  Material *material = nullptr;  // Raw pointer to increase performance.
  double t = 0.0;
  double u = 0.0;
  double v = 0.0;
  bool is_front_face = false;

  void SetFaceNormal(const Ray &ray, const Vector3D &outward_normal) {
    is_front_face = Vector3D::DotProduct(ray.Direction(), outward_normal) < 0;
    normal = is_front_face ? outward_normal : -outward_normal;
  }
};

/**
 * Represents a collidable object in 3D space. All objects in the scene should inherit this class.
 */
class Collidable {
 public:
  virtual ~Collidable() = default;

  /**
   * Compute collision of a ray and the collidable object.
   * @param[in] ray inbound ray
   * @param[in] t_min minimum threshold
   * @param[in] t_max maximum threshold
   * @param[out] collision receives collision information
   * @return true if collision happened, false otherwise
   */
  virtual bool Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const = 0;

  /**
   * Compute a minimum AABB that surrounds the collidable object.
   * @param[in] time0 minimum threshold
   * @param[in] time1 maximum threshold
   * @param[out] bounding_box receives the computed AABB
   * @return true if an AABB exists, false otherwise
   */
  virtual bool BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const = 0;

  /**
   * Compute value of probability density function from the given point and direction.
   * @param[in] origin point
   * @param[in] vector direction
   * @return value of probability density function
   */
  [[nodiscard]] virtual double PDFValue(const Point3D &origin, const Vector3D &vector) const {
    return 0.0;
  }

  /**
   * Generate a Random direction towards the collidable object from the given point.
   * @param[in] origin point
   * @return random direction
   */
  [[nodiscard]] virtual Vector3D Random(const Vector3D &origin) const {
    return {1, 0, 0};
  }
};
