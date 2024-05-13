#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <iostream>
#include <string>

namespace APPROACH_RENDER_PNG {

enum ColorspaceType {
  RGB_SPACE,
  HSV_SPACE,
  XYZ_SPACE,
};

typedef struct {
  uint8_t R;
  uint8_t G;
  uint8_t B;
} RGB8;

typedef struct {
  uint16_t R;
  uint16_t G;
  uint16_t B;
} RGB16;

typedef struct {
  uint32_t R;
  uint32_t G;
  uint32_t B;
} RGB24;

typedef struct {
  double H;
  double S;
  double V;
} HSV8;

typedef struct {
  double H;
  double S;
  double V;
} HSV16;

typedef struct {
  double H;
  double S;
  double V;
} HSV24;

typedef struct {
  double X;
  double Y;
  double Z;
} XYZ8;

typedef struct {
  double X;
  double Y;
  double Z;
} XYZ16;

typedef struct {
  double X;
  double Y;
  double Z;
} XYZ24;

union Colorspace8 {
  RGB8 rgb;
  HSV8 hsv;
  XYZ8 xyz;
};

union Colorspace16 {
  RGB16 rgb;
  HSV16 hsv;
  XYZ16 xyz;
};

union Colorspace24 {
  RGB24 rgb;
  HSV24 hsv;
  XYZ24 xyz;
};

inline void linear_rgb(float &r, float &g, float &b) {
  if (r < 0.04045) {
    r = r / 12.92f;
  } else {
    r = pow((r + 0.055) / 1.055, 2.4);
  }
  if (g < 0.04045) {
    g = g / 12.92f;
  } else {
    g = pow((g + 0.055) / 1.055, 2.4);
  }
  if (b < 0.04045) {
    b = b / 12.92f;
  } else {
    b = pow((b + 0.055) / 1.055, 2.4);
  }
};

struct Converter8 {

  static std::string rgb_hex(RGB8 rgb) {
    int r, g, b;
    r = static_cast<int>(rgb.R);
    g = static_cast<int>(rgb.G);
    b = static_cast<int>(rgb.B);

    std::stringstream ss;

    ss << std::setfill('0') << std::setw(2) << std::hex << r;
    ss << std::setfill('0') << std::setw(2) << std::hex << g;
    ss << std::setfill('0') << std::setw(2) << std::hex << b;

    return ss.str();
  }

  XYZ8 rgb_to_xyz(RGB8 rgb) {
    XYZ8 xyz;

    float r = rgb.R / 255.0f;
    float g = rgb.G / 255.0f;
    float b = rgb.B / 255.0f;

    linear_rgb(r, g, b);

    xyz.X = (0.4124564f * r) + (0.3575761f * g) + (0.1804375f * b);
    xyz.Y = (0.2126729f * r) + (0.7151522f * g) + (0.0721750f * b);
    xyz.Z = (0.0193339f * r) + (0.1191920f * g) + (0.9503041f * b);

    return xyz;
  }

  RGB8 xyz_rgb(XYZ8 xyz) {
    RGB8 rgb;
    float r = (xyz.X * 3.2404542) - (xyz.Y * 1.5371385) - (xyz.Z * 0.4985314);
    float g = (xyz.X * -0.9692660) + (xyz.Y * 1.8760108) + (xyz.Z * 0.0415560);
    float b = (xyz.X * 0.0556434) - (xyz.Y * 0.2040259) + (xyz.Z * 1.0572252);

    linear_rgb(r, g, b);

    r = std::max(0.0f, std::min(1.0f, r));
    g = std::max(0.0f, std::min(1.0f, g));
    b = std::max(0.0f, std::min(1.0f, b));

    rgb.R = static_cast<int>(r * 255);
    rgb.G = static_cast<int>(g * 255);
    rgb.B = static_cast<int>(b * 255);

    return rgb;
  }

  HSV8 rgb_hsv(RGB8 rgb) {
    HSV8 hsv;
    float r = rgb.R / 255.0;
    float g = rgb.B / 255.0;
    float b = rgb.G / 255.0;

    float cmax = std::max(std::max(r, g), b);
    float cmin = std::min(std::min(r, g), b);
    float delta = cmax - cmin;

    if (delta == 0) {
      hsv.H = 0;
    } else if (cmax == r) {
      // if (g > b)
      hsv.H = 60.0f * (std::fmod(((g - b) / delta), 6.0f));
      // else
      // hsv.H = 60.0f * (std::fmod(((b - g) / delta), 6.0f));
    } else if (cmax == g) {
      hsv.H = 60.0f * (((b - r) / delta) + 2);
    } else {
      hsv.H = 60.0f * (((r - g) / delta) + 4);
    }

    if (hsv.H < 0) {
      hsv.H = 360 + hsv.H;
    }

    if (cmax == 0) {
      hsv.S = 0;
    } else {
      hsv.S = (delta / cmax) * 100;
    }

    hsv.V = cmax * 100;

    return hsv;
  }

  RGB8 hsv_rgb(HSV8 hsv) {
    RGB8 rgb;

    float h, s, v, r, g, b;

    h = hsv.H;
    s = hsv.S;
    v = hsv.V;

    h /= 360.0f;
    s /= 100.0f;
    v /= 100.0f;

    if (s == 0) {
      r = g = b = v;
    } else {
      h *= 6;
      auto i = std::floor(h);
      float f = h - i;
      auto p = v * (1 - s);
      auto q = v * (1 - s * f);
      auto t = v * (1 - s * (1 - f));

      if (i == 0) {
        r = v;
        g = t;
        b = p;
      } else if (i == 1) {
        r = q;
        g = v;
        b = p;
      } else if (i == 2) {
        r = p;
        g = v;
        b = t;
      } else if (i == 3) {
        r = p;
        g = q;
        b = v;
      } else if (i == 4) {
        r = t;
        g = p;
        b = v;
      } else {
        r = v;
        g = p;
        b = q;
      }
    }

    rgb.R = static_cast<int>(r * 255);
    rgb.G = static_cast<int>(b * 255);
    rgb.B = static_cast<int>(g * 255);

    return rgb;
  }
};

inline Colorspace8 convert_color_space(ColorspaceType srcSpace,
                                       ColorspaceType destSpace,
                                       Colorspace8 &src) {
  Converter8 conv;
  Colorspace8 dest;
  RGB8 rgb;
  HSV8 hsv;
  XYZ8 xyz;
  switch (srcSpace) {
  case RGB_SPACE:
    switch (destSpace) {
    case XYZ_SPACE:
      dest.xyz = conv.rgb_to_xyz(src.rgb);
    case RGB_SPACE:
      dest.rgb = src.rgb;
      break;
    case HSV_SPACE:
      dest.hsv = conv.rgb_hsv(src.rgb);
      break;
    }
    break;
  case HSV_SPACE:
    switch (destSpace) {
    case RGB_SPACE:
      dest.rgb = conv.hsv_rgb(src.hsv);
      break;
    case HSV_SPACE:
      dest.hsv = src.hsv;
      break;
    case XYZ_SPACE:
      rgb = conv.hsv_rgb(src.hsv);
      dest.xyz = conv.rgb_to_xyz(rgb);
      break;
    }
    break;
  case XYZ_SPACE:
    switch (destSpace) {
    case RGB_SPACE:
      dest.rgb = conv.xyz_rgb(src.xyz);
    case HSV_SPACE:
      rgb = conv.xyz_rgb(src.xyz);
      dest.hsv = conv.rgb_hsv(rgb);
    case XYZ_SPACE:
      dest.xyz = src.xyz;
      break;
    }
    break;
  }
  return dest;
};

struct Converter16 {
  XYZ16 rgb_to_xyz(RGB16 rgb) {
    XYZ16 xyz;

    float r = rgb.R / 65535.0f;
    float g = rgb.G / 65535.0f;
    float b = rgb.B / 65535.0f;

    linear_rgb(r, g, b);

    xyz.X = (0.4124564f * r) + (0.3575761f * g) + (0.1804375f * b);
    xyz.Y = (0.2126729f * r) + (0.7151522f * g) + (0.0721750f * b);
    xyz.Z = (0.0193339f * r) + (0.1191920f * g) + (0.9503041f * b);

    return xyz;
  }

  RGB16 xyz_to_rgb(XYZ16 xyz) {
    RGB16 rgb;
    float r = (xyz.X * 3.2404542) - (xyz.Y * 1.5371385) - (xyz.Z * 0.4985314);
    float g = (xyz.X * -0.9692660) + (xyz.Y * 1.8760108) + (xyz.Z * 0.0415560);
    float b = (xyz.X * 0.0556434) - (xyz.Y * 0.2040259) + (xyz.Z * 1.0572252);

    linear_rgb(r, g, b);

    r = std::max(0.0f, std::min(1.0f, r));
    g = std::max(0.0f, std::min(1.0f, g));
    b = std::max(0.0f, std::min(1.0f, b));

    rgb.R = static_cast<int>(r * 65535);
    rgb.G = static_cast<int>(g * 65535);
    rgb.B = static_cast<int>(b * 65535);

    return rgb;
  }

  // FIXME: Fix the value field
  HSV16 rgb_to_hsv(RGB16 rgb) {
    HSV16 hsv;
    float r = rgb.R / 65535.0f;
    float g = rgb.G / 65535.0f;
    float b = rgb.B / 65535.0f;

    float cmax = std::max(std::max(r, g), b);
    float cmin = std::min(std::min(r, g), b);
    float delta = cmax - cmin;

    if (delta == 0) {
      hsv.H = 0;
    } else if (cmax == r) {
      if (g > b)
        hsv.H = 60.0f * (std::fmod(((g - b) / delta), 6.0f));
      else
        hsv.H = 60.0f * (std::fmod(((b - g) / delta), 6.0f));
    } else if (cmax == g) {
      hsv.H = 60.0f * (((b - r) / delta) + 2);
    } else {
      hsv.H = 60.0f * (((r - g) / delta) + 4);
    }

    if (cmax == 0) {
      hsv.S = 0;
    } else {
      hsv.S = (delta / cmax) * 100;
    }

    hsv.V = cmax * 100;

    return hsv;
  }

  RGB16 hsv_to_rgb(HSV16 hsv) {
    RGB16 rgb;

    float h, s, v, r, g, b;

    h = hsv.H;
    s = hsv.S;
    v = hsv.V;

    h /= 360.0f;
    s /= 100.0f;
    v /= 100.0f;

    if (s == 0) {
      r = g = b = v;
    } else {
      h *= 6;
      auto i = std::floor(h);
      float f = h - i;
      auto p = v * (1 - s);
      auto q = v * (1 - s * f);
      auto t = v * (1 - s * (1 - f));

      if (i == 0) {
        r = v;
        g = t;
        b = p;
      } else if (i == 1) {
        r = q;
        g = v;
        b = p;
      } else if (i == 2) {
        r = p;
        g = v;
        b = t;
      } else if (i == 3) {
        r = p;
        g = q;
        b = v;
      } else if (i == 4) {
        r = t;
        g = p;
        b = v;
      } else {
        r = v;
        g = p;
        b = q;
      }
    }

    rgb.R = static_cast<int>(r * 65535);
    rgb.G = static_cast<int>(g * 65535);
    rgb.B = static_cast<int>(b * 65535);

    return rgb;
  }
};

inline Colorspace16 convert_color_space(ColorspaceType srcSpace,
                                        ColorspaceType destSpace,
                                        Colorspace16 &src) {
  Converter16 conv;
  Colorspace16 dest;
  RGB16 rgb;
  HSV16 hsv;
  XYZ16 xyz;
  switch (srcSpace) {
  case RGB_SPACE:
    switch (destSpace) {
    case XYZ_SPACE:
      dest.xyz = conv.rgb_to_xyz(src.rgb);
    case RGB_SPACE:
      dest.rgb = src.rgb;
      break;
    case HSV_SPACE:
      dest.hsv = conv.rgb_to_hsv(src.rgb);
      break;
    }
    break;
  case HSV_SPACE:
    switch (destSpace) {
    case RGB_SPACE:
      dest.rgb = conv.hsv_to_rgb(src.hsv);
      break;
    case HSV_SPACE:
      dest.hsv = src.hsv;
      break;
    case XYZ_SPACE:
      rgb = conv.hsv_to_rgb(src.hsv);
      dest.xyz = conv.rgb_to_xyz(rgb);
      break;
    }
    break;
  case XYZ_SPACE:
    switch (destSpace) {
    case RGB_SPACE:
      dest.rgb = conv.xyz_to_rgb(src.xyz);
    case HSV_SPACE:
      rgb = conv.xyz_to_rgb(src.xyz);
      dest.hsv = conv.rgb_to_hsv(rgb);
    case XYZ_SPACE:
      dest.xyz = src.xyz;
      break;
    }
    break;
  }
  return dest;
};

struct Converter24 {
  XYZ24 rgb_to_xyz(RGB24 rgb) {
    XYZ24 xyz;

    float r = rgb.R / 255.0f;
    float g = rgb.G / 255.0f;
    float b = rgb.B / 255.0f;

    linear_rgb(r, g, b);

    xyz.X = (0.4124564f * r) + (0.3575761f * g) + (0.1804375f * b);
    xyz.Y = (0.2126729f * r) + (0.7151522f * g) + (0.0721750f * b);
    xyz.Z = (0.0193339f * r) + (0.1191920f * g) + (0.9503041f * b);

    return xyz;
  }

  RGB24 xyz_rgb(XYZ24 xyz) {
    RGB24 rgb;
    float r = (xyz.X * 3.2404542) - (xyz.Y * 1.5371385) - (xyz.Z * 0.4985314);
    float g = (xyz.X * -0.9692660) + (xyz.Y * 1.8760108) + (xyz.Z * 0.0415560);
    float b = (xyz.X * 0.0556434) - (xyz.Y * 0.2040259) + (xyz.Z * 1.0572252);

    linear_rgb(r, g, b);

    r = std::max(0.0f, std::min(1.0f, r));
    g = std::max(0.0f, std::min(1.0f, g));
    b = std::max(0.0f, std::min(1.0f, b));

    rgb.R = static_cast<int>(r * 255);
    rgb.G = static_cast<int>(g * 255);
    rgb.B = static_cast<int>(b * 255);

    return rgb;
  }

  HSV24 rgb_hsv(RGB24 rgb) {
    HSV24 hsv;
    float r = rgb.R / 255.0;
    float g = rgb.B / 255.0;
    float b = rgb.G / 255.0;

    float cmax = std::max(std::max(r, g), b);
    float cmin = std::min(std::min(r, g), b);
    float delta = cmax - cmin;

    if (delta == 0) {
      hsv.H = 0;
    } else if (cmax == r) {
      // if (g > b)
      hsv.H = 60.0f * (std::fmod(((g - b) / delta), 6.0f));
      // else
      // hsv.H = 60.0f * (std::fmod(((b - g) / delta), 6.0f));
    } else if (cmax == g) {
      hsv.H = 60.0f * (((b - r) / delta) + 2);
    } else {
      hsv.H = 60.0f * (((r - g) / delta) + 4);
    }

    if (hsv.H < 0) {
      hsv.H = 360 + hsv.H;
    }

    if (cmax == 0) {
      hsv.S = 0;
    } else {
      hsv.S = (delta / cmax) * 100;
    }

    hsv.V = cmax * 100;

    return hsv;
  }

  RGB24 hsv_rgb(HSV24 hsv) {
    RGB24 rgb;

    float h, s, v, r, g, b;

    h = hsv.H;
    s = hsv.S;
    v = hsv.V;

    h /= 360.0f;
    s /= 100.0f;
    v /= 100.0f;

    if (s == 0) {
      r = g = b = v;
    } else {
      h *= 6;
      auto i = std::floor(h);
      float f = h - i;
      auto p = v * (1 - s);
      auto q = v * (1 - s * f);
      auto t = v * (1 - s * (1 - f));

      if (i == 0) {
        r = v;
        g = t;
        b = p;
      } else if (i == 1) {
        r = q;
        g = v;
        b = p;
      } else if (i == 2) {
        r = p;
        g = v;
        b = t;
      } else if (i == 3) {
        r = p;
        g = q;
        b = v;
      } else if (i == 4) {
        r = t;
        g = p;
        b = v;
      } else {
        r = v;
        g = p;
        b = q;
      }
    }

    rgb.R = static_cast<int>(r * 255);
    rgb.G = static_cast<int>(b * 255);
    rgb.B = static_cast<int>(g * 255);

    return rgb;
  }
};

inline Colorspace24 convert_color_space(ColorspaceType srcSpace,
                                        ColorspaceType destSpace,
                                        Colorspace24 &src) {
  Converter24 conv;
  Colorspace24 dest;
  RGB24 rgb;
  HSV24 hsv;
  XYZ24 xyz;
  switch (srcSpace) {
  case RGB_SPACE:
    switch (destSpace) {
    case XYZ_SPACE:
      dest.xyz = conv.rgb_to_xyz(src.rgb);
    case RGB_SPACE:
      dest.rgb = src.rgb;
      break;
    case HSV_SPACE:
      dest.hsv = conv.rgb_hsv(src.rgb);
      break;
    }
    break;
  case HSV_SPACE:
    switch (destSpace) {
    case RGB_SPACE:
      dest.rgb = conv.hsv_rgb(src.hsv);
      break;
    case HSV_SPACE:
      dest.hsv = src.hsv;
      break;
    case XYZ_SPACE:
      rgb = conv.hsv_rgb(src.hsv);
      dest.xyz = conv.rgb_to_xyz(rgb);
      break;
    }
    break;
  case XYZ_SPACE:
    switch (destSpace) {
    case RGB_SPACE:
      dest.rgb = conv.xyz_rgb(src.xyz);
    case HSV_SPACE:
      rgb = conv.xyz_rgb(src.xyz);
      dest.hsv = conv.rgb_hsv(rgb);
    case XYZ_SPACE:
      dest.xyz = src.xyz;
      break;
    }
    break;
  }
  return dest;
};

// A Small debug struct, just so that I don't have to repeat the print
// statements
struct Debug {
  template <typename HSV> static void print_hsv(HSV &hsv) {
    std::cout << "HSV: (" << hsv.H << ", " << hsv.S << "%, " << hsv.V << "%)";
  }

  template <typename RGB> static void print_rgb(RGB &rgb) {
    std::cout << "RGB: (" << +rgb.R << ", " << +rgb.G << ", " << +rgb.B << ")";
  }

  template <typename XYZ> static void print_xyz(XYZ &xyz) {
    std::cout << "XYZ: (" << xyz.X << ", " << xyz.Y << ", " << xyz.Z << ")";
  }
};
} // namespace APPROACH_RENDER_PNG
