#pragma once

#include "element.h"

namespace esphome::elements {

/// An element that can be customized with a bunch of lambda functions.
class CustomElement : public Element {
 public:
  CustomElement(ElementComponent* component, Element* parent)
      : Element(ElementType::CUSTOM, component, parent) {}

  typedef std::function<void(CustomElement&, display::Display&)> DrawFunction;
  typedef std::function<void(CustomElement&)> EventFunction;
  typedef std::function<bool(CustomElement&)> PredicateFunction;

  void set_draw(DrawFunction draw) { draw_ = draw; }
  void set_on_show(EventFunction on_show) { on_show_ = on_show; }
  void set_on_hide(EventFunction on_hide) { on_hide_ = on_hide; }
  void set_on_next(EventFunction on_next) { on_next_ = on_next; }
  void set_is_active(PredicateFunction is_active) { is_active_ = is_active; }

  void draw(display::Display& display) override;
  void on_show() override;
  void on_hide() override;
  void on_next() override;
  bool is_active() override;

 protected:
  DrawFunction draw_;
  EventFunction on_show_;
  EventFunction on_hide_;
  EventFunction on_next_;
  PredicateFunction is_active_;

  void test_draw(display::Display& display);
};

}  // namespace esphome::elements
