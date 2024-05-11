#include "colors.h"
#include <iostream>
#include <ostream>

int main() {
  Colorspace8 c8;
  c8.rgb = {255, 128, 64};

  Colorspace8 res = convert_color_space(ColorspaceType::RGB_SPACE,
                                 ColorspaceType::XYZ_SPACE, c8);
  std::cout << res.xyz.X << " " << res.xyz.Y << " " << res.xyz.Z << " "
            << std::endl;
}
