#pragma once

#include "component.h"
#include "esphome/components/display/display.h"

namespace esphome::elements {

/// The different types of elements.
enum class ElementType {
  UNKNOWN = 0,
  // Alphabetically sorted.
  CLOCK,
  CUSTOM,
  DELAY,
  DYNAMIC_TEXT,
  HORIZONTAL,
  IMAGE,
  OVERLAY,
  PRIORITY,
  SEQUENCE,
  STATIC_TEXT,
  TIME_TEXT,
  TIMEOUT,
  VERTICAL,
};

/// Abstract superclass of all elements.
class Element {
public:
  Element(ElementType type, ElementComponent *component, Element *parent)
      : type_(type), component_(*component), parent_(parent) {}

  /// Returns the type of this element.
  ElementType get_type() { return type_; }
  const char *get_type_string();

  /// Returns the parent element of this element, if any.
  Element *get_parent() { return parent_; }

  /// Returns the component of this element.
  ElementComponent &get_component() { return component_; }

  /// Abstract method that dumps this element configuration.
  virtual void dump_config(int level);

  /// Helper to indent and log the dump configuration from `dump_config`.
  void dump_config(int level, const char *format, ...)
      __attribute__((format(printf, 3, 4)));

  /// Abstract method that draws this element on the given display.
  virtual void draw(display::Display &display) = 0;

  /// Event passed down the element tree when an element is about to be
  /// shown, that is before the `draw` method is being called repeatedly.
  virtual void on_show();

  /// Event passed down the element tree when an element is about to be
  /// hidden, that is after the `draw` method is no longer being called
  /// repeatedly.
  virtual void on_hide();

  /// Event passed up the element tree when an elements wants to pass on
  /// control to a sibling. By default the signal is passed upwards, unless
  /// handled.
  virtual void on_next();

  /// Property of an element indicating if it wants to be shown. Parents
  /// might decide to skip the display of the element if it returns false.
  virtual bool is_active() { return true; }

protected:
  ElementType type_;
  ElementComponent &component_;
  Element *parent_;
};

} // namespace esphome::elements
