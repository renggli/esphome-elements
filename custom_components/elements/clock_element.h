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
  float start;   // fractional start of the line
  float end;     // fractional end of the line
  Color color;   // color of the line
  bool visible;  // whether the line is drawn
  bool smooth;   // whether the line moves continous
};

class AnalogClockElement : public ClockElement {
 public:
  explicit AnalogClockElement() : ClockElement(ANALOG_CLOCK_ELEMENT) {}

  void set_hour_marks(AnalogClockOptions options) { hour_marks_ = options; }
  void set_trihoral_marks(AnalogClockOptions options) {
    trihoral_marks_ = options;
  }
  void set_second_arm(AnalogClockOptions options) { second_arm_ = options; }
  void set_minute_arm(AnalogClockOptions options) { minute_arm_ = options; }
  void set_hour_arm(AnalogClockOptions options) { hour_arm_ = options; }

  void draw(const Context& context, display::Display& display) override;

 protected:
  void draw_line_(display::Display& display, const AnalogClockOptions& options,
                  int center_x, int center_y, int radius, int angle_deg);

  AnalogClockOptions hour_marks_ = {
      .start = 0.8,
      .end = 1.0,
      .color = Color(0, 0, 255),
      .visible = true,
  };
  AnalogClockOptions trihoral_marks_ = {
      .start = 0.7,
      .end = 1.0,
      .color = Color(0, 0, 255),
      .visible = true,
  };
  AnalogClockOptions second_arm_ = {
      .start = 0.0,
      .end = 0.9,
      .color = Color(255, 0, 0),
      .visible = true,
      .smooth = true,
  };
  AnalogClockOptions minute_arm_ = {
      .start = 0.0,
      .end = 0.9,
      .color = Color::WHITE,
      .visible = true,
      .smooth = true,
  };
  AnalogClockOptions hour_arm_ = {
      .start = 0.0,
      .end = 0.6,
      .color = Color::WHITE,
      .visible = true,
      .smooth = true,
  };
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
