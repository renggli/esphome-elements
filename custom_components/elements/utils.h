#pragma once

#include "esphome/components/display/display.h"

namespace esphome::elements {

/// If the value of v is within [lo, hi], returns v; otherwise returns the
/// nearest boundary. Replacement for C++17.
template<class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
    return v < lo ? lo : hi < v ? hi : v;
}

/// A sub-display for an existing display.
class SubDisplay : public display::Display {
 public:
  SubDisplay(display::Display& display, int x, int y, int w, int h)
      : display_(display),
        x_(clamp(x, 0, display.get_width())),
        y_(clamp(y, 0, display.get_height())),
        w_(clamp(w, 0, display.get_width() - x)),
        h_(clamp(h, 0, display.get_height() - y)) {}

  void HOT draw_pixel_at(int x, int y, Color color) override {
    if (0 <= x && x < w_ && 0 <= y && y < h_) {
      display_.draw_pixel_at(x_ + x, y_ + y, color);
    }
  }

  display::DisplayType get_display_type() override {
    return display_.get_display_type();
  }

 protected:
  display::Display& display_;
  int x_, y_, w_, h_;

  void update() override {}

  int get_width_internal() override { return w_; }
  int get_height_internal() override { return h_; }
};

}  // namespace esphome::elements
