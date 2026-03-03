#pragma once

#include "esphome/components/display/display.h"
#include "esphome/components/time/real_time_clock.h"

#include "element.h"

namespace esphome::elements {

/// Options for the various graphical elements of the analog clock.
struct ClockOptions {
  float start = 0.0f;
  float end = 1.0f;
  Color color = Color::WHITE;
  bool visible = false;
  bool smooth = false;
};

/// Analog clock element.
class ClockElement : public Element {
 public:
  using Element::Element;

  const char *get_type_name() const override { return "clock"; }

  void set_time(time::RealTimeClock *time) { time_ = time; }

  void set_minute_markers(const ClockOptions minute_markers) { minute_markers_ = minute_markers; }
  void set_hour_markers(const ClockOptions hour_markers) { hour_markers_ = hour_markers; }
  void set_quarter_markers(const ClockOptions quarter_markers) { quarter_markers_ = quarter_markers; }
  void set_second_hand(const ClockOptions second_hand) { second_hand_ = second_hand; }
  void set_minute_hand(const ClockOptions minute_hand) { minute_hand_ = minute_hand; }
  void set_hour_hand(const ClockOptions hour_hand) { hour_hand_ = hour_hand; }

  void draw(display::Display &display) override;

 protected:
  time::RealTimeClock *time_;
  ClockOptions minute_markers_;
  ClockOptions hour_markers_;
  ClockOptions quarter_markers_;
  ClockOptions second_hand_;
  ClockOptions minute_hand_;
  ClockOptions hour_hand_;
};

}  // namespace esphome::elements
