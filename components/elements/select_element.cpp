#include "select_element.h"

#include <limits>

namespace esphome::elements {

// ---------------------------------------------------------------------------
// SelectElement
// ---------------------------------------------------------------------------

void SelectElement::visit_children(const std::function<void(Element *, bool)> &fn) {
  for (int i = 0; i < (int) elements_.size(); i++) {
    fn(elements_[i], i == index_);
  }
}

void SelectElement::go_to(int index) { index_ = index; }

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
