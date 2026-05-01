#pragma once

#include "display.h"
#include "element.h"
#include "esphome/components/display/display.h"
#include "geometry.h"

namespace esphome::elements {

/// Element that displays a static image, with optional scaling.
class ImageElement : public Element {
 public:
  using Element::Element;

  const char* get_type_name() const override { return "image"; }

  void set_image(display::BaseImage* image) { image_ = image; }
  void clear_image() { image_ = nullptr; }

  void set_anchor(Anchor anchor) { anchor_ = anchor; }
  void set_align(display::ImageAlign align) { align_ = align; }

  /// Scale factor applied when drawing the image. 1.0 = no scaling (default).
  /// Values > 1.0 enlarge, values < 1.0 shrink. Uses nearest-neighbor sampling.
  void set_scale(float scale) { scale_ = scale; }

  void draw(display::Display& display) override;
  bool is_active() override;

 protected:
  display::BaseImage* image_ = nullptr;
  Anchor anchor_ = {};
  display::ImageAlign align_ = display::ImageAlign::CENTER;
  float scale_ = 1.0f;
};

}  // namespace esphome::elements
