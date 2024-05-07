#include "stream.h"
#include <cstdint>
#include <sstream>
#include <vector>
#include <zlib.h>

#ifndef PNG_H
#define PNG_H

// The Png seems to be made up of three components that all need
//  appropriate start tags:
//  1. IHDR - Image Header (Contains information about the head)
//  2. IDAT - Image Data (Containers the actual image data)
//  3. IEND - Image End (Ends the image)
//
//  So, these can be neatly labled as `RenderHead`, `RenderCorpus`, `RenderTail`
//
//  INFO: Only supports a single RGB color for now
class PngStream : Stream {
public:
  // Make a png with the width and the height
  PngStream(int width, int height) : width(width), height(height) {}
  uint width;
  uint height;
  // To Actually hold the pixels
  // the content would be a vector of binary uint_8 rather than a string
  std::vector<unsigned char> content = {
      0,   0,   0,   0,  // Chunk length
      'I', 'D', 'A', 'T' // Chunk type
  };

  // Writes some simple image data to the pixel_data
  // Images are stoed as a continous array of 3 elements: RGB
  // This method of * 3 is quite weird, but it seemed quick and dirty
  // or some of the other stuff I found was quite complex
  //
  // NOTE: This has to called before render to actually write to the image data
  // before rendering it out
  // The content vector can be directly edited as well
  //
  // TODO: Make it actually and customizable
  void generate_data(uint8_t red, uint8_t blue, uint8_t green) {
    for (int y = 0; y < height; ++y) {
      content.push_back(0); // Filter type
      for (int x = 0; x < width; ++x) {
        content.push_back(red);
        content.push_back(green);
        content.push_back(blue);
      }
    }
  }

  // Makes it actually a recognizable PNG format
  // ie writes out PNG in binary ascii
  void write_signature(std::stringstream &stream) {
    for (auto i : PNG_SIGNATURE) {
      stream << i;
    }
  }

  void RenderHead(std::stringstream &stream) {
    write_signature(stream);

    std::vector<unsigned char> ihdr = {0, 0, 0, 13, 'I', 'H', 'D', 'R', 0, 0, 0,
                                       0, 0, 0, 0,  0,   8,   6,   0,   0, 0};

    ihdr[8] = (width >> 24) & 0xFF;
    ihdr[9] = (width >> 16) & 0xFF;
    ihdr[10] = (width >> 8) & 0xFF;
    ihdr[11] = width & 0xFF;
    ihdr[12] = (height >> 24) & 0xFF;
    ihdr[13] = (height >> 16) & 0xFF;
    ihdr[14] = (height >> 8) & 0xFF;
    ihdr[15] = height & 0xFF;

    add_crc(ihdr);

    for (auto i : ihdr) {
      stream << i;
    }
  }

  void RenderCorpus(std::stringstream &stream) {
    // NOTE: Well for now, this image doesn't really have a recursive part,
    // but if it were to, it would be here

    int dataSize = width * height * 3;
    content[0] = (dataSize >> 24) & 0xFF;
    content[1] = (dataSize >> 16) & 0xFF;
    content[2] = (dataSize >> 8) & 0xFF;
    content[3] = dataSize & 0xFF;

    add_crc(content);

    for (auto i : content) {
      stream << i;
    }
  }

  void RenderTail(std::stringstream &stream) {
    std::vector<unsigned char> iend = {
        0,    0,    0,    0,   // Chunk length
        'I',  'E',  'N',  'D', // Chunk type
        0x00, 0x00, 0x00, 0x00,
    };

    add_crc(iend);

    for (auto i : iend) {
      stream << i;
    }
  }

  void render(std::stringstream &stream) {
    this->RenderHead(stream);
    this->RenderCorpus(stream);
    this->RenderTail(stream);
  }

private:
  const uint8_t PNG_SIGNATURE[8] = {137, 80, 78, 71, 13, 10, 26, 10};

  // Not used anymore
  uint32_t big_endian_convert(uint32_t value) {
    return ((value & 0xFF000000) >> 24) | ((value & 0x00FF0000) >> 8) |
           ((value & 0x0000FF00) << 8) | ((value & 0x000000FF) << 24);
  }

  void add_crc(std::vector<unsigned char> &chunk) {
    unsigned long crc = crc32(0L, 0, 0);
    crc = crc32(crc, &chunk[4], chunk.size() - 8);
    chunk[chunk.size() - 4] = (crc >> 24) & 0xFF;
    chunk[chunk.size() - 3] = (crc >> 16) & 0xFF;
    chunk[chunk.size() - 2] = (crc >> 8) & 0xFF;
    chunk[chunk.size() - 1] = crc & 0xFF;
  }
};

#endif
