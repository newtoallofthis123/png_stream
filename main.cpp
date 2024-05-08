#include "pngstream.h"
#include <fstream>
#include <sstream>

using namespace APPROACH_RENDER_PNG;

int main() {
  PNG p(RGB{220, 180, 101}, 800, 600);

  std::stringstream stream;

  p.generate_bg();

  auto output = p.render(stream);

  std::cout << output << std::endl;

  std::ofstream outfile("hello.png", std::ios::binary);
  outfile << output;
  outfile.close();
}
