#include "container_element.h"

#include "display.h"

namespace esphome::elements {

// ---------------------------------------------------------------------------
// ContainerElement
// ---------------------------------------------------------------------------

void ContainerElement::add_element(Element* element) {
  elements_.push_back(element);
}

void ContainerElement::dump_config(int level) {
  Element::dump_config(level);
  for (Element* element : elements_) {
    element->dump_config(level + 1);
  }
}

bool ContainerElement::is_active() {
  switch (active_mode_) {
    case ActiveMode::ALWAYS:
      return true;
    case ActiveMode::ANY:
      for (Element* element : elements_) {
        if (element->is_active()) return true;
      }
      return false;
    case ActiveMode::ALL:
      for (Element* element : elements_) {
        if (!element->is_active()) return false;
      }
      return true;
    case ActiveMode::NEVER:
      return false;
  }
  return false;
}

void ContainerElement::update_state() {
  Element::update_state();
  for (Element* element : elements_) {
    element->update_state();
  }
}

void ContainerElement::update_visibility(bool now_visible) {
  Element::update_visibility(now_visible);
  for (Element* element : elements_) {
    element->update_visibility(this->visible_);
  }
}

// ---------------------------------------------------------------------------
// OverlayElement
// ---------------------------------------------------------------------------

void OverlayElement::draw(display::Display& display) {
  for (Element* element : elements_) {
    element->draw(display);
  }
}

// ---------------------------------------------------------------------------
// HorizontalElement
// ---------------------------------------------------------------------------

void HorizontalElement::draw(display::Display& display) {
  if (!elements_.empty()) {
    int width = display.get_width() / elements_.size();
    for (int i = 0; i < (int)elements_.size(); i++) {
      auto sub_display =
          SubDisplay(display, i * width, 0, width, display.get_height());
      elements_[i]->draw(sub_display);
    }
  }
}

// ---------------------------------------------------------------------------
// VerticalElement
// ---------------------------------------------------------------------------

void VerticalElement::draw(display::Display& display) {
  if (!elements_.empty()) {
    int height = display.get_height() / elements_.size();
    for (int i = 0; i < (int)elements_.size(); i++) {
      auto sub_display =
          SubDisplay(display, 0, i * height, display.get_width(), height);
      elements_[i]->draw(sub_display);
    }
  }
}

}  // namespace esphome::elements
