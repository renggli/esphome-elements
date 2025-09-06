#pragma once

#include "esphome/components/display/display.h"
#include "esphome/core/helpers.h"

namespace esphome::elements {

/// A canvas provides means for drawing graphics.
class Canvas {
 public:
  virtual int get_width() const = 0;
  virtual int get_height() const = 0;

  virtual HOT Color &get_pixel(int x, int y) const = 0;
  virtual HOT Color &get_pixel(int x, int y) = 0;
  HOT void set_pixel(int x, int y, Color color);
  HOT void set_pixel(int x, int y, Color color, float alpha);

  void clear();
  void fill(Color color);
  void line(int x1, int y1, int x2, int y2, Color color);
  void line_at_angle(int x, int y, int angle, int length, Color color);
  void line_at_angle(int x, int y, int angle, int start_radius, int stop_radius, Color color);

  void copy_to(display::Display &display, int x = 0, int y = 0) const;
};

/// A canvas backed with a memory buffer.
class MemoryCanvas : public Canvas {
 public:
  MemoryCanvas();
  MemoryCanvas(int width, int height);
  ~MemoryCanvas();

  void resize(int width, int height);
  bool is_valid() const { return this->buffer_ != nullptr; }

  int get_width() const override { return width_; };
  int get_height() const override { return height_; }

  HOT Color &get_pixel(int x, int y) const override { return buffer_[y * width_ + x]; }
  HOT Color &get_pixel(int x, int y) override { return buffer_[y * width_ + x]; }

 protected:
  RAMAllocator<Color> allocator_;
  int width_ = 0, height_ = 0;
  Color *buffer_ = nullptr;
};

/// A canvas that draws into a sub-region of another canvas.
class SubCanvas : public Canvas {
 public:
  SubCanvas(Canvas &canvas, int x, int y, int w, int h)
      : canvas_(canvas),
        x_(clamp(x, 0, canvas.get_width())),
        y_(clamp(y, 0, canvas.get_height())),
        w_(clamp(w, 0, canvas.get_width() - x)),
        h_(clamp(h, 0, canvas.get_height() - y)) {}

  int get_width() const override { return w_; };
  int get_height() const override { return h_; }

  HOT Color &get_pixel(int x, int y) const override { return canvas_.get_pixel(x + x_, y + y_); };
  HOT Color &get_pixel(int x, int y) override { return canvas_.get_pixel(x + x_, y + y_); };

 protected:
  Canvas &canvas_;
  int x_, y_, w_, h_;
};

/// Helper to convert a hex color to a color object.
Color hex_to_color(const std::string &hex);

}  // namespace esphome::elements
