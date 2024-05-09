#include "colors.h"
#include <iostream>

int main() {
  RGB rgb{0, 0, 0};
  rgb.rgb16.R = 255;
  rgb.rgb16.G = 128;
  rgb.rgb16.B = 64;
  // auto xyz = XYZ{0.49889, 0.37075, 0.09378};
  XYZ xyz{0, 0, 0};
  HSV hsv{0, 0, 0};

  convert_color_space(Colorspace::RGB_SPACE, rgb, Colorspace::XYZ_SPACE, xyz);
  convert_color_space(Colorspace::XYZ_SPACE, xyz, Colorspace::HSV_SPACE, hsv);

  std::cout << rgb.rgb16.R << " " << rgb.rgb16.G << " " << rgb.rgb16.B
            << std::endl;
  // std::cout << hsv.H << " " << hsv.S << " " << hsv.V << std::endl;
  std::cout << xyz.X * 100 << " " << xyz.Y * 100 << " " << xyz.Z * 100
            << std::endl;
}
