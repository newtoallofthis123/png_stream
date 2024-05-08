#include "stream.h"
#include <cstdint>
#include <png.h>
#include <pngconf.h>
#include <sstream>
#include <string>
#include <vector>

namespace APPROACH_RENDER_PNG {
// To Represent the RGB colors
struct RGB {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

// The needed options for the PNG Generation
// use default_png_options() for sensible defaults
struct Options {
  RGB colors;
  unsigned int width;
  unsigned int height;
  uint8_t bit_depth;
  uint8_t color_type;
  uint8_t compression_method;
  uint8_t filter_method;
  uint8_t interlace_method;
};

class PNG : Stream {
public:
  PNG(Options ihdr) : options(ihdr) { setup(); }
  PNG(uint8_t red, uint8_t green, uint8_t blue, unsigned int width,
      unsigned int height)
      : options(default_png_options(RGB{red, green, blue}, width, height)) {
    setup();
  }
  PNG(RGB rgb) : options(default_png_options(rgb, 800, 600)) { setup(); }
  PNG(RGB rgb, unsigned int width, unsigned int height)
      : options(default_png_options(rgb, width, height)) {
    setup();
  }

  png_structp png_ptr;
  std::vector<std::vector<png_bytep>> content;

  void StreamHead(std::stringstream &stream) {
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

  void StreamCorpus(std::stringstream &stream) {
    for (auto rows : content) {
      for (auto row : rows) {
        png_write_row(png_ptr, row);
      }
    }
  }

  void StreamTail(std::stringstream &stream) { png_write_end(png_ptr, NULL); }

  void stream(std::stringstream &stream) {
    this->StreamHead(stream);
    this->StreamCorpus(stream);
    this->StreamTail(stream);
  }

  std::string render(std::stringstream &str_stream) {
    stream(str_stream);

    return str_stream.str();
  }

  ~PNG() { png_destroy_write_struct(&png_ptr, &png_info); }

private:
  Options options;
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

  Options default_png_options(RGB rgb, unsigned int width,
                              unsigned int height) {
    return Options{rgb,
                   width,
                   height,
                   8,
                   PNG_COLOR_TYPE_RGB,
                   PNG_INTERLACE_NONE,
                   PNG_COMPRESSION_TYPE_DEFAULT,
                   PNG_FILTER_TYPE_DEFAULT};
  }
};
} // namespace APPROACH_RENDER_PNG
