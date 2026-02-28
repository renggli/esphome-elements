#pragma once

#include "element.h"
#include "esphome/core/helpers.h"

namespace esphome::elements {

enum class ArtsyColorScheme : std::uint8_t {
  MONOCHROMATIC,
  ANALOGOUS,
  COMPLEMENTARY,
  TRIADIC,
  SPLIT_COMPLEMENTARY,
  DUAL_COMPLEMENTARY,
  RAINBOW,
};

enum class ArtsyPattern : std::uint8_t {
  METABALLS,
  AURORA,
  KALEIDOSCOPE,
  PLASMA,
  RIPPLES,
  SPIRAL,
  VORONOI,
  INTERFERENCE,
  JULIA,
  MATRIX,
  LISSAJOUS,
  FIRE,
  TUNNEL,
  WAVE,
  STARS,
};

class ArtsyElement : public Element {
 public:
  ArtsyElement(ElementComponent *component, Element *parent) : Element(ElementType::ARTSY, component, parent) {}

  void set_color(Color color) { color_ = color; }
  void set_color_scheme(ArtsyColorScheme color_scheme) { color_scheme_ = color_scheme; }
  void set_pattern(ArtsyPattern pattern) { pattern_ = pattern; }
  void set_speed(float speed) { speed_ = speed; }
  void set_density(float density) { density_ = density; }
  void set_min_brightness(float min_brightness) { min_brightness_ = min_brightness; }

  void on_show() override;
  void draw(display::Display &display) override;

 protected:
  Color color_;
  ArtsyColorScheme color_scheme_{ArtsyColorScheme::MONOCHROMATIC};
  ArtsyPattern pattern_{ArtsyPattern::METABALLS};

  float base_h_{0};
  float base_s_{0.8f};
  float base_v_{0.8f};

  float speed_{1.0f};
  float density_{1.0f};
  float min_brightness_{0.3f};

  void draw_metaballs_(display::Display &display, int width, int height, uint32_t current_ms);
  void draw_aurora_(display::Display &display, int width, int height, uint32_t current_ms);
  void draw_kaleidoscope_(display::Display &display, int width, int height, uint32_t current_ms);
  void draw_plasma_(display::Display &display, int width, int height, uint32_t current_ms);
  void draw_ripples_(display::Display &display, int width, int height, uint32_t current_ms);
  void draw_spiral_(display::Display &display, int width, int height, uint32_t current_ms);
  void draw_voronoi_(display::Display &display, int width, int height, uint32_t current_ms);
  void draw_interference_(display::Display &display, int width, int height, uint32_t current_ms);
  void draw_julia_(display::Display &display, int width, int height, uint32_t current_ms);
  void draw_matrix_(display::Display &display, int width, int height, uint32_t current_ms);
  void draw_lissajous_(display::Display &display, int width, int height, uint32_t current_ms);
  void draw_fire_(display::Display &display, int width, int height, uint32_t current_ms);
  void draw_tunnel_(display::Display &display, int width, int height, uint32_t current_ms);
  void draw_wave_(display::Display &display, int width, int height, uint32_t current_ms);
  void draw_stars_(display::Display &display, int width, int height, uint32_t current_ms);

  Color get_gradient_color_(float p) const;
};

}  // namespace esphome::elements
