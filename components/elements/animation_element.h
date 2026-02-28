#pragma once

#include <cstdint>
#include "color.h"
#include "element.h"

namespace esphome::elements {

class AnimationElement : public Element {
 public:
  AnimationElement(ElementComponent *component, Element *parent) : Element(ElementType::ANIMATION, component, parent) {}

  void set_color_scheme(ColorScheme *color_scheme) { color_scheme_ = color_scheme; }
  void set_speed(float speed) { speed_ = speed; }

  void draw(display::Display &display) override;
  virtual void draw(display::Display &display, int width, int height, uint32_t time) = 0;

 protected:
  ColorScheme *color_scheme_{nullptr};
  float speed_{1.0f};

  Color get_gradient_color_(float p);
};

// METABALLS

class MetaballsElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

// AURORA

class AuroraElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

// KALEIDOSCOPE

class KaleidoscopeElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

// PLASMA

class PlasmaElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

// RIPPLES

class RipplesElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

// SPIRAL

class SpiralElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

// VORONOI

class VoronoiElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

// INTERFERENCE

class InterferenceElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

// JULIA

class JuliaElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

// MATRIX

class MatrixElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  void set_density(float density) { density_ = density; }
  void set_length(float length) { length_ = length; }

  void draw(display::Display &display, int width, int height, uint32_t time) override;

 protected:
  float length_{10.0f};
  float density_{0.5f};
};

// GRADIENT

class GradientElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

// FIRE

class FireElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  void set_strength(float strength) { strength_ = strength; }

  void draw(display::Display &display, int width, int height, uint32_t time) override;

 protected:
  float strength_{2.0f};
};

// TUNNEL

class TunnelElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

// WAVE

class WaveElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  void draw(display::Display &display, int width, int height, uint32_t time) override;
};

// STARS

class StarsElement : public AnimationElement {
 public:
  using AnimationElement::AnimationElement;
  void set_density(float density) { density_ = density; }

  void draw(display::Display &display, int width, int height, uint32_t time) override;

 protected:
  float density_{0.02f};
};

}  // namespace esphome::elements
