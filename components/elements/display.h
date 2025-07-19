#pragma once

#include "esphome/components/display/display.h"
#include "esphome/core/helpers.h"

namespace esphome::elements {

/// An image that is backed by a display buffer.
class ImageDisplay : public display::BaseImage, public display::Display {
 public:
  ImageDisplay(int width, int height);
  ~ImageDisplay();

  display::DisplayType get_display_type() override { return display::DisplayType::DISPLAY_TYPE_COLOR; }

  void HOT draw_pixel_at(int x, int y, Color color) override;
  void draw(int x, int y, Display *display, Color color_on, Color color_off) override;

 protected:
  int width_, height_;
  RAMAllocator<Color> allocator_;
  Color *buffer_ = nullptr;

  void update() override {}

  int get_width_internal() override { return width_; }
  int get_height_internal() override { return height_; }
};

/// A sub-display of an existing display.
class SubDisplay : public display::Display {
 public:
  SubDisplay(display::Display &display, int x, int y, int w, int h)
      : display_(display),
        x_(clamp(x, 0, display.get_width())),
        y_(clamp(y, 0, display.get_height())),
        w_(clamp(w, 0, display.get_width() - x)),
        h_(clamp(h, 0, display.get_height() - y)) {}

  display::DisplayType get_display_type() override { return display_.get_display_type(); }

  void HOT draw_pixel_at(int x, int y, Color color) override {
    if (0 <= x && x < w_ && 0 <= y && y < h_) {
      display_.draw_pixel_at(x_ + x, y_ + y, color);
    }
  }

 protected:
  display::Display &display_;
  int x_, y_, w_, h_;

  void update() override {}

  int get_width_internal() override { return w_; }
  int get_height_internal() override { return h_; }
};

}  // namespace esphome::elements
