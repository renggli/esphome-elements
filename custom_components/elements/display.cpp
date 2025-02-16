#include "display.h"

namespace esphome::elements {

AliasedDisplay::AliasedDisplay(display::Display& display) : display_(display) {
  buffer_ =
      new Color[scale_squared_ * display.get_width() * display.get_height()];
  clear();
}

AliasedDisplay::~AliasedDisplay() { delete buffer_; }

void AliasedDisplay::commit() {
  for (int x1 = 0; x1 < display_.get_width(); x1++) {
    for (int y1 = 0; y1 < display_.get_height(); y1++) {
      int r = 0, g = 0, b = 0;
      for (int x2 = scale_ * x1; x2 < scale_ * x1 + scale_; x2++) {
        for (int y2 = scale_ * y1; y2 < scale_ * y1 + scale_; y2++) {
          Color source = buffer_[x2 + y2 * get_width()];
          r += source.r;
          g += source.g;
          b += source.b;
        }
      }
      display_.draw_pixel_at(
          x1, y1,
          Color(r / scale_squared_, g / scale_squared_, b / scale_squared_));
    }
  }
  clear();
}

}  // namespace esphome::elements
