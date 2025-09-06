#pragma once

#include "element.h"

namespace esphome::elements {

/// Element that delegates to another element.
class DelegateElement : public Element {
 public:
  DelegateElement(ElementType type, ElementComponent *component, Element *parent) : Element(type, component, parent) {}

  void set_element(Element *element) { element_ = element; };

  void dump_config(int level) override;
  void draw(Canvas &canvas) override;

  void on_show() override;
  void on_hide() override;
  bool is_active() override;

 protected:
  Element *element_;
};

// An element that generates a 'next' event after a specified timeout.
class TimeoutElement : public DelegateElement {
 public:
  TimeoutElement(ElementComponent *component, Element *parent)
      : DelegateElement(ElementType::TIMEOUT, component, parent) {}

  void set_duration(uint32_t duration_ms) { duration_ms_ = duration_ms; }

  void draw(Canvas &canvas) override;

  void on_show() override;

 protected:
  uint32_t duration_ms_ = 0;
  uint32_t next_ms_ = 0;
};

// An element that generates a 'next' event after observing a specified count
// of 'next' events.
class DelayElement : public DelegateElement {
 public:
  DelayElement(ElementComponent *component, Element *parent) : DelegateElement(ElementType::DELAY, component, parent) {}

  void set_count(uint32_t count) { count_ = count; }

  void on_show() override;
  void on_next() override;

 protected:
  uint32_t count_ = 0;
  uint32_t counter_ = 0;
};

}  // namespace esphome::elements
