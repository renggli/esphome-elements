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
  int count_{6};
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

}  // namespace esphome::elements
