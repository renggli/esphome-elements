#include "custom_element.h"

namespace esphome::elements {

namespace {
using display::Display;
}

void CustomElement::draw(Display &display) {
  if (draw_) {
    draw_(*this, display);
  } else {
    test_draw(display);
  }
}

void CustomElement::on_show() {
  if (on_show_) {
    on_show_(*this);
  }
}

void CustomElement::on_hide() {
  if (on_hide_) {
    on_hide_(*this);
  }
}

void CustomElement::on_next() {
  if (on_next_) {
    on_next_(*this);
  } else {
    Element::on_next();
  }
}

bool CustomElement::is_active() {
  if (is_active_) {
    return is_active_(*this);
  } else {
    return Element::is_active();
  }
}

void CustomElement::test_draw(Display &display) {
  int center_x = display.get_width() / 2, center_y = display.get_height() / 2;
  float delta = fmod(get_component().get_current_ms() / 2000.0, 2.0 * PI);
  int x1 = center_x, y1 = center_y * sin(delta) + center_y;
  for (int i = 1; i <= 0xff; i++) {
    float t = 2.0 * PI * i / 0xff, r, g, b;
    hsv_to_rgb(360 * i / 0xff, 1.0, 1.0, r, g, b);
    int x2 = center_x * sin(2.0 * t) + center_x;
    int y2 = center_y * sin(3.0 * t + delta) + center_y;
    display.line(x1, y1, x2, y2, Color(0xff * r, 0xff * g, 0xff * b));
    x1 = x2;
    y1 = y2;
  }
}
} // namespace esphome::elements
