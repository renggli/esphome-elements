#include "container_element.h"

#include "utils.h"

namespace esphome::elements {

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
  if (start_ms_ == 0) start_ms_ = context.current_ms;
  if (start_ms_ + duration_ms_ <= context.current_ms) {
    index_ = (index_ + 1) % elements_.size();
    start_ms_ = context.current_ms;
  }
  elements_[index_]->draw(context, display);
}

}  // namespace esphome::elements
