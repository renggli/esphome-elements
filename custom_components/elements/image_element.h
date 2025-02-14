#pragma once

#include "element.h"
#include "esphome/components/display/display.h"
#include "geometry.h"

namespace esphome::elements {

using display::BaseImage;
using display::ImageAlign;

/// Elements that displays a static image.
class ImageElement : public Element {
 public:
  explicit ImageElement(ElementComponent* component, Element* parent)
      : Element(ElementType::IMAGE, component, parent) {}

  void set_image(BaseImage* image) { image_ = image; }
  void set_anchor(Anchor anchor) { anchor_ = anchor; }
  void set_align(ImageAlign align) { align_ = align; }

  void draw(display::Display& display) override;

 protected:
  BaseImage* image_ = nullptr;
  Anchor anchor_;
  ImageAlign align_;
};

}  // namespace esphome::elements
