#pragma once

#include <limits>
#include <cstdint>

namespace esphome::elements {

/// A class that expires and resets after the configured duration passes.
class Timer {
 public:
  Timer(uint32_t duration_ms = NEVER)
      : duration_ms_(duration_ms) {}

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
    if (is_enabled() && next_ms_ <= current_ms) {
      reset(current_ms);
      return true;
    }
    return false;
  }

  /// If this timer is triggering.
  bool is_enabled() { return duration_ms_ != NEVER; }

  /// The duration that will never trigger.
  constexpr static uint32_t NEVER = std::numeric_limits<uint32_t>::max();

 protected:
  uint32_t duration_ms_ = NEVER;
  uint32_t next_ms_ = 0;
};

}  // namespace esphome::elements
