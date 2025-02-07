#pragma once

#include "component.h"
#include "esphome/components/display/display.h"

namespace esphome::elements {

enum ElementType {
  ANALOG_CLOCK_ELEMENT,
  DIGITAL_CLOCK_ELEMENT,
  ERROR_ELEMENT,
  SCHEDULER_ELEMENT,
};

class Element {
 public:
  explicit Element(ElementType type) : type_(type) {}

  virtual void draw(const Context& context, display::Display& display) {};

 protected:
  ElementType type_;
};

}  // namespace esphome::elements
