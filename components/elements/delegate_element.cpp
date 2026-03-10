#include "delegate_element.h"
#include <cstdint>

namespace esphome::elements {

void DelegateElement::dump_config(int level) {
  Element::dump_config(level);
  if (element_ != nullptr) {
    element_->dump_config(level + 1);
  }
}

bool DelegateElement::is_active() const { return element_ != nullptr && element_->is_active(); }

void DelegateElement::update_visibility(bool now_visible) {
  Element::update_visibility(now_visible);
  if (element_ != nullptr) {
    element_->update_visibility(this->visible_);
  }
}

void DelegateElement::update_state() {
  if (element_ != nullptr) {
    element_->update_state();
  }
}

void DelegateElement::draw(display::Display &display) {
  if (element_ != nullptr) {
    element_->draw(display);
  }
}

// ---------------------------------------------------------------------------
// TimeoutElement
// ---------------------------------------------------------------------------

static const char *const TIMEOUT_ELEMENT_TAG = "elements.timeout";

void TimeoutElement::update_state() {
  DelegateElement::update_state();
  if (this->visible_) {
    uint32_t current_ms = get_component()->get_current_ms();
    if (start_time_ != 0 && current_ms - start_time_ >= duration_ms_) {
      start_time_ = current_ms;
      on_complete();
    }
  }
}

void TimeoutElement::on_show() {
  start_time_ = get_component()->get_current_ms();
  if (start_time_ == 0)
    start_time_ = 1;
  DelegateElement::on_show();
}

void TimeoutElement::on_hide() {
  start_time_ = 0;
  DelegateElement::on_hide();
}

void TimeoutElement::on_complete() {
  ESP_LOGI(TIMEOUT_ELEMENT_TAG, "Triggering `on_complete` for %s (%p): %u ms", get_type_name(), this, duration_ms_);
  on_complete_callbacks_.call(this);
}

}  // namespace esphome::elements
