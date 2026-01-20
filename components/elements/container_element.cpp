#include "container_element.h"

#include "display.h"
#include <limits>

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
  if (!elements_.empty()) {
    int width = display.get_width() / elements_.size();
    for (int i = 0; i < elements_.size(); i++) {
      auto sub_display = SubDisplay(display, i * width, 0, width, display.get_height());
      elements_[i]->draw(sub_display);
    }
  }
}

void VerticalElement::draw(display::Display &display) {
  if (!elements_.empty()) {
    int height = display.get_height() / elements_.size();
    for (int i = 0; i < elements_.size(); i++) {
      auto sub_display = SubDisplay(display, 0, i * height, display.get_width(), height);
      elements_[i]->draw(sub_display);
    }
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

void RandomElement::update_history() {
  if (index_ != -1) {
    history_.push_back(index_);
    while (history_.size() > 2 * elements_.size()) {
      history_.pop_front();
    }
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

void RandomElement::on_prev() {
  while (!history_.empty()) {
    int index = history_.back();
    history_.pop_back();
    if (elements_[index]->is_active()) {
      go_to(index);
      return;
    }
  }
}

void RandomElement::on_next() {
  std::vector<int> candidates;
  int min = std::numeric_limits<int>::max();
  for (int index = 0; index < elements_.size(); index++) {
    if (!elements_[index]->is_active())
      continue;
    int count = 0;
    for (int hist_index : history_) {
      if (hist_index == index) {
        count++;
      }
    }
    if (count < min) {
      min = count;
      candidates.clear();
      candidates.push_back(index);
    } else if (count == min) {
      candidates.push_back(index);
    }
  }
  if (!candidates.empty()) {
    int index = candidates[get_component().get_current_ms() % candidates.size()];
    go_to(index);
    update_history();
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

void SequenceElement::on_prev() {
  int count = elements_.size();
  for (int offset = 1; offset < count; offset++) {
    int new_index = (index_ - offset + count) % count;
    if (elements_[new_index]->is_active()) {
      go_to(new_index);
      return;
    }
  }
}

void SequenceElement::on_next() {
  int count = elements_.size();
  for (int offset = 1; offset < count; offset++) {
    int new_index = (index_ + offset) % count;
    if (elements_[new_index]->is_active()) {
      go_to(new_index);
      return;
    }
  }
}

}  // namespace esphome::elements
