#pragma once

#include "element.h"

namespace esphome::elements {

/// Element that delegates to another element.
class DelegateElement : public Element {
 public:
  using Element::Element;

  void set_element(Element *element) { element_ = element; };

  void dump_config(int level) override;
  void draw(display::Display &display) override;
  bool is_active() override;

  void on_show() override;
  void on_hide() override;

 protected:
  Element *element_;
};

/// An element that generates an complete event after a specified time the element is visible.
class TimeoutElement : public DelegateElement {
 public:
  using DelegateElement::DelegateElement;

  const char *get_type_name() const override { return "timeout"; }

  void set_duration(uint32_t duration_ms) { duration_ms_ = duration_ms; }

  void draw(display::Display &display) override;

  void on_show() override;
  void on_complete();

  void add_on_complete_callback(std::function<void(TimeoutElement *)> &&callback) {
    on_complete_callbacks_.add(std::move(callback));
  }

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
