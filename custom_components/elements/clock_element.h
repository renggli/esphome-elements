#pragma once

#include "element.h"
#include "esphome/components/display/display.h"
#include "esphome/components/time/real_time_clock.h"

namespace esphome::elements {

static const char* TAG = "elements.clock";

class ClockElement : public Element {
 public:
  explicit ClockElement(ElementType type) : Element(type) {}

  void set_time(time::RealTimeClock* time) { time_ = time; }

 protected:
  time::RealTimeClock* time_;
};

struct AnalogClockOptions {
  float start;
  float end;
  Color color;
  bool visible;
  bool smooth;
};

class AnalogClockElement : public ClockElement {
 public:
  explicit AnalogClockElement() : ClockElement(ANALOG_CLOCK_ELEMENT) {}

  void set_minute_markers(AnalogClockOptions minute_markers) {
    minute_markers_ = minute_markers;
  }
  void set_hour_markers(AnalogClockOptions hour_markers) {
    hour_markers_ = hour_markers;
  }
  void set_quarter_markers(AnalogClockOptions quarter_markers) {
    quarter_markers_ = quarter_markers;
  }
  void set_second_hand(AnalogClockOptions second_hand) {
    second_hand_ = second_hand;
  }
  void set_minute_hand(AnalogClockOptions minute_hand) {
    minute_hand_ = minute_hand;
  }
  void set_hour_hand(AnalogClockOptions hour_hand) {
    hour_hand_ = hour_hand;
  }

  void draw(const Context& context, display::Display& display) override;

 protected:
  AnalogClockOptions minute_markers_;
  AnalogClockOptions hour_markers_;
  AnalogClockOptions quarter_markers_;
  AnalogClockOptions second_hand_;
  AnalogClockOptions minute_hand_;
  AnalogClockOptions hour_hand_;
};

class DigitalClockElement : public ClockElement {
 public:
  explicit DigitalClockElement() : ClockElement(DIGITAL_CLOCK_ELEMENT) {}

  void set_font(display::BaseFont* font) { font_ = font; }
  void set_color(Color color) { color_ = color; }
  void set_format(const char* format) { format_ = format; }

  void draw(const Context& context, display::Display& display) override;

 protected:
  display::BaseFont* font_ = nullptr;
  Color color_ = Color::WHITE;
  const char* format_ = "%H:%M:%S";
};

}  // namespace esphome::elements
