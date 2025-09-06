#include "canvas.h"

#include "esphome/core/log.h"

namespace esphome::elements {

void Canvas::set_pixel(int x, int y, Color color) { get_pixel(x, y) = color; }

void Canvas::set_pixel(int x, int y, Color color, float alpha) {
  Color &target = get_pixel(x, y);
  if (alpha >= 1.0) {
    target = color;
  } else if (alpha > 0.0) {
    target.r = static_cast<uint8_t>(color.r * alpha + target.r * (1.0f - alpha));
    target.g = static_cast<uint8_t>(color.g * alpha + target.g * (1.0f - alpha));
    target.b = static_cast<uint8_t>(color.b * alpha + target.b * (1.0f - alpha));
  }
}

void Canvas::clear() { fill(Color::BLACK); }

void Canvas::fill(Color color) {
  for (int y = 0; y < get_height(); y++) {
    for (int x = 0; x < get_width(); x++) {
      get_pixel(x, y) = color;
    }
  }
}

void Canvas::line(int x1, int y1, int x2, int y2, Color color) {
  bool steep = std::abs(y2 - y1) > std::abs(x2 - x1);
  if (steep) {
    std::swap(x1, y1);
    std::swap(x2, y2);
  }
  if (x1 > x2) {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }
  float dx = static_cast<float>(x2 - x1);
  float dy = static_cast<float>(y2 - y1);
  float gradient = (dx == 0) ? 0.0f : dy / dx;
  for (int x = x1; x <= x2; x++) {
    float y = static_cast<float>(y1) + gradient * (static_cast<float>(x) - static_cast<float>(x1));
    float frac_y = y - std::floor(y);
    float alpha1 = 1.0f - frac_y;
    float alpha2 = frac_y;
    int y_floor = static_cast<int>(std::floor(y));
    if (steep) {
      set_pixel(y_floor, x, color, alpha1);
      set_pixel(y_floor + 1, x, color, alpha2);
    } else {
      set_pixel(x, y_floor, color, alpha1);
      set_pixel(x, y_floor + 1, color, alpha2);
    }
  }
}

void Canvas::line_at_angle(int x, int y, int angle, int length, Color color) {
  line_at_angle(x, y, angle, 0, length, color);
}

void Canvas::line_at_angle(int x, int y, int angle, int start_radius, int stop_radius, Color color) {
  int x1 = static_cast<int>(start_radius * cos(angle * M_PI / 180.0)) + x;
  int y1 = static_cast<int>(start_radius * sin(angle * M_PI / 180.0)) + y;
  int x2 = static_cast<int>(stop_radius * cos(angle * M_PI / 180.0)) + x;
  int y2 = static_cast<int>(stop_radius * sin(angle * M_PI / 180.0)) + y;
  this->line(x1, y1, x2, y2, color);
}

void Canvas::copy_to(display::Display &display, int x, int y) const {
  for (int j = 0; j < get_height(); j++) {
    for (int i = 0; i < get_width(); i++) {
      display.draw_pixel_at(x + i, y + j, get_pixel(i, j));
    }
  }
}

MemoryCanvas::MemoryCanvas() : width_(0), height_(0), buffer_(nullptr) {}

MemoryCanvas::MemoryCanvas(int width, int height) : MemoryCanvas() { resize(width, height); }

MemoryCanvas::~MemoryCanvas() {
  if (buffer_ != nullptr) {
    allocator_.deallocate(buffer_, width_ * height_);
    width_ = height_ = 0;
    buffer_ = nullptr;
  }
}

void MemoryCanvas::resize(int width, int height) {
  if (width == width_ && height == height_) {
    return;
  }
  if (buffer_ != nullptr) {
    allocator_.deallocate(buffer_, width_ * height_);
    width_ = height_ = 0;
    buffer_ = nullptr;
  }
  width_ = width;
  height_ = height;
  if (width_ > 0 && height_ > 0) {
    buffer_ = allocator_.allocate(width_ * height_);
    if (buffer_ == nullptr) {
      ESP_LOGE(__FUNCTION__, "Unable to allocate canvas buffer of size %d * %d.", width_, height_);
      width_ = height_ = 0;
      buffer_ = nullptr;
    } else {
      clear();
    }
  }
}

Color hex_to_color(const std::string &hex) {
  std::string_view str = hex;
  if (!str.empty() && str[0] == '#') {
    str.remove_prefix(1);
  }
  uint8_t r, g, b;
  switch (str.length()) {
    case 3: {
      if (sscanf(std::string(str).c_str(), "%1hhx%1hhx%1hhx", &r, &g, &b) == 3) {
        return Color(0x10 * r, 0x10 * g, 0x10 * b);
      }
      break;
    }
    case 6: {
      if (sscanf(std::string(str).c_str(), "%02hhx%02hhx%02hhx", &r, &g, &b) == 3) {
        return Color(r, g, b);
      }
      break;
    }
  }
  ESP_LOGE(__FUNCTION__, "Unable to parse \"%s\" as hex color", hex.c_str());
  return Color::BLACK;
}

}  // namespace esphome::elements
