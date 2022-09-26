#include "metal.h"

#include "random.h"

namespace rt {
Metal::Metal(glm::vec3 albedo, float fuzziness)
    : albedo_{albedo}, fuzziness_{fuzziness < 1.0f ? fuzziness : 1.0f} {}

bool Metal::Scatter(const Ray& ray, const Collision& collision, glm::vec3& attenuation, Ray& scattered) const {
  const glm::vec3 reflected = glm::reflect(ray.Direction(), collision.normal);
  scattered = Ray{collision.point, reflected + fuzziness_ * random::InUnitSphere()};
  attenuation = albedo_;
  return glm::dot(scattered.Direction(), collision.normal) > 0;
}

}  // namespace rt
