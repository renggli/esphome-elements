#include "clock_element.h"

#include <sys/time.h>

namespace esphome::elements {

void AnalogClockElement::draw(const Context& context,
                              display::Display& display) {
  const int center_x = display.get_width() / 2;
  const int center_y = display.get_height() / 2;
  const int radius = std::min(center_x, center_y);

  // Draw the marks.
  for (int i = 0; i < 12; i++) {
    if (i % 3 == 0 && trihoral_marks_visible_) {
      display.line_at_angle(
          center_x, center_y, 30 * i, radius * trihoral_marks_start_,
          radius * trihoral_marks_end_, trihoral_marks_color_);
    } else if (hour_marks_visible_) {
      display.line_at_angle(center_x, center_y, 30 * i,
                            radius * hour_marks_start_,
                            radius * hour_marks_end_, hour_marks_color_);
    }
  }

  // Compute fractional times.
  const auto time = time_->now();
  float seconds = time.second;
  if (second_arm_smooth_) {
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == 0) {
      seconds += tv.tv_usec / 1000000.0;
    }
  }
  float minutes = time.minute;
  if (minute_arm_smooth_) minutes += seconds / 60.0;
  float hours = time.hour % 12;
  if (hour_arm_smooth_) hours += minutes / 60.0;

  // Draw the arms.
  if (hour_arm_visible_) {
    display.line_at_angle(center_x, center_y, 360.0 * hours / 12.0 - 90.0,
                          radius * hour_arm_start_, radius * hour_arm_end_,
                          hour_arm_color_);
  }
  if (minute_arm_visible_) {
    display.line_at_angle(center_x, center_y, 360.0 * minutes / 60.0 - 90.0,
                          radius * minute_arm_start_, radius * minute_arm_end_,
                          minute_arm_color_);
  }
  if (second_arm_visible_) {
    display.line_at_angle(center_x, center_y, 360.0 * seconds / 60.0 - 90.0,
                          radius * second_arm_start_, radius * second_arm_end_,
                          second_arm_color_);
  }
}

void DigitalClockElement::draw(const Context& context,
                               display::Display& display) {
  display.strftime(display.get_width() / 2, display.get_height() / 2, font_,
                   color_, display::TextAlign::CENTER, format_, time_->now());
}

}  // namespace esphome::elements
