#include "container_element.h"

#include "display.h"

namespace esphome::elements {

const char* CONTAINER_ELEMENT_TAG = "elements.container";

void ContainerElement::add_element(Element* element) {
  elements_.push_back(element);
}

void ContainerElement::on_show() {
  for (Element* element : elements_) {
    element->on_show();
  }
}

void ContainerElement::on_hide() {
  for (Element* element : elements_) {
    element->on_hide();
  }
}

void OverlayElement::draw(display::Display& display) {
  for (Element* element : elements_) {
    element->draw(display);
  }
}

void HorizontalElement::draw(display::Display& display) {
  int width = display.get_width() / elements_.size();
  for (int i = 0; i < elements_.size(); i++) {
    auto sub_display =
        SubDisplay(display, i * width, 0, width, display.get_height());
    elements_[i]->draw(sub_display);
  }
}

void VerticalElement::draw(display::Display& display) {
  int height = display.get_height() / elements_.size();
  for (int i = 0; i < elements_.size(); i++) {
    auto sub_display =
        SubDisplay(display, 0, i * height, display.get_height(), height);
    elements_[i]->draw(sub_display);
  }
}

void SequenceElement::draw(display::Display& display) {
  if (timer_.check(get_context().current_ms)) {
    ESP_LOGD(CONTAINER_ELEMENT_TAG, "Duration exceeded");
    on_next();
  }
  elements_[index_]->draw(display);
}

void SequenceElement::go_to(int index) {
  if (index_ == index) return;
  elements_[index_]->on_hide();
  index_ = index;
  timer_.reset(get_context().current_ms);
  elements_[index_]->on_show();
}

void SequenceElement::on_show() {
  index_ = 0;
  timer_.reset(get_context().current_ms);
  elements_[index_]->on_show();
}

void SequenceElement::on_hide() { elements_[index_]->on_hide(); }

void SequenceElement::on_next() { go_to((index_ + 1) % elements_.size()); }

}  // namespace esphome::elements
