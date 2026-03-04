#include "delegate_element.h"
#include <cstdint>

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

bool DelegateElement::is_active() const { return element_ != nullptr && element_->is_active(); }

void DelegateElement::on_show() {
  if (element_ != nullptr) {
    element_->on_show();
  }
  Element::on_show();
}

void DelegateElement::on_hide() {
  if (element_ != nullptr) {
    element_->on_hide();
  }
  Element::on_hide();
}

// Timeout Element

static const char *const TIMEOUT_ELEMENT_TAG = "elements.timeout";

void TimeoutElement::draw(display::Display &display) {
  uint32_t current_ms = get_component()->get_current_ms();
  if (next_ms_ != 0 && next_ms_ <= current_ms) {
    next_ms_ = current_ms + duration_ms_;
    on_complete();
  }
  DelegateElement::draw(display);
}

void TimeoutElement::on_show() {
  next_ms_ = get_component()->get_current_ms() + duration_ms_;
  DelegateElement::on_show();
}

void TimeoutElement::on_hide() {
  next_ms_ = 0;
  DelegateElement::on_hide();
}

void TimeoutElement::on_complete() {
  ESP_LOGI(TIMEOUT_ELEMENT_TAG, "Triggering `on_complete` callbacks after %u ms", duration_ms_);
  on_complete_callbacks_.call(this);
}

}  // namespace esphome::elements
