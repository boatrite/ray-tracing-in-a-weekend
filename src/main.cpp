#include <iostream>
#include "ray.h"

const float NO_HIT = -1.0;

float hit_sphere(const vec3& center, float radius, const ray& r) {
  vec3 oc = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = 2.0 * dot(oc, r.direction());
  float c = dot(oc, oc) - radius*radius;
  float disc = b*b - 4*a*c;
  if (disc < 0) {
    return NO_HIT;
  } else {
    return (-b - sqrt(disc)) / (2.0*a);
  }
}

vec3 color(const ray& r) {
  vec3 sphere_center(0, 0, -1);
  float t = hit_sphere(sphere_center, 0.5, r);
  if (t > 0.0) {
    vec3 N = unit_vector(r.point_at_parameter(t) - sphere_center);
    return 0.5 * vec3(N.x()+1, N.y()+1, N.z()+1);
  }

  vec3 unit_direction = unit_vector(r.direction());
  float s = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - s) * vec3(1.0, 1.0, 1.0) + s * vec3(0.5, 0.7, 1.0);
}

int main() {
  int nx = 200;
  int ny = 100;
  std::cout << "P3\n" << nx << " " << ny << "\n255\n";
  vec3 lower_left_corner(-2.0, -1.0, -1.0);
  vec3 horizontal(4.0, 0.0, 0.0);
  vec3 vertical(0.0, 2.0, 0.0);
  vec3 origin(0.0, 0.0, 0.0);
  for (int j = ny - 1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      float u = float(i) / float(nx);
      float v = float(j) / float(ny);
      ray r(origin, lower_left_corner + u*horizontal + v*vertical);
      vec3 col = color(r);
      auto ir = int(255.99 * col[0]);
      auto ig = int(255.99 * col[1]);
      auto ib = int(255.99 * col[2]);
      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }
}
