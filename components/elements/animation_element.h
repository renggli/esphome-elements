#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <deque>
#include <numbers>
#include <vector>

#include "color.h"
#include "element.h"

namespace esphome::elements {

class AnimationElement : public Element {
 public:
  using Element::Element;

  void set_color_scheme(const ColorScheme* color_scheme) {
    color_scheme_ = color_scheme;
  }
  void set_speed(float speed) { speed_ = speed; }

  void draw(display::Display& display) override;
  virtual void draw(display::Display& display, int width, int height,
                    uint32_t time) = 0;

  void on_show() override;

 protected:
  const ColorScheme* color_scheme_{nullptr};
  uint32_t start_time_{0};
  float speed_{1.0f};

  Color get_color_(float p) { return color_scheme_->get_color(p); }
};

class MetaballsAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char* get_type_name() const override { return "meatballs_animation"; }

  void set_count(int count) { count_ = count; }
  void draw(display::Display& display, int width, int height,
            uint32_t time) override;

 protected:
  int count_{3};
};

class AuroraAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char* get_type_name() const override { return "aurora_animation"; }

  void draw(display::Display& display, int width, int height,
            uint32_t time) override;
};

class KaleidoscopeAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char* get_type_name() const override {
    return "kaleidoscope_animation";
  }

  void draw(display::Display& display, int width, int height,
            uint32_t time) override;
};

class PlasmaAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char* get_type_name() const override { return "plasma_animation"; }

  void draw(display::Display& display, int width, int height,
            uint32_t time) override;
};

class RipplesAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char* get_type_name() const override { return "ripples_animation"; }

  void set_count(int count) { count_ = count; }
  void draw(display::Display& display, int width, int height,
            uint32_t time) override;

 protected:
  int count_{8};
};

class SpiralAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char* get_type_name() const override { return "spiral_animation"; }

  void draw(display::Display& display, int width, int height,
            uint32_t time) override;
};

class VoronoiAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char* get_type_name() const override { return "voronoi_animation"; }

  void set_count(int count) { count_ = count; }
  void draw(display::Display& display, int width, int height,
            uint32_t time) override;

 protected:
  int count_{5};
};

class InterferenceAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char* get_type_name() const override {
    return "interference_animation";
  }

  void set_count(int count) { count_ = count; }
  void draw(display::Display& display, int width, int height,
            uint32_t time) override;

 protected:
  int count_{3};
};

class JuliaAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char* get_type_name() const override { return "julia_animation"; }

  void draw(display::Display& display, int width, int height,
            uint32_t time) override;
};

class MatrixAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char* get_type_name() const override { return "matrix_animation"; }

  void set_density(float density) { density_ = density; }
  void set_length(float length) { length_ = length; }

  void draw(display::Display& display, int width, int height,
            uint32_t time) override;

 protected:
  float length_{10.0f};
  float density_{0.5f};
};

class FireAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char* get_type_name() const override { return "fire_animation"; }

  void set_strength(float strength) { strength_ = strength; }
  void set_cooling(float cooling) { cooling_ = cooling; }

  void on_hide() override;
  void draw(display::Display& display, int width, int height,
            uint32_t time) override;

 protected:
  float strength_{0.98f};
  float cooling_{0.02f};
  std::vector<float> heat_buffer_;
};

class TunnelAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char* get_type_name() const override { return "tunnel_animation"; }

  void draw(display::Display& display, int width, int height,
            uint32_t time) override;
};

class WaveAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char* get_type_name() const override { return "wave_animation"; }

  void draw(display::Display& display, int width, int height,
            uint32_t time) override;
};

class StarsAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char* get_type_name() const override { return "stars_animation"; }

  void set_density(float density) { density_ = density; }

  void draw(display::Display& display, int width, int height,
            uint32_t time) override;

 protected:
  float density_{0.05f};
};

struct Vec3 {
  float x, y, z;
};

struct Edge {
  uint8_t a, b;
};

class SolidAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char* get_type_name() const override { return "solid_animation"; }

  void set_points(const Vec3* points, size_t num_points) {
    points_ = points;
    num_points_ = num_points;
  }
  void set_edges(const Edge* edges, size_t num_edges) {
    edges_ = edges;
    num_edges_ = num_edges;
  }

  void draw(display::Display& display, int width, int height,
            uint32_t time) override;

 protected:
  const Vec3* points_{nullptr};
  size_t num_points_{0};
  const Edge* edges_{nullptr};
  size_t num_edges_{0};
};

class ParallaxAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char* get_type_name() const override { return "parallax_animation"; }

  void set_num_layers(int num_layers) { num_layers_ = num_layers; }

  void draw(display::Display& display, int width, int height,
            uint32_t time) override;

 protected:
  int num_layers_{3};
};

class LorenzAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char* get_type_name() const override { return "lorenz_animation"; }

  void set_length(int length) { length_ = length; }
  void set_sigma(float sigma) { sigma_ = sigma; }
  void set_rho(float rho) { rho_ = rho; }
  void set_beta(float beta) { beta_ = beta; }

  void on_show() override;
  void draw(display::Display& display, int width, int height,
            uint32_t time) override;

 protected:
  void step_();

  std::deque<Vec3> points_;
  float x_{0.1f}, y_{0.0f}, z_{0.0f};
  int length_{300};
  float sigma_{10.0f};
  float rho_{28.0f};
  float beta_{8.0f / 3.0f};
};

}  // namespace esphome::elements
