#pragma once

#include "collidable.h"
#include "ray.h"
#include "vector3d.h"

class Sphere : public Collidable {
 public:
  Sphere(const Point3D &center, double radius);

  bool Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const override;

 private:
  Point3D center_;
  double radius_;
};
