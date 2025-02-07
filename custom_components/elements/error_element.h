#pragma once

#include "element.h"
#include "esphome/components/display/display.h"

namespace esphome::elements {

class ErrorElement : public Element {
 public:
  explicit ErrorElement() : Element(ERROR_ELEMENT) {}

  virtual void draw(const Context& context, display::Display& display) override;
};

}  // namespace esphome::elements
