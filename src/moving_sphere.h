#pragma once

#include <memory>

#include "collidable.h"
#include "material.h"
#include "ray.h"
#include "vector3d.h"

class MovingSphere : public Collidable {
 public:
  MovingSphere(const Point3D &center0,
               const Point3D &center1,
               double time0,
               double time1,
               double radius,
               std::shared_ptr<Material> material);

  /**
   * Get the center of the moving sphere at the given time.
   * @param time center at this time
   * @return center at the given time
   */
  [[nodiscard]] Point3D CenterAt(double time) const;

  /**
   * Compute collision of a ray and the moving sphere object.
   * @param ray inbound ray
   * @param t_min minimum threshold
   * @param t_max maximum threshold
   * @param collision receives collision information
   * @return true if collision happened, false otherwise
   */
  bool Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const override;

 private:
  Point3D center0_, center1_;
  double time0_, time1_;
  double radius_;
  std::shared_ptr<Material> material_;
};
