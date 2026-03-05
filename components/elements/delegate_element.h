#pragma once

#include "element.h"

namespace esphome::elements {

/// Element that delegates drawing to exactly one child element.
class DelegateElement : public Element {
 public:
  using Element::Element;

  void set_element(Element *element) { element_ = element; };

  void dump_config(int level) override;
  void draw(display::Display &display) override;

  /// Property indicating whether this element is active (deferred to child).
  bool is_active() const override;

  /// The child is visible whenever this element is visible.
  void visit_children(const std::function<void(Element *, bool)> &fn) override;

  void update_state() override;

 protected:
  Element *element_ = nullptr;
};

/// Element that fires on_complete after a configurable duration of being visible.
class TimeoutElement : public DelegateElement {
 public:
  using DelegateElement::DelegateElement;

  const char *get_type_name() const override { return "timeout"; }

  void set_duration(uint32_t duration_ms) { duration_ms_ = duration_ms; }

  void draw(display::Display &display) override;

  /// Called when the element becomes visible; starts the countdown.
  void on_show() override;

  /// Called when the element is hidden; cancels the countdown.
  void on_hide() override;

  // ---------------------------------------------------------------------------
  // on_complete event
  // ---------------------------------------------------------------------------

  /// Register a callback for when the timeout elapses.
  /// Fires once per duration_ms while the element remains visible.
  void add_on_complete_callback(std::function<void(TimeoutElement *)> &&callback) {
    on_complete_callbacks_.add(std::move(callback));
  }

  /// Called when the timeout elapses.
  void on_complete();

 protected:
  uint32_t duration_ms_ = 0;
  uint32_t next_ms_ = 0;

  LazyCallbackManager<void(TimeoutElement *)> on_complete_callbacks_;
};

class TimeoutElementCompleteTrigger : public Trigger<TimeoutElement &> {
 public:
  explicit TimeoutElementCompleteTrigger(TimeoutElement *parent) {
    parent->add_on_complete_callback([this](TimeoutElement *element) { trigger(*element); });
  }
};

}  // namespace esphome::elements
