#pragma once

#include "component.h"
#include "esphome/components/display/display.h"
#include "esphome/core/component.h"

namespace esphome::elements {

class Element;
class ElementComponent;

/// The drawing context passed to the drawing function.
struct Context {
  // Component initiating this call.
  ElementComponent *component = nullptr;
  // Milliseconds passed since the start of the engine.
  uint32_t current_ms = 0;
  // Milliseconds passed since the last draw.
  uint32_t delta_ms = 0;
};

/// The ESPHome component that displays elements.
class ElementComponent : public Component {
 public:
  void setup() override;
  void dump_config() override;

  void set_display(display::Display *display) { display_ = display; }
  void set_root(Element *root) {
    root_ = root;
    request_on_show_ = true;
  }

  void draw();
  void draw(display::Display &display);

 protected:
  display::Display *display_ = nullptr;
  std::unique_ptr<display::DisplayPage> display_page_ = nullptr;
  bool request_on_show_ = false;

  Context context_;
  Element *root_ = nullptr;
};

}  // namespace esphome::elements
