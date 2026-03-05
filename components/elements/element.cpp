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
  ESP_LOGI(ELEMENT_TAG, "Triggering `on_show` for %s", get_type_name());
  on_show_callbacks_.call(this);
}

void Element::on_hide() {
  ESP_LOGI(ELEMENT_TAG, "Triggering `on_hide` for %s", get_type_name());
  on_hide_callbacks_.call(this);
}

void Element::update_visibility_(bool now_visible) {
  // Fire transition events.
  if (!visible_ && now_visible) {
    visible_ = true;
    on_show();
  } else if (visible_ && !now_visible) {
    visible_ = false;
    on_hide();
  }

  // Recurse into children with their own visibility.
  visit_children([](Element *child, bool child_visible) { child->update_visibility_(child_visible); });
}

}  // namespace esphome::elements
