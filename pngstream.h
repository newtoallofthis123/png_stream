#include "stream.h"
#include <cstdint>
#include <png.h>
#include <pngconf.h>
#include <sstream>
#include <vector>

#ifndef PNGSTREAM_H
#define PNGSTREAN_H

// To Represent the RGB colors
struct RGB {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

// The needed options for the PNG Generation
// use default_png_options() for sensible defaults
struct PngStream_Options {
  RGB colors;
  unsigned int width;
  unsigned int height;
  uint8_t bit_depth;
  uint8_t color_type;
  uint8_t compression_method;
  uint8_t filter_method;
  uint8_t interlace_method;
};

class PngStream : Stream {
public:
  PngStream(PngStream_Options ihdr) : options(ihdr) { setup(); }
  PngStream(uint8_t red, uint8_t green, uint8_t blue, unsigned int width,
            unsigned int height)
      : options(default_png_options(RGB{red, green, blue}, width, height)) {
    setup();
  }
  PngStream(RGB rgb) : options(default_png_options(rgb, 800, 600)) { setup(); }
  PngStream(RGB rgb, unsigned int width, unsigned int height)
      : options(default_png_options(rgb, width, height)) {
    setup();
  }

  png_structp png_ptr;
  png_bytep png_row;
  std::vector<std::vector<png_bytep>> content;

  void RenderHead(std::stringstream &stream) {
    // After this step, the png itself becomes our stream and all the activities
    // done on the png would be automatically written to the stream
    png_set_write_fn(
        png_ptr, &stream,
        [](png_structp png_ptr, png_bytep data, png_size_t length) {
          std::stringstream *stream =
              static_cast<std::stringstream *>(png_get_io_ptr(png_ptr));
          stream->write(reinterpret_cast<const char *>(data), length);
        },
        NULL);

    png_set_IHDR(png_ptr, png_info, options.width, options.height,
                 options.bit_depth, options.color_type,
                 options.interlace_method, options.compression_method,
                 options.filter_method);

    png_write_info(png_ptr, png_info);
  }

  // This would be like the default way
  // The usual way would be to manipulate the
  // content directly
  void generate_bg() {
    auto data = std::vector<png_bytep>();
    auto row = (png_bytep)malloc(3 * options.width * sizeof(png_byte));

    for (int y = 0; y < options.height; y++) {
      for (int x = 0; x < options.width; x++) {
        row[x * 3] = options.colors.red;
        row[x * 3 + 1] = options.colors.green;
        row[x * 3 + 2] = options.colors.blue;
      }
      data.push_back(row);
    }

    content.push_back(data);
  }

  void RenderCorpus(std::stringstream &stream) {
    for (auto rows : content) {
      for (auto row : rows) {
        png_write_row(png_ptr, row);
      }
    }
  }

  void RenderTail(std::stringstream &stream) { png_write_end(png_ptr, NULL); }

  void render(std::stringstream &stream) {
    this->RenderHead(stream);
    this->RenderCorpus(stream);
    this->RenderTail(stream);
  }

  ~PngStream() { png_destroy_write_struct(&png_ptr, &png_info); }

private:
  PngStream_Options options;
  png_infop png_info;

  void setup() {
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
      std::cerr << "Error creating write struct" << std::endl;
      return;
    }

    png_info = png_create_info_struct(png_ptr);
    if (!png_info) {
      std::cerr << "Error creating info struct" << std::endl;
      png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
      return;
    }
  }

  PngStream_Options default_png_options(RGB rgb, unsigned int width,
                                        unsigned int height) {
    return PngStream_Options{rgb,
                             width,
                             height,
                             8,
                             PNG_COLOR_TYPE_RGB,
                             PNG_INTERLACE_NONE,
                             PNG_COMPRESSION_TYPE_DEFAULT,
                             PNG_FILTER_TYPE_DEFAULT};
  }
};

#endif
