#pragma once

#include "element.h"
#include "esphome/components/display/display.h"

namespace esphome::elements {

using display::BaseImage;
using display::ImageAlign;

/// Elements that displays a static or animated image.
class ImageElement : public Element {
 public:
  explicit ImageElement(ElementComponent* component, Element* parent)
      : Element(ElementType::IMAGE, component, parent) {}

  void set_image(BaseImage* image) { image_ = image; }
  void set_align(ImageAlign align) { align_ = align; }
  void set_position_x(float position_x) { position_x_ = position_x; }
  void set_position_y(float position_y) { position_y_ = position_y; }

  void draw(display::Display& display) override;

 protected:
  BaseImage* image_;
  ImageAlign align_ = ImageAlign::CENTER;
  float position_x_ = 0.5, position_y_ = 0.5;
};

}  // namespace esphome::elements
