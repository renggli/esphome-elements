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

  void set_root(Element *root);
  void set_display(display::Display *display) { display_ = display; }
  void set_aliased(bool aliased) { aliased_ = aliased; }

  /// Return the root element of this component.
  Element *get_root() const { return root_; }

  /// Return the current time in milliseconds at the start of the last update.
  uint32_t get_current_ms() const { return current_ms_; }

  /// Return the delta time in milliseconds since the last update.
  uint32_t get_delta_ms() const { return delta_ms_; }

  /// Draws the element onto the provided display.
  void draw(display::Display &display);

protected:
  Element *root_ = nullptr;
  display::Display *display_ = nullptr;
  bool aliased_ = false;

  bool request_on_show_ = false;
  uint32_t current_ms_ = millis();
  uint32_t delta_ms_ = 0;
};

} // namespace esphome::elements
