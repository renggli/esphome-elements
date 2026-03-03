#include "element.h"
#include "esphome/core/log.h"

namespace esphome::elements {

static const char *const ELEMENT_TAG = "elements.element";

void Element::dump_config(int level) { dump_config(level, "- type: %s", get_type_name()); }

void Element::dump_config(int level, const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  char buffer[256];
  int ret = vsnprintf(buffer, sizeof(buffer), format, arg);
  if (ret > 0) {
    ESP_LOGCONFIG(ELEMENT_TAG, "%s%s", std::string(2 * level, ' ').c_str(), buffer);
  }
  va_end(arg);
}

void Element::on_show() {
  ESP_LOGI(ELEMENT_TAG, "Triggering `on_show` callbacks");
  on_show_callbacks_.call(this);
}

void Element::on_hide() {
  ESP_LOGI(ELEMENT_TAG, "Triggering `on_hide` callbacks");
  on_hide_callbacks_.call(this);
}

}  // namespace esphome::elements
