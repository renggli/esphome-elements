#pragma once

#include <deque>

#include "element.h"

namespace esphome::elements {

enum class SchedulerInsertion { CURRENT, NEXT, LAST };

struct SchedulerOptions {
  // Identifier of the element.
  std::string id;
  // Priority for the initial scheduling.
  SchedulerInsertion insertion = SchedulerInsertion::LAST;
  // Duration to show the element, before switching to the next one. A value of
  // 0 means the element never switches to the next one.
  uint32_t duration_ms = 5000;
  // Duration to show the element, before removing from the queue. A value of 0
  // means the element is never removed from the queue.
  uint32_t total_duration_ms = 0;
  // Number of times to show the element, before removing from the queue. A
  // value of 0 means the element is never removed from the queue.
  uint32_t total_count = 0;
};

struct SchedulerState {
  // The scheduled element itself.
  Element* element;
  // The options of the scheduled element.
  SchedulerOptions options;
  // The total duration this element has been shown.
  uint32_t total_duration_ms = 0;
  // The total number of times this element has been shown.
  uint32_t total_count = 0;
};

class SchedulerElement : public Element {
 public:
  explicit SchedulerElement() : Element(ElementType::SCHEDULER) {}

  void add(Element* element, const SchedulerOptions& options);

  virtual void draw(const Context& context, display::Display& display) override;

 protected:
  // The currently display element and its state.
  std::unique_ptr<SchedulerState> current_state_;

  // The time the state has changed to the current element.
  uint32_t current_state_start_ms_;

  // Queue of all the available elements to show.
  std::deque<std::unique_ptr<SchedulerState>> queue_ = {};

  void schedule_next_();
  void schedule_update_(const Context& context);
};

}  // namespace esphome::elements
