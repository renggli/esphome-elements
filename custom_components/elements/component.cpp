#include "component.h"

#include "esphome.h"

namespace esphome::elements {

static const char *ELEMENT_COMPONENT_TAG = "elements.component";

void ElementComponent::setup() {
  // If we have a display, setup automatic drawing. Otherwise users need to
  // manually call `Elements::draw(Display&)` with the desired display as
  // argument.
  if (display_) {
    display::display_writer_t writer = [this](display::Display &it) {
      draw(it);
    };
    display_page_ = make_unique<display::DisplayPage>(writer);
    display_->show_page(display_page_.get());
  }
}

void ElementComponent::dump_config() {
  ESP_LOGCONFIG(ELEMENT_COMPONENT_TAG, "Elements Component");
}

void ElementComponent::set_root(Element* root) {
  root_ = root;
  request_on_show_ = true;
}

void ElementComponent::draw() {
  if (display_ == nullptr) {
    ESP_LOGE(ELEMENT_COMPONENT_TAG, "draw() called without a display");
    return;
  }
  draw(*display_);
}

void ElementComponent::draw(display::Display &display) {
  // Check the preconditions.
  if (root_ == nullptr) {
    ESP_LOGE(ELEMENT_COMPONENT_TAG, "draw() called without a root element");
    return;
  }

  // Update the time.
  u_int32_t now = millis();
  delta_ms_ = now - current_ms_;
  current_ms_ = now;

  // Call on-show the first time.
  if (request_on_show_) {
    root_->on_show();
    request_on_show_ = false;
  }

  // Draw the update.
  root_->draw(display);
}

}  // namespace esphome::elements
