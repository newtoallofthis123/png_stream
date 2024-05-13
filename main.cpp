#include "colors.h"
#include <iostream>
#include <ostream>

int main() {
  APPROACH_RENDER_PNG::Colorspace8 c8;
  c8.rgb = {25, 23, 36};

  auto res =
      convert_color_space(APPROACH_RENDER_PNG::ColorspaceType::RGB_SPACE,
                          APPROACH_RENDER_PNG::ColorspaceType::HSV_SPACE, c8);
  std::cout << res.hsv.H << " " << res.hsv.S << " " << res.hsv.V << " "
            << std::endl;
}
