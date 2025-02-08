#pragma once

#include "component.h"
#include "esphome/components/display/display.h"

namespace esphome::elements {

/// The different types of elements.
enum class ElementType {
  ANALOG_CLOCK,
  DIGITAL_CLOCK,
  ERROR,
  FLOW,
  HORIZONTAL,
  OVERLAY,
  SCHEDULER,
  SEQUENCE,
  TEXTUAL,
  UNKNOWN,
  VERTICAL,
};

/// Abstract superclass of all elements.
class Element {
 public:
  explicit Element(ElementType type) : type_(type) {}

  ElementType get_type() { return type_; }

  void set_parent(Element* parent) { parent_ = parent; };
  Element* get_parent() { return parent_; }

  virtual void draw(const Context& context, display::Display& display) = 0;

 protected:
  ElementType type_ = ElementType::UNKNOWN;
  Element* parent_ = nullptr;
};

}  // namespace esphome::elements
