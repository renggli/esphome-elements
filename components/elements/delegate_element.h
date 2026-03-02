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

}  // namespace esphome::elements
