#pragma once

#include "element.h"
#include "esphome/components/display/display.h"
#include "esphome/components/image/image.h"
#include "geometry.h"

namespace esphome::elements {

/// Element that displays a static image, with optional scaling.
class ImageElement : public Element {
 public:
  using Element::Element;

  const char* get_type_name() const override { return "image"; }

  void set_image(image::Image* image) { image_ = image; }
  void clear_image() { image_ = nullptr; }

  void set_anchor(Anchor anchor) { anchor_ = anchor; }
  void set_align(display::ImageAlign align) { align_ = align; }
  void set_scale(float scale) { scale_ = scale; }

  void draw(display::Display& display) override;
  bool is_active() override;

 protected:
  Point<int> calculate_alignment_(int x, int y, int dst_w, int dst_h) const;
  void draw_scaled_(display::Display& display, int x, int y, int src_w,
                    int src_h, int dst_w, int dst_h) const;
  Color sample_box_(int x_start, int x_end, int y_start, int y_end) const;

  image::Image* image_ = nullptr;
  Anchor anchor_ = {};
  display::ImageAlign align_ = display::ImageAlign::CENTER;
  float scale_ = 1.0f;
};

}  // namespace esphome::elements
