#pragma once

#include <memory>

#include "collidable.h"
#include "ray.h"
#include "vector3d.h"

class Sphere : public Collidable {
 public:
  Sphere(const Point3D &center, double radius, std::shared_ptr<Material> material);

  /**
   * Compute collision of a ray and the sphere object.
   * @param ray inbound ray
   * @param t_min minimum threshold
   * @param t_max maximum threshold
   * @param collision receives collision information
   * @return true if collision happened, false otherwise
   */
  bool Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const override;

 private:
  Point3D center_;
  double radius_;
  std::shared_ptr<Material> material_;
};
