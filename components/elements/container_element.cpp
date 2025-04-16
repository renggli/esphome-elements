#include "container_element.h"

#include "display.h"

namespace esphome::elements {

void ContainerElement::add_element(Element *element) {
  elements_.push_back(element);
}

void ContainerElement::dump_config(int level) {
  Element::dump_config(level);
  for (Element *element : elements_) {
    element->dump_config(level + 1);
  }
}

void ContainerElement::on_show() {
  for (Element *element : elements_) {
    element->on_show();
  }
}

void ContainerElement::on_hide() {
  for (Element *element : elements_) {
    element->on_hide();
  }
}

bool ContainerElement::is_active() {
  switch (active_mode_) {
  case ActiveMode::ALWAYS:
    return true;
  case ActiveMode::ANY:
    for (Element *element : elements_) {
      if (element->is_active())
        return true;
    }
    return false;
  case ActiveMode::ALL:
    for (Element *element : elements_) {
      if (!element->is_active())
        return false;
    }
    return true;
  case ActiveMode::NEVER:
    return false;
  }
  return false;
}

void OverlayElement::draw(display::Display &display) {
  for (Element *element : elements_) {
    element->draw(display);
  }
}

static const char *PRIORITY_ELEMENT_TAG = "elements.priority";

void PriorityElement::draw(display::Display &display) {
  int index = find_active_index_();
  if (index != index_) {
    ESP_LOGI(PRIORITY_ELEMENT_TAG, "Switching from %i to %i", index_, index);
    on_hide();
    index_ = index;
    on_show();
  }
  if (index_ != -1) {
    elements_[index_]->draw(display);
  }
}

void PriorityElement::on_show() {
  if (index_ != -1) {
    elements_[index_]->on_show();
  }
}

void PriorityElement::on_hide() {
  if (index_ != -1) {
    elements_[index_]->on_hide();
  }
}

int PriorityElement::find_active_index_() {
  for (int i = 0; i < elements_.size(); i++) {
    if (elements_[i]->is_active()) {
      return i;
    }
  }
  return -1;
}

void HorizontalElement::draw(display::Display &display) {
  int width = display.get_width() / elements_.size();
  for (int i = 0; i < elements_.size(); i++) {
    auto sub_display =
        SubDisplay(display, i * width, 0, width, display.get_height());
    elements_[i]->draw(sub_display);
  }
}

void VerticalElement::draw(display::Display &display) {
  int height = display.get_height() / elements_.size();
  for (int i = 0; i < elements_.size(); i++) {
    auto sub_display =
        SubDisplay(display, 0, i * height, display.get_width(), height);
    elements_[i]->draw(sub_display);
  }
}

static const char *SEQUENCE_ELEMENT_TAG = "elements.sequence";

void SequenceElement::draw(display::Display &display) {
  if (index_ == -1) {
    on_next();
  }
  if (index_ != -1) {
    elements_[index_]->draw(display);
  }
}

void SequenceElement::go_to(int index) {
  if (index_ != index) {
    ESP_LOGI(SEQUENCE_ELEMENT_TAG, "Switching from %i to %i", index_, index);
    on_hide();
    index_ = index;
    on_show();
  }
}

void SequenceElement::on_show() {
  if (index_ != -1) {
    elements_[index_]->on_show();
  }
}

void SequenceElement::on_hide() {
  if (index_ != -1) {
    elements_[index_]->on_hide();
  }
}

void SequenceElement::on_next() {
  for (int offset = 1; offset < elements_.size(); offset++) {
    int new_index = (index_ + offset) % elements_.size();
    if (elements_[new_index]->is_active()) {
      go_to(new_index);
      return;
    }
  }
}

} // namespace esphome::elements
