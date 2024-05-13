#include "colors.h"
#include <iostream>
using namespace std;
using namespace APPROACH_RENDER_PNG;

template <typename Colorspace> void testHSV(Colorspace c);

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
  testHSV(a1);

  a2.rgb = {5, 84, 11};

  std::cout << "2. ";
  testHSV(a2);

  a3.rgb = {221, 15, 208};

  std::cout << "3. ";
  testHSV(a3);
  std::cout << std::endl;

  Colorspace16 b1;
  Colorspace16 b2;

  b1.rgb = {65533, 32767, 1000};
  b2.rgb = {655, 377, 189};

  std::cout << "16 Bit Spaces: " << std::endl;
  std::cout << "1. ";
  testHSV(b1);

  std::cout << "2. ";
  testHSV(b2);
  std::cout << std::endl;

  Colorspace24 c1;
  Colorspace24 c2;

  c1.rgb = {123, 41, 21};

  std::cout << "24 Bit Spaces: " << std::endl;

  std::cout << "1. ";
  testHSV(c1);

  std::cout << std::endl;
}

template <typename Colorspace> void testHSV(Colorspace c) {
  Colorspace res;
  Colorspace other;

  res = convert_color_space(ColorspaceType::RGB_SPACE,
                            ColorspaceType::HSV_SPACE, c);

  Debug::print_rgb(c.rgb);
  std::cout << " -> ";
  Debug::print_hsv(res.hsv);
  std::cout << " -> ";
  other = convert_color_space(ColorspaceType::HSV_SPACE,
                              ColorspaceType::RGB_SPACE, res);
  Debug::print_rgb(other.rgb);
  std::cout << std::endl;
}
