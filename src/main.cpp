#include <iostream>
#include "camera.h"
#include "float.h"
#include "hitable_list.h"
#include "sphere.h"
#include "string.h"

vec3 random_in_unit_sphere() {
  vec3 p;
  do {
    p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1,1,1);
  } while (p.squared_length() >= 1.0);
  return p;
}

vec3 color(const ray& r, hitable *world) {
  hit_record rec;
  if(world->hit(r, 0.0, MAXFLOAT, rec)) {
    // Normal map
    // return 0.5 * vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);

    vec3 target = rec.p + rec.normal + random_in_unit_sphere();
    return 0.5 * color(ray(rec.p, target-rec.p), world);
  } else {
    vec3 unit_direction = unit_vector(r.direction());
    float s = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - s) * vec3(1.0, 1.0, 1.0) + s * vec3(0.5, 0.7, 1.0);
  }
}

int main(int argc, char*argv[]) {
  bool aa = true;

  if (argc > 1) {
    for (int i = 0; i < argc; i++) {
      auto flag = argv[i];
      if (strcmp(flag, "--no-aa")) {
        aa = false;
      }
    }
  }

  int nx = 200;
  int ny = 100;
  int ns = 100;
  std::cout << "P3\n" << nx << " " << ny << "\n255\n";

  hitable *list[2];
  list[0] = new sphere(vec3(0, 0, -1), 0.5);
  list[1] = new sphere(vec3(0, -100.5, -1), 100);
  hitable *world = new hitable_list(list, 2);

  camera cam;

  for (int j = ny - 1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      vec3 col(0, 0, 0);

      if (aa) {
        for (int s = 0; s < ns; s++) {
          float u = float(i + drand48()) / float(nx);
          float v = float(j + drand48()) / float(ny);
          ray r = cam.get_ray(u, v);
          col += color(r, world);
        }
      } else {
        float u = float(i) / float(nx);
        float v = float(j) / float(ny);
        for (int s = 0; s < ns; s++) {
          ray r = cam.get_ray(u, v);
          col += color(r, world);
        }
      }

      col /= float(ns);
      auto ir = int(255.99 * col[0]);
      auto ig = int(255.99 * col[1]);
      auto ib = int(255.99 * col[2]);
      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }
}
