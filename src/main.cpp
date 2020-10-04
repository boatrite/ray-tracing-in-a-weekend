#include <iostream>
#include "vec3.h"

int main() {
  int nx = 200;
  int ny = 100;
  std::cout << "P3\n" << nx << " " << ny << "\n255\n";
  for (int j = ny - 1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      float r = float(i) / float(nx);
      float g = float(j) / float(ny);
      float b = 0.2;
      auto ir = int(255.99 * r);
      auto ig = int(255.99 * g);
      auto ib = int(255.99 * b);
      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }
}
