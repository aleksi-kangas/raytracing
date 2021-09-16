#include "sphere.h"

#include <cmath>
#include <utility>

#include "onb.h"
#include "utils.h"

Sphere::Sphere(const Point3D &center, double radius, std::shared_ptr<Material> material)
    : center_(center), radius_(radius), material_(std::move(material)) {}

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
  collision.material = material_.get();
  GetUVCoordinates(outward_normal, collision.u, collision.v);
  return true;
}

bool Sphere::BoundingBox(double time0, double time1, AxisAlignedBoundingBox &bounding_box) const {
  bounding_box = AxisAlignedBoundingBox(
      center_ - Vector3D(radius_, radius_, radius_),
      center_ + Vector3D(radius_, radius_, radius_)
  );
  return true;
}

void Sphere::GetUVCoordinates(const Point3D &point, double &u, double &v) {
  double theta = acos(-point.Y());
  double phi = atan2(-point.Z(), point.X()) + utils::kPI;
  u = phi / (2 * utils::kPI);
  v = theta / utils::kPI;
}

double Sphere::PDFValue(const Point3D &origin, const Vector3D &vector) const {
  Collision collision;
  if (!Collide(Ray(origin, vector), 0.001, utils::kInfinity, collision)) {
    return 0;
  }
  double cos_theta_max = sqrt(1.0 - radius_ * radius_ / (center_ - origin).LengthSquared());
  double solid_angle = 2.0 * utils::kPI * (1.0 - cos_theta_max);
  return 1.0 / solid_angle;
}

Vector3D Sphere::Random(const Point3D &origin) const {
  Vector3D direction = center_ - origin;
  double distance_squared = direction.LengthSquared();
  OrthonormalBasis onb(direction);
  return onb.Local(Vector3D::RandomToSphere(radius_, distance_squared));
}
