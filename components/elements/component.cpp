#include "component.h"

#include "element.h"
#include <cstdint>

namespace esphome::elements {

static const char *ELEMENT_COMPONENT_TAG = "elements.component";

void ElementComponent::setup() {
  // If we have a display, setup automatic drawing. Otherwise users need to
  // manually call `Elements::draw(Display&)` with the desired display as
  // argument.
  if (display_ != nullptr) {
    display::display_writer_t writer = [this](display::Display &it) {
      display_->clear();
      draw(*display_);
    };
    display_->show_page(new display::DisplayPage(writer));
  } else {
    ESP_LOGW(ELEMENT_COMPONENT_TAG, "No display setup, drawing needs to be manually called.");
  }
}

void ElementComponent::dump_config() {
  ESP_LOGCONFIG(ELEMENT_COMPONENT_TAG, "Elements Component");
  if (root_ != nullptr) {
    root_->dump_config(0);
  }
}

void ElementComponent::set_root(Element *root) {
  root_ = root;
  request_on_show_ = true;
}

void ElementComponent::draw(display::Display &display) {
  // Check the preconditions.
  if (root_ == nullptr) {
    ESP_LOGE(ELEMENT_COMPONENT_TAG, "draw() called without a root element.");
    return;
  }

  // Update the time.
  uint32_t now = millis();
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
