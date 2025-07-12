#include "container_element.h"

#include "display.h"

namespace esphome::elements {

void ContainerElement::add_element(Element *element) { elements_.push_back(element); }

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

static const char *const PRIORITY_ELEMENT_TAG = "elements.priority";

void PriorityElement::draw(display::Display &display) {
  int index = find_active_index_();
  if (index_ != index) {
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
    auto sub_display = SubDisplay(display, i * width, 0, width, display.get_height());
    elements_[i]->draw(sub_display);
  }
}

void VerticalElement::draw(display::Display &display) {
  int height = display.get_height() / elements_.size();
  for (int i = 0; i < elements_.size(); i++) {
    auto sub_display = SubDisplay(display, 0, i * height, display.get_width(), height);
    elements_[i]->draw(sub_display);
  }
}

static const char *const RANDOM_ELEMENT_TAG = "elements.random";

void RandomElement::draw(display::Display &display) {
  if (index_ == -1) {
    on_next();
  }
  if (index_ != -1) {
    elements_[index_]->draw(display);
  }
}

void RandomElement::go_to(int index) {
  if (index_ != index) {
    ESP_LOGI(RANDOM_ELEMENT_TAG, "Switching from %i to %i", index_, index);
    on_hide();
    index_ = index;
    on_show();
  }
}

void RandomElement::on_show() {
  if (index_ != -1) {
    elements_[index_]->on_show();
  }
}

void RandomElement::on_hide() {
  if (index_ != -1) {
    elements_[index_]->on_hide();
  }
}

void RandomElement::on_next() {
  // Find active elements not shown recently.
  std::vector<int> eligible_indices;
  for (int i = 0; i < elements_.size(); i++) {
    if (elements_[i]->is_active()) {
      bool in_history = false;
      for (int j : history_) {
        if (i == j) {
          in_history = true;
          break;
        }
      }
      if (!in_history) {
        eligible_indices.push_back(i);
      }
    }
  }
  // No eligible indices, reset the history.
  if (eligible_indices.empty()) {
    history_.clear();
    for (int i = 0; i < elements_.size(); i++) {
      if (elements_[i]->is_active() && i != index_) {
        eligible_indices.push_back(i);
      }
    }
  }
  // Pick an index, if we have candidates.
  if (!eligible_indices.empty()) {
    int index = eligible_indices[get_component().get_current_ms() % eligible_indices.size()];
    history_.push_back(index);
    go_to(index);
  }
}

static const char *const SEQUENCE_ELEMENT_TAG = "elements.sequence";

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

}  // namespace esphome::elements
