#pragma once

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
   * @param ray inbound ray
   * @param t_min minimum threshold
   * @param t_max maximum threshold
   * @param collision receives collision information
   * @return true if collision happened, false otherwise
   */
  virtual bool Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const = 0;
};
