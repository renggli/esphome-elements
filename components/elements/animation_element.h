#pragma once

#include <cstdint>
#include <vector>

#include "color.h"
#include "element.h"

namespace esphome::elements {

class AnimationElement : public Element {
 public:
  using Element::Element;

  void set_color_scheme(ColorScheme *color_scheme) { color_scheme_ = color_scheme; }
  void set_speed(float speed) { speed_ = speed; }

  void draw(display::Display &display) override;
  virtual void draw(display::Display &display, int width, int height, uint32_t time) = 0;

 protected:
  ColorScheme *color_scheme_{nullptr};
  float speed_{1.0f};

  Color get_gradient_color_(float p);
};

class MetaballsAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char *get_type_name() const override { return "meatballs_animation"; }

  void set_count(int count) { count_ = count; }
  void draw(display::Display &display, int width, int height, uint32_t time) override;

 protected:
  int count_{3};
};

class AuroraAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char *get_type_name() const override { return "aurora_animation"; }

  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

class KaleidoscopeAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char *get_type_name() const override { return "kaleidoscope_animation"; }

  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

class PlasmaAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char *get_type_name() const override { return "plasma_animation"; }

  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

class RipplesAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char *get_type_name() const override { return "ripples_animation"; }

  void set_count(int count) { count_ = count; }
  void draw(display::Display &display, int width, int height, uint32_t time) override;

 protected:
  int count_{8};
};

class SpiralAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char *get_type_name() const override { return "spiral_animation"; }

  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

class VoronoiAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char *get_type_name() const override { return "voronoi_animation"; }

  void set_count(int count) { count_ = count; }
  void draw(display::Display &display, int width, int height, uint32_t time) override;

 protected:
  int count_{5};
};

class InterferenceAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char *get_type_name() const override { return "interference_animation"; }

  void set_count(int count) { count_ = count; }
  void draw(display::Display &display, int width, int height, uint32_t time) override;

 protected:
  int count_{3};
};

class JuliaAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char *get_type_name() const override { return "julia_animation"; }

  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

class MatrixAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char *get_type_name() const override { return "matrix_animation"; }

  void set_density(float density) { density_ = density; }
  void set_length(float length) { length_ = length; }

  void draw(display::Display &display, int width, int height, uint32_t time) override;

 protected:
  float length_{10.0f};
  float density_{0.5f};
};

class GradientAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char *get_type_name() const override { return "gradient_animation"; }

  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

class FireAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char *get_type_name() const override { return "fire_animation"; }

  void set_strength(float strength) { strength_ = strength; }
  void set_cooling(float cooling) { cooling_ = cooling; }

  void on_hide() override;
  void draw(display::Display &display, int width, int height, uint32_t time) override;

 protected:
  float strength_{0.98f};
  float cooling_{0.02f};
  std::vector<float> heat_buffer_;
};

class TunnelAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char *get_type_name() const override { return "tunnel_animation"; }

  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

class WaveAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char *get_type_name() const override { return "wave_animation"; }

  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

class StarsAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char *get_type_name() const override { return "stars_animation"; }

  void set_density(float density) { density_ = density; }

  void draw(display::Display &display, int width, int height, uint32_t time) override;

 protected:
  float density_{0.05f};
};

class GameOfLifeAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char *get_type_name() const override { return "game_of_life_animation"; }

  void set_density(float density) { density_ = density; }
  void set_fade_steps(int fade_steps) { fade_steps_ = fade_steps; }

  void on_hide() override;
  void draw(display::Display &display, int width, int height, uint32_t time) override;

 protected:
  void seed_grid_(int size, uint32_t time);

  float density_{0.25f};
  int fade_steps_{8};

  // 1 byte per pixel:
  // - high nibble (0x10): alive flag
  // - low nibble  (0x0F): fade counter (fade_steps_..0, decremented each step while dying)
  std::vector<uint8_t> grid_;
  uint32_t last_step_time_{0};
  uint32_t step_interval_{300};  // ms between generations
  uint32_t near_dead_count_{0};  // steps with ≤2 live cells; triggers reseed
};

enum class Shape : uint8_t {
  TETRAHEDRON,
  CUBE,
  OCTAHEDRON,
  ICOSAHEDRON,
  DODECAHEDRON,
  SPHERE,
};

class SolidAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char *get_type_name() const override { return "solid_animation"; }

  void set_shape(Shape shape) { shape_ = shape; }

  void draw(display::Display &display, int width, int height, uint32_t time) override;

 protected:
  Shape shape_{Shape::ICOSAHEDRON};
};

}  // namespace esphome::elements
