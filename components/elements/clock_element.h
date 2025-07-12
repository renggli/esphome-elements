#pragma once

#include "element.h"
#include "esphome/components/time/real_time_clock.h"

namespace esphome::elements {

/// Options for the various graphical elements of the analog clock.
struct AnalogClockOptions {
  float start;
  float end;
  Color color;
  bool visible;
  bool smooth;
};

/// Analog clock element.
class ClockElement : public Element {
public:
  ClockElement(ElementComponent *component, Element *parent)
      : Element(ElementType::CLOCK, component, parent) {}

  void set_time(time::RealTimeClock *time) { time_ = time; }

  void set_minute_markers(const AnalogClockOptions minute_markers) {
    minute_markers_ = minute_markers;
  }
  void set_hour_markers(const AnalogClockOptions hour_markers) {
    hour_markers_ = hour_markers;
  }
  void set_quarter_markers(const AnalogClockOptions quarter_markers) {
    quarter_markers_ = quarter_markers;
  }
  void set_second_hand(const AnalogClockOptions second_hand) {
    second_hand_ = second_hand;
  }
  void set_minute_hand(const AnalogClockOptions minute_hand) {
    minute_hand_ = minute_hand;
  }
  void set_hour_hand(const AnalogClockOptions hour_hand) {
    hour_hand_ = hour_hand;
  }

  void draw(display::Display &display) override;

protected:
  time::RealTimeClock *time_;
  AnalogClockOptions minute_markers_;
  AnalogClockOptions hour_markers_;
  AnalogClockOptions quarter_markers_;
  AnalogClockOptions second_hand_;
  AnalogClockOptions minute_hand_;
  AnalogClockOptions hour_hand_;
};

} // namespace esphome::elements
