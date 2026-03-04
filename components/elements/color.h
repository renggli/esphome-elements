#pragma once

#include <vector>

#include "esphome/core/color.h"

namespace esphome::elements {

// ----------------------------------------------------------------------------
// HSV ↔ RGB utilities (inline for header-only use in other files)
// ----------------------------------------------------------------------------

Color hsv_to_color(float h, float s, float v);
void color_to_hsv(Color c, float &h, float &s, float &v);
float lerp_hue(float from_h, float to_h, float t);

class ColorScheme {
 public:
  virtual ~ColorScheme() = default;

  // Primary interface: p in [0, 1] → HSV output
  virtual void get_hsv(float p, float &h, float &s, float &v) = 0;

  // Convenience wrapper: calls get_hsv and converts to RGB Color.
  Color get_color(float p);
};

// Returns a single fixed HSV color regardless of p.
class StaticColorScheme : public ColorScheme {
 public:
  void set_color(Color c);
  void set_hsv(float h, float s, float v);

  void get_hsv(float p, float &h, float &s, float &v) override;

 private:
  float h_ = 0.0f, s_ = 1.0f, v_ = 1.0f;
};

// Interpolates two HSV endpoints using shortest-path hue blending.
class GradientColorScheme : public ColorScheme {
 public:
  void set_from(Color c);
  void set_from_hsv(float h, float s, float v);

  void set_to(Color c);
  void set_to_hsv(float h, float s, float v);

  void get_hsv(float p, float &h, float &s, float &v) override;

 private:
  float from_h_ = 0.0f, from_s_ = 1.0f, from_v_ = 1.0f;
  float to_h_ = 180.0f, to_s_ = 1.0f, to_v_ = 1.0f;
};

// Wraps another scheme and reflects p: maps [0,1] to a triangle 0→1→0.
// This makes the scheme go from start to end and back seamlessly.
class MirrorColorScheme : public ColorScheme {
 public:
  void set_scheme(ColorScheme *scheme);
  void get_hsv(float p, float &h, float &s, float &v) override;

 private:
  ColorScheme *scheme_ = nullptr;
};

// Wraps another scheme and inverts p: maps [0,1] to [1,0].
class InverseColorScheme : public ColorScheme {
 public:
  void set_scheme(ColorScheme *scheme);
  void get_hsv(float p, float &h, float &s, float &v) override;

 private:
  ColorScheme *scheme_ = nullptr;
};

// Splits [0,1] into segments and maps each to a child scheme.
class SequenceColorScheme : public ColorScheme {
 public:
  void add_scheme(ColorScheme *scheme, float fraction = 1.0f);
  void get_hsv(float p, float &h, float &s, float &v) override;

 private:
  struct SequenceItem {
    ColorScheme *scheme;
    float fraction;
    float cumulative_fraction;
  };
  std::vector<SequenceItem> schemes_;
  float total_fraction_ = 0.0f;
};

// Palette factory functions
ColorScheme *make_monochromatic(float h, float s, float v, float min_v = 0.2f);
ColorScheme *make_analogous(float h, float s, float v, float sweep_deg = 45.0f);
ColorScheme *make_complementary(float h, float s, float v, float sweep_deg = 30.0f);
ColorScheme *make_split_complementary(float h, float s, float v, float sweep_deg = 20.0f);
ColorScheme *make_triadic(float h, float s, float v, float sweep_deg = 20.0f);
ColorScheme *make_square(float h, float s, float v, float sweep_deg = 15.0f);

}  // namespace esphome::elements
