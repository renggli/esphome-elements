#pragma once

#include "esphome/components/display/display.h"
#include "esphome/components/display/display_buffer.h"

namespace esphome::elements {

/// A sub-display of an existing display.
class SubDisplay : public display::Display {
 public:
  SubDisplay(display::Display& display, int x, int y, int w, int h)
      : display_(display),
        x_(clamp(x, 0, display.get_width())),
        y_(clamp(y, 0, display.get_height())),
        w_(clamp(w, 0, display.get_width() - x)),
        h_(clamp(h, 0, display.get_height() - y)) {}

  display::DisplayType get_display_type() override {
    return display_.get_display_type();
  }

  void HOT draw_pixel_at(int x, int y, Color color) override {
    if (0 <= x && x < w_ && 0 <= y && y < h_) {
      display_.draw_pixel_at(x_ + x, y_ + y, color);
    }
  }

 protected:
  display::Display& display_;
  int x_, y_, w_, h_;

  void update() override {}

  int get_width_internal() override { return w_; }
  int get_height_internal() override { return h_; }
};

/// An aliased display of an existing display.
class AliasedDisplay : public display::Display {
 public:
  AliasedDisplay(display::Display& display);
  ~AliasedDisplay();

  display::DisplayType get_display_type() override {
    return display_.get_display_type();
  }

  void HOT draw_pixel_at(int x, int y, Color color) override {
    if (0 <= x && x < get_width() && 0 <= y && y < get_height()) {
      buffer_[x + y * get_width()] = color;
    }
  }

  void commit();

 protected:
  static const int scale_ = 2;
  static const int scale_squared_ = scale_ * scale_;

  display::Display& display_;
  Color* buffer_;

  void update() override {}

  int get_width_internal() override { return scale_ * display_.get_width(); }
  int get_height_internal() override { return scale_ * display_.get_height(); }
};

}  // namespace esphome::elements
