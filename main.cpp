#include "colors.h"
#include <iostream>
#include <ostream>

int main() {
  APPROACH_RENDER_PNG::Colorspace8 c16;
  // c16.rgb = {255, 128, 64};
  c16.hsv = {20.1047, 74.902, 100};
  // c16.rgb = {32550, 65500, 12600};

  // auto res = convert_color_space(ColorspaceType::RGB_SPACE,
  //                                ColorspaceType::XYZ_SPACE, c16);
  // std::cout << res.xyz.X << " " << res.xyz.Y << " " << res.xyz.Z << " "
  //           << std::endl;

  auto res =
      convert_color_space(APPROACH_RENDER_PNG::ColorspaceType::HSV_SPACE,
                          APPROACH_RENDER_PNG::ColorspaceType::RGB_SPACE, c16);
  std::cout << +res.rgb.R << " " << +res.rgb.G << " " << +res.rgb.B << " "
            << std::endl;
  // std::cout << res.hsv.H << " " << res.hsv.S << " " << res.hsv.V << " "
  //           << std::endl;
}
