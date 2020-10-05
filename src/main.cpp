#include "common.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

enum class LightingType { normals, alt_approx_lambertian, true_lambertian, hacky_lambertian };

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant) ) / a;
    }
}

color ray_color(const ray& r, const hittable& world, int depth, LightingType lighting_type) {
  hit_record rec;

  // If we've exceeded the ray bouce limit, no more light is gathered.
  if (depth <= 0) {
    return color(0,0,0);
  }

  if(world.hit(r, 0.001, infinity, rec)) {
    if (lighting_type == LightingType::normals) {
      return 0.5 * (rec.normal + color(1,1,1));
    } else {
      point3 target;
      switch (lighting_type) {
        case LightingType::hacky_lambertian:
          target = rec.p + rec.normal + random_in_unit_sphere();
          break;
        case LightingType::true_lambertian:
          target = rec.p + rec.normal + random_unit_vector();
          break;
        case LightingType::alt_approx_lambertian:
          target = rec.p + rec.normal + random_in_hemisphere(rec.normal);
          break;
        default:
          target = rec.p + rec.normal + random_in_unit_sphere();
          break;
      }
      return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1, lighting_type);
    }
  }
  vec3 unit_direction = unit_vector(r.direction());
  auto t = 0.5*(unit_direction.y() + 1.0);
  return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main(int argc, char** argv) {
  // Render options
  bool aa = true;
  bool gamma_correct = true;
  LightingType lighting_type = LightingType::true_lambertian;

  // Get args
  for (auto i = 0; i < argc; ++i) {
    std::cerr << i << ": " << argv[i] << std::endl;

    if (strcmp(argv[i], "--no-aa") == 0) {
      aa = false;
    }

    if (strcmp(argv[i], "--no-gamma-correct") == 0) {
      gamma_correct = false;
    }

    if (strcmp(argv[i], "--normals") == 0) {
      lighting_type = LightingType::normals;
      gamma_correct = false;
    }

    if (strcmp(argv[i], "--hacky_lambertian") == 0) {
      lighting_type = LightingType::hacky_lambertian;
    }

    if (strcmp(argv[i], "--true_lambertian") == 0) {
      lighting_type = LightingType::true_lambertian;
    }

    if (strcmp(argv[i], "--alt_approx_lambertian") == 0) {
      lighting_type = LightingType::alt_approx_lambertian;
    }
  }

  // Image
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 100;
  const int max_depth = 50;

  // World
  hittable_list world;
  world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
  world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

  // Camera
  camera cam;

  // Render
  std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

  for (int j = image_height-1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) {
      color pixel_color(0,0,0);
      for (int s = 0; s < samples_per_pixel; ++s) {
        double u,v;

        if (aa) {
          u = (i + random_double()) / (image_width-1);
          v = (j + random_double()) / (image_height-1);
        } else {
          u = double(i) / (image_width-1);
          v = double(j) / (image_height-1);
        }
        ray r = cam.get_ray(u, v);
        pixel_color += ray_color(r, world, max_depth, lighting_type);
      }
      write_color(std::cout, pixel_color, samples_per_pixel, gamma_correct);
    }
  }

  std::cerr << "\nDone.\n";
}
