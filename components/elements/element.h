#pragma once

#include "component.h"
#include "esphome/components/display/display.h"

namespace esphome::elements {

/// Abstract superclass of all elements.
class Element {
 public:
  Element(ElementComponent *component, Element *parent) : component_(*component), parent_(parent) {}

  /// Returns the type name of this element as a string.
  virtual const char *get_type_name() const = 0;

  /// Returns the parent element of this element, if any.
  Element *get_parent() const { return parent_; }

  /// Returns the component of this element.
  ElementComponent &get_component() const { return component_; }

  /// Abstract method that dumps this element's configuration.
  virtual void dump_config(int level);

  /// Helper to indent and log the dump configuration from `dump_config`.
  void dump_config(int level, const char *format, ...) __attribute__((format(printf, 3, 4)));

  /// Abstract method that draws this element on the given display.
  virtual void draw(display::Display &display) = 0;

  /// Event passed up the element tree when an element is about to be
  /// shown, i.e. before the `draw` method is being called repeatedly.
  virtual void on_show();

  /// Event passed up the element tree when an element is about to be
  /// hidden, i.e. after the `draw` method is no longer being called
  /// repeatedly.
  virtual void on_hide();

  /// Event passed up the element tree when an element wants to pass on
  /// control to a next sibling. By default, the signal is passed upwards,
  /// unless handled.
  virtual void on_next();

  /// Event passed up the element tree when an element wants to pass on
  /// control to a previous sibling. By default, the signal is passed upwards,
  /// unless handled.
  virtual void on_prev();

  /// Property of an element indicating if it wants to be shown. Parents
  /// might decide to skip the display of the element if it returns false.
  virtual bool is_active() { return true; }

 protected:
  ElementComponent &component_;
  Element *parent_;
};

}  // namespace esphome::elements
