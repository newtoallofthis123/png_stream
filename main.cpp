#include "pngstream.h"
#include <fstream>
#include <sstream>

int main() {
  PngStream p(RGB{220, 180, 101});

  std::stringstream stream;

  p.generate_bg();

  p.render(stream);

  std::cout << stream.str() << std::endl;

  std::ofstream outfile("hello.png", std::ios::binary);
  outfile << stream.str();
  outfile.close();

  // std::cout << "PNG data written to hello.png" << std::endl;
}
