#include "colors.h"
#include <iostream>
using namespace std;
using namespace APPROACH_RENDER_PNG;

void testHSV8(Colorspace8 c8);
void testHSV16(Colorspace16 c16);
void testHSV24(Colorspace24 c24);

int main() {
  Colorspace8 a1;
  Colorspace8 a2;
  Colorspace8 a3;

  std::cout << std::endl;
  std::cout << "---------" << std::endl;
  std::cout << "HSV Tests" << std::endl;
  std::cout << "---------" << std::endl;

  a1.rgb = {198, 15, 61};

  std::cout << "8 Bit Spaces: " << std::endl;
  std::cout << "1. ";
  testHSV8(a1);

  a2.rgb = {5, 84, 11};

  std::cout << "2. ";
  testHSV8(a2);

  a3.rgb = {221, 15, 208};

  std::cout << "3. ";
  testHSV8(a3);
  std::cout << std::endl;

  Colorspace16 b1;
  Colorspace16 b2;

  b1.rgb = {65533, 32767, 1000};
  b2.rgb = {655, 377, 189};

  std::cout << "16 Bit Spaces: " << std::endl;
  std::cout << "1. ";
  testHSV16(b1);

  std::cout << "2. ";
  testHSV16(b2);
  std::cout << std::endl;

  Colorspace24 c1;
  Colorspace24 c2;

  c1.rgb = {123, 41, 21};

  std::cout << "24 Bit Spaces: " << std::endl;

  std::cout << "1. ";
  testHSV24(c1);

  std::cout << std::endl;
}

void testHSV8(Colorspace8 c8) {
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

void testHSV16(Colorspace16 c16) {
  Colorspace16 res;
  Colorspace16 other;

  res = convert_color_space(ColorspaceType::RGB_SPACE,
                            ColorspaceType::HSV_SPACE, c16);

  Debug::print_rgb(c16.rgb);
  std::cout << " -> ";
  Debug::print_hsv(res.hsv);
  std::cout << " -> ";
  other = convert_color_space(ColorspaceType::HSV_SPACE,
                              ColorspaceType::RGB_SPACE, res);
  Debug::print_rgb(other.rgb);
  std::cout << std::endl;
}

void testHSV24(Colorspace24 c24) {
  Colorspace24 res;
  Colorspace24 other;

  res = convert_color_space(ColorspaceType::RGB_SPACE,
                            ColorspaceType::HSV_SPACE, c24);

  Debug::print_rgb(c24.rgb);
  std::cout << " -> ";
  Debug::print_hsv(res.hsv);
  std::cout << " -> ";
  other = convert_color_space(ColorspaceType::HSV_SPACE,
                              ColorspaceType::RGB_SPACE, res);
  Debug::print_rgb(other.rgb);

  std::cout << std::endl;
}
