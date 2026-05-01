#include "image_element.h"

#include <cmath>

namespace esphome::elements {

static const char* const TAG = "elements.image";

bool ImageElement::is_active() { return image_ != nullptr; }

void ImageElement::draw(display::Display& display) {
  if (image_ == nullptr) {
    return;
  }
  Point<int> point = anchor_.get(Point<int>::from_extent(display));
  if (scale_ == 1.0f) {
    display.image(point.x, point.y, image_, align_);
    return;
  }
  const int src_w = image_->get_width();
  const int src_h = image_->get_height();
  const int dst_w = std::lround(src_w * scale_);
  const int dst_h = std::lround(src_h * scale_);
  if (dst_w <= 0 || dst_h <= 0) return;
  Point<int> aligned = calculate_alignment_(point.x, point.y, dst_w, dst_h);
  draw_scaled_(display, aligned.x, aligned.y, src_w, src_h, dst_w, dst_h);
}

Point<int> ImageElement::calculate_alignment_(int x, int y, int dst_w,
                                              int dst_h) const {
  const auto x_align = display::ImageAlign(
      int(align_) & int(display::ImageAlign::HORIZONTAL_ALIGNMENT));
  switch (x_align) {
    case display::ImageAlign::RIGHT:
      x -= dst_w;
      break;
    case display::ImageAlign::CENTER_HORIZONTAL:
      x -= dst_w / 2;
      break;
    default:
      break;
  }
  const auto y_align = display::ImageAlign(
      int(align_) & int(display::ImageAlign::VERTICAL_ALIGNMENT));
  switch (y_align) {
    case display::ImageAlign::BOTTOM:
      y -= dst_h;
      break;
    case display::ImageAlign::CENTER_VERTICAL:
      y -= dst_h / 2;
      break;
    default:
      break;
  }
  return Point<int>(x, y);
}

void ImageElement::draw_scaled_(display::Display& display, int x, int y,
                                int src_w, int src_h, int dst_w,
                                int dst_h) const {
  for (int dst_y = 0; dst_y < dst_h; dst_y++) {
    int src_y_start = dst_y * src_h / dst_h;
    int src_y_end = (dst_y + 1) * src_h / dst_h;
    if (src_y_end == src_y_start) {
      src_y_end++;
    }
    for (int dst_x = 0; dst_x < dst_w; dst_x++) {
      int src_x_start = dst_x * src_w / dst_w;
      int src_x_end = (dst_x + 1) * src_w / dst_w;
      if (src_x_end == src_x_start) {
        src_x_end++;
      }
      Color color = sample_box_(src_x_start, src_x_end, src_y_start, src_y_end);
      if (color.w != 0 || color.r != 0 || color.g != 0 || color.b != 0) {
        display.draw_pixel_at(x + dst_x, y + dst_y, color);
      }
    }
  }
}

Color ImageElement::sample_box_(int x_start, int x_end, int y_start,
                                int y_end) const {
  int count = 0;
  uint32_t r = 0, g = 0, b = 0, w = 0;
  for (int sy = y_start; sy < y_end; sy++) {
    for (int sx = x_start; sx < x_end; sx++) {
      const Color color = image_->get_pixel(sx, sy);
      if (color.w == 0 && color.r == 0 && color.g == 0 && color.b == 0) {
        continue;
      }
      r += color.r;
      g += color.g;
      b += color.b;
      w += color.w;
      count++;
    }
  }
  if (count > 0) {
    return {
        static_cast<uint8_t>(r / count),
        static_cast<uint8_t>(g / count),
        static_cast<uint8_t>(b / count),
        static_cast<uint8_t>(w / count),
    };
  }
  return Color::BLACK;
}

}  // namespace esphome::elements
