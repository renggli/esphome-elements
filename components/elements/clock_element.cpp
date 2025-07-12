#include "clock_element.h"

#include <sys/time.h>

namespace esphome::elements {

void ClockElement::draw(display::Display &display) {
  // Center the clock in the middle of the display.
  const int center_x = display.get_width() / 2;
  const int center_y = display.get_height() / 2;
  const int radius = std::min(center_x, center_y);

  // Draw the markers.
  for (int i = 0; i < 60; i++) {
    if (quarter_markers_.visible && i % 15 == 0) {
      display.line_at_angle(
          center_x, center_y, 6 * i, radius * quarter_markers_.start,
          radius * quarter_markers_.end, quarter_markers_.color);
    } else if (hour_markers_.visible && i % 5 == 0) {
      display.line_at_angle(center_x, center_y, 6 * i,
                            radius * hour_markers_.start,
                            radius * hour_markers_.end, hour_markers_.color);
    } else if (minute_markers_.visible) {
      display.line_at_angle(
          center_x, center_y, 6 * i, radius * minute_markers_.start,
          radius * minute_markers_.end, minute_markers_.color);
    }
  }

  // Compute fractional times.
  const auto time = time_->now();
  float seconds = time.second;
  if (second_hand_.smooth) {
    struct timeval tv;
    if (gettimeofday(&tv, nullptr) == 0) {
      seconds += tv.tv_usec / 1000000.0;
    }
  }
  float minutes = time.minute;
  if (minute_hand_.smooth) {
    minutes += seconds / 60.0;
  }
  float hours = time.hour % 12;
  if (hour_hand_.smooth) {
    hours += minutes / 60.0;
  }

  // Draw the hands.
  if (hour_hand_.visible) {
    display.line_at_angle(center_x, center_y, 360.0 * hours / 12.0 - 90.0,
                          radius * hour_hand_.start, radius * hour_hand_.end,
                          hour_hand_.color);
  }
  if (minute_hand_.visible) {
    display.line_at_angle(center_x, center_y, 360.0 * minutes / 60.0 - 90.0,
                          radius * minute_hand_.start,
                          radius * minute_hand_.end, minute_hand_.color);
  }
  if (second_hand_.visible) {
    display.line_at_angle(center_x, center_y, 360.0 * seconds / 60.0 - 90.0,
                          radius * second_hand_.start,
                          radius * second_hand_.end, second_hand_.color);
  }
}

} // namespace esphome::elements
