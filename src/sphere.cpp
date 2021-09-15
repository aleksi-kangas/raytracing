#include "sphere.h"

#include <cmath>

Sphere::Sphere(const Point3D &center, double radius) : center_(center), radius_(radius) {}

bool Sphere::Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const {
  Vector3D to_ray_origin = ray.Origin() - center_;
  double a = ray.Direction().LengthSquared();
  double half_b = Vector3D::DotProduct(to_ray_origin, ray.Direction());
  double c = to_ray_origin.LengthSquared() - radius_ * radius_;

  double discriminant = half_b * half_b - a * c;
  if (discriminant < 0) {
    return false;
  }
  double sqrt_d = sqrt(discriminant);

  double root = (-half_b - sqrt_d) / a;
  if (root < t_min || t_max < root) {
    root = (-half_b + sqrt_d) / a;
    if (root < t_min || t_max < root) {
      return false;
    }
  }

  collision.t = root;
  collision.point = ray.PointAt(root);

  Vector3D outward_normal = (collision.point - center_) / radius_;
  collision.SetFaceNormal(ray, outward_normal);
  return true;
}
