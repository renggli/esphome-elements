#pragma once

#include "component.h"
#include "esphome/components/display/display.h"

namespace esphome::elements {

enum ElementType {
  ANALOG_CLOCK_ELEMENT,
  DIGITAL_CLOCK_ELEMENT,
  ERROR_ELEMENT,
  FLOW_ELEMENT,
  OVERLAY_ELEMENT,
  SCHEDULER_ELEMENT,
  SEQUENCE_ELEMENT,
  HORIZONTAL_ELEMENT,
  VERTICAL_ELEMENT,
};

class Element {
 public:
  explicit Element(ElementType type) : type_(type) {}

  virtual void draw(const Context& context, display::Display& display) = 0;

 protected:
  ElementType type_;
};

}  // namespace esphome::elements
