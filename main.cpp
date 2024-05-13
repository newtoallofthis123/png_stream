#include "colors.h"
using namespace APPROACH_RENDER_PNG;

int main() {
  Colorspace8 c8;

  c8.rgb = {25, 23, 36};

  auto res = convert_color_space(ColorspaceType::RGB_SPACE,
                                 ColorspaceType::HSV_SPACE, c8);

  std::cout << "Info for: " << Converter8::rgb_hex(c8.rgb) << std::endl;
  Debug::print_rgb(c8.rgb);
  Debug::print_hsv(res.hsv);
}
