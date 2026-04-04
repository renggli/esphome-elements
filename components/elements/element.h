#pragma once

#include "component.h"
#include "esphome/components/display/display.h"
#include "esphome/core/automation.h"
#include "esphome/core/helpers.h"

namespace esphome::elements {

/// Abstract superclass of all elements.
class Element {
 public:
  Element(ElementComponent* component, Element* parent)
      : component_(component), parent_(parent) {}

  /// Returns the type name of this element as a string.
  virtual const char* get_type_name() const = 0;

  /// Returns the parent element of this element, if any.
  Element* get_parent() const { return parent_; }

  /// Returns the component of this element.
  ElementComponent* get_component() const { return component_; }

  /// Abstract method that dumps this element's configuration.
  virtual void dump_config(int level);

  /// Helper to indent and log the dump configuration from `dump_config`.
  static void dump_config(int level, const char* format, ...)
      __attribute__((format(printf, 2, 3)));

  /// Abstract method that draws this element on the given display.
  virtual void draw(display::Display& display) = 0;

  /// Property of an element indicating if it wants to be shown. Parents
  /// might decide to skip the display of the element if it returns false.
  virtual bool is_active() { return true; }

  /// Property indicating if this element is currently visible (drawn by its
  /// parent).
  bool is_visible() const { return visible_; }

  /// Called before drawing each frame to let elements update their internal
  /// state (e.g. which child to display). Must not fire any events.
  virtual void update_state() {}

  /// Recursively updates visible_ and fires on_show/on_hide as needed.
  /// Called by ElementComponent before draw, after update_state.
  virtual void update_visibility(bool now_visible);

  /// Register a callback that is called when the element is about to be shown.
  void add_on_show_callback(std::function<void(Element*)>&& callback) {
    on_show_callbacks_.add(std::move(callback));
  }

  /// Called when the element transitions from hidden to visible.
  virtual void on_show();

  /// Register a callback that is called when the element is about to be hidden.
  void add_on_hide_callback(std::function<void(Element*)>&& callback) {
    on_hide_callbacks_.add(std::move(callback));
  }

  /// Called when the element transitions from visible to hidden.
  virtual void on_hide();

 protected:
  ElementComponent* component_;
  Element* parent_;

  /// True while this element is drawn by the component tree.
  bool visible_ = false;

  LazyCallbackManager<void(Element*)> on_show_callbacks_;
  LazyCallbackManager<void(Element*)> on_hide_callbacks_;
};

class ElementShowTrigger : public Trigger<Element&> {
 public:
  explicit ElementShowTrigger(Element* parent) {
    parent->add_on_show_callback(
        [this](Element* element) { trigger(*element); });
  }
};

class ElementHideTrigger : public Trigger<Element&> {
 public:
  explicit ElementHideTrigger(Element* parent) {
    parent->add_on_hide_callback(
        [this](Element* element) { trigger(*element); });
  }
};

}  // namespace esphome::elements
