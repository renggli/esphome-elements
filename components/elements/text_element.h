#pragma once

#include "element.h"
#include "esphome/components/time/real_time_clock.h"
#include "geometry.h"
#include "timer.h"

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
  TextElement(ElementType elementType, ElementComponent* component,
              Element* parent)
      : Element(elementType, component, parent) {}

  void set_font(display::BaseFont* font) { font_ = font; }
  void set_color(Color color) { color_ = color; }
  void set_background_color(Color background_color) {
    background_color_ = background_color;
  }
  void set_anchor(Anchor anchor) { anchor_ = anchor; }
  void set_align(display::TextAlign align) { align_ = align; }

  void set_scroll_mode(ScrollMode scroll_mode) { scroll_mode_ = scroll_mode; }
  void set_scroll_speed(float scroll_speed) { scroll_speed_ = scroll_speed; }

  void draw(display::Display& display) override;
  void on_show() override;
  bool is_active() override;

 protected:
  virtual std::string get_text() = 0;

  display::BaseFont* font_ = nullptr;
  Color color_ = Color::WHITE;
  Color background_color_ = Color::BLACK;
  Anchor anchor_;
  display::TextAlign align_;

  ScrollMode scroll_mode_ = ScrollMode::NONE;
  float scroll_speed_ = 10.0;  // pixel per second
  float scroll_offset_ = 0.0;

  std::string text_;
  bool request_measurement_ = true;
  int bounds_x_, bounds_y_, bounds_w_, bounds_h_;
};

class StaticTextElement : public TextElement {
 public:
  StaticTextElement(ElementComponent* component, Element* parent)
      : TextElement(ElementType::STATIC_TEXT, component, parent) {}

  void set_text(std::string text) { text_ = text; }

 protected:
  std::string get_text() override;

  std::string text_;
};

class DynamicTextElement : public TextElement {
 public:
  DynamicTextElement(ElementComponent* component, Element* parent)
      : TextElement(ElementType::DYNAMIC_TEXT, component, parent) {}

  void set_lambda(std::function<std::string(DynamicTextElement&)> lambda) {
    lambda_ = lambda;
  }

 protected:
  std::string get_text() override;

  std::function<std::string(DynamicTextElement&)> lambda_;
};

class TimeTextElement : public TextElement {
 public:
  TimeTextElement(ElementComponent* component, Element* parent)
      : TextElement(ElementType::TIME_TEXT, component, parent) {}

  void set_time(time::RealTimeClock* time) { time_ = time; }
  void set_format(std::string format) { format_ = format; }

 protected:
  std::string get_text() override;

  time::RealTimeClock* time_;
  std::string format_;
};

}  // namespace esphome::elements
