#pragma once

#include "color.h"
#include "element.h"

namespace esphome::elements {

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
  GRADIENT,
  FIRE,
  TUNNEL,
  WAVE,
  STARS,
};

class ArtsyElement : public Element {
 public:
  ArtsyElement(ElementComponent *component, Element *parent) : Element(ElementType::ARTSY, component, parent) {}

  void set_color(Color color) { color_ = color; }
  void set_color_scheme(ColorScheme *color_scheme) { color_scheme_ = color_scheme; }
  void set_pattern(ArtsyPattern pattern) { pattern_ = pattern; }
  void set_speed(float speed) { speed_ = speed; }
  void set_density(float density) { density_ = density; }
  void set_strength(float strength) { strength_ = strength; }
  void set_min_brightness(float min_brightness) { min_brightness_ = min_brightness; }

  void on_show() override;
  void draw(display::Display &display) override;

 protected:
  Color color_;
  ColorScheme *color_scheme_{nullptr};
  ArtsyPattern pattern_{ArtsyPattern::METABALLS};

  float base_h_{0};
  float base_s_{0.8f};
  float base_v_{0.8f};
  float min_brightness_{0.3f};

  float speed_{1.0f};
  float density_{1.0f};
  float strength_{1.0f};

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
  void draw_gradient_(display::Display &display, int width, int height, uint32_t current_ms);
  void draw_fire_(display::Display &display, int width, int height, uint32_t current_ms);
  void draw_tunnel_(display::Display &display, int width, int height, uint32_t current_ms);
  void draw_wave_(display::Display &display, int width, int height, uint32_t current_ms);
  void draw_stars_(display::Display &display, int width, int height, uint32_t current_ms);

  Color get_gradient_color_(float p);
};

}  // namespace esphome::elements
