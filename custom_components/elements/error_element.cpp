#include "error_element.h"

namespace esphome::elements {

void ErrorElement::draw(const Context& context, display::Display& display) {
  const int cx = display.get_width() / 2;
  const int cy = display.get_height() / 2;
  const int rc = std::min(cx, cy);
  const int rx = std::max(rc - 2, 0);

  display.filled_circle(cx, cy, rc, Color(255, 0, 0));
  display.line_at_angle(cx, cy, 45, -rx, rx, Color(255, 255, 255));
  display.line_at_angle(cx, cy, -45, -rx, rx, Color(255, 255, 255));
}

}  // namespace esphome::elements
