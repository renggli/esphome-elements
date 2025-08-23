#pragma once

#include "component.h"
#include "esphome/components/display/display.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"

#ifdef USE_WEBSERVER
#include "esphome/components/web_server_base/web_server_base.h"
#endif

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
  [[nodiscard]] Element *get_root() const { return root_; }

  /// Return the current time in milliseconds at the start of the last update.
  [[nodiscard]] uint32_t get_current_ms() const { return current_ms_; }

  /// Return the delta time in milliseconds since the last update.
  [[nodiscard]] uint32_t get_delta_ms() const { return delta_ms_; }

  /// Draws the element onto the provided display.
  void draw(display::Display &display);

 protected:
  Element *root_ = nullptr;
  display::Display *display_ = nullptr;
  bool aliased_ = false;

  bool request_on_show_ = false;
  uint32_t current_ms_ = millis();
  uint32_t delta_ms_ = 0;

#ifdef USE_WEBSERVER
  friend class ElementComponentHandler;
#endif
};

#ifdef USE_WEBSERVER
class ElementComponentHandler : public AsyncWebHandler {
 public:
  ElementComponentHandler(ElementComponent *component) : component_(component) {}

  bool canHandle(AsyncWebServerRequest *request) const override;
  void handleRequest(AsyncWebServerRequest *request) override;

 protected:
  ElementComponent *component_;
};
#endif

}  // namespace esphome::elements
