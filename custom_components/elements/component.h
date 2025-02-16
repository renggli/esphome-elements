#pragma once

#include "component.h"
#include "esphome/components/display/display.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"

namespace esphome::elements {

class Element;

/// The ESPHome component that displays elements.
class ElementComponent : public Component {
 public:
  void setup() override;
  void dump_config() override;

  void set_display(display::Display *display) { display_ = display; }
  void set_root(Element *root);

  /// Return the root element of this component.
  Element *get_root() { return root_; }

  /// Return the current time in milliseconds at the start of the last update.
  uint32_t get_current_ms() { return current_ms_;  }

  /// Return the delta time in milliseconds since the last update.
  uint32_t get_delta_ms() { return delta_ms_; }

  void draw();
  void draw(display::Display &display);

 protected:
  display::Display *display_ = nullptr;
  std::unique_ptr<display::DisplayPage> display_page_ = nullptr;
  bool request_on_show_ = false;

  uint32_t current_ms_ = millis();
  uint32_t delta_ms_ = 0;
  Element *root_ = nullptr;
};

}  // namespace esphome::elements
