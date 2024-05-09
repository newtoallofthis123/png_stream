#include "colors.h"
#include <iostream>
#include <ostream>

int main() {
  RGB rgb{0, 0, 0};
  // rgb.rgb16.R = 255;
  // rgb.rgb16.G = 128;
  // rgb.rgb16.B = 64;
  // auto xyz = XYZ{0.49889, 0.37075, 0.09378};
  XYZ xyz{0, 0, 0};
  // HSV hsv{0, 0, 0};
  HSV hsv{49.8894, 37.0746, 9.37843};

  // convert_color_space(Colorspace::RGB_SPACE, rgb, Colorspace::XYZ_SPACE,
  // xyz); convert_color_space(Colorspace::XYZ_SPACE, xyz,
  // Colorspace::HSV_SPACE, hsv);
  convert_color_space(Colorspace::HSV_SPACE, hsv, Colorspace::RGB_SPACE, rgb);
  convert_color_space(Colorspace::HSV_SPACE, hsv, Colorspace::XYZ_SPACE, xyz);

  std::cout << std::endl;
  std::cout << "RGB: " << rgb.rgb16.R << " " << rgb.rgb16.G << " "
            << rgb.rgb16.B << std::endl;
  std::cout << "HSV: " << hsv.H << " " << hsv.S << " " << hsv.V << std::endl;
  std::cout << "XYZ: " << xyz.X * 100 << " " << xyz.Y * 100 << " "
            << xyz.Z * 100 << std::endl;
  std::cout << std::endl;
}
