#pragma once

#include <limits>

#include "component.h"
#include "esphome/components/display/display.h"
#include "esphome/core/helpers.h"

namespace esphome::elements {

/// A sub-display for an existing display.
class SubDisplay : public display::Display {
 public:
  SubDisplay(display::Display& display, int x, int y, int w, int h)
      : display_(display),
        x_(clamp(x, 0, display.get_width())),
        y_(clamp(y, 0, display.get_height())),
        w_(clamp(w, 0, display.get_width() - x)),
        h_(clamp(h, 0, display.get_height() - y)) {}

  void HOT draw_pixel_at(int x, int y, Color color) override {
    if (0 <= x && x < w_ && 0 <= y && y < h_) {
      display_.draw_pixel_at(x_ + x, y_ + y, color);
    }
  }

  display::DisplayType get_display_type() override {
    return display_.get_display_type();
  }

 protected:
  display::Display& display_;
  int x_, y_, w_, h_;

  void update() override {}

  int get_width_internal() override { return w_; }
  int get_height_internal() override { return h_; }
};

/// A class that expires and resets after the configured duration passes.
class Timer {
 public:
  Timer(uint32_t duration_ms = NEVER)
      : duration_ms_(duration_ms), next_ms_(0) {}

  /// Sets the duration between timer expiry. Unless the timer is reset, this
  /// will cause it to immediately trigger on the next check.
  void set_duration(uint32_t duration_ms) {
    duration_ms_ = duration_ms;
    next_ms_ = 0;
  }

  /// Resets the timer to restart.
  void reset(uint32_t current_ms) { next_ms_ = current_ms + duration_ms_; }

  /// Returns `true` if the timer has exceed, and reset the internal state so
  /// that it only triggers again after `duration_ms` has passed.
  bool check(uint32_t current_ms) {
    if (duration_ms_ != NEVER && next_ms_ <= current_ms) {
      reset(current_ms);
      return true;
    }
    return false;
  }

  /// The duration that will never trigger.
  constexpr static uint32_t NEVER = std::numeric_limits<uint32_t>::max();

 protected:
  uint32_t duration_ms_ = NEVER;
  uint32_t next_ms_;
};

}  // namespace esphome::elements
