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

  // Initialize the context.
  context_.component = this;
  context_.current_ms = millis();
  context_.delta_ms = 0;
}

void ElementComponent::dump_config() {
  ESP_LOGCONFIG(ELEMENT_COMPONENT_TAG, "Elements Component");
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
  context_.delta_ms = now - context_.current_ms;
  context_.current_ms = now;

  // Call on-show the first time.
  if (request_on_show_) {
    root_->on_show(context_);
    request_on_show_ = false;
  }

  // Draw the update.
  root_->draw(context_, display);
}

}  // namespace esphome::elements
