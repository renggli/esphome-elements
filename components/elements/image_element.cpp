#include "image_element.h"

namespace esphome::elements {

void ImageElement::draw(display::Display &display) {
  if (image_ == nullptr) {
    return;
  }
  Point<int> point = anchor_.get(Point<int>::fromExtent(display));
  display.image(point.x, point.y, image_, align_);
}

} // namespace esphome::elements
