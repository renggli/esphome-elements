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

  void on_show() override;
  void on_hide() override;
  bool is_active() override;

 protected:
  Element *element_;
};

/// An element that generates a 'next' event after a specified timeout.
class TimeoutElement : public DelegateElement {
 public:
  using DelegateElement::DelegateElement;

  const char *get_type_name() const override { return "timeout"; }

  void set_duration(uint32_t duration_ms) { duration_ms_ = duration_ms; }

  void draw(display::Display &display) override;

  void on_show() override;

 protected:
  uint32_t duration_ms_ = 0;
  uint32_t next_ms_ = 0;
};

/// An element that generates a 'next' event after observing a specified count
/// of 'next' events.
class DelayElement : public DelegateElement {
 public:
  using DelegateElement::DelegateElement;

  const char *get_type_name() const override { return "delay"; }

  void set_count(uint32_t count) { count_ = count; }

  void on_show() override;
  void on_next() override;

 protected:
  uint32_t count_ = 0;
  uint32_t counter_ = 0;
};

}  // namespace esphome::elements
