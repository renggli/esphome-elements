#include "image_element.h"

namespace esphome::elements {

bool ImageElement::is_active() { return image_ != nullptr; }

void ImageElement::draw(Canvas &canvas) {
  if (image_ == nullptr) {
    return;
  }
  Point<int> point = anchor_.get(Point<int>::from_extent(canvas));
  // TODO display.image(point.x, point.y, image_, align_);
}

}  // namespace esphome::elements
