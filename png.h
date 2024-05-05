#include "stream.h"
#include <cstdint>
#include <sstream>
#include <vector>
#include <zlib.h>

#ifndef PNG_H
#define PNG_H

class PngStream : Stream {
public:
  // Make a png with the width and the height
  PngStream(int width, int height) : width(width), height(height) {
    pixel_data.resize(width * height * 3);
  }
  uint width;
  uint height;
  std::vector<uint8_t> pixel_data;

  // Writes some simple image data to the pixel_data
  // TODO: Make it actually work
  void generate_data(uint8_t red, uint8_t blue, uint8_t green) {
    for (uint32_t y = 0; y < height; ++y) {
      for (uint32_t x = 0; x < width; ++x) {
        this->pixel_data[(y * width + x) * 3] = red;
        this->pixel_data[(y * width + x) * 3 + 1] = green;
        this->pixel_data[(y * width + x) * 3 + 2] = blue;
      }
    }
  }

  // Makes it actually a recognizable PNG format
  // ie writes out PNG in binary ascii
  void write_signature(std::stringstream &stream) {
    stream.write(reinterpret_cast<const char *>(PNG_SIGNATURE),
                 sizeof(PNG_SIGNATURE));
  }

  void RenderHead(std::stringstream &stream) {
    this->write_signature(stream);

    const char chunk_type[] = "IHDR";
    // I don't know why we write 8,2,0,0,0 but it seems to be important
    // for the PNG specification
    const uint32_t head_data[] = {width, height, 8, 2, 0, 0, 0};
    const uint32_t crc =
        crc32(0, reinterpret_cast<const Bytef *>(chunk_type), 4);
    const uint32_t chunk_length = 13;

    stream.write(reinterpret_cast<const char *>(&chunk_length),
                 sizeof(uint32_t));
    stream.write(chunk_type, 4);
    stream.write(reinterpret_cast<const char *>(head_data), sizeof(head_data));
    stream.write(reinterpret_cast<const char *>(&crc), sizeof(uint32_t));
  }

  void RenderCorpus(std::stringstream &stream) {
    // NOTE: Well for now, this image doesn't really have a recursive part,
    // but if it were to, it would be here

    // This is the "content" of the image
    const uint32_t chunk_length = pixel_data.size();

    const char chunk_type[] = "IDAT";
    const uint32_t crc =
        crc32(0, reinterpret_cast<const Bytef *>(chunk_type), 4);

    stream.write(reinterpret_cast<const char *>(&chunk_length),
                 sizeof(uint32_t));
    stream.write(chunk_type, 4);
    stream.write(reinterpret_cast<const char *>(pixel_data.data()),
                 pixel_data.size());
    stream.write(reinterpret_cast<const char *>(&crc), sizeof(uint32_t));
  }

  void RenderTail(std::stringstream &stream) {
    const uint32_t chunk_length = 0;
    const char chunk_type[] = "IEND";
    const uint32_t crc =
        crc32(0, reinterpret_cast<const Bytef *>(chunk_type), 4);

    stream.write(reinterpret_cast<const char *>(&chunk_length),
                 sizeof(uint32_t));
    stream.write(chunk_type, 4);
    stream.write(reinterpret_cast<const char *>(&crc), sizeof(uint32_t));
  }

  void render(std::stringstream &stream) {
    this->RenderHead(stream);
    this->RenderCorpus(stream);
    this->RenderTail(stream);
  }

private:
  // This is how the string is identified as a png:
  // @see: http://www.libpng.org/pub/png/spec/1.2/PNG-Structure.html
  const uint8_t PNG_SIGNATURE[8] = {137, 80, 78, 71, 13, 10, 26, 10};
};

#endif
