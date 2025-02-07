#include "esphome/core/log.h"
#include "scheduler_element.h"

namespace esphome::elements {

static const char* SCHEDULER_ELEMENT_TAG = "elements.scheduler";

void SchedulerElement::add(Element* element, const SchedulerOptions& options) {
  auto state = make_unique<SchedulerState>();
  state->element = element;
  state->options = options;
  switch (options.insertion) {
    case SchedulerInsertion::CURRENT:
    case SchedulerInsertion::NEXT:
      queue_.emplace_front(state);
    case SchedulerInsertion::LAST:
      queue_.emplace_back(state);
  }
  if (options.insertion == SchedulerInsertion::CURRENT) {
    schedule_next_();
  }
}

void SchedulerElement::draw(const Context& context, display::Display& display) {
  schedule_update_(context);
  // current_->element_->show(context, display);
}

void SchedulerElement::schedule_next_() {

}

void SchedulerElement::schedule_update_(const Context& context) {

}

}  // namespace esphome::elements
