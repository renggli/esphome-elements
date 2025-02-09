#pragma once

#include "component.h"
#include "esphome/components/display/display.h"

namespace esphome::elements {

/// The different types of elements.
enum class ElementType {
  ANALOG_CLOCK,
  DIGITAL_CLOCK,
  HORIZONTAL,
  OVERLAY,
  SCHEDULER,
  SEQUENCE,
  TEXT,
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

  /// Abstract method that draws this element on the given display.
  virtual void draw(const Context& context, display::Display& display) = 0;

  /// Event passed down the element tree when an element is about to be shown,
  /// that is before the `draw` method is being called repeatedly.
  virtual void on_show(const Context& context);

  /// Event passed down the element tree when an element is about to be hidden,
  /// that is after the `draw` method is no longer being called repeatedly.
  virtual void on_hide(const Context& context);

  /// Event passed up the element tree when an elements wants to pass on control
  /// to a sibling. By default the signal is passed upwards, unless handled.
  virtual void on_next(const Context& context);

 protected:
  ElementType type_ = ElementType::UNKNOWN;
  Element* parent_ = nullptr;
};

}  // namespace esphome::elements
