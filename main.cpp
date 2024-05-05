#include "png.h"
#include <iostream>
#include <sstream>
// #include <fstream>

using namespace std;

int main() {
  PngStream p(10, 10);

  p.generate_data(255, 15, 2);

  stringstream strstream;

  p.render(strstream);

  auto str = strstream.str();
  cout << str;

  // Uncomment this to write to a file called hello.png
  // auto bin = std::vector<uint8_t>(str.begin(), str.end());
  // std::ofstream file("hello.png", std::ios::out | std::ios::binary);
  // file.write(reinterpret_cast<const char *>(bin.data()), bin.size());
  // file.close();

  return 0;
}
