#include "colors.h"
#include <iostream>
using namespace std;
using namespace APPROACH_RENDER_PNG;

void test(Colorspace8 c8);

int main() {
  Colorspace8 c1;
  Colorspace8 c2;
  Colorspace8 c3;

  std::cout << std::endl;
  std::cout << "---------" << std::endl;
  std::cout << "HSV Tests" << std::endl;
  std::cout << "---------" << std::endl;

  c1.rgb = {198, 15, 61};

  std::cout << "1. ";
  test(c1);

  c2.rgb = {5, 84, 11};

  std::cout << "2. ";
  test(c2);

  c3.rgb = {221, 15, 208};

  std::cout << "3. ";
  test(c3);
  std::cout << std::endl;
}

void test(Colorspace8 c8) {
  Colorspace8 res;
  Colorspace8 other;

  res = convert_color_space(ColorspaceType::RGB_SPACE,
                            ColorspaceType::HSV_SPACE, c8);

  Debug::print_rgb(c8.rgb);
  std::cout << " -> ";
  Debug::print_hsv(res.hsv);
  std::cout << " -> ";
  other = convert_color_space(ColorspaceType::HSV_SPACE,
                              ColorspaceType::RGB_SPACE, res);
  Debug::print_rgb(other.rgb);
  std::cout << std::endl;
}
