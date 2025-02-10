#pragma once

#include "element.h"

namespace esphome::elements {

enum class ScrollMode {
  NONE,
  LEFT_TO_RIGHT,
  RIGHT_TO_LEFT,
  BOTTOM_TO_TOP,
  TOP_TO_BOTTOM,
};

class TextElement : public Element {
 public:
  explicit TextElement(ElementComponent* component, Element* parent)
      : Element(ElementType::TEXT, component, parent) {}

  void set_font(display::BaseFont* font) { font_ = font; }
  void set_color(Color color) { color_ = color; }
  void set_background_color(Color background_color) {
    background_color_ = background_color;
  }
  void set_position_x(float position_x) { position_x_ = position_x; }
  void set_position_y(float position_y) { position_y_ = position_y; }
  void set_align(display::TextAlign align) { align_ = align; }

  void set_scroll_mode(ScrollMode scroll_mode) { scroll_mode_ = scroll_mode; }
  void set_scroll_speed(float scroll_speed) { scroll_speed_ = scroll_speed; }

  void set_update_interval(uint32_t update_interval_ms) {
    update_interval_ms_ = update_interval_ms;
  }
  void set_text(std::string text);
  void set_lambda(std::function<std::string(const Context&)> lambda);

  void draw(display::Display& display) override;

  void on_show() override;

 protected:
  display::BaseFont* font_ = nullptr;
  Color color_ = Color::WHITE;
  Color background_color_ = Color::BLACK;
  display::TextAlign align_ = display::TextAlign::CENTER;
  float position_x_ = 0.5, position_y_ = 0.5;

  ScrollMode scroll_mode_ = ScrollMode::NONE;
  float scroll_speed_ = 10.0;  // pixel per second
  float scroll_offset_ = 0.0;

  uint32_t update_interval_ms_ = 250;
  optional<std::string> text_;
  optional<std::function<std::string(const Context&)>> lambda_;

  uint32_t last_update_ms_;
  bool request_measurement_ = false;
  int bounds_x_, bounds_y_, bounds_w_, bounds_h_;
};

}  // namespace esphome::elements
