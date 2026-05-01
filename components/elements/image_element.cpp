#include "image_element.h"

#include "esphome/core/log.h"

namespace esphome::elements {

static const char* const TAG = "elements.image";

bool ImageElement::is_active() { return image_ != nullptr; }

void ImageElement::draw(display::Display& display) {
  if (image_ == nullptr) {
    return;
  }
  Point<int> point = anchor_.get(Point<int>::from_extent(display));

  // Fast path: no scaling — delegate entirely to ESPHome (handles all image
  // types, transparency, etc. and has zero overhead).
  if (scale_ == 1.0f) {
    display.image(point.x, point.y, image_, align_);
    return;
  }

  const int src_w = image_->get_width();
  const int src_h = image_->get_height();
  const int dst_w = static_cast<int>(src_w * scale_ + 0.5f);
  const int dst_h = static_cast<int>(src_h * scale_ + 0.5f);

  if (dst_w <= 0 || dst_h <= 0) {
    return;
  }

  // Render the source image into an off-screen buffer so we can sample
  // individual pixels regardless of the concrete BaseImage subtype. This
  // avoids depending on RTTI (dynamic_cast) which is disabled for ESP32
  // builds (-fno-rtti).
  ImageDisplay src_buf(src_w, src_h);
  if (!src_buf.is_valid()) {
    ESP_LOGE(TAG, "Failed to allocate source buffer for scaling.");
    return;
  }
  image_->draw(0, 0, &src_buf, display::COLOR_ON, display::COLOR_OFF);

  // Resolve alignment against the scaled image dimensions, mirroring the
  // logic in Display::image().
  auto x_align = display::ImageAlign(
      int(align_) & int(display::ImageAlign::HORIZONTAL_ALIGNMENT));
  auto y_align = display::ImageAlign(
      int(align_) & int(display::ImageAlign::VERTICAL_ALIGNMENT));

  int x = point.x;
  int y = point.y;

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

  // Nearest-neighbor scaling: for each destination pixel map back to the
  // nearest source pixel and draw it.
  for (int dst_y = 0; dst_y < dst_h; dst_y++) {
    const int src_y = dst_y * src_h / dst_h;
    for (int dst_x = 0; dst_x < dst_w; dst_x++) {
      const int src_x = dst_x * src_w / dst_w;
      const Color color = src_buf.get_pixel(src_x, src_y);
      if (color.w == 0 && color.r == 0 && color.g == 0 && color.b == 0) {
        continue;
      }
      display.draw_pixel_at(x + dst_x, y + dst_y, color);
    }
  }
}

}  // namespace esphome::elements
