#include "element.h"

namespace esphome::elements {

void Element::on_show(const Context& context) {}

void Element::on_hide(const Context& context) {}

void Element::on_next(const Context& context) {
  if (parent_ != nullptr) parent_->on_next(context);
}

}  // namespace esphome::elements
