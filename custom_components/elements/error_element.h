#pragma once

#include "element.h"

namespace esphome::elements {

/// Element to be shown in case of an error.
class ErrorElement : public Element {
 public:
  explicit ErrorElement() : Element(ElementType::ERROR) {}

  virtual void draw(const Context& context, display::Display& display) override;
};

}  // namespace esphome::elements
