#include "element.h"

namespace esphome::elements {

const char *ELEMENT_TAG = "elements.element";

const char *element_type_get_type_string(ElementType type) {
  switch (type) {
  case ElementType::UNKNOWN:
    return "unknown";
  case ElementType::CLOCK:
    return "clock";
  case ElementType::CUSTOM:
    return "custom";
  case ElementType::DELAY:
    return "delay";
  case ElementType::DYNAMIC_TEXT:
    return "dynamic_text";
  case ElementType::HORIZONTAL:
    return "horizontal";
  case ElementType::IMAGE:
    return "image";
  case ElementType::OVERLAY:
    return "overlay";
  case ElementType::PRIORITY:
    return "priority";
  case ElementType::RANDOM:
    return "random";
  case ElementType::SEQUENCE:
    return "sequence";
  case ElementType::STATIC_TEXT:
    return "static_text";
  case ElementType::TIME_TEXT:
    return "time_text";
  case ElementType::TIMEOUT:
    return "timeout";
  case ElementType::VERTICAL:
    return "vertical";
  }
  return "unknown";
}

const char *Element::get_type_string() const {
  return element_type_get_type_string(get_type());
}

void Element::dump_config(int level) {
  dump_config(level, "- type: %s", get_type_string());
}

void Element::dump_config(int level, const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  char buffer[256];
  int ret = vsnprintf(buffer, sizeof(buffer), format, arg);
  if (ret > 0) {
    std::string indent(2 * level, ' ');
    ESP_LOGCONFIG(ELEMENT_TAG, "%s%s", indent.c_str(), buffer);
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

} // namespace esphome::elements
