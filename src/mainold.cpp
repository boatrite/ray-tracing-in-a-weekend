#include <iostream>

#include "material.h"

#include "camera.h"
#include "color.h"
#include "float.h"
#include "hitable_list.h"
#include "sphere.h"
#include "string.h"

vec3 ray_color(const ray& r, hitable *world, int depth) {
  hit_record rec;
  if(world->hit(r, 0.001, MAXFLOAT, rec)) {
    // Normal map
    return 0.5 * vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);

    ray scattered;
    vec3 attenuation;
    std::cerr << "deref" << rec.mat_ptr->scatter(r, rec, attenuation, scattered) << std::endl;
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return attenuation * ray_color(scattered, world, depth + 1);
    } else {
      return vec3(0, 0, 0);
    }
  } else {
    vec3 unit_direction = unit_vector(r.direction());
    float s = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - s) * vec3(1.0, 1.0, 1.0) + s * vec3(0.5, 0.7, 1.0);
  }
}

int main(int argc, char **argv) {
  // Render options
  bool aa = true;
  bool gamma_correct = true;

  // Get args
  for (int i = 0; i < argc; ++i) {
    std::cerr << i << ": " << argv[i] << std::endl;

    if (strcmp(argv[i], "--no-aa") == 0) {
      std::cerr << "Disabling AA" << std::endl;
      aa = false;
    }

    if (strcmp(argv[i], "--no-gamma-correct") == 0) {
      std::cerr << "Disabling gamma correction" << std::endl;
      gamma_correct = false;
    }
  }

  // Image
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int ns = 100;

  // Camera

  // Render
  std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

  hitable *list[2];
  list[0] = new sphere(vec3(0, 0, -1), 0.5, std::make_shared<lambertian>(vec3(0.8, 0.3, 0.3)));
  list[1] = new sphere(vec3(0, -100.5, -1), 100, std::make_shared<lambertian>(vec3(0.8, 0.8, 0.0)));
  hitable *world = new hitable_list(list, 2);

  camera cam;

  for (int j = image_height - 1; j >= 0; j--) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; i++) {
      vec3 col(0, 0, 0);

      if (aa) {
        for (int s = 0; s < ns; s++) {
          auto u = float(i + drand48()) / float(image_width);
          auto v = float(j + drand48()) / float(image_height);
          ray r = cam.get_ray(u, v);
          col += ray_color(r, world, 0);
        }
      } else {
        auto u = float(i) / float(image_width-1);
        auto v = float(j) / float(image_height-1);
        for (int s = 0; s < ns; s++) {
          ray r = cam.get_ray(u, v);
          col += ray_color(r, world, 0);
        }
      }

      col /= float(ns);
      if (gamma_correct) {
        col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2])); // Gamma correction
      }
      auto ir = int(255.99 * col[0]);
      auto ig = int(255.99 * col[1]);
      auto ib = int(255.99 * col[2]);
      color pixel_color(ir, ig, ib);
      write_color(std::cout, pixel_color);
    }
  }
  std::cerr << "\nDone.\n";
}
