#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"

#include "hittable.h"

class material {
  public:
    virtual bool scatter(
      const ray& r, const hit_record& rec, color& attenuation, ray& scattered, const LightingType& lighting_type
    ) const = 0;
};

class lambertian : public material {
  public:
    lambertian(const color& a) : albedo(a) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, const LightingType& lighting_type
      ) const override {

      vec3 scatter_direction;
      switch (lighting_type) {
        case LightingType::hacky_lambertian:
          scatter_direction = rec.normal + random_in_unit_sphere();
          break;
        case LightingType::true_lambertian:
          scatter_direction = rec.normal + random_unit_vector();
          break;
        case LightingType::alt_approx_lambertian:
          scatter_direction = rec.normal + random_in_hemisphere(rec.normal);
          break;
        default:
          scatter_direction = rec.normal + random_unit_vector();
          break;
      }

      scattered = ray(rec.p, scatter_direction);
      attenuation = albedo;
      return true;
    }

  public:
    color albedo;
};

class metal : public material {
  public:
    metal(const color& a, double f) : albedo(a), fuzz(f) {}

    virtual bool scatter(
      const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, const LightingType& lighting_type
    ) const override {
      vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
      scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
      attenuation = albedo;
      return (dot(scattered.direction(), rec.normal) > 0);
    }

  public:
    color albedo;
    double fuzz;
};

#endif
