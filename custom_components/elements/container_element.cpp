#include "container_element.h"

#include "utils.h"

namespace esphome::elements {

const char* CONTAINER_ELEMENT_TAG = "elements.container";

void ContainerElement::add_element(Element* element) {
  elements_.push_back(element);
  element->set_parent(this);
}

void ContainerElement::on_show(const Context& context) {
  for (Element* element : elements_) {
    element->on_show(context);
  }
}

void ContainerElement::on_hide(const Context& context) {
  for (Element* element : elements_) {
    element->on_hide(context);
  }
}

void OverlayElement::draw(const Context& context, display::Display& display) {
  for (Element* element : elements_) {
    element->draw(context, display);
  }
}

void HorizontalElement::draw(const Context& context,
                             display::Display& display) {
  int width = display.get_width() / elements_.size();
  for (int i = 0; i < elements_.size(); i++) {
    auto sub_display =
        SubDisplay(display, i * width, 0, width, display.get_height());
    elements_[i]->draw(context, sub_display);
  }
}

void VerticalElement::draw(const Context& context, display::Display& display) {
  int height = display.get_height() / elements_.size();
  for (int i = 0; i < elements_.size(); i++) {
    auto sub_display =
        SubDisplay(display, 0, i * height, display.get_height(), height);
    elements_[i]->draw(context, sub_display);
  }
}

void SequenceElement::draw(const Context& context, display::Display& display) {
  if (duration_ms_.has_value() &&
      start_ms_ + *duration_ms_ <= context.current_ms) {
    ESP_LOGD(CONTAINER_ELEMENT_TAG, "Duration exceeded");
    on_next(context);
  }
  elements_[index_]->draw(context, display);
}

void SequenceElement::on_show(const Context& context) {
  index_ = 0;
  start_ms_ = context.current_ms;
  elements_[index_]->on_show(context);
}

void SequenceElement::on_hide(const Context& context) {
  elements_[index_]->on_hide(context);
}

void SequenceElement::on_next(const Context& context) {
  if (elements_.size() <= 1) return;
  elements_[index_]->on_hide(context);
  index_ = (index_ + 1) % elements_.size();
  start_ms_ = context.current_ms;
  elements_[index_]->on_show(context);
}

}  // namespace esphome::elements
