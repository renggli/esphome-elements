#include "clock_element.h"

#include <sys/time.h>

namespace esphome::elements {

void AnalogClockElement::draw(const Context& context,
                              display::Display& display) {
  const int center_x = display.get_width() / 2;
  const int center_y = display.get_height() / 2;
  const int radius = std::min(center_x, center_y);

  for (int i = 0; i < 12; i++) {
    if (i % 3 == 0 && trihoral_marks_.visible) {
      draw_line_(display, trihoral_marks_, center_x, center_y, radius, 30 * i);
    } else {
      draw_line_(display, hour_marks_, center_x, center_y, radius, 30 * i);
    }
  }

  const auto time = time_->now();
  float seconds = time.second;
  if (second_arm_.smooth) {
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == 0) {
      seconds += tv.tv_usec / 1000000.0;
    }
  }
  float minutes = time.minute;
  if (minute_arm_.smooth) {
    minutes += seconds / 60.0;
  }
  float hours = time.hour % 12;
  if (hour_arm_.smooth) {
    hours += minutes / 60.0;
  }

  int seconds_deg = 360.0 * seconds / 60.0 - 90.0;
  int minutes_deg = 360.0 * minutes / 60.0 - 90.0;
  int hours_deg = 360.0 * hours / 12.0 - 90.0;

  draw_line_(display, hour_marks_, center_x, center_y, radius, hours_deg);
  draw_line_(display, hour_marks_, center_x, center_y, radius, minutes_deg);
  draw_line_(display, hour_marks_, center_x, center_y, radius, seconds_deg);
}

void AnalogClockElement::draw_line_(display::Display& display,
                                    const AnalogClockOptions& options,
                                    int center_x, int center_y, int radius,
                                    int angle_deg) {
  if (options.visible) {
    display.line_at_angle(center_x, center_y, angle_deg, radius * options.start,
                          radius * options.end, options.color);
  }
}

void DigitalClockElement::draw(const Context& context,
                               display::Display& display) {
  display.strftime(display.get_width() / 2, display.get_height() / 2, font_,
                   color_, display::TextAlign::CENTER, format_, time_->now());
}

}  // namespace esphome::elements
