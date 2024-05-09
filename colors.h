#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <type_traits>

enum Colorspace {
  RGB_SPACE,
  HSV_SPACE,
  XYZ_SPACE,
};

typedef union {
  struct {
    uint8_t R;
    uint8_t G;
    uint8_t B;
  } rgb8;

  struct {
    uint16_t R;
    uint16_t G;
    uint16_t B;
  } rgb16;

  struct {
    uint32_t R;
    uint32_t G;
    uint32_t B;
  } rgb24;

} RGB;

typedef struct {
  float X;
  float Y;
  float Z;
} XYZ;

typedef struct {
  float H;
  float S;
  float V;
} HSV;

struct Converter {
  XYZ rgb_to_xyz(RGB rgb) {
    XYZ xyz;

    float r = rgb.rgb16.R / 255.0f;
    float g = rgb.rgb16.G / 255.0f;
    float b = rgb.rgb16.B / 255.0f;

    // To handle the SRGB Alpha channel correction
    // that is one long weird word, but it is necessary
    if (r < 0.04045) {
      r = r / 12.92;
    } else {
      r = pow((r + 0.055) / 1.055, 2.4);
    }
    if (g < 0.04045) {
      g = g / 12.92;
    } else {
      g = pow((g + 0.055) / 1.055, 2.4);
    }
    if (b < 0.04045) {
      b = b / 12.92;
    } else {
      b = pow((b + 0.055) / 1.055, 2.4);
    }

    xyz.X = (0.4124564f * r) + (0.3575761f * g) + (0.1804375f * b);
    xyz.Y = (0.2126729f * r) + (0.7151522f * g) + (0.0721750f * b);
    xyz.Z = (0.0193339f * r) + (0.1191920f * g) + (0.9503041f * b);

    return xyz;
  }

  RGB xyz_to_rgb(XYZ xyz) {
    RGB rgb;
    float r = (xyz.X * 3.2404542) - (xyz.Y * 1.5371385) - (xyz.Z * 0.4985314);
    float g = (xyz.X * -0.9692660) + (xyz.Y * 1.8760108) + (xyz.Z * 0.0415560);
    float b = (xyz.X * 0.0556434) - (xyz.Y * 0.2040259) + (xyz.Z * 1.0572252);

    if (r > 0.0031308) {
      r = (1.055 * pow(r, (1 / 2.4)) - 0.055);
    } else {
      r = 12.92 * r;
    }

    if (g > 0.0031308) {
      g = ((1.055 * pow(g, 1 / 2.4)) - 0.055);
    } else {
      g = 12.92 * g;
    }

    if (b > 0.0031308) {
      b = (1.055 * pow(b, (1 / 2.4)) - 0.055);
    } else {
      b = 12.92 * b;
    }

    r = std::max(0.0f, std::min(1.0f, r));
    g = std::max(0.0f, std::min(1.0f, g));
    b = std::max(0.0f, std::min(1.0f, b));

    rgb.rgb16.R = static_cast<int>(r * 255);
    rgb.rgb16.G = static_cast<int>(g * 255);
    rgb.rgb16.B = static_cast<int>(b * 255);

    return rgb;
  }

  HSV rgb_to_hsv(RGB rgb) {
    HSV hsv;
    float r = rgb.rgb16.R / 255.0;
    float g = rgb.rgb16.B / 255.0;
    float b = rgb.rgb16.G / 255.0;

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

  RGB hsv_rgb(HSV hsv) {
    RGB rgb;

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

    rgb.rgb16.R = static_cast<int>(r * 255);
    rgb.rgb16.G = static_cast<int>(g * 255);
    rgb.rgb16.B = static_cast<int>(b * 255);

    return rgb;
  }
};

template <typename srcType, typename destType>
void convert_color_space(Colorspace srcSpace, srcType &src,
                         Colorspace destSpace, destType &dest) {
  Converter conv;
  RGB rgb;
  XYZ xyz;
  HSV hsv;

  switch (srcSpace) {
  case RGB_SPACE:
    switch (destSpace) {
    case XYZ_SPACE:
      if constexpr (std::is_same_v<destType, XYZ> &&
                    std::is_same_v<srcType, RGB>) {
        xyz = conv.rgb_to_xyz(src);
        dest.X = xyz.X;
        dest.Y = xyz.Y;
        dest.Z = xyz.Z;
      }
      break;
    case HSV_SPACE:
      if constexpr (std::is_same_v<destType, HSV> &&
                    std::is_same_v<srcType, RGB>) {
        hsv = conv.rgb_to_hsv(src);
        dest.H = hsv.H;
        dest.S = hsv.S;
        dest.V = hsv.V;
      }
      break;
    case RGB_SPACE:
      break;
    }
    break;
  case XYZ_SPACE:
    switch (destSpace) {
    case RGB_SPACE:
      if constexpr (std::is_same_v<destType, RGB> &&
                    std::is_same_v<srcType, XYZ>) {
        rgb = conv.xyz_to_rgb(src);
        dest.rgb16.R = rgb.rgb16.R;
        dest.rgb16.G = rgb.rgb16.G;
        dest.rgb16.B = rgb.rgb16.B;
      }
      break;
    case HSV_SPACE:
      if constexpr (std::is_same_v<destType, HSV> &&
                    std::is_same_v<srcType, XYZ>) {
        rgb = conv.xyz_to_rgb(src);
        hsv = conv.rgb_to_hsv(rgb);
        dest.H = hsv.H;
        dest.S = hsv.S;
        dest.V = hsv.V;
      }
      break;
    case XYZ_SPACE:
      break;
    }
    break;
  case HSV_SPACE:
    switch (destSpace) {
    case RGB_SPACE:
      if constexpr (std::is_same_v<destType, RGB> &&
                    std::is_same_v<srcType, HSV>) {
        rgb = conv.hsv_rgb(src);
        dest.rgb16.R = rgb.rgb16.R;
        dest.rgb16.G = rgb.rgb16.G;
        dest.rgb16.B = rgb.rgb16.B;
      }
      break;
    case HSV_SPACE:
      break;
    case XYZ_SPACE:
      if constexpr (std::is_same_v<destType, XYZ> &&
                    std::is_same_v<srcType, HSV>) {
        rgb = conv.hsv_rgb(src);
        xyz = conv.rgb_to_xyz(rgb);
        dest.X = xyz.X;
        dest.Y = xyz.Y;
        dest.Z = xyz.Z;
      }
      break;
    }
    break;
  }
}
