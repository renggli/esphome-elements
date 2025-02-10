#include "element.h"

namespace esphome::elements {

void Element::on_show() {}

void Element::on_hide() {}

void Element::on_next() {
  if (parent_ != nullptr) parent_->on_next();
}

}  // namespace esphome::elements
