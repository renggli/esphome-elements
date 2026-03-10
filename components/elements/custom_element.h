#pragma once

#include <utility>

#include "element.h"

namespace esphome::elements {

/// An element that can be customized with lambda functions.
class CustomElement : public Element {
 public:
  using Element::Element;

  const char *get_type_name() const override { return "custom"; }

  using DrawFunction = std::function<void(CustomElement &, display::Display &)>;
  using PredicateFunction = std::function<bool(const CustomElement &)>;

  void set_draw(DrawFunction draw) { draw_ = std::move(draw); }
  void set_is_active(PredicateFunction is_active) { is_active_ = std::move(is_active); }

  void draw(display::Display &display) override;

  bool is_active() const override;

 protected:
  DrawFunction draw_;
  PredicateFunction is_active_;

  void test_draw_(display::Display &display);
};

}  // namespace esphome::elements
