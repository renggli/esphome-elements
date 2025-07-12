#include "image_element.h"

namespace esphome::elements {

bool ImageElement::is_active() { return image_ != nullptr; }

void ImageElement::draw(display::Display &display) {
  if (image_ == nullptr) {
    return;
  }
  Point<int> point = anchor_.get(Point<int>::from_extent(display));
  display.image(point.x, point.y, image_, align_);
}

}  // namespace esphome::elements
