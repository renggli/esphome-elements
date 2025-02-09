#pragma once

#include "element.h"

namespace esphome::elements {

enum class TextScroll {
  NONE,
  LEFT_TO_RIGHT,
  RIGHT_TO_LEFT,
  TOP_TO_BOTTOM,
  BOTTOM_TO_TOP,
}

class TextElement : public Element {
 public:
  explicit TextElement() : Element(ElementType::TEXT) {}

  void set_font(display::BaseFont* font) { font_ = font; }
  void set_color(Color color) { color_ = color; }
  void set_background_color(Color background_color) { background_color_ = background_color; }
  void set_position_x(float position_x) { position_x_ = position_x; }
  void set_position_y(float position_y) { position_y_ = position_y; }
  void set_align(display::TextAlign align) { align_ = align; }

  void set_text(std::string text) {
    text_ = text;
    lambda_ = nullopt;
    request_measurement_ = true;
  }

  void set_lambda(std::function<std::string(const Context&)> lambda) {
    text_ = nullopt;
    lambda_ = lambda;
    request_measurement_ = true;
  }

  void draw(const Context& context, display::Display& display) override;

  void on_show(const Context& context) override;

 protected:
  display::BaseFont* font_ = nullptr;
  Color color_ = Color::WHITE;
  Color background_color_ = Color::BLACK;
  float position_x_ = 0.5, position_y_ = 0.5;
  display::TextAlign align_ = display::TextAlign::CENTER;
  uint32_t update_every_ms_ = 1000;

  optional<std::string> text_;
  optional<std::function<std::string(const Context&)>> lambda_;

  uint32_t last_update_ms_;
  bool request_measurement_ = true;
  int bounds_x_, bounds_y_, bounds_w_, bounds_h_;
};

}  // namespace esphome::elements
