#include "element.h"

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

void Element::on_show() {}

void Element::on_hide() {}

void Element::on_next() {
  if (parent_ == nullptr) {
    ESP_LOGW(ELEMENT_TAG, "Unhandled next-event");
  } else {
    parent_->on_next();
  }
}

void Element::on_prev() {
  if (parent_ == nullptr) {
    ESP_LOGW(ELEMENT_TAG, "Unhandled prev-event");
  } else {
    parent_->on_prev();
  }
}

}  // namespace esphome::elements
