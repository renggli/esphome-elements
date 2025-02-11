#include "image_element.h"

namespace esphome::elements {

void ImageElement::draw(display::Display& display) {
  if (image_ == nullptr) return;

  // Compute the placement of the image.
  int x = position_x_ * display.get_width();
  int y = position_y_ * display.get_height();

  // Draw the image.
  display.image(x, y, image_, align_);
}

}  // namespace esphome::elements
