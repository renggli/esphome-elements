#include "custom_element.h"

#include <numbers>

namespace esphome::elements {

void CustomElement::draw(display::Display& display) {
  if (draw_) {
    draw_(*this, display);
  } else {
    test_draw_(display);
  }
}

bool CustomElement::is_active() {
  if (is_active_) {
    return is_active_(*this);
  }
  return Element::is_active();
}

void CustomElement::test_draw_(display::Display& display) {
  int center_x = display.get_width() / 2;
  int center_y = display.get_height() / 2;
  float delta = fmod(millis() / 2000.0f, 2.0f * std::numbers::pi_v<float>);
  int x1 = center_x;
  int y1 = (center_y * sin(delta)) + center_y;
  for (int i = 1; i <= 0xff; i++) {
    float t = 2.0f * std::numbers::pi * i / 0xff;
    float r;
    float g;
    float b;
    hsv_to_rgb(360 * i / 0xff, 1.0f, 1.0f, r, g, b);
    int x2 = (center_x * sin(2.0f * t)) + center_x;
    int y2 = (center_y * sin((3.0f * t) + delta)) + center_y;
    display.line(x1, y1, x2, y2, Color(0xff * r, 0xff * g, 0xff * b));
    x1 = x2;
    y1 = y2;
  }
}
}  // namespace esphome::elements
