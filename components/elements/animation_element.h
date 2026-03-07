#pragma once

#include <array>
#include <cmath>
#include <algorithm>
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

struct Vec3 {
  float x, y, z;
};

struct Edge {
  uint8_t a, b;
};

template<size_t NumPoints, size_t NumEdges> class SolidAnimationElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  const char *get_type_name() const override { return "solid_animation"; }

  void set_points(const std::array<Vec3, NumPoints> &points) { points_ = points; }
  void set_edges(const std::array<Edge, NumEdges> &edges) { edges_ = edges; }

  void draw(display::Display &display, int width, int height, uint32_t time) override {
    float t = time / 8000.0f;
    float ax = t * 2.0f * (float) M_PI * 0.397f;
    float ay = t * 2.0f * (float) M_PI;
    float cx_r = std::cos(ax), sx_r = std::sin(ax);
    float cy_r = std::cos(ay), sy_r = std::sin(ay);
    float scale = std::min(width, height) * 0.42f;
    float ox = width * 0.5f, oy = height * 0.5f;
    // Transform and project points.
    std::array<float, NumPoints> rz;
    std::array<int, NumPoints> px;
    std::array<int, NumPoints> py;
    for (size_t i = 0; i < NumPoints; i++) {
      const Vec3 &vert = points_[i];
      float y1 = vert.y * cx_r - vert.z * sx_r;
      float z1 = vert.y * sx_r + vert.z * cx_r;
      float x2 = vert.x * cy_r + z1 * sy_r;
      rz[i] = -vert.x * sy_r + z1 * cy_r;
      px[i] = (int) (ox + x2 * scale);
      py[i] = (int) (oy - y1 * scale);
    }
    // Clear display.
    display.clear();
    // Draw background lines first
    for (size_t i = 0; i < NumEdges; i++) {
      const Edge &edge = edges_[i];
      if (edge.a >= NumPoints || edge.b >= NumPoints)
        continue;
      float depth = ((rz[edge.a] + rz[edge.b]) * 0.5f + 1.0f) * 0.5f;
      if (depth < 0.5f) {
        display.line(px[edge.a], py[edge.a], px[edge.b], py[edge.b], get_gradient_color_(depth));
      }
    }
    // Draw foreground lines on top
    for (size_t i = 0; i < NumEdges; i++) {
      const Edge &edge = edges_[i];
      if (edge.a >= NumPoints || edge.b >= NumPoints)
        continue;
      float depth = ((rz[edge.a] + rz[edge.b]) * 0.5f + 1.0f) * 0.5f;
      if (depth >= 0.5f) {
        display.line(px[edge.a], py[edge.a], px[edge.b], py[edge.b], get_gradient_color_(depth));
      }
    }
  }

 protected:
  std::array<Vec3, NumPoints> points_{};
  std::array<Edge, NumEdges> edges_{};
};

}  // namespace esphome::elements
