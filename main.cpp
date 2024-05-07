#include "png.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

int main() {
  PngStream p(10, 10);

  p.generate_data(255, 0, 0);

  stringstream strstream;

  p.render(strstream);

  cout << strstream.str();

  // Uncomment this to write to a file called hello.png
  std::ofstream file("hello.png", std::ios::out | std::ios::binary);
  file << strstream.rdbuf();
  file.close();

  return 0;
}
