#include "display.h"

#include "esphome/core/log.h"

namespace esphome::elements {

static const char *const IMAGE_DISPLAY_TAG = "elements.display";

ImageDisplay::ImageDisplay(int width, int height) : width_(width), height_(height) {
  buffer_ = allocator_.allocate(width_ * height_);
  if (buffer_ == nullptr) {
    ESP_LOGE(IMAGE_DISPLAY_TAG, "Could not allocate buffer of ImageDisplay.");
    return;
  }
  clear();
}

ImageDisplay::~ImageDisplay() { allocator_.deallocate(buffer_, width_ * height_); }

void ImageDisplay::draw_pixel_at(int x, int y, Color color) {
  if (0 <= x && x < width_ && 0 <= y && y < height_) {
    buffer_[x + (y * width_)] = color;
  }
}

void ImageDisplay::draw(int x, int y, display::Display *display, Color color_on, Color color_off) {
  for (int img_x = 0; img_x < width_; img_x++) {
    for (int img_y = 0; img_y < height_; img_y++) {
      display->draw_pixel_at(x + img_x, y + img_y, buffer_[img_x + (img_y * width_)]);
    }
  }
}

}  // namespace esphome::elements
