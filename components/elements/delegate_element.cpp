#include "delegate_element.h"

namespace esphome::elements {

void DelegateElement::dump_config(int level) {
  Element::dump_config(level);
  if (element_ != nullptr) {
    element_->dump_config(level + 1);
  }
}

void DelegateElement::draw(display::Display &display) {
  if (element_ != nullptr) {
    element_->draw(display);
  }
}

bool DelegateElement::is_active() { return element_ != nullptr && element_->is_active(); }

void DelegateElement::on_show() {
  if (element_ != nullptr) {
    element_->on_show();
  }
}

void DelegateElement::on_hide() {
  if (element_ != nullptr) {
    element_->on_hide();
  }
}

}  // namespace esphome::elements
