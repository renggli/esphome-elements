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

bool DelegateElement::is_active() { return element_ != nullptr && element_->is_active(); }

static const char *const TIMEOUT_ELEMENT_TAG = "elements.timeout";

void TimeoutElement::draw(display::Display &display) {
  if (next_ms_ <= get_component().get_current_ms()) {
    ESP_LOGI(TIMEOUT_ELEMENT_TAG, "Dispatching `on_next` after %u ms", duration_ms_);
    DelegateElement::on_next();
  }
  DelegateElement::draw(display);
}

void TimeoutElement::on_show() {
  next_ms_ = get_component().get_current_ms() + duration_ms_;
  DelegateElement::on_show();
}

static const char *const DELAY_ELEMENT_TAG = "elements.delay";

void DelayElement::on_show() {
  counter_ = 0;
  DelegateElement::on_show();
}

void DelayElement::on_next() {
  counter_++;
  if (0 < count_ && count_ <= counter_) {
    ESP_LOGI(DELAY_ELEMENT_TAG, "Dispatching `on_next` after %u events", counter_);
    DelegateElement::on_next();
  }
}

}  // namespace esphome::elements
