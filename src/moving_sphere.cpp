#include "moving_sphere.h"

#include <cmath>
#include <utility>

MovingSphere::MovingSphere(const Point3D &center0,
                           const Point3D &center1,
                           double time0,
                           double time1,
                           double radius,
                           std::shared_ptr<Material> material)
    : center0_(center0),
      center1_(center1),
      time0_(time0),
      time1_(time1),
      radius_(radius),
      material_(std::move(material)) {}

Point3D MovingSphere::CenterAt(double time) const {
  double time_delta = (time - time0_) / (time1_ - time0_);
  Vector3D direction = center1_ - center0_;
  return center0_ + time_delta * direction;
}

bool MovingSphere::Collide(const Ray &ray, double t_min, double t_max, Collision &collision) const {
  Vector3D to_ray_origin = ray.Origin() - CenterAt(ray.Time());
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
  collision.material = material_.get();

  Vector3D outward_normal = (collision.point - CenterAt(ray.Time())) / radius_;
  collision.SetFaceNormal(ray, outward_normal);
  return true;
}

bool MovingSphere::BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const {
  AxisAlignedBoundingBox box0(
      CenterAt(time0) - Vector3D(radius_, radius_, radius_),
      CenterAt(time0) + Vector3D(radius_, radius_, radius_)
  );
  AxisAlignedBoundingBox box1(
      CenterAt(time1) - Vector3D(radius_, radius_, radius_),
      CenterAt(time1) + Vector3D(radius_, radius_, radius_)
  );
  bounding_box = AxisAlignedBoundingBox::SurroundingBox(box0, box1);
  return true;
}
