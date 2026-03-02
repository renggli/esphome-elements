#pragma once

#include "esphome/components/display/display.h"
#include "esphome/core/automation.h"
#include "esphome/core/helpers.h"

#include "component.h"

namespace esphome::elements {

/// Abstract superclass of all elements.
class Element {
 public:
  Element(ElementComponent *component, Element *parent) : component_(component), parent_(parent) {}

  /// Returns the type name of this element as a string.
  virtual const char *get_type_name() const = 0;

  /// Returns the parent element of this element, if any.
  Element *get_parent() const { return this->parent_; }

  /// Returns the component of this element.
  ElementComponent *get_component() const { return this->component_; }

  /// Abstract method that dumps this element's configuration.
  virtual void dump_config(int level);

  /// Helper to indent and log the dump configuration from `dump_config`.
  void dump_config(int level, const char *format, ...) __attribute__((format(printf, 3, 4)));

  /// Abstract method that draws this element on the given display.
  virtual void draw(display::Display &display) = 0;

  /// Property of an element indicating if it wants to be shown. Parents
  /// might decide to skip the display of the element if it returns false.
  virtual bool is_active() { return true; }

  /// Register a callback that is called when the element is about to be shown.
  void add_on_show_callback(std::function<void(Element *)> &&callback) {
    this->on_show_callbacks_.add(std::move(callback));
  }

  /// Called by when the element is about to be shown.
  virtual void on_show();

  /// Register a callback that is called when the element is about to be hidden.
  void add_on_hide_callback(std::function<void(Element *)> &&callback) {
    this->on_hide_callbacks_.add(std::move(callback));
  }

  /// Called by when the element is about to be hidden.
  virtual void on_hide();

 protected:
  ElementComponent *component_;
  Element *parent_;

  LazyCallbackManager<void(Element *)> on_show_callbacks_;
  LazyCallbackManager<void(Element *)> on_hide_callbacks_;
};

class ElementShowTrigger : public Trigger<Element &> {
 public:
  explicit ElementShowTrigger(Element *parent) {
    parent->add_on_show_callback([this](Element *element) { this->trigger(*element); });
  }
};

class ElementHideTrigger : public Trigger<Element &> {
 public:
  explicit ElementHideTrigger(Element *parent) {
    parent->add_on_hide_callback([this](Element *element) { this->trigger(*element); });
  }
};

}  // namespace esphome::elements
