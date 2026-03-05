#include "container_element.h"

#include <limits>

#include "display.h"

namespace esphome::elements {

// ---------------------------------------------------------------------------
// ContainerElement
// ---------------------------------------------------------------------------

void ContainerElement::add_element(Element *element) { elements_.push_back(element); }

void ContainerElement::dump_config(int level) {
  Element::dump_config(level);
  for (Element *element : elements_) {
    element->dump_config(level + 1);
  }
}

bool ContainerElement::is_active() const {
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

void ContainerElement::visit_children(const std::function<void(Element *, bool)> &fn) {
  for (Element *element : elements_) {
    fn(element, true);
  }
}

void ContainerElement::update_state() {
  for (Element *element : elements_) {
    element->update_state();
  }
}

// ---------------------------------------------------------------------------
// SelectElement
// ---------------------------------------------------------------------------

void SelectElement::visit_children(const std::function<void(Element *, bool)> &fn) {
  for (int i = 0; i < (int) elements_.size(); i++) {
    fn(elements_[i], i == index_);
  }
}

void SelectElement::go_to(int index) { index_ = index; }

void SelectElement::next() {
  int count = elements_.size();
  for (int offset = 1; offset < count; offset++) {
    int new_index = (index_ + offset) % count;
    if (elements_[new_index]->is_active()) {
      go_to(new_index);
      return;
    }
  }
}

void SelectElement::prev() {
  int count = elements_.size();
  for (int offset = 1; offset < count; offset++) {
    int new_index = (index_ - offset + count) % count;
    if (elements_[new_index]->is_active()) {
      go_to(new_index);
      return;
    }
  }
}

// ---------------------------------------------------------------------------
// OverlayElement
// ---------------------------------------------------------------------------

void OverlayElement::draw(display::Display &display) {
  for (Element *element : elements_) {
    element->draw(display);
  }
}

// ---------------------------------------------------------------------------
// PriorityElement
// ---------------------------------------------------------------------------

static const char *const PRIORITY_ELEMENT_TAG = "elements.priority";

int PriorityElement::find_active_index_() const {
  for (int i = 0; i < (int) elements_.size(); i++) {
    if (elements_[i]->is_active()) {
      return i;
    }
  }
  return -1;
}

void PriorityElement::update_state() {
  // Update children first, then re-evaluate which one has priority.
  ContainerElement::update_state();
  int new_index = find_active_index_();
  if (new_index != index_) {
    ESP_LOGI(PRIORITY_ELEMENT_TAG, "Switching from %i to %i", index_, new_index);
    go_to(new_index);
  }
}

void PriorityElement::draw(display::Display &display) {
  if (index_ != -1) {
    elements_[index_]->draw(display);
  }
}

// ---------------------------------------------------------------------------
// HorizontalElement
// ---------------------------------------------------------------------------

void HorizontalElement::draw(display::Display &display) {
  if (!elements_.empty()) {
    int width = display.get_width() / elements_.size();
    for (int i = 0; i < (int) elements_.size(); i++) {
      auto sub_display = SubDisplay(display, i * width, 0, width, display.get_height());
      elements_[i]->draw(sub_display);
    }
  }
}

// ---------------------------------------------------------------------------
// VerticalElement
// ---------------------------------------------------------------------------

void VerticalElement::draw(display::Display &display) {
  if (!elements_.empty()) {
    int height = display.get_height() / elements_.size();
    for (int i = 0; i < (int) elements_.size(); i++) {
      auto sub_display = SubDisplay(display, 0, i * height, display.get_width(), height);
      elements_[i]->draw(sub_display);
    }
  }
}

// ---------------------------------------------------------------------------
// RandomElement
// ---------------------------------------------------------------------------

static const char *const RANDOM_ELEMENT_TAG = "elements.random";

void RandomElement::update_state() {
  // Update children first, then initialise if we have no selection yet.
  ContainerElement::update_state();
  if (index_ == -1) {
    next();
  }
}

void RandomElement::draw(display::Display &display) {
  if (index_ != -1) {
    elements_[index_]->draw(display);
  }
}

void RandomElement::update_history_() {
  if (index_ != -1) {
    history_.push_back(index_);
    while (history_.size() > 2 * elements_.size()) {
      history_.pop_front();
    }
  }
}

void RandomElement::prev() {
  while (!history_.empty()) {
    int index = history_.back();
    history_.pop_back();
    if (elements_[index]->is_active()) {
      go_to(index);
      return;
    }
  }
}

void RandomElement::next() {
  std::vector<int> candidates;
  int min = std::numeric_limits<int>::max();
  for (int index = 0; index < (int) elements_.size(); index++) {
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
    int index = candidates[get_component()->get_current_ms() % candidates.size()];
    update_history_();
    go_to(index);
  }
}

// ---------------------------------------------------------------------------
// SequenceElement
// ---------------------------------------------------------------------------

static const char *const SEQUENCE_ELEMENT_TAG = "elements.sequence";

void SequenceElement::update_state() {
  // Update children first, then initialise if we have no selection yet.
  ContainerElement::update_state();
  if (index_ == -1) {
    next();
  }
}

void SequenceElement::draw(display::Display &display) {
  if (index_ != -1) {
    elements_[index_]->draw(display);
  }
}

void SequenceElement::prev() {
  int count = elements_.size();
  for (int offset = 1; offset < count; offset++) {
    int new_index = (index_ - offset + count) % count;
    if (elements_[new_index]->is_active()) {
      ESP_LOGI(SEQUENCE_ELEMENT_TAG, "Switching from %i to %i", index_, new_index);
      go_to(new_index);
      return;
    }
  }
}

void SequenceElement::next() {
  int count = elements_.size();
  for (int offset = 1; offset < count; offset++) {
    int new_index = (index_ + offset) % count;
    if (elements_[new_index]->is_active()) {
      ESP_LOGI(SEQUENCE_ELEMENT_TAG, "Switching from %i to %i", index_, new_index);
      go_to(new_index);
      return;
    }
  }
}

}  // namespace esphome::elements
