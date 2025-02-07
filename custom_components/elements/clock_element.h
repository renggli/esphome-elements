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

class AnalogClockElement : public ClockElement {
 public:
  explicit AnalogClockElement() : ClockElement(ANALOG_CLOCK_ELEMENT) {}

  void set_hour_marks_start(float hour_marks_start) {
    hour_marks_start_ = hour_marks_start;
  }

  void set_hour_marks_end(float hour_marks_end) {
    hour_marks_end_ = hour_marks_end;
  }

  void set_hour_marks_color(Color hour_marks_color) {
    hour_marks_color_ = hour_marks_color;
  }

  void set_hour_marks_visible(bool hour_marks_visible) {
    hour_marks_visible_ = hour_marks_visible;
  }

  void set_trihoral_marks_start(float trihoral_marks_start) {
    trihoral_marks_start_ = trihoral_marks_start;
  }

  void set_trihoral_marks_end(float trihoral_marks_end) {
    trihoral_marks_end_ = trihoral_marks_end;
  }

  void set_trihoral_marks_color(Color trihoral_marks_color) {
    trihoral_marks_color_ = trihoral_marks_color;
  }

  void set_trihoral_marks_visible(bool trihoral_marks_visible) {
    trihoral_marks_visible_ = trihoral_marks_visible;
  }

  void set_second_arm_start(float second_arm_start) {
    second_arm_start_ = second_arm_start;
  }

  void set_second_arm_end(float second_arm_end) {
    second_arm_end_ = second_arm_end;
  }

  void set_second_arm_color(Color second_arm_color) {
    second_arm_color_ = second_arm_color;
  }

  void set_second_arm_visible(bool second_arm_visible) {
    second_arm_visible_ = second_arm_visible;
  }

  void set_second_arm_smooth(bool second_arm_smooth) {
    second_arm_smooth_ = second_arm_smooth;
  }

  void set_minute_arm_start(float minute_arm_start) {
    minute_arm_start_ = minute_arm_start;
  }

  void set_minute_arm_end(float minute_arm_end) {
    minute_arm_end_ = minute_arm_end;
  }

  void set_minute_arm_color(Color minute_arm_color) {
    minute_arm_color_ = minute_arm_color;
  }

  void set_minute_arm_visible(bool minute_arm_visible) {
    minute_arm_visible_ = minute_arm_visible;
  }

  void set_minute_arm_smooth(bool minute_arm_smooth) {
    minute_arm_smooth_ = minute_arm_smooth;
  }

  void set_hour_arm_start(float hour_arm_start) {
    hour_arm_start_ = hour_arm_start;
  }

  void set_hour_arm_end(float hour_arm_end) { hour_arm_end_ = hour_arm_end; }

  void set_hour_arm_color(Color hour_arm_color) {
    hour_arm_color_ = hour_arm_color;
  }

  void set_hour_arm_visible(bool hour_arm_visible) {
    hour_arm_visible_ = hour_arm_visible;
  }

  void set_hour_arm_smooth(bool hour_arm_smooth) {
    hour_arm_smooth_ = hour_arm_smooth;
  }

  void draw(const Context& context, display::Display& display) override;

 protected:
  float hour_marks_start_ = 0.8;
  float hour_marks_end_ = 1.0;
  Color hour_marks_color_ = Color(0, 0, 255);
  bool hour_marks_visible_ = true;

  float trihoral_marks_start_ = 0.7;
  float trihoral_marks_end_ = 1.0;
  Color trihoral_marks_color_ = Color(0, 0, 255);
  bool trihoral_marks_visible_ = true;

  float second_arm_start_ = 0.0;
  float second_arm_end_ = 0.9;
  Color second_arm_color_ = Color(255, 0, 0);
  bool second_arm_visible_ = true;
  bool second_arm_smooth_ = true;

  float minute_arm_start_ = 0.0;
  float minute_arm_end_ = 0.9;
  Color minute_arm_color_ = Color::WHITE;
  bool minute_arm_visible_ = true;
  bool minute_arm_smooth_ = true;

  float hour_arm_start_ = 0.0;
  float hour_arm_end_ = 0.6;
  Color hour_arm_color_ = Color::WHITE;
  bool hour_arm_visible_ = true;
  bool hour_arm_smooth_ = true;
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
