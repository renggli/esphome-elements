#pragma once

#include "element.h"
#include "esphome/components/display/display.h"
#include "geometry.h"

namespace esphome::elements {

/// Element that displays a static image.
class ImageElement : public Element {
 public:
  using Element::Element;

  const char* get_type_name() const override { return "image"; }

  void set_image(display::BaseImage* image) { image_ = image; }
  void clear_image() { image_ = nullptr; }

  void set_anchor(Anchor anchor) { anchor_ = anchor; }
  void set_align(display::ImageAlign align) { align_ = align; }

  void draw(display::Display& display) override;
  bool is_active() override;

 protected:
  display::BaseImage* image_ = nullptr;
  Anchor anchor_ = {};
  display::ImageAlign align_ = display::ImageAlign::CENTER;
};

}  // namespace esphome::elements
